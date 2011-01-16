<div class="form">

<?php $form=$this->beginWidget('CActiveForm', array(
	'id'=>'yum-profile-fields-group-form',
	'enableAjaxValidation'=>false,
)); ?>

	<?php echo Yum::requiredFieldNote(); ?>

	<?php echo $form->errorSummary($model); ?>

	<div class="row">
		<?php echo $form->labelEx($model,'group_name'); ?>
		<?php echo $form->textField($model,'group_name',array('size'=>50,'maxlength'=>50)); ?>
		<?php echo $form->error($model,'group_name'); ?>
		<p class="hint"><?php echo Yii::t("UserModule.user","Allowed are lowercase letters and digits."); ?></p>
	</div>

	<div class="row">
		<?php echo $form->labelEx($model,'title'); ?>
		<?php echo $form->textField($model,'title',array('size'=>60,'maxlength'=>255)); ?>
		<?php echo $form->error($model,'title'); ?>
		<p class="hint"><?php echo Yii::t("UserModule.user",'Group name on the language of "sourceLanguage".'); ?></p>
	</div>

	<div class="row">
		<?php echo $form->labelEx($model,'position'); ?>
		<?php echo $form->textField($model,'position'); ?>
		<?php echo $form->error($model,'position'); ?>
		<p class="hint"><?php echo Yii::t("UserModule.user",'Display order of group.'); ?></p>
	</div>

	<div class="row buttons">
	<?php echo CHtml::submitButton($model->isNewRecord 
			? Yii::t('UserModule.user', 'Create') 
			: Yii::t('UserModule.user', 'Save')); ?>
	</div>

<?php $this->endWidget(); ?>

</div><!-- form -->
