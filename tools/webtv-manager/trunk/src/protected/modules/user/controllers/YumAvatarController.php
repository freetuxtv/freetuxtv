<?php

class YumAvatarController extends YumController {

	public function actionRemoveAvatar() {
		$model = YumUser::model()->findByPk(Yii::app()->user->id);
		$model->avatar = '';
		$model->save();
		$this->redirect(array('user/profile'));	
	}

	public function beforeAction($action) {
		if(Yii::app()->user->isGuest)
			$this->redirect(Yum::module()->loginUrl);
		return parent::beforeAction($action);
	}

	public function actionEditAvatar() {
		$model = YumUser::model()->findByPk(Yii::app()->user->id);
		$model->setScenario('avatarUpload');

		if(isset($_POST['YumUser'])) {
			$model->updateAvatar();

			if($model->validate() && $model->save())
				$this->redirect(array('user/profile'));	
		}

		$this->render('edit_avatar', array('model' => $model));
	}

}
