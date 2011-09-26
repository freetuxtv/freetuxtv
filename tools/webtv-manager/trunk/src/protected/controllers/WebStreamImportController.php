<?php

class WebStreamImportController extends Controller
{
	/**
	 * @var string the default layout for the views. Defaults to '//layouts/column2', meaning
	 * using two-column layout. See 'protected/views/layouts/column2.php'.
	 */
	public $layout='//layouts/column2';

	/**
	 * @var CActiveRecord the currently loaded data model instance.
	 */
	private $_model;

	/**
	 * @return array action filters
	 */
	public function filters()
	{
		return array(
			'accessControl', // perform access control for CRUD operations
		);
	}

	/**
	 * Specifies the access control rules.
	 * This method is used by the 'accessControl' filter.
	 * @return array access control rules
	 */
	public function accessRules()
	{
		return array(
			array('allow',  // allow all users to perform 'index' and 'checkimport' actions
				'actions'=>array('index', 'checkimport'),
				'users'=>array('*'),
			),
			array('allow',  // allow all users to perform 'import' actions
				'actions'=>array('import'),
				'roles'=>array('importWebStream'),
			),
			array('deny',  // deny all users
				'users'=>array('*'),
			),
		);
	}

	public function actionIndex()
	{
		$modelImportForm = new WebStreamImportForm;

		if(isset($_GET['WebStreamImportForm']))
		{
			$modelImportForm->attributes=$_GET['WebStreamImportForm'];
		}

		$this->render('index', array(
			'modelImportForm' => $modelImportForm,
		));
	}

	public function actionCheckImport()
	{
		$modelImportForm = new WebStreamImportForm;
		$webStreamList = null;
		$modelWebStreamData = new WebStream;

		// collect user input data
		if(isset($_GET['WebStreamImportForm']))
		{
			$modelImportForm->attributes=$_GET['WebStreamImportForm'];

			$XMLData = file_get_contents($modelImportForm->UrlWSLFEData);
			$webStreamList = $this->parseWSLEF($XMLData);

			 $i = 0;

			foreach($webStreamList as &$webstream) {
				$attrs = array("Url" => $webstream['URL']);
				$model = WebStream::model()->findByAttributes($attrs);

				$i++;

				$webstream['id'] = $i;
				
				if($model != null){
					$webstream['WebStream'] = $model;
				}
				
			}

			$this->render('checkimport', array(
				'modelImportForm' => $modelImportForm,
				'modelWebStreamData' => $modelWebStreamData,
				'webStreamList' => $webStreamList,
			));
		}
		else
			throw new CHttpException(400,'Invalid request. Please do not repeat this request again.');
	}

	public function actionImport()
	{
		$modelImportForm = new WebStreamImportForm;
		$modelWebStreamData = new WebStream;

		if(isset($_POST['WebStreamImportForm']))
		{
			$modelImportForm->attributes=$_POST['WebStreamImportForm'];
		}

		// collect user input data
		if(isset($_POST['Import']))
		{
			$modelWebStreamData->attributes = $_POST['WebStream'];

			$subject = Yii::app()->name.' - New WebStreams imported';
			$message = 'A list of WebStream have been imported by an user :<br><br>';

			foreach($_POST['Import'] as $webstream) {
				if(isset($webstream["Edit"])){

					$model = new WebStream;
					$model->attributes = $_POST['WebStream'];
					$model->Url = $webstream["URL"];
					$model->Name = $webstream["Name"];
					$model->StreamStatusCode = WebStream::WEBSTREAM_STATUS_SUBMITTED;

					if($model->LangCode==""){
						$model->LangCode=null;
					}
					if($model->CountryCode==""){
						$model->CountryCode=null;
					}
					if($model->save()){
						$history = History::createNew(History::ENTITYTYPE_WEBSTREAM, History::ACTIONTYPE_WEBSTREAM_IMPORT, $model->Id);
						if($history->save()){

							// Send a mail to the admin
							$link = Yii::app()->getRequest()->getHostInfo().Yii::app()->createUrl("WebStream/view", array("id" => $model->Id));
							$message .= '<u>Name :</u> '.$model->Name.'<br>';
							$message .= '<u>Url :</u> <a href="'.$model->Url.'">'.$model->Url.'</a><br>';
							$message .= '<br>';
							$message .= 'Click here to view the details of the WebStream : <a href="'.$link.'">'.$link.'</a><br><br>';
						}
					}
				}
			}

			$this->sendMailToAdmin($subject, $message);

			$this->redirect(array('index', 'WebStreamImportForm[UrlWSLFEData]'=>$modelImportForm->UrlWSLFEData, 'sucess'=>'on'));
		}

		$this->render('index', array(
			'modelImportForm' => $modelImportForm,
			'modelWebStreamData' => $modelWebStreamData,
		));
	}

	public function parseWSLEF($xmlData) {

		$webStreamList = array();

		$reader = new XMLReader();

		$reader->xml($xmlData);

		while($reader->read()){
			
			if ($reader->nodeType == XMLREADER::ELEMENT && $reader->localName == "stream") {
				$stream = array();

				$name = $reader->getAttribute("name");
				if($name != NULL){
					$stream['Name'] = $name;
				}

		        if ($reader->read()) {
					$stream['URL'] = $reader->value;
		        }

				$webStreamList[] = $stream;
			}
		}
		
		return $webStreamList;
	}

	protected function sendMailToAdmin($subject, $message)
	{
		if(Yii::app()->params['UseSMTP']){
			Yii::app()->mailer->IsSMTP();
			Yii::app()->mailer->Host = Yii::app()->params['SMTPHost'];
			Yii::app()->mailer->Username = Yii::app()->params['SMTPUsername'];
			Yii::app()->mailer->Password = Yii::app()->params['SMTPPassword'];
		}else{
			Yii::app()->mailer->IsSendMail();
		}
		Yii::app()->mailer->From = Yii::app()->params['appEmail'];
		Yii::app()->mailer->FromName = Yii::app()->params['appAuthor'];
		Yii::app()->mailer->AddAddress(Yii::app()->params['adminEmail']);
		Yii::app()->mailer->Subject = $subject;
		Yii::app()->mailer->MsgHTML($message);
		Yii::app()->mailer->Send();
	}

}
