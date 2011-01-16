<?php

/**
 * This is the model class for table "{{messages}}".
 *
 * The followings are the available columns in table '{{messages}}':
 * @property integer $id
 * @property integer $from_user_id
 * @property integer $to_user_id
 * @property string $title
 * @property string $message
 * @property integer $message_read
 * @property integer $draft
 * 
 * Relations:
 * @property YumUser $to_user
 * @property YumUser $from_user
 */
class YumMessage extends YumActiveRecord
{
	const MSG_NONE = 'None';
	const MSG_PLAIN = 'Plain';
	const MSG_DIALOG = 'Dialog';

	// set $omit_mail to true to avoid e-mail notification of the 
	// received message. It is mainly user for the privacy settings
	// (receive profile comment email/friendship request email/...)
	public $omit_mail = false;

	/**
	 * @param string $className
	 * @return YumMessage
	 */
	public static function model($className=__CLASS__) {
		return parent::model($className);
	}

	public function beforeValidate() {
		$users = $this->to_user_id;
		if(!is_array($users)) 
			$users = array($this->to_user_id);

		foreach($users as $to_user_id) {
			$to_user = YumUser::model()->findByPk($to_user_id);
			if($to_user && isset($to_user->privacy)) {
				if(in_array($this->from_user->username, $to_user->privacy->getIgnoredUsers()))
					$this->addError('to_user_id', Yum::t('One of the recipients ({username}) has ignored you. Message will not be sent!', array('{username}' => $to_user->username)));
			}
		}
		return parent::beforeValidate();
	}

	public function beforeSave() {
		$this->timestamp = time();
		return parent::beforeSave();
	}

	public function afterSave() {
		// If the user has activated email receiving, send a email
		if($this->to_user->privacy && $this->to_user->privacy->message_new_message)
			YumMailer::send($this->to_user->profile[0]->email,
					$this->title,
					YumTextSettings::getText('text_message_new', array(
							'{user}' => $this->from_user->username,
							'{message}' => $this->message)));
		return parent::afterSave();
	}

	public static function write($to, $from, $subject, $body, $mail = true) {
		$message = new YumMessage();

		if(!$mail)
			$message->omit_mail = true;

		if(is_object($from))
			$message->from_user_id = (int) $from->id;
		else if(is_numeric($from))
			$message->from_user_id = $from;
		else 
			return false;


		if(is_object($to))
			$message->to_user_id = (int) $to->id;
		else if(is_numeric($to))
			$message->to_user_id = $to;
		else 
			return false;

		$message->title = $subject;
		$message->message = $body;
		if($message->save())
			return true;
		return false;
	}

	/**
	 * Returns resolved table name (incl. table prefix when it is set in db configuration)
	 * Following algorith of searching valid table name is implemented:
	 *  - try to find out table name stored in currently used module
	 *  - if not found try to get table name from UserModule configuration
	 *  - if not found user default {{message}} table name
	 * @return string
	 */
	public function tableName()
	{
		if (isset(Yum::module()->messagesTable))
			$this->_tableName = Yum::module()->messagesTable;
		elseif (isset(Yii::app()->modules['user']['messagesTable'])) 
			$this->_tableName = Yii::app()->modules['user']['messagesTable'];
		else
			$this->_tableName = 'wtvmT_Messages'; // fallback if nothing is set

		return Yum::resolveTableName($this->_tableName,$this->getDbConnection());
	}

	public function rules()
	{
		return array(
				array('from_user_id, to_user_id, title', 'required'),
				array('from_user_id, draft, message_read, answered', 'numerical', 'integerOnly'=>true),
				array('title', 'length', 'max'=>255),
				array('message', 'safe'),
				);
	}

	public function getTitle()
	{
		if($this->message_read)
			return $this->title;
		else
			return '<strong>' . $this->title . '</strong>';
	}

	public function getStatus() {
		if($this->from_user_id == Yii::app()->user->id)
			return Yum::t('sent');
		if($this->answered)
			return Yum::t('answered');
		if($this->message_read)
			return Yum::t('read');
		else
			return Yum::t('new');
	}


	public function unread($id = false) 
	{
		if(!$id)
			$id = Yii::app()->user->id;

		$this->getDbCriteria()->mergeWith(array(
					'condition' => "to_user_id = {$id} and message_read = 0"
				));
		return $this;
	}

	public function scopes() {
		$id = Yii::app()->user->id;
		return array(
				'all' => array(
					'condition' => "to_user_id = {$id} or from_user_id = {$id}"), 
				'read' => array(
					'condition' => "to_user_id = {$id} and message_read = 1"),
				'sent' => array(
					'condition' => "from_user_id = {$id}"),
				'answered' => array(
					'condition' => "to_user_id = {$id} and answered = 1"),
				);
	}

	public function getDate()
	{
		return date(Yii::app()->getModule('user')->dateTimeFormat, $this->timestamp);
	}


	public function relations()
	{
		return array(
				'from_user' => array(self::BELONGS_TO, 'YumUser', 'from_user_id'),
				'to_user' => array(self::BELONGS_TO, 'YumUser', 'to_user_id'),
				);
	}

	public function attributeLabels()
	{
		return array(
				'id' => Yum::t('#'),
				'from_user_id' => Yum::t('From'),
				'to_user_id' => Yum::t('To'),
				'title' => Yum::t('Title'),
				'message' => Yum::t('Message'),
				);
	}

}
