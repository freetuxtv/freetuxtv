<?php

Yii::setPathOfAlias( 'YumRoleModule' , dirname(__FILE__) );

class RoleModule extends YumWebModule
{
	
	public $version = '0.5';
	public $debug = false;
	public $rolesTable = "roles";
	public $userRoleTable = "user_has_role";
	public $installDemoData = true;
	
	public $controllerMap=array(
		'role'=>array('class'=>'YumRoleModule.controllers.YumRoleController'),
	);	
	
	public function init()
	{
		$this->setImport(array(
			'YumRoleModule.models.*',
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
