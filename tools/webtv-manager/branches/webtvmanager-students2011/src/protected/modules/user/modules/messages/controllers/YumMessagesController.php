<?php

class YumMessagesController extends YumController
{
	private $_model;
	
	public function accessRules()
	{
		return array(
			array('allow',
				'users'=>array('@')
			),
			#deny all other users
			array('deny',
				'users'=>array('*')
			)
		);
	}	

	public function actionView()
	{
		$model = $this->loadModel();
		if(!$model->message_read) {
			$model->message_read = true;
			$model->save();
		}

		$this->render('view',array('model'=>$model));
	}

	public function actionCompose()
	{
		$model=new YumMessages;

	  	$this->performAjaxValidation($model);

		if(isset($_POST['YumMessages']))
		{			
			foreach($_POST['SendTo'] as $user_id) {
				$model = new YumMessages;
				$model->attributes=$_POST['YumMessages'];
				$model->to_user_id = $user_id;
				$model->save();
			}
			$this->redirect(array('success'));
		}

		if(isset($_GET['to_user_id']))
			$model->to_user_id = $_GET['to_user_id'];
		$this->render('compose',array(
			'model'=>$model,
		));
	}

	public function actionSuccess() 
	{
		$this->render('success');
	}

	public function actionUpdate()
	{
		$model=$this->loadModel();

	 $this->performAjaxValidation($model);

		if(isset($_POST['YumMessages']))
		{
			$model->attributes=$_POST['YumMessages'];
			if($model->save())
				$this->redirect(array('view','id'=>$model->id));
		}

		$this->render('update',array(
			'model'=>$model,
		));
	}

	public function actionDelete()
	{
		if(Yii::app()->request->isPostRequest)
		{
			$this->loadModel()->delete();
			if(!isset($_POST['ajax']))
				$this->redirect(array('index'));
		}
		else
			throw new CHttpException(400,'Invalid request. Please do not repeat this request again.');
	}

	public function actionIndex()
	{
		$uid = Yii::app()->user->id;
		$this->render('index',array(
			//'models'=>YumMessages::model()->findAll('to = :uid', array(':uid' => $uid)),
			'dataProvider'=>new CActiveDataProvider('YumMessages', array(
				'criteria' => array('condition' => 'to_user_id = '. $uid
		)))));
	}


	/**
	 * @return YumMessage
	 */
	public function loadModel()
	{
		if($this->_model===null)
		{
			if(isset($_GET['id']))
				$this->_model=YumMessages::model()->findbyPk($_GET['id']);
			if($this->_model===null)
				throw new CHttpException(404,Yii::t('App', 'The requested page does not exist.'));
		}
		return $this->_model;
	}

	protected function performAjaxValidation($model)
	{
		if(isset($_POST['ajax']) && $_POST['ajax']==='yum-messages-form')
		{
			echo CActiveForm::validate($model);
			Yii::app()->end();
		}
	}
}
