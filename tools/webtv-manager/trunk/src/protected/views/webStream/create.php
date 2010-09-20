<?php
$this->breadcrumbs=array(
	'Web Streams'=>array('index'),
	'Create',
);

$this->menu=array(
	array('label'=>'List WebStream', 'url'=>array('index')),
	array('label'=>'Manage WebStream', 'url'=>array('admin')),
);
?>

<h1>Create WebStream</h1>

<?php echo $this->renderPartial('_form', array('model'=>$model)); ?>
