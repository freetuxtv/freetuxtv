<?php
/**
 * This is the model class for table "{{profile_fields}}".
 *
 * The followings are the available columns in table '{{profile_fields}}':
 * Fields:
 * @property integer $id
 * @property integer $field_group_id
 * @property string $varname
 * @property string $title
 * @property string $hint
 * @property string $field_type
 * @property integer $field_size
 * @property integer $field_size_min
 * @property integer $required
 * @property string $match
 * @property string $range
 * @property string $error_message
 * @property string $other_validator
 * @property string $default
 * @property integer $position
 * @property integer $visible
 * 
 * Relations
 * @property YumProfileFieldsGroup $group
 * 
 * Scopes:
 * @method YumProfileField forAll
 * @method YumProfileField forUser
 * @method YumProfileField forOwner
 * @method YumProfileField forRegistration
 * @method YumProfileField sort
 */
class YumProfileField extends YumActiveRecord
{
	const VISIBLE_ALL=3;
	const VISIBLE_REGISTER_USER=2;
	const VISIBLE_ONLY_OWNER=1;
	const VISIBLE_NO=0;

	/**
     * Returns the static model of the specified AR class.
	 * @param string $className
	 * @return YumProfileField
	 */
	public static function model($className=__CLASS__)
	{
		return parent::model($className);
	}


	/**
	 * Returns resolved table name (incl. table prefix when it is set in db configuration)
	 * Following algorith of searching valid table name is implemented:
	 *  - try to find out table name stored in currently used module
	 *  - if not found try to get table name from UserModule configuration
	 *  - if not found user default {{profile_fields}} table name
	 * @return string
	 */	
  	public function tableName()
  	{
    	if (isset(Yii::app()->controller->module->profileFieldsTable))
      		$this->_tableName = Yii::app()->controller->module->profileFieldsTable;
    	elseif (isset(Yii::app()->modules['user']['profileFieldsTable']))
      		$this->_tableName = Yii::app()->modules['user']['profileFieldsTable'];
    	else
      		$this->_tableName = '{{profile_fields}}'; // fallback if nothing is set

    	return YumHelper::resolveTableName($this->_tableName,$this->getDbConnection());
  	}

	public function rules()
	{
		return array(
			array('varname, title, field_type', 'required'),
			array('varname', 'match', 'pattern' => '/^[a-z_0-9]+$/u','message' => Yii::t("UserModule.user", "Incorrect symbol's. (a-z)")),
			array('varname', 'unique', 'message' => Yii::t("UserModule.user", "This field already exists.")),
			array('varname, field_type', 'length', 'max'=>50),
			array('field_group_id, field_size, field_size_min, required, position, visible', 'numerical', 'integerOnly'=>true),
			array('hint','safe'),
			array('title, match, range, error_message, other_validator, default', 'length', 'max'=>255),
		);
	}

	public function relations()
	{
		return array(
			'group'=>array(self::BELONGS_TO, 'YumProfileFieldsGroup', 'field_group_id')
		);
	}

	public function attributeLabels()
	{
		return array(
			'id' => Yii::t("UserModule.user", '#'),
			'field_group_id' => Yii::t("UserModule.user", 'Field group'),
			'varname' => Yii::t("UserModule.user", 'Variable name'),
			'title' => Yii::t("UserModule.user", 'Title'),
			'hint' => Yii::t("UserModule.user", 'Hint'),
			'field_type' => Yii::t("UserModule.user", 'Field type'),
			'field_size' => Yii::t("UserModule.user", 'Field size'),
			'field_size_min' => Yii::t("UserModule.user", 'Field size min'),
			'required' => Yii::t("UserModule.user", 'Required'),
			'match' => Yii::t("UserModule.user", 'Match'),
			'range' => Yii::t("UserModule.user", 'Range'),
			'error_message' => Yii::t("UserModule.user", 'Error message'),
			'other_validator' => Yii::t("UserModule.user", 'Other validator'),
			'default' => Yii::t("UserModule.user", 'Default'),
			'position' => Yii::t("UserModule.user", 'Position'),
			'visible' => Yii::t("UserModule.user", 'Visible'),
		);
	}
	
	public function scopes()
    {
        return array(
            'forAll'=>array(
                'condition'=>'visible='.self::VISIBLE_ALL,
            ),
            'forUser'=>array(
                'condition'=>'visible>='.self::VISIBLE_REGISTER_USER,
            ),
            'forOwner'=>array(
                'condition'=>'visible>='.self::VISIBLE_ONLY_OWNER,
            ),
            'forRegistration'=>array(
                'condition'=>'required>0',
            ),
            'sort'=>array(
                'order'=>'field_group_id ASC, t.position ASC',
            ),
            
        );
    }

	
	public static function itemAlias($type,$code=NULL) {
		$_items = array(
			'field_type' => array(
				'INTEGER' => Yii::t("UserModule.user", 'INTEGER'),
				'VARCHAR' => Yii::t("UserModule.user", 'VARCHAR'),
				'TEXT'=> Yii::t("UserModule.user", 'TEXT'),
				'DATE'=> Yii::t("UserModule.user", 'DATE'),
			//	'FLOAT'=> Yii::t("user", 'FLOAT'),
			//	'BOOL'=> Yii::t("user", 'BOOL'),
			//	'BLOB'=> Yii::t("user", 'BLOB'),
			//	'BINARY'=> Yii::t("user", 'BINARY'),
			//	'FILE'=> 'FILE',
			),
			'required' => array(
				'0' => Yii::t("UserModule.user", 'No'),
				'2' => Yii::t("UserModule.user", 'No, but show on registration form'),
				'1' => Yii::t("UserModule.user", 'Yes and show on registration form'),
			),
			'visible' => array(
				self::VISIBLE_ALL => Yii::t("UserModule.user", 'For all'),
				self::VISIBLE_REGISTER_USER => Yii::t("UserModule.user", 'Registered users'),
				self::VISIBLE_ONLY_OWNER => Yii::t("UserModule.user", 'Only owner'),
				'0' => Yii::t("UserModule.user", 'Hidden'),
			),
		);
		if (isset($code))
			return isset($_items[$type][$code]) ? $_items[$type][$code] : false;
		else
			return isset($_items[$type]) ? $_items[$type] : false;
	}
}
