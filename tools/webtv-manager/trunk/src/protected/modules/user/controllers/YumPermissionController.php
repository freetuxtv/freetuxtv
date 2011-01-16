<?php

class YumPermissionController extends YumController
{
	public $defaultAction = 'admin';

	public function accessRules()
	{
		return array(
				array('allow',
					'actions'=>array('admin', 'create', 'index'),
					'users'=>array('admin')
					),
				array('deny',  // deny all other users
					'users'=>array('*'),
					),
				);
	}

	public function actionIndex() {
		$this->render('view', array(
					'actions' => YumAction::model()->findAll()));
	}

	public function actionAdmin()
	{
		$model=new YumPermission('search');
		$model->unsetAttributes();  

		if(isset($_GET['YumPermission']))
			$model->attributes=$_GET['YumPermission'];

		$this->render('admin',array(
					'model'=>$model,
					));
	}



	public function actionCreate() {
		$model = new YumPermission;

		$this->performAjaxValidation($model, 'permission-create-form');

		if(isset($_POST['YumPermission'])) {
			$model->attributes=$_POST['YumPermission'];
			if($model->validate()) {
				if($_POST['YumPermission']['type'] == 'user')  {
					$model->subordinate = $_POST['YumPermission']['subordinate_id'];
					$model->principal = $_POST['YumPermission']['principal_id'];
				} else if($_POST['YumPermission']['type'] == 'role')  {
					$model->subordinate_role = $_POST['YumPermission']['subordinate_id'];
					$model->principal_role = $_POST['YumPermission']['principal_id'];
				}
				if($model->save())
					$this->redirect(array('admin'));
				return;
			}
		}
		$model->type = 'user'; // preselect 'user'
		$this->render('create',array('model'=>$model));

	}

}
