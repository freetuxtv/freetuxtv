<?php
$this->title = Yum::t('Friendship request for {username} has been sent', array(
			'{username}' => $friendship->invited->username));
$this->breadcrumbs = array('Friendship', 'Invitation', $friendship->invited->username);

echo CHtml::link(Yum::t('Go to profile of {username}', array(
				'{username}' => $friendship->invited->username)), array(
				'profile/view', 'id' => $friendship->invited->id));
echo '<br />';
echo CHtml::link(Yum::t('Back to my profile'), array(
				'profile/view', 'id' => Yii::app()->user->id));

?>

