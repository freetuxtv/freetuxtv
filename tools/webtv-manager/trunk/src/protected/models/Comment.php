<?php

/**
 * This is the model class for table "Comment".
 *
 * The followings are the available columns in table 'Comment':
 */
class Comment extends CActiveRecord
{
	public $Username;
	public $Date;
	const COMMENT_STATUS_HIDE		= 0;
	const COMMENT_STATUS_ACTIVE		= 1;
	const COMMENT_STATUS_SUBMITTED	= 2;

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
            array('WebStreamId, HistoryId, Content, Statut', 'required'),
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
			
		);
	}

	/**
	 * @return array customized attribute labels (name=>label)
	 */
	public function attributeLabels()
	{
		return array(
			'Date'=>'Date',
			'IdWebStream'=>'IdWebStream',
			'IdHistory'=>'IdHistory',
			'Content'=>'Content',
		);
	}

	public function getCommentStatut()
	{
		switch($this->Statut){
			case Comment::COMMENT_STATUS_HIDE : return "Hide";
			case Comment::COMMENT_STATUS_SUBMITTED : return "Submit";
			case Comment::COMMENT_STATUS_ACTIVE : return "Active";
		}
		return "";
	}

	protected function beforeSave()
	{
		    return true;
	}
	
	/**
	 * @return create a Comment line
	 */
	public static function createNew($WebStreamId, $HistoryId, $Content, $Statut = Comment::COMMENT_STATUS_SUBMITTED)
	{
		$Comment = new Comment;
		
		$Comment->WebStreamId=$WebStreamId;
		$Comment->HistoryId=$HistoryId;
		$Comment->Content=$Content;
		$Comment->Statut=$Statut;
		return $Comment;
	}
}
