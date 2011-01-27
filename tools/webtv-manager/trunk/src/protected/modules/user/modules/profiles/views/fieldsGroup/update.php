<?php
$this->title = Yii::t('UserModule.user', 'Update profile fields group {group_name}',array('{group_name}'=>$model->group_name));
#breadcrumbs
$this->breadcrumbs=array(
	Yii::t('UserModule.user','Profile fields groups')=>array('admin'),
	$model->title=>array('view','id'=>$model->id),
	Yii::t('UserModule.user', 'Update'));

$this->menu=array(
	YumMenuItemHelper::manageUsers(),
	YumMenuItemHelper::manageFields(),
	YumMenuItemHelper::createField(array('in_group'=>$model->id)),
	YumMenuItemHelper::manageFieldsGroups());
?>

<?php echo $this->renderPartial('_form', array('model'=>$model)); ?>