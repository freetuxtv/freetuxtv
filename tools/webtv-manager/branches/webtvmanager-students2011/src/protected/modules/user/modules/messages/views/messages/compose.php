<?php 
#heading
$this->title = Yii::t('UserModule.user','Composing new message');
#breadcrumbs
$this->breadcrumbs = array(
	Yii::t('UserModule.user', 'Messages') => array('index'),
	Yii::t('UserModule.user', 'Compose new message'),
);
#menu
$this->menu = array(YumMenuItemHelper::backToInbox());
?>

<div class="form">

<?php $form=$this->beginWidget('CActiveForm', array(
	'id'=>'yum-messages-form',
	'enableAjaxValidation'=>true,
)); ?>

	<?php echo YumHelper::requiredFieldNote(); ?>

	<?php echo $form->errorSummary($model); ?>

	<?php echo CHtml::activeHiddenField($model,'from_user_id',array('value' => Yii::app()->user->id)); ?>

	<div class="row">
		<p> <?php echo Yii::t('UserModule.user', 
		'Select multiple recipients by holding the CTRL key'); ?> </p>

<?php 
		echo CHtml::ListBox('SendTo', isset($_GET['to_user_id'])?$_GET['to_user_id']:"", CHtml::listData( 
		YumUser::model()->active()->findAll(), 'id', 'username'),
			array('multiple' => 'multiple'));
		?>
		<?php echo $form->error($model,'to_user_id'); ?>
	</div>

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
		<?php echo CHtml::submitButton($model->isNewRecord ? Yii::t("UserModule.user", 'Send') : Yii::t('UserModule.user', 'Save')); ?>
	</div>

<?php $this->endWidget(); ?>

</div><!-- form -->
