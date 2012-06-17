<?php

/**
 * This is the model class for table "History".
 *
 * The followings are the available columns in table 'History':
 */
class History extends CActiveRecord
{
	const ENTITYTYPE_WEBSTREAM = 1;

	const ACTIONTYPE_WEBSTREAM_IMPORT = 1;
	const ACTIONTYPE_WEBSTREAM_ADD = 2;
	const ACTIONTYPE_WEBSTREAM_EDIT = 3;
	const ACTIONTYPE_WEBSTREAM_CHANGESTATUS = 4;
	const ACTIONTYPE_WEBSTREAM_EDITREQUEST = 5;
	const ACTIONTYPE_WEBSTREAM_EDITREQUESTREJECT = 6;

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
		return 'wtvmT_History';
	}

	/**
	 * @return array validation rules for model attributes.
	 */
	public function rules()
	{
		// NOTE: you should only define rules for those attributes that
		// will receive user inputs.
		return array(
            array('ActionType, EntityType, EntityId', 'required'),
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
            'User'=>array(self::BELONGS_TO, 'User', 'UserId'),
            'WebStream'=>array(self::BELONGS_TO, 'WebStream', 'EntityId'),
            'Comments'=>array(self::HAS_ONE, 'Comment', 'HistoryId'),
            'EditRequest'=>array(self::HAS_MANY, 'EditRequest', 'HistoryId'),
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
				$this->RemoteAddr=Yii::app()->getRequest()->getUserHostAddress();
			}
		    return true;
		}
		else
		    return false;
	}

	public function getHistoryActionName()
	{
		switch($this->EntityType){
		case History::ENTITYTYPE_WEBSTREAM:
			switch($this->ActionType){
			case History::ACTIONTYPE_WEBSTREAM_IMPORT : return "Imported";
			case History::ACTIONTYPE_WEBSTREAM_ADD : return "Added";
			case History::ACTIONTYPE_WEBSTREAM_EDIT : return "Edited";
			case History::ACTIONTYPE_WEBSTREAM_CHANGESTATUS : return "Status changed";
			case History::ACTIONTYPE_WEBSTREAM_EDITREQUEST : return "Request edit";
			case History::ACTIONTYPE_WEBSTREAM_EDITREQUESTREJECT : return "Request edit rejected";
			}
		}
		return "";
	}

	/**
	 * @return create an history line
	 */
	public static function createNew($entityType, $actionType, $entityId, $actionDetails = null)
	{
		$history = new History;
		
		$history->EntityType=$entityType;
		$history->ActionType=$actionType;
		$history->EntityId=$entityId;
		$history->ActionDetails=$actionDetails;

		return $history;
	}

	public function getTimeAgo()
	{
		$timestamp = CDateTimeParser::parse($this->Date, 'yyyy-MM-dd HH:mm:ss');

		$difference = time() - $timestamp;
		$periods = array("second", "minute", "hour", "day", "week", "month", "years", "decade");
		$lengths = array("60","60","24","7","4.35","12","10");
		for($j = 0; $difference >= $lengths[$j]; $j++)
		$difference /= $lengths[$j];
		$difference = round($difference);
		if($difference != 1) $periods[$j].= "s";
		$text = "$difference $periods[$j] ago";
		return $text;
	}
}
