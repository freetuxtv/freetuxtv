<?php

class YumSettings extends YumActiveRecord {

    public static function model($className=__CLASS__) {
        return parent::model($className);
    }

    public function tableName() {
        if (isset(Yum::module()->settingsTable))
            $this->_tableName = Yum::module()->settingsTable;
        elseif (isset(Yii::app()->modules['user']['settingsTable']))
            $this->_tableName = Yii::app()->modules['user']['settingsTable'];
        else
            $this->_tableName = 'wtvmT_YumSettings'; // fallback if nothing is set
 return Yum::resolveTableName($this->_tableName, $this->getDbConnection());
    }

    public function getActive() {
        return YumSettings::model()->find('is_active')->id;
    }

    public function rules() {
        return array(
            array('title, loginType, messageSystem, notifyType, enableAvatar, notifyemailchange', 'required'),
            array('password_expiration_time, preserveProfiles, registrationType, enableRecovery, enableProfileHistory, readOnlyProfiles, enableCaptcha, ldap_port, ldap_protocol, ldap_tls, ldap_autocreate, ldap_transfer_attr, ldap_transfer_pw', 'numerical', 'integerOnly' => true),
            array('title, ldap_basedn, ldap_host', 'length', 'max' => 255),
            array('loginType', 'required', 'requiredValue' => true),
            array('id, title, preserveProfiles, registrationType, enableRecovery, enableProfileHistory, readOnlyProfiles, loginType, enableCaptcha', 'safe', 'on' => 'search'),
        );
    }

    public function relations() {
        return array(
        );
    }

    public function attributeLabels() {
        return array(
            'id' => Yum::t('ID'),
            'title' => Yum::t('Title'),
            'preserveProfiles' => Yum::t('Preserve profiles'),
            'registrationType' => Yum::t('Registration type'),
            'enableRecovery' => Yum::t('Enable recovery'),
            'enableProfileHistory' => Yum::t('Enable profile History'),
            'messageSystem' => Yum::t('Messaging system'),
            'notifyType' => Yum::t('Notify type'),
            'enableAvatar' => Yum::t('Enable avatar upload'),
            'readOnlyProfiles' => Yum::t('Read only Profiles'),
            'loginType' => Yum::t('Login Type'),
            'enableCaptcha' => Yum::t('Enable Captcha'),
            'notifyemailchange'  => Yum::t('Notify user on email change'),
						'ldap_host' => Yum::t('Hostname/URL'),
						'ldap_port' => Yum::t('Port'),
						'ldap_protocol' => Yum::t('Protocol version'),
						'ldap_basedn' => Yum::t('BaseDN'),
						'ldap_autocreate' => Yum::t('Automatically create database user'),
						'ldap_tls' => Yum::t('Start TLS'),
						'ldap_transfer_attr' => Yum::t('Transfer ldap attributes to user profile'),
						'ldap_transfer_pw' => Yum::t('Transfer ldap user password to database'),
        );
    }

    public function search() {
        $criteria = new CDbCriteria;

        $criteria->compare('id', $this->id);

        $criteria->compare('title', $this->title, true);

        $criteria->compare('preserveProfiles', $this->preserveProfiles);

        $criteria->compare('registrationType', $this->registrationType);

        $criteria->compare('enableRecovery', $this->enableRecovery);

        $criteria->compare('enableProfileHistory', $this->enableProfileHistory);

        $criteria->compare('readOnlyProfiles', $this->readOnlyProfiles);

        $criteria->compare('loginType', $this->loginType, true);

        $criteria->compare('enableCaptcha', $this->enableCaptcha);

        return new CActiveDataProvider(get_class($this), array(
            'criteria' => $criteria,
        ));
    }

}
