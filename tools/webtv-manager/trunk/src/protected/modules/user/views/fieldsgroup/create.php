<?php
$this->title = Yii::t("UserModule.user", 'Create profile fields group');
$this->breadcrumbs=array(
	Yii::t('UserModule.user','Profile fields groups')=>array('admin'),
	Yii::t('UserModule.user','Create'));
?>

<?php echo $this->renderPartial('_form', array('model'=>$model)); ?>
