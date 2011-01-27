<?php
#title
$this->title = Yii::t("UserModule.user", 'View profile fields group #{group_name}',array('{group_name}'=>$model->group_name)); 
#breadcrumbs
$this->breadcrumbs=array(
	Yii::t('UserModule.user','Profile fields groups') => array('admin'),
	Yii::t('UserModule.user',$model->title));
#menu
$this->menu=array(
	YumMenuItemHelper::manageUsers(),
	YumMenuItemHelper::manageFields(),
	YumMenuItemHelper::createFieldsGroup(),
	YumMenuItemHelper::updateFieldsGroup(array('id'=>$model->id)),
	YumMenuItemHelper::manageFieldsGroups());
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
