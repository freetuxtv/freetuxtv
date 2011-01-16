<?php

class YumFieldsGroupController extends YumController {
	/**
	 * Specifies the access control rules.
	 * This method is used by the 'accessControl' filter.
	 * @return array access control rules
	 */
	public function accessRules()
	{
		return array(
			array('allow', 
				'actions'=>array('create','update','view','admin','delete'),
				'users'=>array(Yii::app()->user->name ),
                'expression' => 'Yii::app()->user->isAdmin()'
			),
			array('deny',  // deny all users
				'users'=>array('*'),
			),
		);
	}

	/**
	 * Displays a particular model.
	 */
	public function actionView() {
		$this->layout = Yum::module()->adminLayout;
		$this->render('view',array(
			'model'=>$this->loadModel(),
		));
	}

	/**
	 * Creates a new model.
	 * If creation is successful, the browser will be redirected to the 'view' page.
	 */
	public function actionCreate() {
		$this->layout = Yum::module()->adminLayout;
		$model=new YumProfileFieldsGroup;

		$this->performAjaxValidation($model, 'yum-profile-fields-form');

		if(isset($_POST['YumProfileFieldsGroup']))
		{
			$model->attributes=$_POST['YumProfileFieldsGroup'];
			if($model->save())
			{
				if(Yum::module()->enableLogging == true)
								{
								$user= YumUser::model()->findbyPK(Yii::app()->user->id);
								YumActivityController::logActivity($user, 'field_group_created');
								}
				$this->redirect(array('view','id'=>$model->id));
		}}

		$this->render('create',array(
			'model'=>$model,
		));
	}

	/**
	 * Updates a particular model.
	 * If update is successful, the browser will be redirected to the 'view' page.
	 */
	public function actionUpdate()
	{
		$this->layout = Yum::module()->adminLayout;
		$model=$this->loadModel();

		// Uncomment the following line if AJAX validation is needed
		$this->performAjaxValidation($model, 'yum-profile-fields-form');

		if(isset($_POST['YumProfileFieldsGroup']))
		{
			$model->attributes=$_POST['YumProfileFieldsGroup'];
			if($model->save())
			{
				if(Yum::module()->enableLogging == true)
								{
								$user= YumUser::model()->findbyPK(Yii::app()->user->id);
								YumActivityController::logActivity($user, 'field_group_updated');
								}
				$this->redirect(array('view','id'=>$model->id));
		}}

		$this->render('update',array(
			'model'=>$model,
		));
	}

	/**
	 * Deletes a particular model.
	 * If deletion is successful, the browser will be redirected to the 'index' page.
	 */
	public function actionDelete()
	{
		$this->layout = Yum::module()->adminLayout;
		if(Yii::app()->request->isPostRequest)
		{
			// we only allow deletion via POST request
			if(Yum::module()->enableLogging == true)
								{
								$user= YumUser::model()->findbyPK(Yii::app()->user->id);
								YumActivityController::logActivity($user, 'field_group_removed');
								}
			$this->loadModel()->delete();

			// if AJAX request (triggered by deletion via admin grid view), we should not redirect the browser
			if(!isset($_GET['ajax']))
				$this->redirect(array('index'));
		}
		else
			throw new CHttpException(400,'Invalid request. Please do not repeat this request again.');
	}

	/**
	 * Manage fields Groups.
	 */
	public function actionAdmin()
	{
		$this->layout = Yum::module()->adminLayout;

		$model = new YumProfileFieldsGroup('search');
		if(isset($_GET['YumProfileFieldsGroup']))
			$model->attributes=$_GET['YumProfileFieldsGroup'];

		$this->render('admin', array( 'model'=>$model ));
	}

}
