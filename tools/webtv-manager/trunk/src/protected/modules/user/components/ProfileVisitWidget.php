<?php
Yii::import('application.modules.user.UserModule');
Yii::import('zii.widgets.CPortlet');

class ProfileVisitWidget extends CPortlet
{
	public function init() {
		$this->title=Yum::t('Profile visits');
		if(Yii::app()->user->isGuest)
			return false;

		parent::init();
	}

	protected function renderContent()
	{
		parent::renderContent();
		if(Yii::app()->user->isGuest)
			return false;

			$this->render('profile_visits', array(
						'visits' => Yii::app()->user->data()->visits));
	}
} 
?>
