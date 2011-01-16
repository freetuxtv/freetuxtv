<?php

class YumUsergroupController extends YumController {
	public function accessRules() {
		return array(
				array('allow',  
					'actions'=>array('index','view'),
					'users'=>array('*'),
					),
				array('allow', 
					'actions'=>array('getOptions', 'create','update', 'browse', 'join'),
					'users'=>array('@'),
					),
				array('allow', 
					'actions'=>array('admin','delete'),
					'users'=>array('admin'),
					),
				array('deny', 
					'users'=>array('*'),
					),
				);
	}

	public function actionJoin() {
		if(isset($_GET['id'])) {
			$p = new YumGroupParticipation();
			$p->user_id = Yii::app()->user->id;
			$p->group_id = $_GET['id'];
			if($p->save())
				$this->redirect(array('//user/groups/view', 'id' => $_GET['id']));
		}
	}

	public function actionView() {
		$this->render('view',array(
					'model' => $this->loadModel(),
					));
	}

	public function loadModel($uid = 0)
	{
		if($this->_model === null)
		{
			if($uid != 0)
				$this->_model = YumUsergroup::model()->findByPk($uid);
			elseif(isset($_GET['id']))
				$this->_model = YumUsergroup::model()->findByPk($_GET['id']);
			if($this->_model === null)
				throw new CHttpException(404,'The requested Usergroup does not exist.');
		}
		return $this->_model;
	}


	public function actionCreate() {
		$model = new YumUsergroup;

		if(isset($_POST['YumUsergroup'])) {
			$model->attributes = $_POST['YumUsergroup'];
			$model->owner_id = Yii::app()->user->id;
			
			$model->validate();

			if(!$model->hasErrors()) {
				$model->save();
				$participant = new YumGroupParticipation();
				$participant->user_id = Yii::app()->user->id;
				$participant->group_id = $model->id;
				$participant->save();

				$this->redirect(array('view','id'=>$model->id));
			}
		}

		$this->render('create',array( 'model'=>$model));
	}


	public function actionUpdate()
	{
		$model = $this->loadModel();

		$this->performAjaxValidation($model, 'usergroup-form');

		if(isset($_POST['YumUsergroup']))
		{
			$model->attributes = $_POST['YumUsergroup'];


			if($model->save()) {

				$this->redirect(array('view','id'=>$model->id));
			}
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

			if(!isset($_GET['ajax']))
			{
				if(isset($_POST['returnUrl']))
					$this->redirect($_POST['returnUrl']); 
				else
					$this->redirect(array('admin'));
			}
		}
		else
			throw new CHttpException(400,
					Yii::t('app', 'Invalid request. Please do not repeat this request again.'));
	}

	public function actionIndex()
	{
		$dataProvider=new CActiveDataProvider('YumUsergroup', array(
				'criteria' => array(
					'condition' => 'owner_id = :owner_id',
					'params' => array(':owner_id' => Yii::app()->user->id))));


		$this->render('index',array(
					'dataProvider'=>$dataProvider,
					));
	}

	public function actionBrowse()
	{
		$model=new YumUsergroup('search');
		$model->unsetAttributes();

		if(isset($_GET['YumUsergroup']))
			$model->attributes = $_GET['YumUsergroup'];

		$this->render('browse',array(
					'model'=>$model,
					));
	}

	public function actionAdmin()
	{
		$model=new YumUsergroup('search');
		$model->unsetAttributes();

		if(isset($_GET['YumUsergroup']))
			$model->attributes = $_GET['YumUsergroup'];

		$this->render('admin',array(
					'model'=>$model,
					));
	}

}
