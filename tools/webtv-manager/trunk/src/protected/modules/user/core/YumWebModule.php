<?php
/**
 * Base class for all modules
 * @author tomasz.suchanek
 * @since 0.6
 * @package Yum.core
 *
 */
class YumWebModule extends CWebModule
{
	
	/**
	 * @var string
	 */
	public $version = null;
	
	/**
	 * @var boolean
	 */
	public $debug = null;

	/**
	 * @var boolean
	 */
	public $installDemoData = null;

	
	public function init()
	{
		if($this->version===null)
			throw new CException(Yii::t('UserModule.core','Please provide module version in Yum module'));
		if($this->debug===null)
			throw new CException(Yii::t('UserModule.core','Please set debug mode in Yum module'));
		if($this->installDemoData===null)
			throw new CException(Yii::t('UserModule.core','Please determine whether to install demo data in Yum module.'));
	}
	
	/** 
	 * Checks if the requested module is a submodule of the user module 
	 * @return boolean
	 */
	public function hasModule($module)
	{
		return in_array($module, array_keys($this->getModules()));
	}	
	
	/**
	 * Find out whether user module is installed
	 * First check if current module is user module,
	 * if not then checks if 'YumModuleAlias' application parameter is defined,
	 * if not then checks if 'user' application module is defined is user module,
	 * if not then it checks if any parent of current controller is user module
	 * if not then throws CException
	 * @since 0.6
	 * @return UserModule
	 */
	public static function yum()
	{
		if(Yii::app()->controller->module instanceof UserModule)
			return Yii::app()->controller->module;
		elseif(isset(Yii::app()->params['YumModuleAlias']))
		{
			$object=Yii::app();
			foreach(explode('.',Yii::app()->params['YumModuleAlias']) as $subModule)
				$object=$object->getModule($subModule);	
			return $object;
		}
		elseif(Yii::app()->getModule('user') instanceof UserModule)
			return Yii::app()->getModule('user');
		else
		{
			while (($parent=$this->getParentModule())!==null)
				if($parent instanceof UserModule)	
					return $parent;
		}
		throw new CException(Yii::t('UserModule.core','Yum Module cannot be found'));		
	}
}
?>