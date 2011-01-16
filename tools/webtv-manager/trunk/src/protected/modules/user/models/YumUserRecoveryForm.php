<?php

/**
 * UserRecoveryForm class.
 * UserRecoveryForm is the data structure for keeping
 * user recovery form data. It is used by the 'recovery' action of 'YumUserController'.
 */
class YumUserRecoveryForm extends YumFormModel {
	public $login_or_email, $user_id;
	
	/**
	 * Declares the validation rules.
	 * The rules state that username and password are required,
	 * and password needs to be authenticated.
	 */
	public function rules()
	{
		return array(
			// username and password are required
			array('login_or_email', 'required'),
			array('login_or_email', 'match', 'pattern' => '/^[A-Za-z0-9@.\s,]+$/u','message' => Yii::t("UserModule.user", "Incorrect symbol's. (A-z0-9)")),
			// password needs to be authenticated
			array('login_or_email', 'checkexists'),
		);
	}
	/**
	 * Declares attribute labels.
	 */
	public function attributeLabels()
	{
		return array(
			'login_or_email'=>Yii::t("UserModule.user", "username or email"),
		);
	}
	
	public function checkexists($attribute,$params) {
		// we only want to authenticate when no input errors
		if(!$this->hasErrors()) {
			if (strpos($this->login_or_email,"@")) {
				$profile = YumProfile::model()->findByAttributes(array(
							'email'=>$this->login_or_email));
				$user = isset($profile->user) ? $profile->user : null;
			} else {
				$user = YumUser::model()->findByAttributes(array(
							'username'=>$this->login_or_email));
			}

			if($user === null) 
			{
				if (strpos($this->login_or_email, "@")) 
					$this->addError("login_or_email",
							Yii::t("UserModule.user", "Email is incorrect."));
				else
					$this->addError("login_or_email",
							Yii::t("UserModule.user", "Username is incorrect."));
			} 
			else
			{
				$this->user_id = $user->id;
			}
		}
	}
}
