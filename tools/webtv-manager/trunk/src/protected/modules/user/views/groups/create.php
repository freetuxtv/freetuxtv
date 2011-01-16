<?php
$this->breadcrumbs=array(
	'Usergroups'=>array(Yii::t('app', 'index')),
	Yii::t('app', 'Create'),
);

$this->menu=array(
	array('label'=>Yii::t('app', 'List') . ' Usergroup', 'url'=>array('index')),
	array('label'=>Yii::t('app', 'Manage') . ' Usergroup', 'url'=>array('admin')),
);
?>

<h1> Create Usergroup </h1>
<?php
$this->renderPartial('_form', array(
			'model' => $model,
			'buttons' => 'create'));

?>

