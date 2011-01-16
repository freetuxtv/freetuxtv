<?php

/**
 * This is the model class for table "User".
 *
 * The followings are the available columns in table 'User':
 */
class User extends user/YumUser
{
	/**
	 * Returns the static model of the specified AR class.
	 * @return User the static model class
	 */
	public static function model($className=__CLASS__)
	{
		return parent::model($className);
	}

	/**
	 * @return array validation rules for model attributes.
	 */
	public function rules()
	{
		// NOTE: you should only define rules for those attributes that
		// will receive user inputs.
		return array(
			// The following rule is used by search().
			// Please remove those attributes that should not be searched.
			array('', 'safe', 'on'=>'search'),
		);
	}

	/**
	 * @return array relational rules.
	 */
	public function relations()
	{
		// NOTE: you may need to adjust the relation name and the related
		// class name for the relations automatically generated below.
		return array(
		);
	}

	/**
	 * @return array customized attribute labels (name=>label)
	 */
	public function attributeLabels()
	{
		return array(
		);
	}

	/**
	 * Retrieves a list of models based on the current search/filter conditions.
	 * @return CActiveDataProvider the data provider that can return the models based on the search/filter conditions.
	 */
	public function search()
	{
		// Warning: Please modify the following code to remove attributes that
		// should not be searched.

		$criteria=new CDbCriteria;

		return new CActiveDataProvider('User', array(
			'criteria'=>$criteria,
		));
	}

	/**
	 * Getter on the model field
	 */
	public function getId()
	{
		return $this->Id;
	}

	public function getUsername()
	{
		return $this->Username;
	}

	public function getHashedPassword()
	{
		return $this->Password;
	}
	
		public function getEmail()
	{
		return $this->Email;
	}
		public function setEmail($email)
	{
		$this->Email=$email;
	}
	/**
	 * @return create an history line
	 */
	public static function createNew($username,$password,$email,$right)
	{
		$user = new User;
		
		$user->Password=md5($password);
		$user->Email=$email;
		$user->Username=$username;
		$user->Rights=$right;
		return $user;
	}
}
