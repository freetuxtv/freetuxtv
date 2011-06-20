<?php
/**
 * RegistrationForm class.
 * RegistrationForm is the data structure for keeping
 * user registration form data. It is used by the 'registration' action of 'YumUserController'.
 * @package Yum.models
 */
class YumRegistrationForm extends YumUser {
	/**
	 * @var string
	 */
	public $verifyPassword;
	
	/**
	 * @var string
	 */
	public $verifyCode;

	public function rules() 
	{
		return CMap::mergeArray(parent::rules(),array(
			array('username, password, verifyPassword', 'required'),
			array('password', 'compare', 'compareAttribute'=>'verifyPassword', 'message' => Yii::t("UserModule.user", "Retype password is incorrect.")),
			array('verifyCode', 'captcha', 'allowEmpty'=>!extension_loaded('gd')||!Yii::app()->controller->module->allowCaptcha)
		));
	}

}
