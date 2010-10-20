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
            array('Name, Url, TypeStream, StreamStatusCode', 'required'),
			array('Name', 'length', 'max'=>100),
			array('Url', 'length', 'max'=>255),
            array('RequiredIsp', 'length', 'max'=>50),
            array('LangCode', 'length', 'max'=>2),
			array('SubmissionDate', 'type', 'type'=>'datetime', 'datetimeFormat'=>'yyyy-MM-dd HH:mm:ss'),
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
			'RequiredIsp'=>'Required ISP',
			'LangCode'=>'Language',
			'StreamStatusCode'=>'Status',
			'TypeStream'=>'Type of stream',
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

	public function getTypeStreamNameById($typeStream)
	{
		switch($typeStream){
		case 1 : return "WebTV";
		case 2 : return "WebRadio";
		case 3 : return "Webcam";
		case 4 : return "WebProgramme";
		case 5 : return "WebLink";
		}
		return "";
	}

	public function getTypeStreamName()
	{
		return $this->getTypeStreamNameById($this->TypeStream);
	}

	public function getTypeStreamList()
	{
		return array(1=>"WebTV", 2=>"WebRadio", 3=>"WebCam", 4=>"WebProgramme", 5=>"WebLink");
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
}
