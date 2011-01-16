<?php
$this->title = Yii::t('UserModule.user', 'Change setting profile') . ' "' . $model->title. '"';
$this->breadcrumbs=array(
	Yii::t('UserModule.user','User administration')=>array('//user/user/admin'),
	Yii::t('UserModule.user', 'Module settings')=>array('index'),
	$model->title=>array('update','id'=>$model->id),
	Yii::t('UserModule.user', 'Update'),
);
?>

<div class="form">

<?php $form=$this->beginWidget('CActiveForm', array(
	'id'=>'yum-settings-form',
	'enableAjaxValidation'=>true,
	'htmlOptions' => array('style' => 'width: 400px;')
)); 
echo $this->renderPartial('/settings/_form', array(
	'model'=>$model,
	'form' =>$form
	)); ?>

<div class="row buttons">
	<?php
	$url = array('//user/yumSettings/index');
	echo CHtml::Button(Yii::t('UserModule.user', 'Cancel'), array('submit' => $url)); ?>&nbsp;

<?php echo CHtml::submitButton(Yii::t('UserModule.user', 'Update')); ?>
</div>

<?php $this->endWidget(); ?>

</div><!-- form -->
