<?php

/**
 * This is the model class for table "Comment".
 *
 * The followings are the available columns in table 'Comment':
 */
class Comment extends CActiveRecord
{

	/**
	 * Returns the static model of the specified AR class.
	 * @return Comment the static model class
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
		return 'wtvmT_Comment';
	}

	/**
	 * @return array validation rules for model attributes.
	 */
	public function rules()
	{
		// NOTE: you should only define rules for those attributes that
		// will receive user inputs.
		return array(
            array('IdWebStream, UserId, Value', 'required'),
			array('Date', 'type', 'type'=>'datetime', 'datetimeFormat'=>'yyyy-MM-dd HH:mm:ss'),
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
            'User'=>array(self::BELONGS_TO, 'User', 'UserId')
		);
	}

	/**
	 * @return array customized attribute labels (name=>label)
	 */
	public function attributeLabels()
	{
		return array(
			'Date'=>'Date',
		);
	}

	protected function beforeSave()
	{
		$this->Date=new CDbExpression('NOW()');
		if(parent::beforeSave())
		{
			$time=time();
			if($this->isNewRecord) {
				$this->UserId=Yii::app()->user->getId();
				$this->Date=new CDbExpression('NOW()');
			}
		    return true;
		}
		else
		    return false;
	}

	/**
	 * @return create a Comment line
	 */
	public static function createNew($IdWebStream, $Value)
	{
		$Comment = new Comment;
		
		$Comment->IdWebStream=$IdWebStream;
		$Comment->Value=$Value;
		return $Comment;
	}
}
