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
/*
$this->menu=array(
	array('label'=>'List WebStream', 'url'=>array('index')),
	array('label'=>'Create WebStream', 'url'=>array('create')),
	array('label'=>'Update WebStream', 'url'=>array('update', 'id'=>$model->Id)),
	array('label'=>'Delete WebStream', 'url'=>'#', 'linkOptions'=>array('submit'=>array('delete','id'=>$model->Id),'confirm'=>'Are you sure you want to delete this item?')),
	array('label'=>'Manage WebStream', 'url'=>array('admin')),
);*/

?>

<h1>Update WebStream <?php echo $model->Name; ?></h1>

<div class="form">

<?php
	 $form=$this->beginWidget('CActiveForm', array(
	'action'=>Yii::app()->createUrl("WebStream/update", array("id" => $model->Id)),
	'id'=>'web-stream-form',
	'enableAjaxValidation'=>false,
)); ?>

	<br/>

	<p class="note">Fields with <span class="required">*</span> are required.</p>

	<?php echo $form->errorSummary($model); ?>

	<div class="row">
		<?php echo $form->labelEx($model,'Name'); ?>
		<?php echo $form->textField($model,'Name',array('size'=>60,'maxlength'=>100)); ?>
		<?php echo $form->error($model,'Name'); ?>
	</div>

	<div class="row">
		<?php echo $form->labelEx($model,'Url'); ?>
		<?php echo $form->textField($model,'Url',array('size'=>60,'maxlength'=>255,'readonly'=>1)); ?>
		<?php echo $form->error($model,'Url'); ?>
	</div>

	<div class="row">
		<?php echo $form->labelEx($model,'TypeStream'); ?>
		<?php
			$dropList = WebStream::getTypeStreamList();
			echo $form->dropDownList($model, 'TypeStream', $dropList, array ('empty' => '-- Unknown --'));
			echo $form->error($model,'TypeStream');
		?>
	</div>

	<div class="row">
		<?php echo $form->labelEx($model,'RequiredIsp'); ?>
		<?php echo $form->textField($model,'RequiredIsp',array('size'=>20,'maxlength'=>20)); ?>
		<?php echo $form->error($model,'RequiredIsp'); ?>
	</div>

	<div class="row">
		<?php echo $form->labelEx($model,'LangCode'); ?>
		<?php
			$dropList = CHtml::listData(Lang::model()->findAll(array('order'=>'Label')), 'Code', 'Label');
			echo $form->dropDownList($model, 'LangCode', $dropList, array ('empty' => '-- None --'));
			echo $form->error($model,'LangCode');
		?>
	</div>

	<div class="row">
		<?php echo $form->labelEx($model,'CountryCode'); ?>
		<?php
			$dropList = CHtml::listData(Country::model()->findAll(array('order'=>'Label')), 'Code', 'Label');
			echo $form->dropDownList($model, 'CountryCode', $dropList, array ('empty' => '-- None --'));
			echo $form->error($model,'CountryCode');
		?>
	</div>

	<div class="row">
		<?php echo $form->labelEx($model,'Comments'); ?>
		<?php echo $form->textArea($model,'Comments',array('cols'=>80,'rows'=>10)); ?>
		<?php echo $form->error($model,'Comments'); ?>
	</div>



	<div class="row buttons">
		<?php echo CHtml::submitButton('Update'); ?>
	</div>

<?php $this->endWidget(); ?>

</div><!-- form -->
