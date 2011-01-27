<?php
#heading
$this->title = Yii::t("UserModule.user", 'Update profile field'). ' ' . $model->varname;
#breadcrumbs
$this->breadcrumbs=array(
	Yii::t("UserModule.user", 'Profile fields')=>array('admin'),
	$model->title=>array('view','id'=>$model->id),
	Yii::t("UserModule.user", 'Update'));
#menu
$this->menu = array(
	YumMenuItemHelper::manageUsers(),
	YumMenuItemHelper::manageFields(),
	YumMenuItemHelper::manageFieldsGroups());
?>

<?php echo $this->renderPartial('_form', array('model'=>$model)); ?>
