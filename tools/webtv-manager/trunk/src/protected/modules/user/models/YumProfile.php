<?php
/**
 * This is the model class for table "{{profiles}}".
 *
 * The followings are the available columns in table '{{profiles}}':
 * @property integer $profile_id
 * @property integer $user_id
 * @property integer $timestamp
 * @property string $privacy
 * @property string $lastname  //FIXME: Shouldn't be mandatory field
 * @property string $firstname //FIXME: Shouldn't be mandatory field
 * @property string $email
 * @property string $about
 * @property string $street
 * 
 * Relations:
 * @property YumUser $user
 */

Yii::import('application.modules.user.components.Compare');
class YumProfile extends YumActiveRecord
{
	const PRIVACY_PRIVATE = 'private';
	const PRIVACY_PUBLIC = 'public';

	/**
	 * @var array of YumProfileFields
	 */
	static $fields=null;

	public function init()
	{
		parent::init();
 // load profile fields only once
		$this->loadProfileFields();
	}

	public function behaviors()  {
		return array_merge(parent::behaviors(), array(
					'Compare' => array(
						'class' => 'Compare'))); 
	}


	/**
	 * @param string $className
	 * @return YumProfile
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
	 *  - if not found user default {{profiles}} table name
	 * @return string
	 */
  	public function tableName()
  	{
    	if (isset(Yum::module()->profileTable))
      		$this->_tableName = Yum::module()->profileTable;
    	elseif (isset(Yii::app()->modules['user']['profileTable'])) 
      		$this->_tableName = Yii::app()->modules['user']['profileTable'];
    	else
      		$this->_tableName = 'wtvmT_Profiles'; // fallback if nothing is set

    	return Yum::resolveTableName($this->_tableName,$this->getDbConnection());
  	}

	public function rules()
	{
		$required = array();
		$numerical = array();           
		$rules = array();
		$safe = array();

		foreach (self::$fields as $field)
		{
			$field_rule = array();

			if ($field->required == 1)
				array_push($required, $field->varname);

			if ($field->field_type == 'int' 
					|| $field->field_type == 'FLOAT'
					|| $field->field_type =='INTEGER')
				array_push($numerical,$field->varname);

			if ($field->field_type == 'DROPDOWNLIST')
				array_push($safe, $field->varname);


			if ($field->field_type =='VARCHAR' 
					||$field->field_type=='TEXT') 
			{
				$field_rule = array($field->varname,
						'length',
						'max'=>$field->field_size,
						'min' => $field->field_size_min);

				if ($field->error_message)
					$field_rule['message'] = Yii::t("UserModule.user", $field->error_message);
				array_push($rules,$field_rule);
			}

			if ($field->field_type=='DATE') 
			{
				$field_rule = array($field->varname,
						'type',
						'type' => 'date',
						'dateFormat' => 'yyyy-mm-dd');

				if ($field->error_message)
					$field_rule['message'] = Yum::t( $field->error_message);
				array_push($rules,$field_rule);
			}

			if ($field->match) 
			{
				$field_rule = array($field->varname,
						'match',
						'pattern' => $field->match);

				if ($field->error_message)
					$field_rule['message'] = Yum::t( $field->error_message);
				array_push($rules,$field_rule);
			}
			if ($field->range) 
			{
				// allow using commas and semicolons
				$range=explode(';',$field->range);
				if(count($range)===1)
					$range=explode(',',$field->range);
				$field_rule = array($field->varname,'in','range' => $range);

				if ($field->error_message)
					$field_rule['message'] = Yum::t( $field->error_message);
				array_push($rules,$field_rule);
			}

			if ($field->other_validator) 
			{
				$field_rule = array($field->varname,
						$field->other_validator);

				if ($field->error_message)
					$field_rule['message'] = Yum::t( $field->error_message);
				array_push($rules, $field_rule);
			}

		}

		array_push($rules,
				array(implode(',',$required), 'required'));
		array_push($rules,
				array(implode(',',$numerical), 'numerical', 'integerOnly'=>true));
		array_push($rules,
				array(implode(',',$safe), 'safe'));

		$rules[] = array('allow_comments, show_friends', 'numerical');
		$rules[] = array('privacy', 'safe');

		return $rules;
	}

	public function relations()
	{
		$relations = array(
			'user' => array(self::BELONGS_TO, 'YumUser', 'user_id'),
		);
		
		$fields = Yii::app()->db->createCommand(
				"select * from ".YumProfileField::model()->tableName()." where field_type = 'DROPDOWNLIST'")->queryAll();

		foreach($fields as $field) {
			$relations[ucfirst($field['varname'])] = array(self::BELONGS_TO, ucfirst($field['varname']), $field['varname']);

		}

		return $relations;

	}

	public function attributeLabels()
	{
		$labels = array(
			'user_id' => Yum::t('User ID'),
			'profile_id' => Yum::t('Profile ID'),
			'privacy' => Yum::t('Privacy'),
			'show_friends' => Yum::t('Show friends'),
			'allow_comments' => Yum::t('Allow profile comments'),
		);

		foreach (self::$fields as $field)
			$labels[$field->varname] = Yii::t("UserModule.user", $field->title);

		return $labels;
	}

    /**
    * Load profile fields.
    * Overwrite this method to get another set of fields
    * @since 0.6
    * @return array of YumProfileFields or empty array
    */
    public function loadProfileFields()
    {
        if(self::$fields===null)
        {
            self::$fields=YumProfileField::model()->forOwner()->findAll();
            if(self::$fields==null)
                self::$fields=array();
        }
        return self::$fields;
    }
	
	/**
	 * Returns formatted name
	 * With default fomatting it returns firstname and lastname 
	 * concatenated and separated with space.
	 * As placeholders in template you can use all profile properties
	 * @param string $template
	 * @since 0.6
	 * @return string
	 * FIXME: Should be in extended class
	 */
/*	public function getFormattedName($template='{firstname} {lastname}')
	{
		foreach($this->getAttributes() as $key=>$attribute)
			$tr["{{$key}}"]=$attribute;
		return strtr($template,$tr);
	}
*/

}

