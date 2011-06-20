<?php
#heading
$this->title = Yii::t("UserModule.user", "Create role");
#breadcrumbs
$this->breadcrumbs=array(
	Yii::t("UserModule.user", 'Roles')=>array('index'),
	Yii::t("UserModule.user", 'Create'));
#menu
$this->menu = array(
	YumMenuItemHelper::manageRoles(),
	YumMenuItemHelper::manageUsers());
?>

<?php echo $this->renderPartial('_form', array('model'=>$model)); ?>
