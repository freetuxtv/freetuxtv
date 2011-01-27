<?php
#heading
$this->title = Yii::t('UserModule.user', 'My inbox');
#breadcrumbs
$this->breadcrumbs=array(
	Yii::t('UserModule.user', 'Messages')=>array('index'),
	Yii::t('UserModule.user', 'My inbox'));
#menu
$this->menu=array(
	YumMenuItemHelper::manageUsers(),
	YumMenuItemHelper::composeMessage(),
	YumMenuItemHelper::backToProfile());
?>

<?php $this->widget('zii.widgets.grid.CGridView', array(
	'id'=>'yum-messages-grid',
	'dataProvider'=>$dataProvider,
	'columns'=>array(
		array(
			'type' => 'raw',
			'name' => Yii::t('UserModule.user', 'from'),
			'value' => 'CHtml::link($data->from_user->username, array(
					Yum::route(\'{user}/profile\'),
					"id" => $data->from_user_id)
				)'
			),
		array(
			'type' => 'raw',
			'name' => Yii::t('UserModule.user', 'title'),
			'value' => '$data->getTitle()',
		),
		array(
			'class'=>'CButtonColumn',
			'template' => '{view}{delete}',
		),
	),
)); ?>
