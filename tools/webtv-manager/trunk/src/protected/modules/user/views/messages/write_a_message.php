<?php
if(Yum::module()->messageSystem != YumMessage::MSG_NONE && $model->id != Yii::app()->user->id) {

	$this->beginWidget('zii.widgets.jui.CJuiDialog', array(
				'id'=>'message',
				'options'=>array(
					'width' => '600px',
					'model' => true,
					'title'=> Yum::t('Write a message to {username}', array(
							'{username}' => $model->username)),
					'autoOpen'=>false,
					),
				));

	$this->renderPartial('/messages/compose', array(
				'model' => new YumMessage,
				'to_user_id' => $model->id), false, true);

	$this->endWidget('zii.widgets.jui.CJuiDialog');

	echo CHtml::link(Yum::t('Write a message to this User'), '#',
			array('onclick'=>'$("#message").dialog("open"); return false;'));
}
?>
