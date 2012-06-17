<?php
	if(!isset($nofield)){
		$nofield=false;
	}
?>

<?php
	if(!$nofield){
?>
<div class="form">
<?php
	}else{
?>
<div class="wide form">
<?php
	}
?>


<?php
	 $form=$this->beginWidget('CActiveForm', array(
	'action'=>Yii::app()->createUrl("WebStream/changestatus", array("id" => $model->Id)),
	'id'=>'web-stream-form',
	'enableAjaxValidation'=>false,
)); ?>

<?php
	if(!$nofield){
?>
	<br/>

	<p class="note">Fields with <span class="required">*</span> are required.</p>

	<?php echo $form->errorSummary($model); ?>

	<div class="row">
		<?php echo $form->labelEx($model,'Name'); ?>
		<?php echo $form->textField($model,'Name',array('size'=>20,'maxlength'=>20,'readonly'=>1)); ?>
		<?php echo $form->error($model,'Name'); ?>
	</div>

	<div class="row">
		<?php echo $form->labelEx($model,'Url'); ?>
		<?php echo $form->textField($model,'Url',array('size'=>60,'maxlength'=>255,'readonly'=>1)); ?>
		<?php echo $form->error($model,'Url'); ?>
	</div>
<?php
	}
?>

	<div class="row">
		<?php echo $form->labelEx($model,'StreamStatusCode'); ?>
		<?php
			$dropList = CHtml::listData(StreamStatus::model()->findAll(), 'Code', 'Label');
			echo $form->dropDownList($model, 'StreamStatusCode', $dropList);
			echo $form->error($model,'StreamStatusCode');
		?>

<?php
	if(!$nofield){
?>
	</div>

	<div class="row buttons">
<?php
	}
?>

		<?php echo CHtml::submitButton('Update'); ?>
	</div>

<?php $this->endWidget(); ?>

</div><!-- form -->
