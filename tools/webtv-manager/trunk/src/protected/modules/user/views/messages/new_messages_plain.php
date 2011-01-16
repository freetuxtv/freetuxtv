<?php 
if($messages) {
	echo '<div class="success">';

	echo Yii::t('UserModule.user', 'You have new messages!');

	echo '<ul>';
	foreach($messages as $message) {
		printf("<li><span> %s</span> &nbsp; %s</li>",
				CHtml::link($message->title, array(
						'//user/messages/view', 'id' => $message->id)),
				CHtml::link(Yum::t('Reply'), array(
							'//user/messages/compose',
							'to_user_id' => $message->from_user_id)));
	}
	echo '</ul>';
	echo '</div>';
} 

?>
