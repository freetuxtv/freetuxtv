<?php
$this->breadcrumbs = array(
	'Usergroups',
	Yii::t('app', 'Index'),
);

echo CHtml::Button(Yum::t('Create new Usergroup'), array('submit' => array('create'))); 

?>

<h1> <?php echo Yum::t('Usergroups'); ?></h1>

<?php $this->widget('zii.widgets.CListView', array(
	'dataProvider'=>$dataProvider,
	'itemView'=>'_view',
)); ?>
