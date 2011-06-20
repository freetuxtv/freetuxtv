<?php

/**
 * This is the model class for table "WebStream".
 *
 * The followings are the available columns in table 'WebStream':
 */
class WebStream extends CActiveRecord
{
	public $email;
	public $username;
	public $Content;
	public $RestrictionValue;
	public $RestrictionCode;
		
	const WEBSTREAM_STATUS_SUBMITTED		= 1;
	const WEBSTREAM_STATUS_WORKING			= 2;

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
		array('Name, Url, TypeStream, StreamStatusCode', 'required'),
		array('Name', 'length', 'max'=>100),
		array('Url', 'length', 'max'=>255),
		array('RequiredIsp', 'length', 'max'=>50),
		array('LangCode', 'length', 'max'=>2),
		array('CountryCode', 'length', 'max'=>2),
		array('SubmissionDate', 'type', 'type'=>'datetime', 'datetimeFormat'=>'yyyy-MM-dd HH:mm:ss'),
        array('email', 'email'),
		array('username', 'length', 'max'=>100),
		array('Content', 'length', 'max'=>255),
		array('RestrictionValue', 'length', 'max'=>200),
		array('RestrictionCode', 'length', 'max'=>50),
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
			'LangC'=>array(self::BELONGS_TO, 'Lang', 'LangCode'),
			'CountryC'=>array(self::BELONGS_TO, 'Cntry', 'CountryCode'),
			'RestrictionV'=>array(self::BELONGS_TO, 'Restriction', 'RestrictionValue'),
			'RestrictionC'=>array(self::BELONGS_TO, 'Restrictiontype', 'RestrictionCode'),
			'comment'=>array(self::HAS_MANY,'Comment','IdWebStream'),
		);
	}

	/**
	 * @return array customized attribute labels (name=>label)
	 */
	public function attributeLabels()
	{
		return array(
			'RequiredIsp'=>'Required ISP',
			'CountryCode'=>'Country',
			'LangCode'=>'Language',
			'email'=>'E-mail',
			'username'=>'Username',
			'StreamStatusCode'=>'Status',
			'TypeStream'=>'Type of stream',
			'RestrictionCode'=>'Restriction Type',
			'RestrictionValue'=>'Restriction Value',
			'SubmissionDate'=>'Date of submission',
		);
	}

	protected function beforeSave()
	{
		if(parent::beforeSave())
		{
			$time=time();
			if($this->isNewRecord) {
				$this->SubmissionDate=new CDbExpression('NOW()');
			}
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

	public function getTypeStreamList()
	{
		return array(1=>"WebTV", 2=>"WebRadio", 3=>"WebCam", 4=>"WebProgramme", 5=>"WebLink");
	}

	public function getTypeStreamName()
	{
		return WebStream::getTypeStreamNameById($this->TypeStream);
	}

	public function getTypeStreamNameById($typeStream)
	{
		$tab = WebStream::getTypeStreamList();
		if(isset($tab[$typeStream])){
			return $tab[$typeStream];
		}
		return "";
	}

	public function getModelDiffMsg($attributes)
	{
		$actionsDetails = "";
		if(isset($attributes["Name"])){
			if($this->Name != $attributes["Name"]){
				if($actionsDetails != ""){
					$actionsDetails .= ", ";
				}
				$actionsDetails .= $this->Name.' => '.$attributes["Name"];
			}
		}
		if(isset($attributes["Url"])){
			if($this->Url != $attributes["Url"]){
				if($actionsDetails != ""){
					$actionsDetails .= ", ";
				}
				$actionsDetails .= $this->Url.' => '.$attributes["Url"];
			}
		}
		if(isset($attributes["TypeStream"])){
			if($this->TypeStream != $attributes["TypeStream"]){
				if($actionsDetails != ""){
					$actionsDetails .= ", ";
				}
				$actionsDetails .= $this->getTypeStreamName().' => '.$this->getTypeStreamNameById($attributes["TypeStream"]);
			}
		}
		if(isset($attributes["CountryCode"])){
			if($this->CountryCode != $attributes["CountryCode"]){
				if($actionsDetails != ""){
					$actionsDetails .= ", ";
				}
				$actionsDetails .= $this->CountryCode.' => '.$attributes["CountryCode"];
			}
		}
		if(isset($attributes["LangCode"])){
			if($this->LangCode != $attributes["LangCode"]){
				if($actionsDetails != ""){
					$actionsDetails .= ", ";
				}
				$actionsDetails .= $this->LangCode.' => '.$attributes["LangCode"];
			}
		}
		
		if(isset($attributes["RestrictionCode"])){
			if($this->RestrictionCode != $attributes["RestrictionCode"]){
				if($actionsDetails != ""){
					$actionsDetails .= ", ";
				}
				$actionsDetails .= $this->RestrictionCode.' => '.$attributes["RestrictionCode"];
			}
		}
		
		if(isset($attributes["RestrictionValue"])){
			if($this->RestrictionValue != $attributes["RestrictionValue"]){
				if($actionsDetails != ""){
					$actionsDetails .= ", ";
				}
				$actionsDetails .= $this->RestrictionValue.' => '.$attributes["RestrictionValue"];
			}
		}
		
		if(isset($attributes["StreamStatusCode"])){
			if($this->StreamStatusCode != $attributes["StreamStatusCode"]){
				if($actionsDetails != ""){
					$actionsDetails .= ", ";
				}
				$newStreamStatus=StreamStatus::model()->findbyPk($attributes["StreamStatusCode"]);
				$actionsDetails .= $this->StreamStatusCode.' => '.$newStreamStatus->Label;
			}
		}
		if(isset($attributes["RequiredIsp"])){
			if($this->RequiredIsp != $attributes["RequiredIsp"]){
				if($actionsDetails != ""){
					$actionsDetails .= ", ";
				}
				$actionsDetails .= $this->RequiredIsp.' => '.$attributes["RequiredIsp"];
			}
		}
		return $actionsDetails;
	}

	public function getPlaylistTypeStreamList()
	{
		return array(1=>"webtv", 2=>"webradio", 3=>"webcam", 4=>"programmes", 5=>"weblink");
	}

	public function getPlaylistTypeStreamNameById($typeStream)
	{
		$tab = WebStream::getPlaylistTypeStreamList();
		if(isset($tab[$typeStream])){
			return $tab[$typeStream];	
		}
		return "";
	}

	public function getPlaylistTypeStreamByName($typeStream)
	{
		$tab = WebStream::getPlaylistTypeStreamList();
		foreach($tab as $key => $val){
			if($val == $typeStream){
				return $key;
			}
		}
		return -1;
	}
}
