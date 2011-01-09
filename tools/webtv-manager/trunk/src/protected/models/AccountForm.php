<?php

/**
 * AccountFrom class.
 * AccountForm is the data structure for keeping
 * check email, username if already in database, whereas add it and can login form data. It is used by the 'Create Account' action of 'SiteController'.
 */
class AccountForm extends CFormModel
{
	public $email;
	public $username;
	public $password;
	
	private $Id;
	private $_identity;

	/**
	 * Declares the validation rules.
	 * The rules state that email and username and password are required
	 */
	public function rules()
	{
		return array(
			// email and username and password are required
			array('email, username, password', 'required'),
			// email must be valid
			array('email', 'email'),
			// email and username needs to be unique
			array('email, username', 'authenticate', 'skipOnError'=>true),
			
		);
	}

	/**
	 * check unicitie of email and/or username.
	 * This is the 'authenticate' validator as declared in rules().
	 */
	public function authenticate($attribute,$params)
	{
		$this->Id=new UserEmail($this->username,$this->password,$this->email);
		if(!$this->Id->authenticate()){
			$this->addError('email','Email already in database.');
			$this->addError('username','Username already in database.');
			}
		
	}

	/**
	 * Logs in the user using the given username and password in the model.
	 * @return boolean whether login is successful
	 */
	public function account()
	{
		if($this->Id!==null)
		{
			$this->Id=new UserIdentity($this->username,$this->password);
			$this->Id->authenticate();
			// SQL requet maybe not in the good place... 
							
				$sql="INSERT INTO wtvmt_user (username, email, password, Rights) VALUES(:username,:email,:password,9)";
				$connection=Yii::app()->db;
				$command=$connection->createCommand($sql);
				// replace ":username" by real username same for email and password Rights to 9
				$command->bindParam(":username",$this->username,PDO::PARAM_STR);
				// remplace la marque ":email" avec la vrai valeur email
				$command->bindParam(":email",$this->email,PDO::PARAM_STR);
				$command->bindParam(":password",md5($this->password),PDO::PARAM_STR);
				$command->execute();
				return true;	
		}
	}
		/**
	 * Logs in the user using the given username and password in the model.
	 * @return boolean whether login is successful
	 */
	public function login()
	{
		if($this->_identity===null)
		{
			$this->_identity=new UserIdentity($this->username,$this->password);
			$this->_identity->authenticate();
		}
		if($this->_identity->errorCode===UserIdentity::ERROR_NONE)
		{
			$duration=0;
			Yii::app()->user->login($this->_identity,$duration);
			return true;
		}
		else
			return false;
	}
}
