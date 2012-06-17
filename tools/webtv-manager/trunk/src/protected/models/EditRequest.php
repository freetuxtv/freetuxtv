<?php

/**
 * This is the model class for table "EditRequest".
 *
 * The followings are the available columns in table 'EditRequest':
 */
class EditRequest extends CActiveRecord
{
	const FIELD_STATUS_SUBMITTED = 0;
	const FIELD_STATUS_APPROVED = 1;
	const FIELD_STATUS_REJECTED = 2;

	const FIELD_WEBSTREAM_STATUS = 1;

	/**
	 * Returns the static model of the specified AR class.
	 * @return History the static model class
	 */
	public static function model($className=__CLASS__)
	{
		return parent::model($className);
	}

	/**
	 * @return string the associated database table name
	 */
	public function tableName()
	{
		return 'wtvmT_EditRequest';
	}

	/**
	 * @return array validation rules for model attributes.
	 */
	public function rules()
	{
		// NOTE: you should only define rules for those attributes that
		// will receive user inputs.
		return array(
            array('Status, Field, OldValue, NewValue', 'required'),
			array('OldValue, NewValue', 'length', 'max'=>100),
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
            'History'=>array(self::BELONGS_TO, 'History', 'HistoryId'),
			'UpdateHistory'=>array(self::BELONGS_TO, 'History', 'HistoryId'),
		);
	}

	/**
	 * @return array customized attribute labels (name=>label)
	 */
	public function attributeLabels()
	{
		return array(
			'Status'=>'Status',
			'Field'=>'Field',
			'OldValue'=>'Old value',
			'NewValue'=>'New value',
			'Value'=>'Value',
		);
	}
	
	protected function beforeValidate()
	{
		if(parent::beforeValidate())
		{
			if($this->isNewRecord) {
				$this->Status = 0;
			}
			return true;
		}
		return false;
	}

	protected function beforeSave()
	{
		if(parent::beforeSave())
		{
		    return true;
		}
		else
		    return false;
	}

	public function getStatusName()
	{
		switch($this->Status){
		case EditRequest::FIELD_STATUS_SUBMITTED:
			return 'Submitted';
		case EditRequest::FIELD_STATUS_APPROVED:
			return 'Approved';
		case EditRequest::FIELD_STATUS_REJECTED:
			return 'Rejected';
		}
	}

}
