<tr>
	<td>
	<!--<div class="view">-->

		<?php echo CHtml::link(CHtml::encode($data->Name), array('view', 'id'=>$data->Id)); ?>
		<br />

		<b><?php echo CHtml::encode($data->getAttributeLabel('Url')); ?>:</b>
		<?php echo CHtml::encode($data->Url); ?>

	<!--</div>-->
	</td>
	<td align="center">
		<?php echo CHtml::encode($data->Status); ?>
	</td>
</tr>
