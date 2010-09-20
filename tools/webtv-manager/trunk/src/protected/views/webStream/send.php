<?php
$this->breadcrumbs=array(
	'Web Streams', 'Send a Web Streams'
);

?>

<h1>Send a Web Streams</h1>

Before to send a new URL, please check it's not corresding to one stream in the following list :

<?php $this->widget('zii.widgets.grid.CGridView', array(
	'dataProvider'=>$dataProvider,
	'columns'=>array(
        array(
            'name'=>'WebStream',
			'value'=>'$data->Name',
        ),
        array(
			'class'=>'CLinkColumn',
            'header'=>'Url',
			'labelExpression'=>'$data->Url',
            'urlExpression'=>'$data->Url',
        ),
        array(
            'name'=>'Language',
			'type'=>'image',
			'htmlOptions' => array('style'=>'text-align:center'),
            'value'=>'"images/lang/softclean/".strtoupper($data->LangCode).".png"',
        ),
        array(
            'name'=>'Status',
			'type'=>'html',
            'value'=>'"<font color=\"".$data->StreamStatus->Color."\">".$data->StreamStatus->Label."</font>"',
        ),
        array(
			'class'=>'CButtonColumn',
            'header'=>'Edit',
        ),
    ),
)); ?>

If not, you can add the new channel :

<div class="form">

<?php
	 $model=new WebStream;
	 $form=$this->beginWidget('CActiveForm', array(
	'action'=>Yii::app()->createUrl("WebStream/add"),
	'id'=>'web-stream-form',
	'enableAjaxValidation'=>false,
)); ?>

	<br/>

	<p class="note">Fields with <span class="required">*</span> are required.</p>

	<?php echo $form->errorSummary($model); ?>

	<div class="row">
		<?php echo $form->labelEx($model,'Name'); ?>
		<?php echo $form->textField($model,'Name',array('size'=>20,'maxlength'=>20)); ?>
		<?php echo $form->error($model,'Name'); ?>
	</div>

	<div class="row">
		<?php echo $form->labelEx($model,'Url'); ?>
		<?php echo $form->textField($model,'Url',array('value'=>CHtml::Encode($_GET['WebStreamUrl']),'size'=>60,'maxlength'=>255)); ?>
		<?php echo $form->error($model,'Url'); ?>
	</div>

	<div class="row">
		<?php echo $form->labelEx($model,'RequiredIsp'); ?>
		<?php echo $form->textField($model,'RequiredIsp',array('size'=>20,'maxlength'=>20)); ?>
		<?php echo $form->error($model,'RequiredIsp'); ?>
	</div>

	<div class="row">
		<?php echo $form->labelEx($model,'LangCode'); ?>
		<?php
			$dropList[""] = "-- Unknown --";
			$dropListTmp = CHtml::listData(Lang::model()->findAll(array('order'=>'Label')), 'Code', 'Label');
			$dropList = array_merge($dropList, $dropListTmp);
			echo $form->dropDownList($model, 'LangCode', $dropList);
			echo $form->error($model,'LangCode');
		?>
	</div>

	<div class="row buttons">
		<?php echo CHtml::submitButton('Add'); ?>
	</div>

<?php $this->endWidget(); ?>

</div><!-- form -->
