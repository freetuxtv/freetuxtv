<?php
$this->title = Yum::t('My friends');
$this->breadcrumbs = array(Yum::t('Friends'));

$form=$this->beginWidget('CActiveForm', array(
			'id'=>'groups-form',
			'enableAjaxValidation'=>false,
			)); 

if($friends) {
	echo '<table>';
	echo '<th></th></th><th>Username</th><th>Status</th>';

	foreach($friends as $friend) {
		$options = array();
		echo CHtml::activeHiddenField($friend, 'inviter_id');
		echo CHtml::activeHiddenField($friend, 'friend_id');

		if($friend->status == 1) { // Confirmation Pending
			if($friend->inviter_id == Yii::app()->user->id) {
				$options = CHtml::submitButton(Yum::t('Cancel request'),array(
							'id'=>'cancel_request', 'name'=>'YumFriendship[cancel_request]'));
			} else {
				$options = CHtml::submitButton(Yum::t('Confirm'), array(
							'id'=>'add_request','name'=>'YumFriendship[add_request]'));
				$options .= CHtml::submitButton(Yum::t('Ignore'), array(
							'id'=>'ignore_request','name'=>'YumFriendship[ignore_request]'));
				$options .= CHtml::submitButton(Yum::t('Deny'), array(
							'id'=>'deny_request','name'=>'YumFriendship[deny_request]'));
			}
		} else if($friend->status == 2) { // Users are friends
			$options = CHtml::submitButton(Yum::t('Remove friend'),array(
						'id'=>'remove_friend','name'=>'YumFriendship[remove_friend]'));
		}
		if($friend->status != YumFriendship::FRIENDSHIP_REJECTED)
		printf('<tr><td>%s</td><td>%s</td><td>%s</td><td>%s</td></tr>',
				$friend->inviter->getAvatar($friend->invited, true),
				CHtml::link($friend->invited->username, array(
						'//user/profile/view', 'id'=>$friend->invited->id)),
				$friend->getStatus(), 
				$options);

	}
		echo '</table>';	
} else {
	echo Yum::t('You do not have any friends yet');
}

$this->endWidget();
?>

