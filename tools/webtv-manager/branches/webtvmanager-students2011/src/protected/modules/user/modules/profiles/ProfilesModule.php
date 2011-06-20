<?php

Yii::setPathOfAlias( 'YumProfileModule' , dirname(__FILE__) );

class ProfilesModule extends YumWebModule
{
	
	public $version = '0.5';
	public $debug = false;
	public $profileFieldsTable = "{{profile_fields}}";
	public $profileFieldsGroupTable = "{{profile_fields_group}}";
	public $profileTable = "{{profiles}}";
	public $installDemoData = true;	
	
	public $controllerMap=array(
		'fields'=>array('class'=>'YumProfileModule.controllers.YumFieldsController'),
		'fieldsGroup'=>array('class'=>'YumProfileModule.controllers.YumFieldsGroupController'),
	);	

	public function init()
	{
		$parentImport=$this->getParentModule() instanceof UserModule 
			? array(
				'YumModule.models.*',
			)
			: array();
		$import=array('YumProfileModule.models*');
		$this->setImport(CMap::mergeArray($parentImport,$import));
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
