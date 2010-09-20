<?php

/**
 * This is the model class for table "WebStream".
 *
 * The followings are the available columns in table 'WebStream':
 */
class WebStream extends CActiveRecord
{
	/**
	 * Returns the static model of the specified AR class.
	 * @return WebStream the static model class
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
		return 'wtvmT_WebStream';
	}

	/**
	 * @return array validation rules for model attributes.
	 */
	public function rules()
	{
		// NOTE: you should only define rules for those attributes that
		// will receive user inputs.
		return array(
            array('Name, Url', 'required'),
			array('Name', 'length', 'max'=>50),
			array('Url', 'length', 'max'=>255),
            array('RequiredIsp', 'length', 'max'=>50),
            array('LangCode', 'length', 'max'=>2),
			array('SubmissionDate', 'type', 'type'=>'date'),
			// The following rule is used by search().
			// Please remove those attributes that should not be searched.
			//array('', 'safe', 'on'=>'search'),
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
            'StreamStatus'=>array(self::BELONGS_TO, 'StreamStatus', 'StreamStatusCode'),
            'Lang'=>array(self::BELONGS_TO, 'Lang', 'LangCode')
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
		if(parent::beforeSave())
		{
			//$this->SubmissionDate=time();
			$time=time();
			$this->SubmissionDate=date('Y-m-d H:i:s', $time);;
		   /* if($this->isNewRecord)
		    {
		        $this->SubmissionDate=time();
		        //$this->author_id=Yii::app()->user->id;
		    }
		    /*else
		        $this->update_time=time();
			*/
		    return true;
		}
		else
		    return false;
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

		return new CActiveDataProvider('WebStream', array(
			'criteria'=>$criteria,
		));
	}
}
