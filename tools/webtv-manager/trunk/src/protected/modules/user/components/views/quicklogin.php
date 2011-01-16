<div class="form">
<?php echo CHtml::beginForm(array('/user/user/login')); 

$link = '//' . Yii::app()->controller->uniqueid .  '/' . Yii::app()->controller->action->id;
echo CHtml::hiddenField('quicklogin', $link);
?>

	<?php echo CHtml::errorSummary($model); ?>
	
	<div class="row">
		<?php echo CHtml::activeLabelEx($model,'username'); ?>
		<?php echo CHtml::activeTextField($model,'username', array('size' => 10)) ?>
	</div>
	
	<div class="row">
		<?php echo CHtml::activeLabelEx($model,'password'); ?>
		<?php echo CHtml::activePasswordField($model,'password', array('size' => 10)); ?>
	</div>
	
	<div class="row">
</div>
	
	<div class="row submit">
		<?php echo CHtml::submitButton(Yii::t("user", "Login")); ?>
	</div>
	
<?php echo CHtml::endForm(); ?>
</div>
