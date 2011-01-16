<?php

/**
 * This is the model class for a User in Yum
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
class YumUser extends YumActiveRecord {
	const STATUS_NOTACTIVE = 0;
	const STATUS_ACTIVE = 1;
	const STATUS_BANNED = -1;

	public $username;
	public $password;
	public $email;
	public $activationKey;
	public $password_changed = false;
	private $_userRoleTable;
	private $_userUserTable;
	private $_friendshipTable;

	public static function model($className=__CLASS__) {
		return parent::model($className);
	}

	public static function generatePassword() { 
		$consonants = array("b","c","d","f","g","h","j","k","l","m","n","p","r","s","t","v","w","x","y","z"); 
		$vocals = array("a","e","i","o","u"); 

		$password = '';

		srand((double) microtime() * 1000000);
		for ($i = 1; $i <= 4; $i++) {
			$password .= $consonants[rand(0, 19)];
			$password .= $vocals[rand(0, 4)];
		}
		$password .= rand(0, 9);

		return $password;
	}

	public function getActiveMemberships() {
		$roles = array();
		foreach($this->memberships as $membership) {
			if($membership->end_date > time())
				$roles[] = $membership->role;
		}

		return $roles;
	}

	public function search() {
		$criteria = new CDbCriteria;

		$criteria->compare('t.id', $this->id, true);
		$criteria->compare('t.username', $this->username, true);
		$criteria->compare('t.status', $this->status);
		$criteria->compare('t.superuser', $this->superuser);
		$criteria->compare('t.createtime', $this->createtime, true);
		$criteria->compare('t.lastvisit', $this->lastvisit, true);

		return new CActiveDataProvider(get_class($this), array(
					'criteria' => $criteria,
					'pagination' => array('pageSize' => 20),
					));
	}

	public function beforeValidate() {

		if ($this->isNewRecord)
			$this->createtime = time();

		$file = CUploadedFile::getInstanceByName('YumUser[avatar]');
		if ($file instanceof CUploadedFile)
			$this->avatar = $file;
		else if ($this->scenario == 'avatarUpload')
			$this->avatar = NULL;

		return true;
	}

	public function beforeSave() {
		if (Yum::module()->enableAvatar)
			$this->updateAvatar();

		if ($this->password_changed)
			$this->password = YumUser::encrypt($this->password);

		return parent::beforeSave();
	}

	public function setPassword($password) {
		if ($password != '') {
			$this->password = $password;
			$this->lastpasswordchange = time();
			$this->password_changed = true;
			return $this->save();
		}
	}

	public function afterSave() {
		$setting = YumPrivacySetting::model()->findByPk($this->id);
		if (!$setting) {
			$setting = new YumPrivacySetting();
			$setting->save();
		}

/*		if(Yum::module()->enableProfiles && !isset($this->profile[0])) {
			$profile = new YumProfile();
			$profile->user_id = $this->id;
			$profile->save(false);
		} */
			

		if(Yum::module()->enableLogging)
			YumActivityController::logActivity($this,
					$this->isNewRecord ? 'user_created' : 'user_updated');

		return parent::afterSave();
	}

	public function getAdministerableUsers() {
		$users = array();
		$users = $this->users;
		foreach ($this->roles as $role) {
			if ($role->roles)
				foreach ($role->roles as $role)
					$users = array_merge($this->users, $role->users);
		}

		return $users;
	}

	/**
	 * Returns resolved table name (incl. table prefix when it is set in db configuration)
	 * Following algorith of searching valid table name is implemented:
	 *  - try to find out table name stored in currently used module
	 *  - if not found try to get table name from UserModule configuration
	 *  - if not found user default {{users}} table name
	 * @return string
	 */
	public function tableName() {
		if (isset(Yum::module()->usersTable))
			$this->_tableName = Yum::module()->usersTable;
		elseif (isset(Yii::app()->modules['user']['usersTable']))
			$this->_tableName = Yii::app()->modules['user']['usersTable'];
		else
			$this->_tableName = 'wtvmT_Users'; // fallback if nothing is set

		return Yum::resolveTableName($this->_tableName, $this->getDbConnection());
	}

	public function rules() {
		$passwordRequirements = Yum::module()->passwordRequirements;
		$usernameRequirements = Yum::module()->usernameRequirements;

		$passwordrule = array_merge(array('password', 'YumPasswordValidator'), $passwordRequirements);

		$rules[] = $passwordrule;

		$rules[] = array('username', 'length',
				'max' => $usernameRequirements['maxLen'],
				'min' => $usernameRequirements['minLen'],
				'message' => Yum::t(
					'Username length needs to be between {minLen} and {maxlen} characters', array(
						'{minLen}' => $usernameRequirements['minLen'],
						'{maxLen}' => $usernameRequirements['maxLen'])));

		$rules[] = array('username', 'unique', 'message' => Yum::t("This user's name already exists."));
		$rules[] = array('username', 'match', 'pattern' => '/^[A-Za-z0-9_]+$/u', 'message' => Yum::t('Incorrect symbol\'s. (A-z0-9)'));
		$rules[] = array('status', 'in', 'range' => array(0, 1, -1));
		$rules[] = array('superuser', 'in', 'range' => array(0, 1));
		$rules[] = array('createtime, lastvisit, lastpasswordchange, superuser, status', 'required');
		$rules[] = array('notifyType', 'safe');
		$rules[] = array('password', 'required', 'on' => array('insert'));
		$rules[] = array('createtime, lastvisit, superuser, status', 'numerical', 'integerOnly' => true);

		if (Yum::module()->enableAvatar) {
			$rules[] = array('avatar', 'required', 'on' => 'avatarUpload');
			$rules[] = array('avatar', 'EPhotoValidator',
					'allowEmpty' => true,
					'mimeType' => array('image/jpeg', 'image/png', 'image/gif'),
					'maxWidth' => 200,
					'maxHeight' => 200,
					'minWidth' => 50,
					'minHeight' => 50);
		}
		return $rules;
	}

	public function hasRole($role_title) {
		foreach ($this->roles as $role)
			if ($role->id == $role_title || $role->title == $role_title)
				return true;

		return false;
	}

	public function getRoles() {
		$roles = '';
		foreach ($this->roles as $role)
			$roles .= ' ' . $role->title;

		return $roles;
	}

	public function can($action) {
		foreach ($this->permissions as $permission)
			if ($permission->action->title == $action)
				return true;

		return false;
	}

	public function relations() {
		if (isset(Yum::module()->userRoleTable))
			$this->_userRoleTable = Yum::module()->userRoleTable;
		elseif (isset(Yii::app()->modules['user']['userRoleTable']))
			$this->_tableName = Yii::app()->modules['user']['userRoleTable'];
		else
			$this->_userRoleTable = 'wtvmT_User_Has_Role';

		if (isset(Yum::module()->friendshipTable))
			$this->_friendshipTable = Yum::module()->friendshipTable;
		elseif (isset(Yii::app()->modules['user']['friendshipTable']))
			$this->_tableName = Yii::app()->modules['user']['friendshipTable'];
		else
			$this->_friendshipTable = 'wtvmT_Friendship';

		// resolve table names to use them in relations definition
		$relationUHRTableName = Yum::resolveTableName($this->_userRoleTable, $this->getDbConnection());
		$relationFRSPTableName = Yum::resolveTableName($this->_friendshipTable, $this->getDbConnection());

		return array(
				'permissions' => array(self::HAS_MANY, 'YumPermission', 'principal_id'),
				'managed_by' => array(self::HAS_MANY, 'YumPermission', 'subordinate_id'),
				'messages' => array(self::HAS_MANY, 'YumMessage', 'to_user_id', 'order' => 'messages.id DESC'),
				'visits' => array(self::HAS_MANY, 'YumProfileVisit', 'visited_id'),
				// An example on how the default_profile relation should be
				//			'default_profile' => array(self::HAS_ONE, 'YumProfile', 'user_id', 'condition' => "profile_id = $this->default_profile"),
				// And then we rename this one below to 'profiles'
				'profile' => array(self::HAS_MANY, 'YumProfile', 'user_id', 'order' => 'profile.profile_id DESC'),
				'friendships' => array(self::HAS_MANY, 'YumFriendship', 'inviter_id'),
				'friendships2' => array(self::HAS_MANY, 'YumFriendship', 'friend_id'),
				'friendship_requests' => array(self::HAS_MANY, 'YumFriendship', 'friend_id', 'condition' => 'status = 1'), // 1 = FRIENDSHIP_REQUEST
				'roles' => array(self::MANY_MANY, 'YumRole', $relationUHRTableName . '(user_id, role_id)'),
				'memberships' => array(self::HAS_MANY, 'YumMembership', 'user_id'),
				'privacy' => array(self::HAS_ONE, 'YumPrivacySetting', 'user_id'),
				);
	}

	public function isFriendOf($invited_id) {
		foreach ($this->getFriendships() as $friendship) {
			if ($friendship->inviter_id == $this->id && $friendship->friend_id == $invited_id)
				return $friendship->status;
		}

		return false;
	}

	public function getFriendships() {
		$condition = 'inviter_id = :uid or friend_id = :uid';
		return YumFriendship::model()->findAll($condition, array(
					':uid' => $this->id));
	}

	// Friends can not be retrieve via the relations() method because a friend
	// can either be in the invited_id or in the friend_id column.
	// set $everything to true to also return pending and rejected friendships
	public function getFriends($everything = false) {
		if ($everything)
			$condition = 'inviter_id = :uid';
		else
			$condition = 'inviter_id = :uid and status = 2';

		$friends = array();
		$friendships = YumFriendship::model()->findAll($condition, array(
					':uid' => $this->id));
		if ($friendships != NULL && !is_array($friendships))
			$friendships = array($friendships);

		if ($friendships)
			foreach ($friendships as $friendship)
				$friends[] = YumUser::model()->findByPk($friendship->friend_id);

		if ($everything)
			$condition = 'friend_id = :uid';
		else
			$condition = 'friend_id = :uid and status = 2';

		$friendships = YumFriendship::model()->findAll($condition, array(
					':uid' => $this->id));

		if ($friendships != NULL && !is_array($friendships))
			$friendships = array($friendships);


		if ($friendships)
			foreach ($friendships as $friendship)
				$friends[] = YumUser::model()->findByPk($friendship->inviter_id);

		return $friends;
	}

	public function register($username=null, $password=null, $email=null) {
		if ($username !== null && $password !== null) {
			// Password equality is checked in Registration Form
			$this->username = $username;
			$this->password = $this->encrypt($password);
		}

		foreach (YumProfile::model()->findAll() as $profile) {
			if ($email == $profile->email)
				return false;
		}

		$this->activationKey = $this->generateActivationKey(false, $password);
		$this->createtime = time();
		$this->superuser = 0;


		switch (Yum::module()->registrationType) {
			case YumRegistration::REG_SIMPLE:
				$this->status = YumUser::STATUS_ACTIVE;
				break;
			case YumRegistration::REG_NO_PASSWORD:
			case YumRegistration::REG_EMAIL_CONFIRMATION:
			case YumRegistration::REG_CONFIRMATION_BY_ADMIN:
				$this->status = YumUser::STATUS_NOTACTIVE;
				break;
			case YumRegistration::REG_NO_PASSWORD_ADMIN_CONFIRMATION:
			case YumRegistration::REG_EMAIL_AND_ADMIN_CONFIRMATION:
				// Users stay banned until they confirm their email address.
				$this->status = YumUser::STATUS_BANNED;
				break;
		}

		if(Yum::module()->enableRoles) 
			$this->roles = YumRole::getAutoassignRoles(); 

		return $this->save();

	}

	public function isPasswordExpired() {
		$distance = Yii::app()->getModule('user')->password_expiration_time * 60 * 60;
		return $this->lastpasswordchange - $distance > time();
	}

	/**
	 * Activation of an user account
	 */
	public function activate($email=null, $activationKey=null) {
		if (isset($email) && isset($activationKey)) {
			$find = YumProfile::model()->findByAttributes(array('email' => $email))->user;
			if ($find->status == 1) {
				$return = true;
			} else if ($find->activationKey == $activationKey) {
				$find->activationKey = $find->generateActivationKey(true);
				$find->status = 1;
				$find->save();
				$return = true;
			} else {
				$return = false;
			}
		} else {
			$return = false;
		}
		return $return;
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
	public function generateActivationKey($activate=false, $password='', array $params=array()) {
		return $activate ? YumUser::encrypt(microtime()) : YumUser::encrypt(microtime() . $this->password);
	}

	public function attributeLabels() {
		return array(
				'id' => Yum::t('#'),
				'username' => Yum::t("Username"),
				'password' => Yum::t("Password"),
				'verifyPassword' => Yum::t("Retype password"),
				'verifyCode' => Yum::t("Verification code"),
				'activationKey' => Yum::t("Activation key"),
				'createtime' => Yum::t("Registration date"),
				'lastvisit' => Yum::t("Last visit"),
				'superuser' => Yum::t("Superuser"),
				'status' => Yum::t("Status"),
				'avatar' => Yum::t("Avatar image"),
				);
	}

	/**
	 * This function is used for password encryption.
	 * @return hash string.
	 */
	public static function encrypt($string = "") {
		$salt = Yum::module()->salt;
		$hashFunc = Yum::module()->hashFunc;
		$string = sprintf("%s%s%s", $salt, $string, $salt);

		if (!function_exists($hashFunc))
			throw new CException('Function `' . $hashFunc . '` is not a valid callback for hashing algorithm.');

		return $hashFunc($string);
	}

	public function scopes() {
		return array(
				'active' => array('condition' => 'status=' . self::STATUS_ACTIVE,),
				'notactive' => array('condition' => 'status=' . self::STATUS_NOTACTIVE,),
				'banned' => array('condition' => 'status=' . self::STATUS_BANNED,),
				'superuser' => array('condition' => 'superuser=1',),
				'public' => array(
					'join' => 'LEFT JOIN privacysetting on t.id = privacysetting.user_id',
					'condition' => 'appear_in_search = 1',),
				);
	}

	public static function itemAlias($type, $code=NULL) {
		$_items = array(
				'NotifyType' => array(
					'None' => Yum::t('None'),
					'Digest' => Yum::t('Digest'),
					'Instant' => Yum::t('Instant'),
					),
				'UserStatus' => array(
					'0' => Yum::t('Not active'),
					'1' => Yum::t('Active'),
					'-1' => Yum::t('Banned'),
					),
				'AdminStatus' => array(
					'0' => Yum::t('No'),
					'1' => Yum::t('Yes'),
					),
				);
		if (isset($code))
			return isset($_items[$type][$code]) ? $_items[$type][$code] : false;
		else
			return isset($_items[$type]) ? $_items[$type] : false;
	}

	/**
	 * Get all users with a specified role.
	 * @param string $roleTitle title of role to be searched
	 * @return array users with specified role. Null if none are found.
	 */
	public static function getUsersByRole($roleTitle) {
		$role = YumRole::model()->findByAttributes(array('title' => $roleTitle));
		return $role ? $role->users : null;
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

	public function updateAvatar() {
		if ($this->avatar !== NULL && isset($_FILES['YumUser'])) {
			// prepend user id to avoid conflicts when two users upload an avatar
			// with the same file name
			$filename = $this->id . '_' . $_FILES['YumUser']['name']['avatar'];
			if (is_object($this->avatar)) {
				$this->avatar->saveAs(Yii::app()->getModule('user')->avatarPath . '/' . $filename);
				$this->avatar = $filename;
			}
		}
	}

	public function getAvatar($friend = null, $thumb = false) {
		if (Yum::module()->enableAvatar) {
			$return = '<div class="avatar">';
			if (!is_object($friend))
				$friend = $this;

			$options = array();
			if ($thumb)
				$options = array('style' => 'width: 50px; height:50px;');

			if (isset($friend->avatar) && $friend->avatar)
				$return .= CHtml::image(Yii::app()->baseUrl . '/'
						. Yum::module()->avatarPath . '/'
						. $friend->avatar, 'Avatar', $options);
			else
				$return .= CHtml::image(Yii::app()->getAssetManager()->publish(
							Yii::getPathOfAlias('YumAssets.images') . ($thumb ? '/no_avatar_available_thumb.jpg' : '/no_avatar_available.jpg'),
							Yum::t('No image available'), array(
								'title' => Yum::t('No image available'))));
			$return .= '</div><!-- avatar -->';
			return $return;
		}
	}
}
