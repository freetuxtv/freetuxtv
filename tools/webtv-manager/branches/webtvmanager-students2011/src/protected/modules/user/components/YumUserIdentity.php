<?php

class YumUserIdentity extends CUserIdentity
{
	private $id;
	const ERROR_EMAIL_INVALID=3;
	const ERROR_STATUS_NOTACTIVE=4;
	const ERROR_STATUS_BANNED=5;

	public function authenticate()
	{
		$user=null;
		$loginType = Yii::app()->controller->module->loginType; 

		if ($loginType == UserModule::LOGIN_BY_USERNAME)
		{
			$user = YumUser::model()->findByAttributes(array('username'=>$this->username));
		}
		else if ($loginType == UserModule::LOGIN_BY_EMAIL) 
		{
			$profile = YumProfile::model()->findByAttributes(array('email'=>$this->username));
			if($profile instanceof YumProfile)
				$user = $profile->user;
		}
		else if ($loginType == UserModule::LOGIN_BY_USERNAME_OR_EMAIL) 
		{
			$user=YumUser::model()->findByAttributes(array('username'=>$this->username));
			if(!is_object($user)) 
				if(($profile=YumProfile::model()->findByAttributes(array('email'=>$this->username))) instanceof YumProfile)
					$user=$profile->user;
		}

		if($user===null)
			if ($loginType == 1) 
			{
				$this->errorCode=self::ERROR_EMAIL_INVALID;
			}
			else 
			{
				$this->errorCode=self::ERROR_USERNAME_INVALID;
			}
		else if(YumUser::encrypt($this->password)!==$user->password)
			$this->errorCode=self::ERROR_PASSWORD_INVALID;
		else if($user->status == 0 && UserModule::$allowInactiveAcctLogin==false)
			$this->errorCode=self::ERROR_STATUS_NOTACTIVE;
		else if($user->status==-1)
			$this->errorCode=self::ERROR_STATUS_BANNED;
		else {
			$this->id=$user->id;
			$this->setState('id', $user->id);
			$this->username=$user->username;
			$this->errorCode=self::ERROR_NONE;
		}
		return !$this->errorCode;
	}

	/**
	 * @return integer the ID of the user record
	 */
	public function getId()
	{
		return $this->id;
	}

	public function getRoles() {
		return $this->Role;
	}

}
