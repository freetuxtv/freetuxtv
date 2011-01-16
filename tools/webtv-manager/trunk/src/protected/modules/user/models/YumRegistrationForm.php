<?php
/**
 * RegistrationForm class.
 * RegistrationForm is the data structure for keeping
 * user registration form data. It is used by the 'registration' action of 'YumUserController'.
 * @package Yum.models
 */
class YumRegistrationForm extends YumUser {
	public $verifyPassword;
	
	/**
	 * @var string
	 */
	public $verifyCode;

	public function rules() 
	{
		$rules = parent::rules();
		if(!Yum::module()->loginType & UserModule::LOGIN_BY_EMAIL)
			$rules[] = array('username', 'required');
		$rules[] = array('password, verifyPassword', 'required');
		$rules[] = array('password', 'compare', 'compareAttribute'=>'verifyPassword', 'message' => Yum::t("Retype password is incorrect."));
		$rules[] = array('verifyCode', 'captcha', 'allowEmpty'=>!extension_loaded('gd')||!Yii::app()->getModule('user')->enableCaptcha);

		return $rules;
	}
	
	public function genRandomString($length = 10)
	{
	$characters = '0123456789abcdefghijklmnopqrstuvwxyz';
	$string ='';    
	for ($p = 0; $p < $length; $p++)
	{
        $string .= $characters[mt_rand(0, strlen($characters)-1)];
	}
	return $string;
	}

}
