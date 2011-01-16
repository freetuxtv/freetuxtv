<?php

class YumPrivacysettingController extends YumController
{
	public function accessRules()
	{
		return array(
				array('allow', 
					'actions'=>array('update'),
					'users'=>array('@'),
					),
				array('deny', 
					'users'=>array('*'),
					),
				);
	}

	public function actionUpdate() {
		$model = YumPrivacySetting::model()->findByPk(Yii::app()->user->id);

		if(isset($_POST['YumPrivacysetting'])) {
			$model->attributes = $_POST['YumPrivacysetting'];
			$model->validate();

		if(isset($_POST['YumProfile'])) {
			$profile = $model->user->profile[0];
			$profile->attributes = $_POST['YumProfile'];
			$profile->validate();
			}

			if(!$model->hasErrors() && !$model->user->profile[0]->hasErrors()) {
				$profile->save();
				$model->save();
				$this->redirect(array('//user/profile/view'));
			}
		}

		if(!$model) {
			$model = new YumPrivacySetting();
			$model->user_id = Yii::app()->user->id;
			$model->save();
			$this->refresh();
		}


		$this->render('update',array(
					'model'=>$model,
					'profile'=> isset($model->user) && isset($model->user->profile[0])
					? $model->user->profile[0] 
					: null
					));
	}

}
