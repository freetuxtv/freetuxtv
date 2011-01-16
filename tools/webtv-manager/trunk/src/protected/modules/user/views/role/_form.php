<div class="form">

<?php echo CHtml::beginForm(); ?>

<?php echo Yum::requiredFieldNote(); ?>

<?php echo CHtml::errorSummary($model); ?>

<div class="row">
<?php echo CHtml::activeLabelEx($model,'title'); ?>
<?php echo CHtml::activeTextField($model,'title',array('size'=>20,'maxlength'=>20)); ?>
<?php echo CHtml::error($model,'title'); ?>
</div>

<div class="row">
<?php echo CHtml::activeLabelEx($model,'description'); ?>
<?php echo CHtml::activeTextArea($model,'description',array('rows'=>6, 'cols'=>50)); ?>
<?php echo CHtml::error($model,'description'); ?>
</div>	

<div class="row" style="float:right;">
<?php echo CHtml::label(Yum::t('This users have been assigned to this role'), ''); ?> 

<?php 
$this->widget('YumModule.components.Relation',
		array('model' => $model,
			'relation' => 'users',
			'style' => 'dropdownlist',
			'fields' => 'username',
			'htmlOptions' => array(
				'checkAll' => Yum::t('Choose All'),
				'template' => '<div style="float:left;margin-right:5px;">{input}</div>{label}'),
			'showAddButton' => false
			));  
?>
</div>

<div class="row">
<?php echo CHtml::activeLabelEx($model,'selectable'); ?>
<?php echo CHtml::activeCheckBox($model, 'selectable'); ?>
<?php echo Yum::hint('When setting "selectable", the role can be choosen at the registration page'); ?>
</div>

<div class="row">
<?php echo CHtml::activeLabelEx($model,'searchable'); ?>
<?php echo CHtml::activeCheckBox($model, 'searchable'); ?>
<?php echo Yum::hint('When selecting searchable, users of this role can be searched in the "user Browse" function'); ?>
</div>

<div class="row">
<?php echo CHtml::activeLabelEx($model,'autoassign'); ?>
<?php echo CHtml::activeCheckBox($model, 'autoassign'); ?>
<?php echo Yum::hint('When selecting autoassign, this role will be assigned to new users automatically'); ?>
</div>


<div class="row">
<?php echo CHtml::activeLabelEx($model,'is_membership_possible'); ?>
<?php echo CHtml::activeCheckBox($model, 'is_membership_possible'); ?>

</div>
<div class="membership">
<div class="row">
<?php echo CHtml::activeLabelEx($model,'price'); ?>
<?php echo CHtml::activeTextField($model, 'price'); ?>
<?php echo CHtml::Error($model, 'price'); ?>
</div>
<div class="hint"> 
<?php echo Yum::t('How expensive is a membership? Set to 0 to disable membership for this role'); ?>
</div>

<div class="row">
<?php echo CHtml::activeLabelEx($model,'duration'); ?>
<?php echo CHtml::activeTextField($model, 'duration'); ?>
<?php echo CHtml::Error($model, 'duration'); ?>
</div>
<div class="hint"> 
<?php echo Yum::t('How many days will the membership be valid after payment?'); ?>
</div>
</div>

<div class="row buttons">
<?php echo CHtml::submitButton($model->isNewRecord 
		? Yum::t('Create role') 
		: Yum::t('Save role')); ?>
</div>

<?php echo CHtml::endForm(); ?>

</div><!-- form -->
