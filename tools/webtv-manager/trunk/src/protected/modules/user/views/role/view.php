<?php
$this->title = Yii::t('UserModule.user','{role}', array(
			'{role}' => $model->title));

$this->breadcrumbs=array(
	Yum::t('Roles')=>array('index'),
	Yum::t('View'),
	$model->title
);

printf('<h2>%s</h2>', $model->title);
echo '<br />';
echo $model->description; ?>

<br />
<?php echo Yum::p('This users have been assigned to this role'); ?> 

<?php 
$this->widget('zii.widgets.grid.CGridView', array(
    'dataProvider'=>$assignedUsers,
    'columns'=>array(
        'username',          
        'status',          
    ),
));
?>
<br />
<?php echo Yum::p('This users have a ordered memberships of this role'); ?> 
<?php
$this->widget('zii.widgets.grid.CGridView', array(
    'dataProvider'=>$activeMemberships,
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

			),
		));

if(Yii::app()->user->isAdmin())
	echo CHtml::Button(Yum::t('Update role'), array('submit' => array('role/update', 'id' => $model->id)));

?>
