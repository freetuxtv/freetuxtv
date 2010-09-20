<?php
$this->breadcrumbs=array(
	'Web Streams'=>array('index'),
	$model->Name,
);

/*
$this->menu=array(
	array('label'=>'List WebStream', 'url'=>array('index')),
	array('label'=>'Create WebStream', 'url'=>array('create')),
	array('label'=>'Update WebStream', 'url'=>array('update', 'id'=>$model->Id)),
	array('label'=>'Delete WebStream', 'url'=>'#', 'linkOptions'=>array('submit'=>array('delete','id'=>$model->Id),'confirm'=>'Are you sure you want to delete this item?')),
	array('label'=>'Manage WebStream', 'url'=>array('admin')),
);*/
?>

<h1>WebStream : <?php echo $model->Name; ?></h1>

<?php $this->widget('zii.widgets.CDetailView', array(
	'data'=>$model,
	'attributes'=>array(
		'Id',
		'Name',
		'Url',
		'RequiredIsp',
		'LangCode',
		'SubmissionDate',
	),
)); ?>
