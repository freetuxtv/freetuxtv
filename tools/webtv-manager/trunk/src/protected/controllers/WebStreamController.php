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
				'actions'=>array('index','view'),
				'users'=>array('*'),
			),
			array('allow',
				'actions'=>array('send', 'editrequest'),
				'roles'=>array('sendWebStream'),
			),
			array('allow',
				'actions'=>array('update', 'updateEditRequest'),
				'roles'=>array('editWebStream'),
			),
			array('allow',
				'actions'=>array('changestatus'),
				'roles'=>array('changeStatusWebStream'),
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
		
		$dataHistory=new CArrayDataProvider($model->Histories, array('keyField'=>'Id'));
		$dataEditRequests=new CArrayDataProvider($model->EditRequests, array('keyField'=>'Id'));

		$this->render('view', array(
			'model'=>$model,
			'dataHistory'=>$dataHistory,
			'dataEditRequests'=>$dataEditRequests,
		));
	}

	public function updateWebStream($model, $saveModel, $action, $actionDetails, $editRequest, $comment)
	{
		if(!$saveModel || $model->save()){
			$history = History::createNew(History::ENTITYTYPE_WEBSTREAM, $action, $model->Id, $actionDetails);
			if($history->save()){
				if($editRequest != null){
					if($editRequest->HistoryId == null){
						$editRequest->HistoryId = $history->Id;
					}else if($editRequest->UpdateHistoryId == null){
						$editRequest->UpdateHistoryId = $history->Id;
					}
					if(!$editRequest->save()){
						throw new CException("Error when saving the edit request");
					}
				}

				if($comment != null){
					$comment->HistoryId = $history->Id;
					if(!$comment->save()){
						throw new CException("Error when saving the comment");
					}
				}
			}else{
				throw new CException("Error when saving the history");
			}
		}else{
			throw new CException("Error when saving the history");
		}
		return true;
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
			if($model->CountryCode==""){
				$model->CountryCode=null;
			}
			if($model->RequiredIsp==""){
				$model->RequiredIsp=null;
			}

			if($model->save()){
				if($actionDetails != ""){
					$history = History::createNew(History::ENTITYTYPE_WEBSTREAM, History::ACTIONTYPE_WEBSTREAM_EDIT, $model->Id, $actionDetails);
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
	public function actionEditRequest()
	{
		$model=$this->loadModel();

		$action = null;

		if(isset($_POST['Action']))
		{
			$action = $_POST['Action'];
			$modelChange = array();
			$comment = null;
			$editRequest = new EditRequest;

			$editRequest->Field = EditRequest::FIELD_WEBSTREAM_STATUS;

			if($action == 'ReportDeadLink'){
				$modelChange['StreamStatusCode'] = StreamStatus::STREAM_STATUS_DEAD;
				$editRequest->OldValue = $model->StreamStatusCode;
				$editRequest->NewValue = StreamStatus::STREAM_STATUS_DEAD;
			}
			$actionDetails = $model->getModelDiffMsg($modelChange);

			if(isset($_POST['Comments']) && $_POST['Comments'] != ""){
				$comment = new Comment;
				$comment->Comment = $_POST['Comments'];
			}

			$transaction=$model->dbConnection->beginTransaction();
			try
			{
				$history = History::createNew(History::ENTITYTYPE_WEBSTREAM, History::ACTIONTYPE_WEBSTREAM_EDITREQUEST, $model->Id, $actionDetails);
				if($history->save()){
					$editRequest->HistoryId = $history->Id;
					if(!$editRequest->save()){
						throw new CException("Error when saving the edit request");
					}

					if($comment != null){
						$comment->HistoryId = $history->Id;
						if(!$comment->save()){
							throw new CException("Error when saving the comment");
						}
					}
				}else{
					throw new CException("Error when saving the history");
				}
				$transaction->commit();

				// Send a link to admin
				$link = Yii::app()->getRequest()->getHostInfo().Yii::app()->createUrl("WebStream/view", array("id" => $model->Id));
				$subject = Yii::app()->name.' - New WebStream change request';
				$message = 'A new WebStream change request has been submitted by an user :<br><br>';
				$message .= '<u>Name :</u> '.$model->Name.'<br>';
				$message .= '<u>Url :</u> <a href="'.$model->Url.'">'.$model->Url.'</a><br>';
				$message .= '<br>';
				$message .= 'Click here to view the details of the request : <a href="'.$link.'">'.$link.'</a><br>';

				$this->sendMailToAdmin($subject, $message);

				$this->redirect(array('view','id'=>$model->Id));

			} catch(Exception $e) {
				$transaction->rollBack();
				throw new CHttpException(400, $e->getMessage());
			}
		}
		else
			throw new CHttpException(400,'Invalid request. Please do not repeat this request again.');
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
				
				$transaction=$model->dbConnection->beginTransaction();
				try
				{
					$newStreamStatus=StreamStatus::model()->findbyPk($model->StreamStatusCode);
					$actionDetails = $oldStreamStatusLabel.' => '.$newStreamStatus->Label;
					$res = $this->updateWebStream($model, true, History::ACTIONTYPE_WEBSTREAM_CHANGESTATUS, $actionDetails, null, null);
					if($res){
						$transaction->commit();
					}else{
						throw new CException("Error when saving the web stream");
					}
				} catch(Exception $e) {
					$transaction->rollBack();
					throw new CHttpException(400, $e->getMessage());
				}
			}
			$this->redirect(array('view','id'=>$model->Id));
		}

		$this->render('changestatus',array(
			'model'=>$model,
		));
	}

	public function actionUpdateEditRequest()
	{
		if(isset($_GET["edit_id"]) && isset($_GET["status"])){
			$model=$this->loadModel();
			$modelEditRequest=EditRequest::model()->findbyPk($_GET["edit_id"]);

			$transaction=$model->dbConnection->beginTransaction();
			try
			{
				$update = false;
				$action = History::ACTIONTYPE_WEBSTREAM_EDITREQUESTREJECT;
				if($_GET["status"] == "accept"){
					$modelEditRequest->Status = EditRequest::FIELD_STATUS_APPROVED;
					$update=true;
				}else if($_GET["status"] == "reject"){
					$modelEditRequest->Status = EditRequest::FIELD_STATUS_REJECTED;
				}else{
					throw new CException("Invalid request");
				}

				if($update){
					switch($modelEditRequest->Field){
					case EditRequest::FIELD_WEBSTREAM_STATUS:
						$modelChange['StreamStatusCode'] = $modelEditRequest->NewValue;
						$action = History::ACTIONTYPE_WEBSTREAM_CHANGESTATUS;
						break;
					default:
						throw new CException("Invalid request");
					}
		
					$actionDetails = $model->getModelDiffMsg($modelChange);
					$model->attributes=$modelChange;
				}else{
					$actionDetails = "";
				}

				$res = $this->updateWebStream($model, $update, $action, $actionDetails, $modelEditRequest, null);
				if($res){
					$transaction->commit();
				}else{
					throw new CException("Error when saving the web stream");
				}

				$this->redirect(array('view','id'=>$model->Id));
			} catch(Exception $e) {
				$transaction->rollBack();
				throw new CHttpException(400, $e->getMessage());
			}
				
		}else
			throw new CHttpException(400,'Invalid request. Please do not repeat this request again.');
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
		
		$distinct = false;
		$conditions = "";
		$join = "";
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
		if(isset($modelSearchForm->Language)){
			if($modelSearchForm->Language != ""){
				if($modelSearchForm->Language == "none"){
					$conditions .= " AND LangCode IS NULL";
					$playlist_params["lng"] = $modelSearchForm->Language;
				}else{
					$conditions .= " AND LangCode=:WebStreamLang";
					$params[':WebStreamLang'] = $modelSearchForm->Language;
					$playlist_params["lng"] = $modelSearchForm->Language;
				}
			}
		}

		if(isset($modelSearchForm->Country)){
			if($modelSearchForm->Country != ""){
				if($modelSearchForm->Country == "none"){
					$conditions .= " AND CountryCode IS NULL";
					$playlist_params["country"] = $modelSearchForm->Country;
				}else{
					$conditions .= " AND CountryCode=:WebStreamCountryCode";
					$params[':WebStreamCountryCode'] = $modelSearchForm->Country;
					$playlist_params["country"] = $modelSearchForm->Country;
				}
			}
		}

		if(isset($modelSearchForm->RequiredISP)){
			if($modelSearchForm->RequiredISP != ""){
				if($modelSearchForm->RequiredISP == "all"){
					$playlist_params["isp"] = $modelSearchForm->RequiredISP;
				}else{
					$conditions .= " AND RequiredISP=:WebStreamRequiredISP";
					$params[':WebStreamRequiredISP'] = $modelSearchForm->RequiredISP;
					$playlist_params["isp"] = $modelSearchForm->RequiredISP;
				}
			}else{
				$conditions .= " AND RequiredISP IS NULL";
				$playlist_params["isp"] = $modelSearchForm->RequiredISP;
			}
		}

		if(isset($modelSearchForm->EditPending)){
			if($modelSearchForm->EditPending != ""){
				if($modelSearchForm->EditPending == true){
					$join='INNER JOIN wtvmT_History ON wtvmT_History.EntityType='.History::ENTITYTYPE_WEBSTREAM.' AND WebStream.Id=wtvmT_History.EntityId
						INNER JOIN wtvmT_EditRequest ON wtvmT_History.Id=wtvmT_EditRequest.HistoryId AND Status='.EditRequest::FIELD_STATUS_SUBMITTED;
					$distinct = true;
				}
			}
		}

		$dataProvider=new CActiveDataProvider('WebStream',array(
			'criteria'=>array(
				'alias'=>"WebStream",
				'distinct' => $distinct,
				'condition'=>$conditions,
				'join'=>$join,
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
	 * Send a new URL.
	 */
	public function actionSend()
	{
		$bRes = false;
		$model=new WebStream;
		$modelSendForm = new WebStreamSendForm;

		if(isset($_POST['WebStream']))
		{
			$model->attributes=$_POST['WebStream'];
			if($model->LangCode==""){
				$model->LangCode=null;
			}
			if($model->CountryCode==""){
				$model->CountryCode=null;
			}
			if($model->RequiredIsp==""){
				$model->RequiredIsp=null;
			}
			$modelSendForm->Url = $model->Url;
			if($model->save()){
				$history = History::createNew(History::ENTITYTYPE_WEBSTREAM, History::ACTIONTYPE_WEBSTREAM_ADD, $model->Id);
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
					$bRes = true;
				}
			}
		}else{
			$modelSendForm->Url = $_GET['WebStreamUrl'];
			$model->Url = $_GET['WebStreamUrl'];
		}
		
		if(!$bRes){
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
				'model'=>$model,
				'modelSendForm'=>$modelSendForm,
			));
		}
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
