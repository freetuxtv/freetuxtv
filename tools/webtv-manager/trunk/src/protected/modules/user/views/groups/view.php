<?php
$this->breadcrumbs=array(
		'Usergroups'=>array('index'),
		$model->title,
		);

?>

<h1> <?php echo $model->title; ?> </h1>

<?php
$this->widget('zii.widgets.CDetailView', array(
			'data'=>$model,
			'attributes'=>array(
				'owner.username',
				'title',
				'description',
				),
			)); 

printf('<h2>%s</h2>', Yum::t('Participants'));

$participants = new CActiveDataProvider('YumGroupParticipation', array(
			'criteria' => array(
				'condition' => 'group_id = :group_id',
				'join' => 'left join usergroup on group_id = usergroup.id',
				'params' => array(':group_id' => $model->id))));

$this->widget('zii.widgets.CListView', array(
    'dataProvider'=>$participants,
    'itemView'=>'_participant', 
));

?>

<div style="clear: both;"> </div>



