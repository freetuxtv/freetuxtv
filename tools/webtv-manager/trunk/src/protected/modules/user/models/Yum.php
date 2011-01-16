<?php
/**
 * Helper class
 * @author tomasz.suchanek@gmail.com
 * @since 0.6
 * @package Yum.core
 *
 */
class Yum
{ 
	/** Register an asset file of Yum */
	public static function register($file)
	{
		$url = Yii::app()->getAssetManager()->publish(Yii::getPathOfAlias('YumAssets'));

		$path = $url . '/' . $file;
		if(strpos($file, 'js') !== false)
			Yii::app()->clientScript->registerScriptFile($path);
		else if(strpos($file, 'css') !== false)
			Yii::app()->clientScript->registerCssFile($path);
	}

	public static function hint($message) 
	{
		return '<div class="hint">' . Yum::t($message) . '</div>'; 
	}

	/* set a flash message to display after the request is done */
	public static function setFlash($message) 
	{
		$_SESSION['yumflash'] = Yum::t($message);
	}

	/* retrieve the flash message again */
	public static function getFlash() {
		if(isset($_SESSION['yumflash'])) {
			$message = $_SESSION['yumflash'];
			unset($_SESSION['yumflash']);
			return $message;
		}
	}

	public static function renderFlash()
	{
		if(isset($_SESSION['yumflash'])) {
			echo '<div class="info">';
			echo Yum::getFlash();
			echo '</div>';
		}
	}

	public static function p($string, $params = array()) {
		return '<p>' . Yum::t($string, $params) . '</p>';
	}

	/** Associate the right translation file depending on the
		controller */
	public static function t($string, $params = array())
	{
		$file = 'yum_'. Yii::app()->controller->id;
		$lang = Yii::app()->language;
		$path = Yii::getPathOfAlias("application.modules.user.messages.{$lang}.{$file}"). '.php';

		if(is_file($path) && $messages = include($path))
			if (array_key_exists($string, $messages) == true)
				return Yii::t('UserModule.'.$file, $string, $params);
		return Yii::t('UserModule.yum_user', $string, $params);
	}

	/**
	 * Resolved table name into table name with prefix if needed
	 * @param string $tablename, e.g {{tablename}}
	 * @param CDbConnection $connection
	 * @since 0.6
	 * @return string resolved table name
	 */
	public static function resolveTableName($tablename, CDbConnection $connection=null)
	{
		$dbConnection = $connection instanceof CDbConnection ? $connection : Yii::app()->getModule('db');
		if(isset($dbConnection->tablePrefix) && $dbConnection->tablePrefix != '') 
		{
			if(substr($dbConnection->tablePrefix, -1) == '_') 
				$tablename = $dbConnection->tablePrefix . $tablename;	
			else
				$tablename = $dbConnection->tablePrefix . '_' . $tablename;	
		}
		return $dbConnection->createCommand($tablename)->getText();
	}

	// returns the yii user module. Mostly used for accessing options
	// by calling Yum::module->option
	public static function module()
	{
		if(Yii::app()->controller->module instanceof UserModule)
			return Yii::app()->controller->module;
		elseif(isset(Yii::app()->params['YumModuleAlias']))
		{
			$object = Yii::app();
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
		throw new CException(Yum::t('Yum Module cannot be found'));		
	}


	/**
	 * Parses url for predefined symbols and returns real routes
	 * Following symbols are allowed:
	 *  - {yum} - points to base path of Yum
	 *  - {users} - points to user controller 
	 *  - {messsages} - points to base messages module
	 *  - {roles} - points to base roles module
	 *  - {profiles} - points to base profile module
	 * @param string $url
	 * @since 0.6
	 * @return string 
	 */
	public static function route($url)
	{
		$yumBaseRoute=Yum::module()->yumBaseRoute;
		$tr=array();
		$tr['{yum}']=$yumBaseRoute;
		$tr['{messages}']=$yumBaseRoute.'/messages';
		$tr['{roles}']=$yumBaseRoute.'/role';
		$tr['{profiles}']=$yumBaseRoute.'/profiles';
		$tr['{user}']=$yumBaseRoute.'/user';
		if(is_array($url))
		{
			$ret=array();
			foreach($url as $k=>$entry)
				$ret[$k]=strtr($entry,$tr);
			return $ret;
		}
		else
			return strtr($url,$tr);

	}

	/**
	 * Produces note: "Field with * are required"
	 * @since 0.6
	 * @return string 
	 */
	public static function requiredFieldNote()
	{
		return CHtml::tag('p',array('class'=>'note'),Yum::t(
					'Fields with <span class="required">*</span> are required.'
					),true);		
	}

}
?>
