<?php
Yii::import('zii.widgets.CPortlet');

class LoginWidget extends CPortlet
{
	public function init()
	{
		$this->title=Yii::t('user', 'Login');
		parent::init();
	}

	protected function renderContent()
	{
		$this->render('quicklogin', array('model' => new YumUserLogin()));
	}
} 
?>
