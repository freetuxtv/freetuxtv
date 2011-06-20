<?php
$this->breadcrumbs=array(
	'Web Streams'=>array('index'),
	$model->Name=>array('view','id'=>$model->Id),
	'Update',
);
?>

<?php
	$this->menu=array();
	$this->menu[] = array('label'=>'View', 'url'=>array('view', 'id'=>$model->Id));
	if(Yii::app()->user->checkAccess('editWebStream')) {
		$this->menu[] = array('label'=>'Edit', 'url'=>array('update', 'id'=>$model->Id));
	}
	if(Yii::app()->user->checkAccess('changeStatusWebStream')) {
		$this->menu[] = array('label'=>'Change status', 'url'=>array('changestatus', 'id'=>$model->Id));
	}
?>

<h1>Update WebStream <?php echo $model->Name; ?></h1>

<div class="form">

<?php
	 $form=$this->beginWidget('CActiveForm', array(
	'action'=>Yii::app()->createUrl("WebStream/changestatus", array("id" => $model->Id)),
	'id'=>'web-stream-form',
	'enableAjaxValidation'=>false,
)); ?>

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

	<div class="row">
		<?php echo $form->labelEx($model,'StreamStatusCode'); ?>
		<?php
			$dropList = CHtml::listData(StreamStatus::model()->findAll(), 'Code', 'Label');
			echo $form->dropDownList($model, 'StreamStatusCode', $dropList);
			echo $form->error($model,'StreamStatusCode');
		?>
	</div>

	<div class="row buttons">
		<?php echo CHtml::submitButton('Update'); ?>
	</div>

<?php $this->endWidget(); ?>

</div><!-- form -->
