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
	public $retype;
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
			array('email, username, password, retype', 'required'),
			// email must be valid
			array('email', 'email'),
			// email and username needs to be unique
			array('email, username, retype', 'authenticate', 'skipOnError'=>true),
			
			
		);
	}

	/**
	 * Authenticates the username and email.
	 * This is the 'authenticate' validator as declared in rules().
	 */
	public function authenticate($attribute,$params)
	{
		$this->_identity=new UserIdentity($this->username,$this->password);
		$this->_identity->setEmail($this->email);
		if($this->retype !== $this->password){
					$this->errorCode=3;
		}
			
		if(!$this->_identity->authenticateNewUser()){
			$identity=new UserIdentity($this->username,$this->password);
	                if(!$this->_identity->authenticateNewUser()){
				if($this->retype !== $this->password){
					$this->addError('retype','Check your password.');
					return false;
				}
			        switch($this->_identity->errorCode)
			        {
			                case 0:
						return true;
			                        break;
			                case 1:
			                        $this->addError('email','Email already in database.');
						return false;
			                        break;
			                case 2:
			                        $this->addError('username','Email already in database.');
						return false;
			                        break;
				}
			}
	        }	
	}
	/**
	 * Creat an account in database using the given email and username and password in the model.
	 * @return boolean whether createAccount is successful
	 */
	public function createAccount()
	{
		if($this->_identity!==null)
		{
			$this->_identity=new UserIdentity($this->username,$this->password);
			$this->_identity->authenticateNewUser();
			if($this->_identity->errorCode===UserIdentity::ERROR_NONE)
			{

				$user = User::createNew($this->username,$this->password,$this->email,0);
				if($user->save() || $this->CreateNew($this->username,$this->password,$this->email,0 && $this->_identity->login())){

					Yii::app()->user->setFlash('contact','Check Your email.',10);
					$headers="From: {$this->email}\r\nReply-To: {$this->email}";
				mail(Yii::app()->params['adminEmail'],'WebTv-Manager: Registration Successful','Thank you for contacting us. We will respond to you as soon as possible.',$headers);
				
					return true;
				}

			}
			return false;
		}
	}
}
