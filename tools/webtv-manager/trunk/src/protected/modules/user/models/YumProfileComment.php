<?php

class YumProfileComment extends YumActiveRecord{
	public static function model($className=__CLASS__)
	{
		return parent::model($className);
	}

	public function tableName()
	{
		return 'wtvmT_Profile_Comment';
	}

	public function rules()
	{
		return array(
			array('user_id, profile_id, comment, createtime', 'required'),
			array('user_id, profile_id, createtime', 'numerical', 'integerOnly'=>true),
			array('id, user_id, profile_id, comment, createtime', 'safe', 'on'=>'search'),
		);
	}

	public function relations()
	{
		return array(
			'user' => array(self::BELONGS_TO, 'YumUser', 'user_id'),
			'profile' => array(self::BELONGS_TO, 'YumProfile', 'profile_id'),
		);
	}

	public function beforeValidate() {
		parent::beforeValidate();
		$this->user_id = Yii::app()->user->id;
		$this->createtime = time();
		return true;
	}

	public function afterSave() {
		// If the user has activated email receiving, send a email
		if($user = $this->profile->user) {
			if($user->privacy && $user->privacy->message_new_friendship) {
				YumMessage::write($user, $this->user_id,
						Yum::t('New profile comment'),
						YumTextSettings::getText('text_profilecomment_new', array(
								'{username}' => $this->user->username,
								'{message}' => $this->comment,
								'{link}' =>
								Yii::app()->controller->createUrl('//user/profile/view'))));
			}
		}
		return parent::afterSave();
	}

	public function attributeLabels()
	{
		return array(
			'id' => Yum::t('ID'),
			'user_id' => Yum::t('Written from'),
			'profile_id' => Yum::t('Profile'),
			'comment' => Yum::t('Comment'),
			'createtime' => Yum::t('Written at'),
		);
	}


	public function search()
	{
		$criteria=new CDbCriteria;

		$criteria->compare('id', $this->id);
		$criteria->compare('user_id', $this->user_id);
		$criteria->compare('profile_id', $this->profile_id);
		$criteria->compare('comment', $this->comment, true);
		$criteria->compare('createtime', $this->createtime);

		return new CActiveDataProvider(get_class($this), array(
			'criteria'=>$criteria,
		));
	}
}
