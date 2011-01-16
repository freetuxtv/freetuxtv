<?php

class YumFriendship extends YumActiveRecord {
	const FRIENDSHIP_NONE = 0; 
	const FRIENDSHIP_REQUEST = 1;
	const FRIENDSHIP_ACCEPTED = 2;
	const FRIENDSHIP_REJECTED = 3;

	public static function model($className=__CLASS__)
	{
		return parent::model($className);
	}

	public function requestFriendship($inviter, $invited, $message = null) {
		if(!is_object($inviter))
			$inviter = YumUser::model()->findByPk($inviter);
		if(!is_object($invited))
			$invited = YumUser::model()->findByPk($invited);

		if($inviter->id == $invited->id)
			return false;

		$friendship_status = $inviter->isFriendOf($invited);
		if($friendship_status !== false)  {
			if($friendship_status == 1)
				$this->addError('invited_id', Yum::t('Friendship request already sent'));
			if($friendship_status == 2)
				$this->addError('invited_id', Yum::t('Users already are friends'));
			if($friendship_status == 3)
				$this->addError('invited_id', Yum::t('Friendship request has been rejected '));

			return false;
		}

		$this->inviter_id = $inviter->id;
		$this->friend_id = $invited->id;
		$this->acknowledgetime = 0;
		$this->requesttime = time();
		$this->updatetime = time();
		if($message !== null)
			$this->message = $message;
		$this->status = 1;
		return($this->save());
	} 

	public function acceptFriendship() {
		$this->acknowledgetime = time();
		$this->status = 2;
		return($this->save());
	} 

	public function getFriend() {
		if($this->friend_id == Yii::app()->user->id)
			return $this->inviter->username;
		else
			return $this->invited->username;
	}

	public function getStatus() {
		switch($this->status) {
			case '0':
				return Yum::t('No friendship requested');
			case '1':
				return Yum::t('Confirmation pending');
			case '2':
				return Yum::t('Friendship confirmed');
			case '3':
				return Yum::t('Friendship rejected');

		}
	}

	public function rejectFriendship() {
		$this->acknowledgetime = time();
		$this->status = 3;
		return($this->save());
	} 

	public function ignoreFriendship() {
		$this->acknowledgetime = time();
		$this->status = 0;
		return($this->save());
	} 

	public function tableName()
	{
		if(isset(Yum::module()->friendshipTable))
			$this->_tableName = Yum::module()->friendshipTable;
		elseif(isset(Yii::app()->modules['user']['friendshipTable']))
			$this->_tableName = Yii::app()->modules['user']['friendshipTable'];
		else
			$this->_tableName = 'wtvmT_Friendship'; // fallback if nothing is set

		return Yum::resolveTableName($this->_tableName, $this->getDbConnection());
	}

	public function rules()
	{
		return array(
				array('inviter_id, friend_id, status, requesttime, acknowledgetime, updatetime', 'required'),
				array('inviter_id, friend_id, status, requesttime, acknowledgetime, updatetime', 'numerical', 'integerOnly'=>true),
				array('message', 'length', 'max'=>255),
				array('inviter_id, friend_id, status, message, requesttime, acknowledgetime, updatetime', 'safe', 'on'=>'search'),
				);
	}

	public function relations()
	{
		return array(
				'inviter' => array(self::BELONGS_TO, 'YumUser', 'inviter_id'),
				'invited' => array(self::BELONGS_TO, 'YumUser', 'friend_id'),
				);
	}

	public function attributeLabels()
	{
		return array(
				'inviter_id' => Yum::t('Inviter'),
				'friend_id' => Yum::t('Friend'),
				'status' => Yum::t('Status'),
				'message' => Yum::t('Message'),
				'requesttime' => Yum::t('Requesttime'),
				'acknowledgetime' => Yum::t('Acknowledgetime'),
				'updatetime' => Yum::t('Updatetime'),
				);
	}

	public function beforeSave() {
		$this->updatetime = time();
		return true;
	}

	public function afterSave() {
		// If the user has activated email receiving, send a email
		if($user = YumUser::model()->findByPk($this->friend_id))  {
			if($user->privacy && $user->privacy->message_new_friendship) {

				YumMessage::write($user, $this->inviter,
						Yum::t('New friendship request from {username}'),
						YumTextSettings::getText('text_friendship_new', array(
								'{user}' => $this->inviter->username,
								'{link_friends}' => Yii::app()->controller->createUrl('//user/friendship/index'),
								'{link_profile}' => Yii::app()->controller->createUrl('//user/profile/view'),
								'{message}' => $this->message)));
			}
		}
		return parent::afterSave();
	}

	public function search()
	{
		$criteria=new CDbCriteria;

		$criteria->compare('inviter_id', $this->inviter_id);
		$criteria->compare('friend_id', $this->friend_id);
		$criteria->compare('status', $this->status);
		$criteria->compare('message', $this->message, true);
		$criteria->compare('requesttime', $this->requesttime);
		$criteria->compare('acknowledgetime', $this->acknowledgetime);
		$criteria->compare('updatetime', $this->updatetime);

		return new CActiveDataProvider(get_class($this), array(
					'criteria'=>$criteria,
					));
	}
}
