<?php

class WebStreamController extends Controller
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
			array('allow',  // allow all users to perform 'index' and 'view' actions
				'actions'=>array('index','view','webstreamsearch','webstreamsend','send'),
				'users'=>array('*'),
			),
			array('allow',
				'actions'=>array('update'),
				'roles'=>array('modoEditWebStream'),
			),
			array('allow',
				'actions'=>array('changestatus'),
				'roles'=>array('modoChangeStatusWebS'),
			),
			array('deny',  // deny all users
				'users'=>array('*'),
			),
		);
	}

	/**
	 * Displays a particular model.
	 */
	public function actionView()
	{
		$model = $this->loadModel();

		$conditions = "";
		$params = array();	
		
		$conditions = "EntityType=:Type AND EntityId=:WebStreamId";
		$params["Type"] = History::ENTITYTYPE_WEBSTREAM;
		$params["WebStreamId"] = $model->Id;

		$dataHistory=new CActiveDataProvider('History',array(
			'criteria'=>array(
				'condition'=>$conditions,
				'params'=>$params,
				'order'=>'Date DESC',
			),
			'pagination'=>array(
				'pageSize'=>20,
			),
		));

		$this->render('view', array(
			'model'=>$model,
			'dataHistory'=>$dataHistory,
		));
	}

	/**
	 * Updates a particular model.
	 * If update is successful, the browser will be redirected to the 'view' page.
	 */
	public function actionUpdate()
	{
		$model=$this->loadModel();

		if(isset($_POST['WebStream']))
		{
			$actionDetails = $model->getModelDiffMsg($_POST['WebStream']);

			$model->attributes=$_POST['WebStream'];
			if($model->LangCode==""){
				$model->LangCode=null;
			}
			
			$model->username=Yii::app()->user->name;
			
			$profile = YumProfile::model()->find('user_id = ' . Yii::app()->user->id);
			$model->email=$profile->email;
			
			if($model->CountryCode==""){
				$model->CountryCode=null;
			}			
			if($model->RequiredIsp==""){
				$model->RequiredIsp=null;
			}

			if($model->save()){
				if($actionDetails != ""){
					$history = History::createNew(History::ENTITYTYPE_WEBSTREAM, History::ACTIONTYPE_WEBSTREAM_EDIT, $model->Id, $actionDetails,$model->email,$model->username);
					if($history->save()){
						$this->redirect(array('view','id'=>$model->Id));
					}
				}else{
					$this->redirect(array('view','id'=>$model->Id));
				}
			}
		}

		$this->render('update',array(
			'model'=>$model,
		));
	}

	/**
	 * Updates a particular model.
	 * If update is successful, the browser will be redirected to the 'view' page.
	 */
	public function actionChangeStatus()
	{
		$model=$this->loadModel();

		if(isset($_POST['WebStream']))
		{
			$oldStreamStatusCode = $model->StreamStatusCode;
			$oldStreamStatusLabel = $model->StreamStatus->Label;

			$model->attributes=$_POST['WebStream'];
			if($oldStreamStatusCode != $model->StreamStatusCode){
				$model->username=Yii::app()->user->name;
				
				$profile = YumProfile::model()->find('user_id = ' . Yii::app()->user->id);
				$model->email=$profile->email;
				if($model->save()){
					$newStreamStatus=StreamStatus::model()->findbyPk($model->StreamStatusCode);

					$actionDetails = $oldStreamStatusLabel.' => '.$newStreamStatus->Label;
					$history = History::createNew(History::ENTITYTYPE_WEBSTREAM, History::ACTIONTYPE_WEBSTREAM_CHANGESTATUS, $model->Id, $actionDetails,$model->email,$model->username);
					if($history->save()){
						$this->redirect(array('view','id'=>$model->Id));
					}
				}
			}else{
				$this->redirect(array('view','id'=>$model->Id));
			}
		}

		$this->render('changestatus',array(
			'model'=>$model,
		));
	}

	/**
	 * Deletes a particular model.
	 * If deletion is successful, the browser will be redirected to the 'index' page.
	 */
	public function actionDelete()
	{
		if(Yii::app()->request->isPostRequest)
		{
			// we only allow deletion via POST request
			$this->loadModel()->delete();

			// if AJAX request (triggered by deletion via admin grid view), we should not redirect the browser
			if(!isset($_GET['ajax']))
				$this->redirect(array('index'));
		}
		else
			throw new CHttpException(400,'Invalid request. Please do not repeat this request again.');
	}

	/**
	 * Lists all models.
	 */
	public function actionIndex()
	{
		$modelSearchForm = new WebStreamSearchForm;

		// collect user input data
		if(isset($_GET['WebStreamSearchForm']))
		{
			$modelSearchForm->attributes=$_GET['WebStreamSearchForm'];
		}
		
		$conditions = "";
		$params = array();
		$playlist_params = array();

		// Security
		$conditions .= " StreamStatusCode NOT IN (SELECT Code FROM wtvmT_StreamStatus WHERE Searchable=0) "; // Remove forbidden link

		if(isset($modelSearchForm->Name)){
			$conditions .= " AND Name LIKE :WebStreamName";
			$params[':WebStreamName'] = '%'.$modelSearchForm->Name.'%';
			if($modelSearchForm->Name != ""){
				$playlist_params["name"] = $modelSearchForm->Name;
			}
		}
		if(isset($modelSearchForm->Type)){
			if($modelSearchForm->Type != ""){
				$conditions .= " AND TypeStream=:WebStreamType";
				$params[':WebStreamType'] = $modelSearchForm->Type;
				$playlist_params["type"] = $modelSearchForm->Type;
			}
		}
		if(isset($modelSearchForm->Status)){
			if($modelSearchForm->Status != ""){
				$conditions .= " AND StreamStatusCode=:WebStreamStatus";
				$params[':WebStreamStatus'] = $modelSearchForm->Status;
				$playlist_params["status"] = $modelSearchForm->Status;
			}
		}
		if(isset($modelSearchForm->Country)){
			if($modelSearchForm->Country != ""){
				$conditions .= " AND CountryCode=:WebCountryCode";
				$params[':WebCountryCode'] = $modelSearchForm->Country;
				$playlist_params["cntcode"] = $modelSearchForm->Country;
			}
		}
		
		if(isset($modelSearchForm->Language)){
			if($modelSearchForm->Language != ""){
				$conditions .= " AND LangCode=:WebLangCode";
				$params[':WebLangCode'] = $modelSearchForm->Language;
				$playlist_params["lngcode"] = $modelSearchForm->Language;
			}
		}

		if(isset($modelSearchForm->Restriction)){
			if($modelSearchForm->Restriction != ""){
				$conditions .= " AND RestrictionCode=:WebRestrictionCode";
				$params[':WebRestrictionCode'] = $modelSearchForm->Restriction;
				$playlist_params["rstcode"] = $modelSearchForm->Restriction;
			}
		}

		$dataProvider=new CActiveDataProvider('WebStream',array(
			'criteria'=>array(
				'condition'=>$conditions,
				'params'=>$params,
				'order'=>'Name',
			),
			'pagination'=>array(
				'pageSize'=>20,
			),
		));

		$this->render('index',array(
			'modelSearchForm'=>$modelSearchForm,
			'dataProvider'=>$dataProvider,
			'playlist_params'=>$playlist_params,
		));
	}
	/**
	 * Displays the Send page
	 */
	 
	public function actionWebStreamSend()
	{
		$modelSendForm = new WebStreamSendForm;

		// collect user input data
		if(isset($_GET['WebStreamSendForm']))
		{
			$modelSendForm->attributes=$_GET['WebStreamSendForm'];
			if($modelSendForm->validate()){
				$this->redirect(array("WebStream/send", "WebStreamUrl" => $modelSendForm->Url));
			}
		}
		
		$this->render('WebStreamSend', array(
			'modelSendForm'=>$modelSendForm,
		));
	}

	/**
	 * Displays the Search page
	 */
	public function actionWebStreamSearch()
	{
		$modelSearchForm = new WebStreamSearchForm;

		// collect user input data
		if(isset($_GET['WebStreamSearchForm']))
		{
			$modelSearchForm->attributes=$_GET['WebStreamSearchForm'];
			if($modelSearchForm->validate()){
				$this->redirect($this->createUrl("WebStream/index", array('WebStreamSearchForm' => $modelSearchForm->attributes)));
			}
		}
		
		$this->render('WebStreamSearch', array(
			'modelSearchForm'=>$modelSearchForm,	
		));
	}

	/**
	 * Send a new URL.
	 */
	public function actionSend()
	{
		$model=new WebStream;
		

		if(isset($_POST['WebStream']))
		{
			$model->attributes=$_POST['WebStream'];
			if($model->LangCode==""){
				$model->LangCode=null;
			}
			if(!Yii::app()->user->isGuest){
				$model->username=Yii::app()->user->name;
				$profile = YumProfile::model()->find('user_id = ' . Yii::app()->user->id);
				$model->email=$profile->email;
			}
			
			if(Yii::app()->user->isGuest){
				if($model->username==""){
					$model->username=null;
				}
				if($model->email==""){
					$model->email=null;
				}
			}
			if($model->CountryCode==""){
				$model->CountryCode=null;
			}
			if($model->RequiredIsp==""){
				$model->RequiredIsp=null;
			}
			
			if($model->RestrictionCode==""){
				$model->RestrictionCode=null;
				$model->RestrictionValue=null;
			}
			if($model->RestrictionValue==""){
				$model->RestrictionCode=null;
				$model->RestrictionValue=null;
			}
			
			if($model->save()){
				if($model->RestrictionCode!=null){
					$restriction = Restriction::createNewValue($model->Id, $model->RestrictionCode, $model->RestrictionValue);
					$restriction->save();
				}
				$history = History::createNew(History::ENTITYTYPE_WEBSTREAM, History::ACTIONTYPE_WEBSTREAM_ADD, $model->Id,'NEW STREAM',$model->email,$model->username);
				if($history->save()){

					// Send a mail to the admin
					$link = Yii::app()->getRequest()->getHostInfo().Yii::app()->createUrl("WebStream/view", array("id" => $model->Id));
					$subject = Yii::app()->name.' - New WebStream submitted';
					$message = 'A new WebStream has been submitted by an user :<br><br>';
					$message .= '<u>Name :</u> '.$model->Name.'<br>';
					$message .= '<u>Url :</u> <a href="'.$model->Url.'">'.$model->Url.'</a><br>';
					$message .= '<br>';
					$message .= 'Click here to view the details of the WebStream : <a href="'.$link.'">'.$link.'</a><br>';

					$this->sendMailToAdmin($subject, $message);

					$this->redirect(array('view','id'=>$model->Id));
				}
			}
		}else{
			$model->Url = $_GET['WebStreamUrl'];
		}

		$conditions = "";
		$params = array();
		$params[':WebStreamUrl'] = $model->Url;
		$conditions = "Url=:WebStreamUrl";

		$dataProvider=new CActiveDataProvider('WebStream',array(
			'criteria'=>array(
				'condition'=>$conditions,
				'params'=>$params,
				'order'=>'Name DESC',
			),
			'pagination'=>array(
				'pageSize'=>20,
			),
		));

		$this->render('send',array(
			'dataProvider'=>$dataProvider,
			'model'=>$model
		));
	}

	/**
	 * Manages all models.
	 */
	public function actionAdmin()
	{
		$model=new WebStream('search');
		$model->unsetAttributes();  // clear any default values
		if(isset($_GET['WebStream']))
			$model->attributes=$_GET['WebStream'];

		$this->render('admin',array(
			'model'=>$model,
		));
	}

	/**
	 * Returns the data model based on the primary key given in the GET variable.
	 * If the data model is not found, an HTTP exception will be raised.
	 */
	public function loadModel()
	{
		if($this->_model===null)
		{
			if(isset($_GET['id']))
				$this->_model=WebStream::model()->findbyPk($_GET['id']);
			if($this->_model===null)
				throw new CHttpException(404,'The requested page does not exist.');
		}
		return $this->_model;
	}

	/**
	 * Performs the AJAX validation.
	 * @param CModel the model to be validated
	 */
	protected function performAjaxValidation($model)
	{
		if(isset($_POST['ajax']) && $_POST['ajax']==='web-stream-form')
		{
			echo CActiveForm::validate($model);
			Yii::app()->end();
		}
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
