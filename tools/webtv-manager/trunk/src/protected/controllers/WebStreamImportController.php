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
		$webStreamList = array();
		$modelWebStreamData = new WebStream;

		$showAll = true;

		// collect user input data
		if(isset($_GET['WebStreamImportForm']))
		{
			$modelImportForm->attributes=$_GET['WebStreamImportForm'];

			$showAll = !$modelImportForm->OnlyNewLinks;

			$XMLData = $this->getRemoteFileContents($modelImportForm->UrlWSLFEData);
			$webStreamListFromXml = $this->parseWSLEF($XMLData);

			 $i = 0;

			foreach($webStreamListFromXml as $webstream) {
				$attrs = array("Url" => $webstream['URL']);
				$model = WebStream::model()->findByAttributes($attrs);

				$i++;

				$webstream['id'] = $i;
				
				if($model != null){
					$webstream['WebStream'] = $model;
				}
				
				if($showAll || $model == null){
					$webStreamList[] = $webstream;
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

		$setStatus = false;
		if(Yii::app()->user->checkAccess('changeStatusWebStream')) {
			$setStatus = true;
		}

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

			$bError = false;
			$countTotalError = -1;
			$countTotalSucceed = 0;

			foreach($_POST['Import'] as $webstream) {
				if(isset($webstream["Edit"])){

					if($countTotalError == -1){
						$countTotalError = 0;
					}

					$model = new WebStream;
					$model->attributes = $_POST['WebStream'];
					$model->Url = $webstream["URL"];
					$model->Name = $webstream["Name"];
					if(!$setStatus){
						$model->StreamStatusCode = WebStream::WEBSTREAM_STATUS_SUBMITTED;
					}

					if($model->LangCode==""){
						$model->LangCode=null;
					}
					if($model->CountryCode==""){
						$model->CountryCode=null;
					}
					if($model->save()){
						$countTotalSucceed++;

						$history = History::createNew(History::ENTITYTYPE_WEBSTREAM, History::ACTIONTYPE_WEBSTREAM_IMPORT, $model->Id);
						if($history->save()){

							// Send a mail to the admin
							$link = Yii::app()->getRequest()->getHostInfo().Yii::app()->createUrl("WebStream/view", array("id" => $model->Id));

							$message .= '<u>Import n°:</u> '.$countTotalSucceed.'<br>';
							$message .= '<u>Name:</u> '.$model->Name.'<br>';
							$message .= '<u>Url:</u> <a href="'.$model->Url.'">'.$model->Url.'</a><br>';
							$message .= 'Click here to view the details of the WebStream : <a href="'.$link.'">'.$link.'</a><br><br>';
						}else{
							$countTotalError++;
						}
					}else{
						$countTotalError++;
					}
				}
			}

			if($countTotalSucceed>0){
				$this->sendMailToAdmin($subject, $message);
			}

			$this->redirect(array('index', 'WebStreamImportForm[UrlWSLFEData]'=>$modelImportForm->UrlWSLFEData, 'error'=>$countTotalError, 'success'=>$countTotalSucceed));
		} else
			throw new CHttpException(400,'Invalid request. Please do not repeat this request again.');
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

	public function getRemoteFileContents($url, $timeout = 10, $localFileName = '', $post = NULL)
	{
		$ret = FALSE;
		if (extension_loaded('curl')) {
		    $ch = curl_init($url);
		    curl_setopt($ch, CURLOPT_TIMEOUT, $timeout);
		    curl_setopt($ch, CURLOPT_CONNECTTIMEOUT, $timeout);
		    if ($localFileName) {
		        $fp = fopen($localFileName, 'w') or die("File '$localFileName' cannot be openned in write mode");
		        curl_setopt($ch, CURLOPT_FILE, $fp);
		    } else {
		        curl_setopt($ch, CURLOPT_RETURNTRANSFER, TRUE);
		    }
		    if ($post) {
		        curl_setopt($ch, CURLOPT_POST, TRUE);
		        curl_setopt($ch, CURLOPT_POSTFIELDS, $post);
		    }
		    $ret = curl_exec($ch);
		    if ($localFileName) {
		        fclose($fp);
		    }
		    if ($ret === FALSE) {
		        die("Encountered error : " . curl_error($ch));
		    }
		    curl_close($ch);
		    return $ret;
		} elseif (ini_get('allow_url_fopen') && in_array(parse_url($url, PHP_URL_SCHEME), stream_get_wrappers())) {
		    $infos = parse_url($url);
		    $contexte = NULL;
		    if (isset($infos['user']) || $post) {
		        $options = array(
		            'http' => array(
		                'method' => $post ? 'POST' : 'GET',
		                'header' => '',
		                'timeout' => $timeout // Effectif pour les versions 5.2.1 et supérieures
		            )
		        );
		        if (isset($infos['user'])) {
		            $options['http']['header'] .= 'Authorization: Basic ' . base64_encode($infos['user'] . ':' . $infos['pass']) . "\r\n";
		        }
		        if ($post) {
		            $data = http_build_query($post);
		            $options['http']['header'] .= "Content-type: application/x-www-form-urlencoded\r\n";
		            $options['http']['header'] .= 'Content-Length: ' . strlen($data) . "\r\n";
		            $options['http']['content'] = $data;
		        }
		        $contexte = stream_context_create($options);
		    }
		    if (!$localFileName) {
		        return file_get_contents($url, FALSE, $contexte);
		    } else {
		        $in = fopen($url, 'r', FALSE, $contexte);
		        stream_set_timeout($in, $timeout);
		        $out = fopen($localFileName, 'w') or die("File '$localFileName' cannot be openned in write mode");
		        while (!feof($in)) {
		            fwrite($out, fread($in, 1024));
		        }
		        fclose($in);
		        fclose($out);
		    }
		} elseif (function_exists('fsockopen')) {
		    $infos = parse_url($url);
		    if ($localFileName) {
		        $out = fopen($localFileName, 'w') or die("File '$localFileName' cannot be openned in write mode");
		    }
		    if (empty($infos['path'])) {
		        $infos['path'] = '/';
		    }
		    if (!isset($infos['port'])) {
		        if ($infos['scheme'] == 'http') {
		            $infos['port'] = 80;
		        } elseif ($infos['scheme'] == 'https') {
		            $infos['port'] = 443;
		        }
		    }
		    @ $fp = fsockopen($infos['host'] == 'https' ? 'ssl://' . $infos['host'] : $infos['host'], $infos['port'], $errno, $errstr, $timeout);
		    if (!$fp) {
		        return FALSE;
		    }
		    stream_set_timeout($fp, $timeout);
		    $requete = ($post ? 'POST ' : 'GET ') . $infos['path'] . '?'  . (isset($infos['query']) ? $infos['query'] : '') . " HTTP/1.1\r\n";
		    $requete .= 'Host: ' . $infos['host'] . ':' . (isset($infos['port']) ? $infos['port'] : '80') . "\r\n";
		    if (isset($infos['user'])) {
		        $requete .= 'Authorization: Basic ' . base64_encode($infos['user'] . ':' . $infos['pass']) . "\r\n";
		    }
		    if ($post) {
		        $data = http_build_query($post);
		        $requete .= "Content-type: application/x-www-form-urlencoded\r\n";
		        $requete .= 'Content-Length: ' . strlen($data) . "\r\n";
		    }
		    fwrite($fp, $requete . "\r\n" . $data);
		    $buffer = '';
		    do { // Saut de l'entête HTTP
		        $buffer .= fgets($fp, 1024);
		    } while (!feof($fp) && strpos($buffer, "\r\n\r\n") === FALSE);
		    $buffer = '';
		    while (!feof($fp)) {
		        $buffer .= fread($fp, 1024);
		    }
		    $buffer = ltrim($buffer);
		    $pos = strpos($buffer, "\r\n");
		    $len = hexdec(substr($buffer, 0, $pos));
		    $buffer = substr($buffer, $pos + strlen("\r\n"), $len);
		    if ($localFileName) {
		        fwrite($out, $buffer);
		        fclose($out);
		        $buffer = TRUE;
		    }
		    fclose($fp);
		    return $buffer;
		} else {
		    die("No medium to download the page is availiable");
		}

		return $ret;
	}
}
