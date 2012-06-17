<?php
$this->breadcrumbs=array(
	'Web Streams'=>array('index'),
	$model->Name=>array('view','id'=>$model->Id),
	'Update',
);
?>

<?php
	$this->menu=array();
	$this->menu[] = array('label'=>'View', 'url'=>array('view', 'id'=>$model->Id));
	if(Yii::app()->user->checkAccess('editWebStream')) {
		$this->menu[] = array('label'=>'Edit', 'url'=>array('update', 'id'=>$model->Id));
	}
	if(Yii::app()->user->checkAccess('changeStatusWebStream')) {
		$this->menu[] = array('label'=>'Change status', 'url'=>array('changestatus', 'id'=>$model->Id));
	}
?>

<h1>Update WebStream <?php echo $model->Name; ?></h1>


<?php echo $this->renderPartial('_changestatus', array('model'=>$model)); ?>
