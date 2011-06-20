<?php
#title
$this->title = Yii::t("UserModule.user", 'Create profile field'); 
#breadcrumbs
$this->breadcrumbs=array(
	Yii::t("UserModule.user", 'Profile fields')=>array('admin'),
	Yii::t("UserModule.user", 'Create'));
#menu
$this->menu = array(
	YumMenuItemHelper::manageUsers(),
	YumMenuItemHelper::manageFields(),
	YumMenuItemHelper::manageFieldsGroups());
?>

<?php echo $this->renderPartial('_form', array('model'=>$model)); ?>
