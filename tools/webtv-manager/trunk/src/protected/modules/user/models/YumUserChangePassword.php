<?php
/**
 * UserChangePassword class.
 * UserChangePassword is the data structure for keeping
 * user change password form data. It is used by the 'changepassword' action 
 * of 'UserController'.
 */

class YumUserChangePassword extends YumFormModel 
{
	public $password;
	public $verifyPassword;

	public function rules() 
	{
		return array(
			array('password, verifyPassword', 'required'),
			array('password', 'length', 'max'=>128, 'min' => 4,
				'message' => Yii::t("UserModule.user", "Incorrect password (minimal length 4 symbols).")),
			array('password', 'compare', 'compareAttribute'=>'verifyPassword',
				'message' => Yii::t("UserModule.user", "Retype password is incorrect.")),
		);
	}

	/**
	 * Declares attribute labels.
	 */
	public function attributeLabels()
	{
		return array(
			'password'=>Yii::t("UserModule.user", "password"),
			'verifyPassword'=>Yii::t("UserModule.user", "Retype password"),
		);
	}
} 
