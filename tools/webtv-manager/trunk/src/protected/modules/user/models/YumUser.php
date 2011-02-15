<?php
/**
 * This is the model class for table "{{users}}".
 *
 * The followings are the available columns in table '{{users}}':
 * @property integer $id
 * @property string $username
 * @property string $password
 * @property string $activationKey
 * @property integer $createtime
 * @property integer $lastvisit
 * @property integer $superuser
 * @property integer $status
 * 
 * Relations
 * @property YumProfile $profile
 * @property array $roles array of YumRole
 * @property array $users array of YumUser
 * 
 * Scopes:
 * @property YumUser $active
 * @property YumUser $notactive
 * @property YumUser $banned
 * @property YumUser $superuser
 * 
 */
class YumUser extends YumActiveRecord
{
	const STATUS_NOTACTIVE = 0;
	const STATUS_ACTIVE = 1;
	const STATUS_BANNED = -1;

	public $username;
	public $password;
	public $email;
	private $_userRoleTable;
	private $_userUserTable;
	
	public static function model($className=__CLASS__)
	{
		return parent::model($className);
	}

	/**
	 * Returns resolved table name (incl. table prefix when it is set in db configuration)
	 * Following algorith of searching valid table name is implemented:
	 *  - try to find out table name stored in currently used module
	 *  - if not found try to get table name from UserModule configuration
	 *  - if not found user default {{users}} table name
	 * @return string
	 */			
	public function tableName()
	{
    	if (isset(Yii::app()->controller->module->usersTable))
      		$this->_tableName = Yii::app()->controller->module->usersTable;
    	elseif (isset(Yii::app()->modules['user']['usersTable'])) 
      		$this->_tableName = Yii::app()->modules['user']['usersTable'];
    	else
      		$this->_tableName = '{{users}}'; // fallback if nothing is set
      		//$this->_tableName = 'users'; // fallback if nothing is set

		return YumHelper::resolveTableName($this->_tableName,$this->getDbConnection());
	}

	public function rules()
	{
		return array(
			array('username', 'length', 'max'=>20, 'min' => 3,'message' => Yii::t("UserModule.user", "Incorrect username (length between 3 and 20 characters).")),
			array('password', 'length', 'max'=>128, 'min' => 4, 'message' => Yii::t("UserModule.user", "Incorrect password (minimal length 4 symbols).")),
			array('username', 'unique', 'message' => Yii::t("UserModule.user", "This user's name already exists.")),
			array('username', 'match', 'pattern' => '/^[A-Za-z0-9_]+$/u','message' => Yii::t("UserModule.user", "Incorrect symbol's. (A-z0-9)")),
			array('status', 'in', 'range'=>array(0,1,-1)),
			array('superuser', 'in', 'range'=>array(0,1)),
			array('username, createtime, lastvisit, superuser, status', 'required'),
			array('password', 'required', 'on'=>array('insert')),
			array('createtime, lastvisit, superuser, status', 'numerical', 'integerOnly'=>true),
		);
	}

	public function relations()
	{
    if (isset(Yii::app()->controller->module->userRoleTable))
      $this->_userRoleTable = Yii::app()->controller->module->userRoleTable;
    elseif (isset(Yii::app()->modules['user']['userRoleTable'])) 
      $this->_userRoleTable = Yii::app()->modules['user']['userRoleTable'];
    else
      $this->_userRoleTable = '{{user_has_role}}';

    if (isset(Yii::app()->controller->module->userUserTable))
      $this->_userUserTable = Yii::app()->controller->module->userUserTable;
    elseif (isset(Yii::app()->modules['user']['userUserTable'])) 
      $this->_userUserTable = Yii::app()->modules['user']['userUserTable'];
    else
      $this->_userUserTable = '{{user_has_user}}';
      
    #resolve table names to use them in relations definition
    $relationUHRTableName=YumHelper::resolveTableName($this->_userRoleTable,$this->getDbConnection());      
    $relationUHUTableName=YumHelper::resolveTableName($this->_userUserTable,$this->getDbConnection());

	return array(
		'profile'=>array(self::HAS_ONE, 'YumProfile', 'user_id', 'order' => 'profile.profile_id DESC'),
		'roles'=>array(self::MANY_MANY, 'YumRole', $relationUHRTableName . '(user_id, role_id)'),
		'users'=>array(self::MANY_MANY, 'YumUser', $relationUHUTableName . '(owner_id, child_id)'),
	);
	}

