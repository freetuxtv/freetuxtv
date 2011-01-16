<?php
$this->breadcrumbs=array(
	Yum::t('Memberships')=>array('index'),
	Yum::t('Manage'),
);
		?>

<h1> <?php echo Yum::t('Memberships'); ?></h1>

<?php
$locale = CLocale::getInstance(Yii::app()->language);

 $this->widget('zii.widgets.grid.CGridView', array(
	'id'=>'membership-grid',
	'dataProvider'=>$model->search(),
	'filter'=>$model,
	'columns'=>array(
		'id',
		'user.username',
	array(
				'name'=>'order_date',
				'value' =>'date("Y. m. d G:i:s", $data->order_date)'),
		array(
				'name'=>'end_date',
				'value' =>'date("Y. m. d G:i:s", $data->end_date)'),
		array(
				'name'=>'payment_date',
				'value' =>'date("Y. m. d G:i:s", $data->payment_date)'),
		'role.price',
		'payment.title',
		array(
			'class'=>'CButtonColumn',
		),
	),
)); ?>
