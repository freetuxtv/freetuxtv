<?php
$this->title = Yii::t('UserModule.user', 'Update profile fields group {group_name}',array('{group_name}'=>$model->group_name));
$this->breadcrumbs=array(
	Yii::t('UserModule.user','Profile fields groups')=>array('admin'),
	$model->title=>array('view','id'=>$model->id),
	Yii::t('UserModule.user', 'Update'));

?>

<?php echo $this->renderPartial('_form', array('model'=>$model)); ?>
