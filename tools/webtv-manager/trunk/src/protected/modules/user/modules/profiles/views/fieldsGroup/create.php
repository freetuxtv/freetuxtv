<?php
$this->title = Yii::t("UserModule.user", 'Create profile fields group');
#breadcrumbs
$this->breadcrumbs=array(
	Yii::t('UserModule.user','Profile fields groups')=>array('admin'),
	Yii::t('UserModule.user','Create'));
#menu
$this->menu=array(
	YumMenuItemHelper::manageUsers(),
	YumMenuItemHelper::manageFields());
?>

<?php echo $this->renderPartial('_form', array('model'=>$model)); ?>
