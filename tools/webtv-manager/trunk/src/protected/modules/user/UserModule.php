<?php

Yii::setPathOfAlias( 'YumModule' , dirname(__FILE__) );
Yii::import('YumModule.core.YumWebModule');
Yii::import('YumModule.core.YumHelper');

/**
 * @property string $registrationUrl
 * @property string $recoveryUrl
 * @property string $loginUrl
 * @property string $returnUrl
 * @property string $returnLogoutUrl
 */
class UserModule extends YumWebModule
{
	
	public $version = '0.6';
	public $debug = false;
	public $usersTable = "users";
	public $messagesTable = "messages";
	public $profileFieldsTable = "profile_fields";
	public $profileFieldsGroupTable = "profile_fields_group";
	public $profileTable = "profiles";
	public $rolesTable = "roles";
	public $userRoleTable = "user_has_role";
	public $userUserTable = "user_has_user";
	public $installDemoData = true;
	public $enableEmailActivation = true;
	public $layout = 'column2';
	public $adminLayout = 'column2';
	public $salt = '';
	 // valid callback function for password hashing ie. sha1
	public $hashFunc = 'md5';	

	public $yumBaseRoute = '/user';

	public static $dateFormat = "m-d-Y";  //"d.m.Y H:i:s"

	// Allow login of inactive User Account
	public static $allowInactiveAcctLogin = false;

	private $_urls=array(
		'registration'=>array('{user}/registration'),
		'recovery'=>array('{user}/recovery'),
		'login'=>array("{user}/login"),
		'return'=>array("{user}/profile"),
		// Page to go to after registration, login etc.	
		'returnLogout'=>array("{user}/login"),
	);

	// Activate profile History (profiles are kept always, and when the 
  // user changes his profile, it gets added to the database rather than
  // updated).
	public $profileHistory = true;
	
	// When readOnlyProfiles is set, only administrators can update Profile
  // Information
	public $readOnlyProfiles = false;

	// When forceProtectedProfiles is set, only administrators and the user 
  // himself can view the profile 
	public $forceProtectedProfiles = false;

	// LoginType :
	// 0: Allow login only by Username
	const LOGIN_BY_USERNAME		= 0;
	// 1: Allow login only by E-Mail (needs profile module)
	const LOGIN_BY_EMAIL			= 1; 
	// 2: Allow login by E-Mail or Username (needs profile module)
	const	LOGIN_BY_USERNAME_OR_EMAIL	= 2; 
	// 3: Allow login only by OpenID (TODO FIXME needs to be implemented) 
	//const LOGIN_OPENID		= 4;
	public $loginType = self::LOGIN_BY_USERNAME;
	
	/**
	 * Whether to use captcha e.g. in registration process
	 * @var boolean
	 */
	public $allowCaptcha=true;
	
	/**
	 * Controller map
	 * @var array
	 */
	public $controllerMap=array(
		'default'=>array('class'=>'YumModule.controllers.YumDefaultController'),
		'install'=>array('class'=>'YumModule.controllers.YumInstallController'),
		'user'=>array('class'=>'YumModule.controllers.YumUserController'),	
	);
	
	/**
	 * Additionally implements support for getting URLs
	 * @param string $name
	 */
	public function __get($name)
	{
		if(substr($name,-3) === 'Url')
			if(isset($this->_urls[substr($name, 0, -3)]))
				return YumHelper::route($this->_urls[substr($name, 0, -3)]);
				
		return parent::__get($name);
	}

	/**
	 * Additionally implements support for setting URLs
	 * @param string $name
	 * @param mixed $value
	 */
	public function __set($name,$value)
	{
		if(substr($name,-3)==='Url')
		{
			if(isset($this->_urls[substr($name,0,-3)]))
				$this->_urls[substr($name,0,-3)]=$value;		
		}
		parent::__set($name,$value);
	}

	public function init()
	{
		$this->setImport(array(
			'YumModule.models.*',
			'YumModule.components.*',
			'YumModule.core.YumActiveRecord',
			'YumModule.core.YumController',
			'YumModule.core.YumFormModel',
			'YumModule.core.YumHelper',
			'YumModule.core.YumMenuItemHelper',
			'YumModule.core.YumWebModule',
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

	/**
	 * Configures the module with the specified configuration.
	 * Override base class implementation to allow static variables.
	 * @param array the configuration array
	 */
	public function configure($config)
	{
		if(is_array($config))
		{
			foreach($config as $key=>$value)
			{
				if(isset(UserModule::${$key}))
				{
					UserModule::${$key} = $value;
				}
				else 
					$this->$key=$value;
			}
		}
	}

}
