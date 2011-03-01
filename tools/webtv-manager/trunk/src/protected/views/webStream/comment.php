<?php
$this->breadcrumbs=array(
	'Web Streams', 'Comment a Web Stream'
);
?>

<h1>Comment WebStream <?php echo $model->Name; ?></h1>

<div class="form">

<?php
	 $form=$this->beginWidget('CActiveForm', array(
	'action'=>Yii::app()->createUrl("WebStream/comment", array("id" => $model->Id)),
	'id'=>'web-stream-form',
	'enableAjaxValidation'=>false,
)); ?>

	<br/>

	<p class="note">Fields with <span class="required">*</span> are required.</p>

	<div class="row">
		<?php echo $form->labelEx($model,'Content'); ?>
		<?php echo $form->textField($model,'Content',array('size'=>60,'maxlength'=>255)); ?>
		<?php echo $form->error($model,'Content'); ?>
	</div>

	<?php
		if(Yii::app()->user->isGuest){
	?>
		<p class="hint">
			You are not logged, please enter your email and username or create an account.
			
		</p>

	<div class="row">
		<?php echo $form->labelEx($model,'email'); ?>
		<?php echo $form->textField($model,'email'); ?>
		<?php echo $form->error($model,'email'); ?>
	</div>
	
	<div class="row">
		<?php echo $form->labelEx($model,'username'); ?>
		<?php echo $form->textField($model,'username'); ?>
		<?php echo $form->error($model,'username'); ?>
	</div>

	<?php
		}
	?>
	
	<?php
		if(!Yii::app()->user->isGuest){
	?>
	<p class="hint">
		You are <?php echo Yii::app()->user->name ?>.
	</p>

	<div class="row">
		<?php echo $form->hiddenField($model,'email'); ?>
	</div>
	
	<div class="row">
		<?php echo $form->hiddenField($model,'username'); ?>
	</div>
	<?php
		}
	?>
	
	<div class="row buttons">
		<?php echo CHtml::submitButton('Comment'); ?>
	</div>

<?php $this->endWidget(); ?>

</div><!-- form -->
