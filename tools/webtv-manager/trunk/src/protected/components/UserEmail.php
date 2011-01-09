<?php

/**
 * UserIdentity represents the data needed to identity a user.
 * It contains the authentication method that checks if the provided
 * data can identity the user.
 */
class UserEmail extends CUserIdentity
{
   
	private $_id;
	public $email;

	/**
	 * New Constructor.
	 * @param string $username username
	 * @param string $email email
	 * @param string $password password
	 */
	public function UserEmail($username,$password,$email)
	{
			$this->email=$email;
			$this->username=$username;
			$this->password=$password;
	}
		
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
        if($user!==null){
            $this->errorCode=self::ERROR_USERNAME_INVALID;
        }else {
		
			$email=strtolower($this->email);
			$emailentred=User::model()->find('LOWER(email)=?',array($email));
			if($emailentred!==null){
				$this->errorCode=9;
			}else{
				$this->errorCode=self::ERROR_NONE;
			}
        }
        return $this->errorCode==self::ERROR_NONE;
	}
 
    public function getId()
    {
        return $this->_id;
    }

}