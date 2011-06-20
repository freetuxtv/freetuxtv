<?php

/**
 * This is the model class for table "Restriction".
 *
 * The followings are the available columns in table 'WebStreamRestrictionAccess ':
 */
class Restriction extends CActiveRecord
{
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
		return 'wtvmT_WebstreamRestrictionAccess';
	}

	/**
	 * @return array validation rules for model attributes.
	 */
 	public function rules()
	{
		// NOTE: you should only define rules for those attributes that
		// will receive user inputs.
		return array(
            array('IdWebStream, IdTypeRestrictionAccess, Value', 'required'),
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

	protected function beforeSave()
	{
		return true;
	}

	/**
	 * @return create a Restriction value line
	 */
	public static function createNewValue($IdWebStream,$IdTypeRestrictionAccess,$Value)
	{
		$Restriction = new Restriction;
		
		$Restriction->IdWebStream=$IdWebStream;
		$Restriction->IdTypeRestrictionAccess=$IdTypeRestrictionAccess;
		$Restriction->Value=$Value;
		return $Restriction;
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

		return new CActiveDataProvider('Restriction', array(
			'criteria'=>$criteria,
		));
	}
}
