<?php 
if(!isset($to_user_id) || $to_user_id === false) 
	throw new CException(Yum::t('No recipient given'));

	$this->title = Yum::t('Composing new message');
	$this->breadcrumbs = array(
			Yum::t('Messages') => array('index'),
			Yum::t('Compose new message'),
			);
?>


<div class="form">

<?php $form=$this->beginWidget('CActiveForm', array(
			'id'=>'yum-messages-form',
			'action' => array('//user/messages/compose'),
			'enableAjaxValidation'=>true,
			)); ?>

<?php echo Yum::requiredFieldNote(); 

echo $form->errorSummary($model); 

echo CHtml::hiddenField('YumMessage[to_user_id][]', $to_user_id);
echo Yum::t('This message will be sent to {username}', array(
			'{username}' => YumUser::model()->findByPk($to_user_id)->username));
?>
<div class="row">
<?php echo $form->labelEx($model,'title'); ?>
<?php echo $form->textField($model,'title',array('size'=>45,'maxlength'=>45)); ?>
<?php echo $form->error($model,'title'); ?>
</div>

<div class="row">
<?php echo $form->labelEx($model,'message'); ?>
<?php echo $form->textArea($model,'message',array('rows'=>6, 'cols'=>50)); ?>
<?php echo $form->error($model,'message'); ?>
</div>

<div class="row buttons">

<?php 
if(Yii::app()->request->isAjaxRequest) {
	echo CHtml::ajaxSubmitButton($model->isNewRecord 
			? Yum::t('Send') 
			: Yum::t('Save'), array('//user/messages/compose'), array(
		'update' => '#message' )); 
} else {
	echo CHtml::submitButton($model->isNewRecord 
			? Yum::t('Send') 
			: Yum::t('Save'));
}
?>

</div>

<?php $this->endWidget(); ?>

</div><!-- form -->
