<?php
$this->title = Yii::t("UserModule.user", 'Manage profile fields'); 

$this->breadcrumbs=array(
	Yii::t("UserModule.user", 'Profile fields')=>array('admin'),
	Yii::t("UserModule.user", 'Manage'));

?>

<?php $this->widget('zii.widgets.grid.CGridView', array(
	'dataProvider'=>$dataProvider,
	'columns'=>array(
		'position',
		'varname',
		array(
			'name'=>'title',
			'value'=>'Yii::t("UserModule.user", $data->title)',
		),
		'field_type',
		'field_size',
		//'field_size_min',
		array(
			'name'=>'required',
			'value'=>'YumProfileField::itemAlias("required",$data->required)',
		),
		//'match',
		//'range',
		//'error_message',
		'other_validator',
		//'default',
		array(
			'name'=>'field_group_id',
			'value'=>'$data->group instanceof YumProfileFieldsGroup ? Yii::t("UserModule.user",$data->group->title) : Yii::t("UserModule.user","Not assigned")'
		),
		//'position',
		array(
			'name'=>'visible',
			'value'=>'YumProfileField::itemAlias("visible",$data->visible)',
		),
		//*/
		array(
			'class'=>'CButtonColumn',
		),
	),
)); ?>
