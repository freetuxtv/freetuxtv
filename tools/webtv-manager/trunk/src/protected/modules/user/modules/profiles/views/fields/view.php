<?php
#heading
$this->title = Yii::t("UserModule.user", 'View profile field #{fieldname}',array('fieldname'=>$model->varname));
#breadcrumbs
$this->breadcrumbs=array(
	Yii::t("UserModule.user", 'Profile fields')=>array('admin'),
	Yii::t("UserModule.user", $model->title));
#menu
$this->menu = array(
	YumMenuItemHelper::manageUsers(),
	YumMenuItemHelper::manageFields(),	
	YumMenuItemHelper::createField(),
	YumMenuItemHelper::updateField(),
	YumMenuItemHelper::manageFieldsGroups());
?>

<?php $this->widget('zii.widgets.CDetailView', array(
	'data'=>$model,
	'attributes'=>array(
		'id',
		'varname',
		'title',
		'hint',
		'field_type',
		'field_size',
		'field_size_min',
		'required',
		'match',
		'range',
		'error_message',
		'other_validator',
		'default',
		'position',
		'visible',
	),
)); ?>
