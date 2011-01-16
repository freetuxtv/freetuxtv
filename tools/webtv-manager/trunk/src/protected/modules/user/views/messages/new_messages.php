<?php 
$messages = YumMessage::model()->findAll(
		'to_user_id = :to and message_read = 0',
		array( ':to' => Yii::app()->user->id)
		);

if(count($messages) > 0) {
	if($this->module->messageSystem == YumMessage::MSG_PLAIN) 
		$this->renderPartial('/messages/new_messages_plain', array(
					'messages' => $messages));
	else if($this->module->messageSystem == YumMessage::MSG_DIALOG)
		$this->renderPartial('/messages/new_messages_dialog', array(
					'messages' => $messages));
}
?>
