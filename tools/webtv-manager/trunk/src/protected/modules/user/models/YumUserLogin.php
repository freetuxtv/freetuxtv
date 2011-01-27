<?php

/**
 * LoginForm class.
 * LoginForm is the data structure for keeping
 * user login form data. It is used by the 'login' action of 'YumUserController'.
 */
class YumUserLogin extends YumFormModel
{
	public $username;
	public $password;
	public $rememberMe;

	/**
	 * Declares the validation rules.
	 * The rules state that username and password are required,
	 * and password needs to be authenticated.
	 */
	public function rules()
	{
		return array(
			// username and password are required
			array('username, password', 'required'),
			// rememberMe needs to be a boolean
			array('rememberMe', 'boolean'),
			// password needs to be authenticated
			array('password', 'authenticate'),
		);
	}

	public function attributeLabels()
	{
	if(Yii::app()->controller->module->loginType == 0)
		$username = Yii::t("UserModule.user", "Username");
	else if(Yii::app()->controller->module->loginType == 1)
		$username = Yii::t("UserModule.user", "Email Address");
	else if(Yii::app()->controller->module->loginType == 2)
		$username = Yii::t("UserModule.user", "Username or Email");

		return array(
			'username'=>$username,
			'password'=>Yii::t("UserModule.user", "Password"),
			'rememberMe'=>Yii::t("UserModule.user", "Remember me next time"),
		);
	}

	/**
	 * Authenticates the password.
	 * This is the 'authenticate' validator as declared in rules().
	 */
	public function authenticate($attribute,$params)
	{
		if(!$this->hasErrors())  // we only want to authenticate when no input errors
		{
			$identity=new YumUserIdentity($this->username,$this->password);
			$identity->authenticate();
			switch($identity->errorCode)
			{
				case YumUserIdentity::ERROR_NONE:
					$duration=$this->rememberMe ? 3600*24*30 : 0; // 30 days
					Yii::app()->user->login($identity,$duration);
					break;
				case YumUserIdentity::ERROR_EMAIL_INVALID:
					$this->addError("username",Yii::t("UserModule.user", "Email is incorrect."));
					break;
				case YumUserIdentity::ERROR_USERNAME_INVALID:
					$this->addError("username",Yii::t("UserModule.user", "Username is incorrect."));
					break;
				case YumUserIdentity::ERROR_STATUS_NOTACTIVE:
					$this->addError("status",Yii::t("UserModule.user", "You account is not activated."));
					break;
				case YumUserIdentity::ERROR_STATUS_BANNED:
					$this->addError("status",Yii::t("UserModule.user", "You account is blocked."));
					break;
				case YumUserIdentity::ERROR_PASSWORD_INVALID:
					$this->addError("password",Yii::t("UserModule.user", "Password is incorrect."));
					break;
			}
		}
	}
}
