<?php

class YumMembership extends YumActiveRecord{
	public static function model($className=__CLASS__) {
		return parent::model($className);
	}

	public function tableName() {
		return 'wtvmT_Membership';
	}

	public function daysLeft() {
		$difference = $this->end_date - time();
		return sprintf('%d', (int) $difference / 86400 + 1);
	}

	public function beforeValidate() {
		if($this->isNewRecord) {
			$this->user_id = Yii::app()->user->id;		
			$this->order_date = time();
		}

		return parent::beforeValidate();
	}

	public function afterSave() {
		if($this->isNewRecord)
			YumMessage::write($this->user, 1,
					Yum::t('Order confirmed'),
					YumTextSettings::getText('text_membership_ordered', array(
							'{membership}' => $this->role->title,
							'{payment}' => $this->payment->title,
							'{order_date}' => date(Yum::module()->dateTimeFormat, $this->order_date),
							'{id}' => $this->id,
							)));

		return parent::afterSave();
	}

	public function primaryKey() {
		return 'id';
	}

	public function rules()
	{
		return array(
				array('membership_id, user_id, payment_id, order_date', 'required'),
				array('end_date', 'default', 'setOnEmpty' => true, 'value' => null),
				array('membership_id, user_id, payment_id, order_date, end_date, payment_date', 'numerical', 'integerOnly'=>true),
				array('membership_id, user_id, payment_id, order_date, end_date, payment_date', 'safe', 'on'=>'search'),
				);
	}

	public function relations()
	{
		return array(
				'user' => array(self::BELONGS_TO, 'YumUser', 'user_id'),
				'role' => array(self::BELONGS_TO, 'YumRole', 'membership_id'),
				'payment' => array(self::BELONGS_TO, 'YumPayment', 'payment_id'),
				);
	}

	public function attributeLabels()
	{
		return array(
				'id' => Yum::t('Order number'),
				'is_membership_possible' => Yum::t('Membership is possible'),
				'membership_id' => Yum::t('Membership'),
				'user_id' => Yum::t('User'),
				'payment_id' => Yum::t('Payment'),
				'order_date' => Yum::t('Order date'),
				'end_date' => Yum::t('End date'),
				'payment_date' => Yum::t('Payment date'),
				);
	}


	public function search()
	{
		$criteria=new CDbCriteria;

		$criteria->compare('membership_id', $this->membership_id);
		$criteria->compare('user_id', $this->user_id);
		$criteria->compare('payment_id', $this->payment_id);
		$criteria->compare('order_date', $this->order_date);
		$criteria->compare('end_date', $this->end_date);
		$criteria->compare('payment_date', $this->payment_date);

		return new CActiveDataProvider(get_class($this), array(
					'criteria'=>$criteria,
					));
	}
}
