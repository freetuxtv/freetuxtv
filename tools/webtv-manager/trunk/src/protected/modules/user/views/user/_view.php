<div class="tooltip" id="tooltip_<?php echo $data->id; ?>"> 
	<?php $this->renderPartial('_tooltip', array('data' =>  $data)); ?>
</div>

<div class="view_user" id="user_<?php echo $data->id;?>"> 
<?php printf('<h3>%s</h3>', $data->username); ?>

<?php echo CHtml::link($data->getAvatar(null, true),
		array(
			'//user/profile/view', 'id' => $data->id)); ?>
</div>

<?php
Yii::app()->clientScript->registerScript('tooltip_'.$data->id, "
$('#user_{$data->id}').tooltip({
'position': 'top',
'offset': [0, -50],
'tip': '#tooltip_{$data->id}',
'predelay': 100,
'fadeOutSpeed': 100,

}); 
");
?>

