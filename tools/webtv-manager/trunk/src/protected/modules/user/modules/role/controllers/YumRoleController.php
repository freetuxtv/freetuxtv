<?php

class YumRoleController extends YumController
{
	private $_model;

	public function actionView()
	{
		$this->layout = YumWebModule::yum()->adminLayout;
		$model = $this->loadModel();
		$this->render('view',array('model'=>$model));
	}

	public function actionCreate() 
	{
		$this->layout = YumWebModule::yum()->adminLayout;
		$model = new YumRole();
		$this->performAjaxValidation($model);
		if(isset($_POST['YumRole'])) {
			$model->attributes = $_POST['YumRole'];
			$model->users = Relation::retrieveValues($_POST, 'YumUser');
			if($model->save())
				$this->redirect(array('admin'));

		}
		$this->render('create', array('model' => $model));
	}

	public function actionUpdate()
	{
		$this->layout = YumWebModule::yum()->adminLayout;
		$model = $this->loadModel();

	 $this->performAjaxValidation($model);

		if(isset($_POST['YumRole']))
		{
			$model->title = $_POST['YumRole']['title'];
			$model->description = $_POST['YumRole']['description'];
			$model->users = Relation::retrieveValues($_POST, 'YumUser');

		if($model->validate() && $model->save())
				$this->redirect(array('view','id'=>$model->id));
		}

		$this->render('update',array(
			'model'=>$model,
		));
	}

	public function actionAdmin() 
	{
		$this->layout = YumWebModule::yum()->adminLayout;
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
		$this->layout = YumWebModule::yum()->adminLayout;	
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
		$this->actionAdmin();
	}


	/**
	 * @return YumRole
	 */
	public function loadModel()
	{
		if($this->_model===null)
		{
			if(isset($_GET['id']))
				$this->_model=YumRole::model()->findbyPk($_GET['id']);
			if($this->_model===null)
				throw new CHttpException(404,Yii::t('App', 'The requested page does not exist.'));
		}
		return $this->_model;
	}

	protected function performAjaxValidation($model)
	{
		if(isset($_POST['ajax']) && $_POST['ajax']==='yum-role-form')
		{
			echo CActiveForm::validate($model);
			Yii::app()->end();
		}
	}
}
