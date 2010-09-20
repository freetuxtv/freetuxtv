<?php
$this->breadcrumbs=array(
	'Web Streams'=>array('index'),
	$model->Name=>array('view','id'=>$model->Id),
	'Update',
);

$this->menu=array(
	array('label'=>'List WebStream', 'url'=>array('index')),
	array('label'=>'Create WebStream', 'url'=>array('create')),
	array('label'=>'View WebStream', 'url'=>array('view', 'id'=>$model->Id)),
	array('label'=>'Manage WebStream', 'url'=>array('admin')),
);
?>

<h1>Update WebStream <?php echo $model->Name; ?></h1>

<?php echo $this->renderPartial('_form', array('model'=>$model)); ?>
