<?php
$this->title = Yii::t("UserModule.user", 'View profile fields group #{group_name}',array('{group_name}'=>$model->group_name)); 
$this->breadcrumbs=array(
	Yii::t('UserModule.user','Profile fields groups') => array('admin'),
	Yii::t('UserModule.user',$model->title));
?>

<?php $this->widget('zii.widgets.CDetailView', array(
	'data'=>$model,
	'attributes'=>array(
		'id',
		'group_name',
		'title',
		'position',
	),
)); ?>
