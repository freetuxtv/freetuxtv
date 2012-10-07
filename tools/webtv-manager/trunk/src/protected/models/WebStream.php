<?php

/**
 * This is the model class for table "WebStream".
 *
 * The followings are the available columns in table 'WebStream':
 */
class WebStream extends CActiveRecord
{

	const WEBSTREAM_STATUS_SUBMITTED		= 1;
	const WEBSTREAM_STATUS_WORKING			= 2;

	const TypeWebTV				= 1;
	const TypeWebRadio			= 2;
	const TypeWebCam			= 3;
	const TypeWebProgram		= 4;
	const TypeWebLink			= 5;

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
            array('Comments', 'length'),
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
            'Lang'=>array(self::BELONGS_TO, 'Lang', 'LangCode'),
			'Country'=>array(self::BELONGS_TO, 'Country', 'CountryCode'),
			'Histories'=>array(self::HAS_MANY, 'History', 'EntityId',
				'condition'=>'EntityType=:EntityType AND ActionType!=:ActionType',
				'params'=>array(":EntityType" => History::ENTITYTYPE_WEBSTREAM, ":ActionType"=>History::ACTIONTYPE_WEBSTREAM_EDITREQUEST),
				'order'=>'Date DESC',
			),
			'EditRequests'=>array(self::HAS_MANY, 'History', 'EntityId',
				'condition'=>'EntityType=:EntityType AND ActionType=:ActionType',
				'params'=>array(":EntityType" => History::ENTITYTYPE_WEBSTREAM, ":ActionType"=>History::ACTIONTYPE_WEBSTREAM_EDITREQUEST),
				'order'=>'Date DESC',
			),
			'RelatedWebStream'=>array(self::HAS_MANY, 'WebStreamRelation', 'WebStreamId2'),
			'WebStreamRelations'=>array(self::HAS_MANY, 'WebStream', 'WebStreamId1', 'through'=>'RelatedWebStream'),
		);
	}

	/**
	 * @return array customized attribute labels (name=>label)
	 */
	public function attributeLabels()
	{
		return array(
			'RequiredIsp'=>'Required ISP',
			'LangCode'=>'Language',
			'CountryCode'=>'Country',
			'StreamStatusCode'=>'Status',
			'TypeStream'=>'Type of stream',
			'SubmissionDate'=>'Date of submission',
			'Comments'=>'Comments',
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
	
	protected function beforeValidate()
	{
		if(parent::beforeValidate())
		{
			if($this->Name){
				$this->Name = trim($this->Name);
			}
			if($this->Url){
				$this->Url = trim($this->Url);
			}

			return true;
		}
		return false;
	}

	public function getTypeStreamList()
	{
		return array(WebStream::TypeWebTV=>"WebTV", WebStream::TypeWebRadio=>"WebRadio", WebStream::TypeWebCam=>"WebCam", WebStream::TypeWebProgram=>"WebProgramme", WebStream::TypeWebLink=>"WebLink");
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
		if(isset($attributes["LangCode"])){
			if($this->LangCode != $attributes["LangCode"]){
				if($actionsDetails != ""){
					$actionsDetails .= ", ";
				}
				$actionsDetails .= $this->LangCode.' => '.$attributes["LangCode"];
			}
		}
		if(isset($attributes["CountryCode"])){
			if($this->CountryCode != $attributes["CountryCode"]){
				if($actionsDetails != ""){
					$actionsDetails .= ", ";
				}
				$actionsDetails .= strtoupper($this->CountryCode).' => '.strtoupper($attributes["CountryCode"]);
			}
		}
		if(isset($attributes["StreamStatusCode"])){
			if($this->StreamStatusCode != $attributes["StreamStatusCode"]){
				if($actionsDetails != ""){
					$actionsDetails .= ", ";
				}
				$oldStreamStatus=StreamStatus::model()->findbyPk($this->StreamStatusCode);
				$newStreamStatus=StreamStatus::model()->findbyPk($attributes["StreamStatusCode"]);
				$actionsDetails .= $oldStreamStatus->Label.' => '.$newStreamStatus->Label;
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
		if(isset($attributes["Comments"])){
			if($this->Comments != $attributes["Comments"]){
				if($actionsDetails != ""){
					$actionsDetails .= ", ";
				}
				$actionsDetails .= 'Comments modified';
			}
		}
		return $actionsDetails;
	}

	public function getPlaylistTypeStreamList()
	{
		return array(1=>"webtv", 2=>"webradio", 3=>"webcam", 4=>"programmes", WebStream::TypeWebLink=>"weblink");
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
