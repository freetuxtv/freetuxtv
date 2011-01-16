<?php

/**
 * This is the model class for table "{{profile_fields_group}}".
 *
 * The followings are the available columns in table '{{profile_fields_group}}':
  * @property integer $id
  * @property string $group_name
  * @property string $title
  * @property integer $position
  * 
  * Relations
  * @property array $fields array of YumProfileField
  */
class YumProfileFieldsGroup extends YumActiveRecord
{
	
    /**
     * Returns the static model of the specified AR class.
     * @return YumProfileFieldsGroup the static model class
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
	 *  - if not found user default {{profile_fields_group}} table name
	 * @return string
	 */	
  	public function tableName()
  	{
    	if (isset(Yum::module()->profileFieldsGroupTable))
      		$this->_tableName = Yum::module()->profileFieldsGroupTable;
    	elseif (isset(Yii::app()->modules['user']['profileFieldsGroupTable']))
      		$this->_tableName = Yii::app()->modules['user']['profileFieldsGroupTable'];
    	else
      		$this->_tableName = '{{profile_fields_group}}'; // fallback if nothing is set

    	return Yum::resolveTableName($this->_tableName,$this->getDbConnection());
  	}

    /**
     * @return array validation rules for model attributes.
     */
    public function rules()
    {
        return array(
            array('group_name, title', 'required'),
            array('group_name', 'unique'),
            array('group_name', 'match', 'pattern' => '/^[a-z_0-9]+$/u','message' => Yii::t("UserModule.user", "Incorrect symbol's. (a-z)")),
            array('position', 'numerical', 'integerOnly'=>true),
            array('group_name', 'length', 'max'=>50),
            array('title', 'length', 'max'=>255),
			#on search
            array('id, group_name, title, position', 'safe', 'on'=>'search'),
        );
    }

    /**
     * @return array relational rules.
     */
    public function relations()
    {
        return array(
        	'fields'=>array(self::HAS_MANY, 'YumProfileField', 'field_group_id'),
        );
    }

    /**
     * @return array customized attribute labels (name=>label)
     */
    public function attributeLabels()
    {
        return array(
            'id' => Yii::t("UserModule.user", '#'),
            'group_name' => Yii::t("UserModule.user", 'Group name'),
            'title' => Yii::t("UserModule.user", 'Title'),
            'position' => Yii::t("UserModule.user", 'Position'),
        );
    }

    /**
     * Retrieves a list of models based on the current search/filter conditions.
     * @return CActiveDataProvider the data provider that can return the models based on the search/filter conditions.
     */
    public function search()
    {

        $criteria=new CDbCriteria;
        $criteria->compare('id',$this->id);
        $criteria->compare('group_name',$this->group_name,true);
        $criteria->compare('title',$this->title,true);
        $criteria->compare('position',$this->position);

        return new CActiveDataProvider(get_class($this), array(
            'criteria'=>$criteria,
        ));
    }
    
} 
