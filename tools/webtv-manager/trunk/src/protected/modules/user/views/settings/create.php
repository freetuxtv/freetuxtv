<?php
$this->title = Yii::t('UserModule.user', 'Create new settings profile');
$this->breadcrumbs=array(
	Yii::t('UserModule.user','User administration panel')=>array('//user/user/adminpanel'),
	Yii::t('UserModule.user', 'Module settings')=>array('index'),
	Yii::t('UserModule.user', 'Create'),
);

$this->menu=array(
	array('label'=>Yii::t('UserModule.user', 'Manage settings profiles'), 'url'=>array('admin')),
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

<div class="buttons">
	<?php
	$url = array(Yii::app()->request->getQuery('returnTo'));
	if(empty($url[0])) 
	$url = array('//user/yumSettings/index');
echo CHtml::Button(Yii::t('app', 'Cancel'), array('submit' => $url)); ?>&nbsp;
<?php echo CHtml::submitButton(Yii::t('app', 'Create')); ?>
</div>

<?php $this->endWidget(); ?>

</div>
