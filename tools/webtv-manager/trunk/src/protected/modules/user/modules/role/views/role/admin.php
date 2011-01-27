<?php
#heading
$this->title = Yii::t('UserModule.user', 'Manage roles'); 
#breadcrumbs
$this->breadcrumbs=array(
	Yii::t("UserModule.user", 'Roles')=>array('index'),
	Yii::t("UserModule.user", 'Manage'),
);
#menu
$this->menu = array(
	YumMenuItemHelper::createRole(),
	YumMenuItemHelper::manageUsers());
?>

<?php $this->widget('zii.widgets.grid.CGridView', array(
	'dataProvider'=>$dataProvider,
	'columns'=>array(
		array(
			'name' => 'id',
			'type' => 'raw',
			'value'=> 'CHtml::link(CHtml::encode($data->id),
				array(YumHelper::route("{roles}/role/update"),"id"=>$data->id))',
		),
		array(
			'name' => 'title',
			'type' => 'raw',
			'value'=> 'CHtml::link(CHtml::encode($data->title),
				array(YumHelper::route("{roles}/role/view"),"id"=>$data->id))',
		),
		array(
			'class'=>'CButtonColumn',
		),
	),
)); ?>
