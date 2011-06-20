<?php

Yii::setPathOfAlias( 'YumMessagesModule' , dirname(__FILE__) );

class MessagesModule extends YumWebModule
{
	
	public $version = '0.5';
	public $debug = false;
	public $messagesTable = "messages";
	public $installDemoData = true;
	
	public $controllerMap=array(
		'messages'=>array('class'=>'YumMessagesModule.controllers.YumMessagesController'),
	);
	
	public function init()
	{
		$this->setImport(array(
			'YumMessagesModule.models.*',
		));
	}

	public function beforeControllerAction($controller, $action)
	{
		if(parent::beforeControllerAction($controller, $action))
		{
			return true;
		}
		else
			return false;
	}
	
}
