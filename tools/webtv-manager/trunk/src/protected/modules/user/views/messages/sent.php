<?php
$this->title = Yum::t('Sent messages');

$this->breadcrumbs=array(
	Yum::t('Messages')=>array('index'),
	Yum::t('Sent messages'));

$this->widget('zii.widgets.grid.CGridView', array(
	'id'=>'yum-messages-grid',
	'dataProvider' => $dataProvider,
	'columns'=>array(
		array(
			'type' => 'raw',
			'name' => Yum::t('To'),
			'value' => 'CHtml::link($data->to_user->username, array(
					Yum::route(\'user/profile\'),
					"id" => $data->to_user_id)
				)'
			),
		array(
			'type' => 'raw',
			'name' => Yum::t('Sent at'),
			'value' => '$data->getDate()',
		),

		array(
			'type' => 'raw',
			'name' => Yum::t('title'),
			'value' => '$data->title',
		),
		array(
			'class'=>'CButtonColumn',
			'template' => '{view}{delete}',
		),
	),
)); ?>