	public function register($username=null, $password=null, $email=null)
	{
		#this function can be used external to
		if($username!==null && $password!==null)
		{
			// Password equality is checked in Registration Form
			$this->username = $username;
			$this->password = $this->encrypt($password);
		}
		$this->activationKey = $this->generateActivationKey(false,$password);
		$this->createtime = time();
		$this->superuser = 0;

		if(YumWebModule::yum()->disableEmailActivation == true) 
			$this->status = YumUser::STATUS_ACTIVE;
		else
			$this->status = YumUser::STATUS_NOTACTIVE;

		$this->lastvisit = ((Yii::app()->user->allowAutoLogin &&
			UserModule::$allowInactiveAcctLogin) ? time() : 0);

		if($this->save()) 
		{
			$profile = new YumProfile();
			$profile->user_id = $this->id;
			$profile->save();
			return true;
		} 
		else
			return false;

	}

	/**
	 * Activation of an user account
	 */
	public function activate($email, $activationKey)
	{
		$find = YumProfile::model()->findByAttributes(array('email'=>$email))->user;
		if ($find->status) 
		{
			return true;
		} 
		elseif($find->activationKey == $activationKey) 
		{
			$find->activationKey = $this->generateActivationKey(true);
			$find->status = 1;
			$find->save();
			return true;
		}
		else
			return false;
	}
	/**
	 * @params boolean $activate Whether to generate activation key when user is registering first time (false)
	 * or when it is activating (true)
	 * @params string $password password entered by user	
	 * @param array $params, optional, to allow passing values outside class in inherited classes
	 * By default it uses password and microtime combination to generated activation key
	 * When user is activating, activation key becomes micortime()
	 * @return string
	 */
	public function generateActivationKey($activate=false,$password='',array $params=array())
	{
		return $activate ? $this->encrypt(microtime()) : $this->encrypt(microtime() . $this->password);
	}

	public function attributeLabels()
	{
		return array(
			'id'=>Yii::t('UserModule.user', '#'),
			'username'=>Yii::t("UserModule.user", "Username"),
			'password'=>Yii::t("UserModule.user", "Password"),
			'verifyPassword'=>Yii::t("UserModule.user", "Retype password"),
			'verifyCode'=>Yii::t("UserModule.user", "Verification code"),
			'activationKey' => Yii::t("UserModule.user", "Activation key"),
			'createtime' => Yii::t("UserModule.user", "Registration date"),
			'lastvisit' => Yii::t("UserModule.user", "Last visit"),
			'superuser' => Yii::t("UserModule.user", "Superuser"),
			'status' => Yii::t("UserModule.user", "Status"),
		);
	}
	
	/**
	 * This function is used for password encryption.
	 * @return hash string.
	 */
	public static function encrypt($string = "")
	{
		$salt = YumWebModule::yum()->salt;
		$hashFunc = YumWebModule::yum()->hashFunc;
		$string = sprintf("%s%s%s", $salt, $string, $salt);
		
		if(!function_exists($hashFunc))
			throw new CException('Function `'.$hashFunc.'` is not a valid callback for hashing algorithm.');
		
		return $hashFunc($string);
	}
	
	public function scopes()
	{
		return array(
			'active'=>array(
				'condition'=>'status='.self::STATUS_ACTIVE,
			),
			'notactive'=>array(
				'condition'=>'status='.self::STATUS_NOTACTIVE,
			),
			'banned'=>array(
				'condition'=>'status='.self::STATUS_BANNED,
			),
			'superuser'=>array(
				'condition'=>'superuser=1',
			),
		);
	}

	public static function itemAlias($type,$code=NULL) 
	{
		$_items = array(
			'UserStatus' => array(
				'0' => Yii::t("UserModule.user", 'Not active'),
				'1' => Yii::t("UserModule.user", 'Active'),
				'-1'=> Yii::t("UserModule.user", 'Banned'),
			),
			'AdminStatus' => array(
				'0' => Yii::t("UserModule.user", 'No'),
				'1' => Yii::t("UserModule.user", 'Yes'),
			),
		);
		if (isset($code))
			return isset($_items[$type][$code]) ? $_items[$type][$code] : false;
		else
			return isset($_items[$type]) ? $_items[$type] : false;
	}

	/**
	 * Return admins.
	 * @return array syperusers names
	 */	
	public static function getAdmins() {
		$admins = YumUser::model()->active()->superuser()->findAll();
		$returnarray = array();
		foreach ($admins as $admin)
			array_push($returnarray, $admin->username);
		return $returnarray;
	}

	
}
