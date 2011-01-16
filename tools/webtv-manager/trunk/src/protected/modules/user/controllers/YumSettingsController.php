<?php

class YumSettingsController extends YumController {
	public function accessRules()
	{
		return array(
			array('allow', 
				'actions'=>array('index', 'view', 'create', 'update','admin', 'delete', 'setActive'),
				'users'=>array('admin'),
			),
			array('deny', 
				'users'=>array('*'),
			),
		);
	}

	public function actionSetActive()
	{
		if(isset($_POST['active_profile'])) {
			foreach(YumSettings::model()->findAll() as $setting) {
				$setting->is_active = false;
				$setting->save();
			}
			$setting = YumSettings::model()->findByPk($_POST['active_profile']);
			$setting->is_active = true;
			$setting->save();
			if(Yum::module()->enableLogging == true)
								{
								$user= YumUser::model()->findbyPK(Yii::app()->user->id);
								YumActivityController::logActivity($user, 'profile_set_active');
								}	
		}
		$this->redirect(array('//user/yumSettings/index'));
	}

	public function actionCreate()
	{
		$this->layout = Yum::module()->adminLayout;
		$model=new YumSettings;

		$this->performAjaxValidation($model, 'yum-settings-form');

		if(isset($_POST['YumSettings'])) {
			$model->attributes = $_POST['YumSettings'];
			$model->loginType = $this->setLoginType($_POST['loginType']);	

			if($model->save()) {
				if(Yum::module()->enableLogging == true)
								{
								$user= YumUser::model()->findbyPK(Yii::app()->user->id);
								YumActivityController::logActivity($user, 'settings_profile_created');
								}
				$this->redirect(array('index'));
			}
		}

		$this->render('/settings/create',array(
			'model'=>$model,
		));
	}

	public function setLoginType($type)
	{
		$logintype = 0;

		if (is_array($type))
			foreach($type as $key => $value)
				$logintype += $key;

		return $logintype;
	}

	/*
	 * used to display CTabView
	 */
	public function getTabParameters()
	{
		$tabParameters = array();

		foreach ($this->clips as $key => $clip)
			$tabParameters['tab' . (count($tabParameters) + 1)] = array('title' => $key, 'content' => $clip);

		return $tabParameters;
	}


	public function actionUpdate()
	{
		$this->layout = Yum::module()->adminLayout;
		if(!isset($_GET['id']))	
			$_GET['id'] = 0;
		if($_GET['id'] == 0)
			$_GET['id'] = YumSettings::model()->getActive();
		$model=$this->loadModel();

		$this->performAjaxValidation($model, 'yum-settings-form');

		$YumSettingsData = Yii::app()->request->getPost('YumSettings');
		if($YumSettingsData !== null)
		{
			$model->attributes = $YumSettingsData;
			$model->loginType = $this->setLoginType($_POST['loginType']);	

			if($model->save())
			{
				if(Yum::module()->enableLogging == true)
								{
								$user= YumUser::model()->findbyPK(Yii::app()->user->id);
								YumActivityController::logActivity($user, 'settings_updated');
								}
				$this->redirect(array('index'));
		}
	}

		$this->render('/settings/update',array(
			'model'=>$model,
		));
	}

	public function actionDelete()
	{
		if(Yii::app()->request->isPostRequest)
		{
			if(Yum::module()->enableLogging == true)
								{
								$user= YumUser::model()->findbyPK(Yii::app()->user->id);
								YumActivityController::logActivity($user, 'settings_profile_removed');
								}
			$this->loadModel()->delete();

			if(Yii::app()->request->getQuery('ajax') === null)
			{
				$this->redirect(array('index'));
			}
		}
		else
			throw new CHttpException(400,
					Yii::t('app', 'Invalid request. Please do not repeat this request again.'));
	}

	public function actionIndex()
	{
		$model=new YumSettings('search');

		$YumSettingsData = Yii::app()->request->getQuery('YumSettings');
		if($YumSettingsData !== null)
			$model->attributes = $YumSettingsData;

		$this->render('/settings/admin',array(
			'model'=>$model,
		));
	}

}
