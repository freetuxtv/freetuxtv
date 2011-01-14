<?php

/**
 * UserIdentity represents the data needed to identity a user.
 * It contains the authentication method that checks if the provided
 * data can identity the user.
 */
class UserIdentity extends CUserIdentity
{
   
	private $_id;
	public $email;


	/**
	 * Authenticates a user.
	 * The example implementation makes sure if the username and password
	 * are both 'demo'.
	 * In practical applications, this should be changed to authenticate
	 * against some persistent user identity storage (e.g. database).
	 * @return boolean whether authentication succeeds.
	 */
	public function authenticate()
	{
		$username=strtolower($this->username);
		$user=User::model()->find('LOWER(username)=?',array($username));
		if($user===null){
			$this->errorCode=self::ERROR_USERNAME_INVALID;
		}else if($user->getHashedPassword() !== md5($this->password)){
			$this->errorCode=self::ERROR_PASSWORD_INVALID;
		}else{
			$this->_id=$user->getId();
			$this->username=$user->getUsername();
			$this->email=$user->getEmail();
			$this->errorCode=self::ERROR_NONE;
		}
		return $this->errorCode==self::ERROR_NONE;
	}

	/**
	 * Authenticates a NewUser (create user account).
	 * The example implementation makes sure if the username and password
	 * are both 'demo'.
	 * In practical applications, this should be changed to authenticate
	 * against some persistent user identity storage (e.g. database).
	 * @return boolean whether authentication succeeds.
	 */
	public function authenticateNewUser()
	{
		$email=strtolower($this->email);
		$emailentred=User::model()->find('LOWER(email)=?',array($email)); //check email in database
		if($emailentred!==null){
			$this->errorCode=1;

		}else{
			$username=strtolower($this->username);
			$user=User::model()->find('LOWER(username)=?',array($username)); //check username in database
			if($user!==null){
				$this->errorCode=2;
			}else {
					$this->errorCode=0;
			}
		}
		return $this->errorCode==0;
	}
 
    public function getId()
    {
        return $this->_id;
    }

	public function setEmail($email)
	{
		$this->email=$email;
	}

}
