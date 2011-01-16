<?php
$this->title = Yii::t('UserModule.user', 'Confirm deletion');

$this->breadcrumbs = array(
	Yii::t("UserModule.user", 'Users') => array('index'),
	Yii::t("UserModule.user", 'Confirm deletion'));

printf('<p>%s</p>', Yii::t('UserModule.user', 
			'Are you really sure you want to delete your Account?')
		);

printf('<p>%s</p>', Yii::t('UserModule.user', 
			'Please enter your password to confirm deletion:')
		);

echo CHtml::form(array('delete'));
echo CHtml::passwordField('confirmPassword') . "<br />";
echo CHtml::linkButton(Yii::t('UserModule.user', 'Cancel deletion'), array(
			'submit' => array('profile')));
echo CHtml::submitButton(Yii::t('UserModule.user', 'Confirm deletion'));
echo CHtml::endForm();
?>
