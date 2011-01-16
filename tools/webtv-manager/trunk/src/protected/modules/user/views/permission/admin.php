<?php
$this->breadcrumbs=array(
		'Permissions'=>array('index'),
		'Manage',
		);

?>

<h1> <?php echo Yum::t('Manage permissions'); ?> </h1>

<?php $this->widget('zii.widgets.grid.CGridView', array(
			'id'=>'action-grid',
			'dataProvider'=>$model->search(),
			'filter'=>$model,
			'columns' => array(
				array(
					'name' => 'type',
					'value' => '$data->type',
					'filter' => array(
						'type' => Yum::t('Type'),
						'role' => Yum::t('Role'),
						)
					),
				array(
					'filter' => false,
					'name' => 'principal',
					'value' => '$data->type == "user" ? $data->principal : $data->principal_role->title'
					),
				array(
					'name' => 'action',
					'value' => '$data->Action->title',
					),
				)
			)
		); ?>
