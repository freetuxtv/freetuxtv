<div class="view">

<div style="float: right;">
<em><?php echo date(Yum::module()->dateTimeFormat, $data->timestamp); ?></em>
<br />
<em><?php echo CHtml::encode($data->remote_addr); ?></em>

</div>

	<h3><?php echo CHtml::link($data->user->username,array('//user/user/view', 'id' => $data->user_id)); ?> </h3>

	<br />


<?php 
if($data->http_user_agent != '') {
echo CHtml::encode($data->getAttributeLabel('http_user_agent')); 
echo CHtml::encode($data->http_user_agent); 
echo '<br />';
}
?>


	<b><?php echo CHtml::encode($data->getAttributeLabel('action')); ?>:</b>
	<?php echo CHtml::encode($data->action); ?>
	<br />


</div>
