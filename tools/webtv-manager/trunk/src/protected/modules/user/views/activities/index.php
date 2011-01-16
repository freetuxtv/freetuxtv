<?php
$this->breadcrumbs = array(
	Yum::t('Activities'),
	Yum::t('Index'),
);
?>

<h1> <?php echo Yum::t('Activities'); ?></h1>

<?php 
echo CHtml::beginForm();

echo CHtml::label(Yum::t('Show activities'). ':' , 'activities');
$filter = 'all';
if(isset($_POST['activities']))
$filter = $_POST['activities'];
echo CHtml::dropDownList('activities',
		$filter,
		YumActivityController::possibleActions(), array(
			'empty' => array('all' => Yum::t('All')),
			'submit' => array('index')));

echo CHtml::label(Yum::t('of user') .': ', 'user');
$users = 'all';
if(isset($_POST['users']))
$users = $_POST['users'];
echo CHtml::dropDownList('users',
		$users,
		CHtml::listData(YumUser::model()->findAll(), 'id', 'username'), array(
			'empty' => array('all' => Yum::t('All')),
			'submit' => array('index')));

echo CHtml::endForm();

$this->widget('zii.widgets.CListView', array(
	'sortableAttributes' => array('timestamp', 'user_id', 'action'),
	'dataProvider'=>$dataProvider,
	'itemView'=>'_view',
)); ?>
