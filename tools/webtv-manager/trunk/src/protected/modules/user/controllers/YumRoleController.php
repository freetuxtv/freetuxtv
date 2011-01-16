<?php

class YumRoleController extends YumController {
	public function accessRules() {
		return array(
				array('allow',
					'actions'=>array('index', 'admin','delete','create','update', 'view'),
					'expression' => 'Yii::app()->user->isAdmin()'
					),
				array('deny',  // deny all other users
						'users'=>array('*'),
						),
				);
	}

	public function actionView() {
		$this->layout = Yum::module()->adminLayout;

		$model = $this->loadModel();

		$assignedUsers = new CActiveDataProvider('YumUser', array(
					'criteria' => array(
						'condition' => "role_id = {$model->id}",
						'join' => 'left join user_has_role on t.id = user_has_role.user_id')));

		$activeMemberships= new CActiveDataProvider('YumMembership', array(
					'criteria' => array(
						'condition' => "membership_id = {$model->id}")));

		$this->render('view',array(
					'assignedUsers' => $assignedUsers,
					'activeMemberships' => $activeMemberships,
					'model'=>$model));
	}

	public function actionCreate() 
	{	
		$this->layout = Yum::module()->adminLayout;
		$model = new YumRole();
		$this->performAjaxValidation($model, 'yum-role-form');
		if(isset($_POST['YumRole'])) {
			$model->attributes = $_POST['YumRole'];

			if(isset($_POST['YumRole']['YumUser']))
				$model->users = $_POST['YumRole']['YumUser'];
			else
				$model->users = array();

			if($model->save())
			{
			if(Yum::module()->enableLogging == true)
								{
								$user= YumUser::model()->findbyPK(Yii::app()->user->id);
								YumActivityController::logActivity($user, 'role_created');
								}
				$this->redirect(array('admin'));
			}

		}
		$this->render('create', array('model' => $model));
	}

	public function actionUpdate()
	{
		$this->layout = Yum::module()->adminLayout;
		$model = $this->loadModel();

	 $this->performAjaxValidation($model, 'yum-role-form');

		if(isset($_POST['YumRole'])) {
			$model->attributes = $_POST['YumRole'];

			if(isset($_POST['YumRole']['YumUser']))
				$model->users = $_POST['YumRole']['YumUser'];
			else
				$model->users = array();

			if($model->validate() && $model->save())
			{
				if(Yum::module()->enableLogging == true)
								{
								$user= YumUser::model()->findbyPK(Yii::app()->user->id);
								YumActivityController::logActivity($user, 'role_updated');
								}
				$this->redirect(array('view','id'=>$model->id));
		}
	}

		$this->render('update',array(
			'model'=>$model,
		));
	}

	public function actionAdmin() 
	{
		$this->layout = Yum::module()->adminLayout;
		$dataProvider=new CActiveDataProvider('YumRole', array(
			'pagination'=>array(
				'pageSize'=>20,
			),
		));

		$this->render('admin',array(
			'dataProvider'=>$dataProvider,
		));

	}

	public function actionDelete()
	{
		$this->layout = Yum::module()->adminLayout;	
		if(Yii::app()->request->isPostRequest)
		{
			if(Yum::module()->enableLogging == true)
			{
				$user= YumUser::model()->findbyPK(Yii::app()->user->id);
				YumActivityController::logActivity($user, 'role_removed');
			}
			$this->loadModel()->delete();

			if(!isset($_POST['ajax']))
				$this->redirect(array('index'));
		}
		else
			throw new CHttpException(400,'Invalid request. Please do not repeat this request again.');
	}

	public function actionIndex()
	{
		$this->actionAdmin();
	}


}
