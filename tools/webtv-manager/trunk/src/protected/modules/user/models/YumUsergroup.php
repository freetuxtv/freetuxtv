<?php

class YumUsergroup extends YumActiveRecord{
	public static function model($className=__CLASS__)
	{
		return parent::model($className);
	}

	public function tableName()
	{
		if(isset(Yum::module()->usergroupTable))
			$this->_tableName = Yum::module()->usergroupTable;
		elseif(isset(Yii::app()->modules['user']['usergroupTable']))
			$this->_tableName = Yii::app()->modules['user']['usergroupTable'];
		else
			$this->_tableName = 'wtvmT_UserGroup'; // fallback if nothing is set
		return Yum::resolveTableName($this->_tableName, $this->getDbConnection());
	}

	public function rules()
	{
		return array(
			array('title, description', 'required'),
			array('id, owner_id', 'numerical', 'integerOnly'=>true),
			array('title', 'length', 'max'=>255),
			array('id, title, description', 'safe', 'on'=>'search'),
		);
	}

	public function relations()
	{
		return array(
			'owner' => array(self::BELONGS_TO, 'YumUser', 'owner_id')
		);
	}

	public function attributeLabels()
	{
		return array(
			'id' => Yum::t('group id'),
			'title' => Yum::t('Group title'),
			'description' => Yum::t('Description'),
			'owner_id' => Yum::t('Group owner'),
		);
	}


	public function search()
	{
		$criteria=new CDbCriteria;

		$criteria->compare('id', $this->id);
		$criteria->compare('title', $this->title, true);
		$criteria->compare('description', $this->description, true);

		return new CActiveDataProvider(get_class($this), array(
			'criteria'=>$criteria,
		));
	}
}
