<?php
/**
 * This is the model class for table "{{roles}}".
 *
 * The followings are the available columns in table '{{roles}}':
 * @property integer $id
 * @property string $title
 * @property string $description
 * 
 * Relations
 * @property array $users array of YumUser
 */
class YumRole extends YumActiveRecord {
	private $_userRoleTable;
	private $_roleRoleTable;

	public static function model($className=__CLASS__)
	{
		return parent::model($className);
	}

	public function getAutoassignRoles() {
		$roles = array();
		foreach(YumRole::model()->autoassign()->findAll() as $role) {
			$roles[] = (int) $role->id;
		}

		return $roles;
	}

	/**
	 * Returns resolved table name (incl. table prefix when it is set in db configuration)
	 * Following algorith of searching valid table name is implemented:
	 *  - try to find out table name stored in currently used module
	 *  - if not found try to get table name from UserModule configuration
	 *  - if not found user default {{roles}} table name
	 * @return string
	 */		
	public function tableName()
	{
		if (isset(Yum::module()->rolesTable))
			$this->_tableName = Yum::module()->rolesTable;
		elseif (isset(Yii::app()->modules['user']['rolesTable'])) 
			$this->_tableName = Yii::app()->modules['user']['rolesTable'];
		else
			$this->_tableName = 'wtvmT_Roles'; // fallback if nothing is set
		return Yum::resolveTableName($this->_tableName,$this->getDbConnection());
	}

	public function rules()
	{
		return array(
				array('title', 'required'),
				array('selectable, searchable, autoassign, is_membership_possible', 'numerical'),
				array('price', 'numerical'),
				array('duration', 'numerical'),
				array('title, description', 'length', 'max' => '255'),
				);
	}


	public function scopes() {
		return array(
				'selectable' => array('condition' => 'selectable = 1'),
				'searchable' => array('condition' => 'searchable = 1'),
				'autoassign' => array('condition' => 'autoassign = 1'),
				'possible_memberships' => array('condition' => 'is_membership_possible = 1'),
				);
	}

	public function relations()
	{
		return array(
				'users'=>array(self::MANY_MANY, 'YumUser', Yii::app()->getModule('user')->userRoleTable . '(role_id, user_id)'),
				'permissions' => array(self::HAS_MANY, 'YumPermission', 'principal_id'),
				'memberships' => array(self::HAS_MANY, 'YumMembership', 'membership_id'),
				'managed_by' => array(self::HAS_MANY, 'YumPermission', 'subordinate_id'),

				);
	}

	public function attributeLabels()
	{
		return array(
				'id'=>Yum::t("#"),
				'title'=>Yum::t("Title"),
				'description'=>Yum::t("Description"),
				'selectable'=>Yum::t("Selectable on registration"),
				'searchable'=>Yum::t("Searchable"),
				'autoassign'=>Yum::t("Assign this role to new users automatically"),
				'price'=>Yum::t("Price"),
				'duration'=>Yum::t("Duration in days"),
				);
	}
}
