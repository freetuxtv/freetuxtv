<div class="view">

<h3> <?php echo CHtml::encode($data->title); ?> </h3> 
	<b><?php echo CHtml::encode($data->getAttributeLabel('owner_id')); ?>:</b>
<?php if(isset($data->owner))
	echo CHtml::encode($data->owner->username); ?>
	<br />


	<b><?php echo CHtml::encode($data->getAttributeLabel('description')); ?>:</b>
	<?php echo CHtml::encode(substr($data->description, 0, 200)) . '... '; ?>
	<br />

	<?php echo CHtml::button(Yum::t('View Details'), array(
				'submit' => array(
					'//user/groups/view', 'id' => $data->id))); ?>
				<?php echo CHtml::button(Yum::t('Join group'), array(
							'submit' => array(
								'//user/groups/join', 'id' => $data->id))); ?>

	</div>
