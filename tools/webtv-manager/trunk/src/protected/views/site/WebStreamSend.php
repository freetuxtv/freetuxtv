<?php
$this->pageTitle=Yii::app()->name . ' - Send new URL';
$this->breadcrumbs=array(
	'Send New URL',
);
?>

<h1>URL</h1>

<p>You can send to the team a new URL :</p>

<div class="wide form">

<?php
	$form=$this->beginWidget('CActiveForm', array(
		'action'=>Yii::app()->createUrl("site/WebStreamSend"),
		'method'=>'get',
		'id'=>'web-stream-search-form',
		'enableAjaxValidation'=>false,
		));
?>
	<div class="row">
		<?php echo $form->labelEx($modelSendForm,'Url'); ?>
		<?php echo $form->textField($modelSendForm,'Url',array('size'=>60,'maxlength'=>255)); ?>
		<?php echo CHtml::submitButton('Confirm'); ?>
		<?php echo $form->error($modelSendForm,'Url'); ?>
	</div>

<?php $this->endWidget(); ?>


</div><!-- web-stream-send-form -->

