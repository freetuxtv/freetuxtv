<div class="form">
<?php 

$form = $this->beginWidget('CActiveForm', array(
			'id'=>'user-form',
			'enableAjaxValidation'=>false));
?>

<div class="note">
<?php echo Yum::requiredFieldNote(); ?>
<?php echo CHtml::errorSummary(array($model,
 $passwordform,
 isset($profile) ? $profile : null )); ?>
</div>

<?php if(Yum::module()->enableRoles) { ?>
<div class="row roles">
<p> <?php echo Yum::t('User belongs to these roles'); ?> </p>

	<?php $this->widget('YumModule.components.Relation', array(
				'model' => $model,
				'relation' => 'roles',
				'style' => 'checkbox',
				'fields' => 'title',
				'htmlOptions' => array(
					'checkAll' => Yum::t('Choose All'),
					'template' => '<div class="checkbox">{input}</div>{label}'),
				'showAddButton' => false
				)); ?>
</div>
<?php } ?>


<div class="row">
<?php echo $form->label($model, 'username');
echo $form->textField($model, 'username');
echo $form->error($model, 'username'); ?>
</div>


<div class="row">
<p> Leave password <em> empty </em> to 
<?php echo $model->isNewRecord ? 'generate a random Password' : 'keep it <em> unchanged </em>'; ?> </p>
<?php $this->renderPartial('/user/passwordfields', array(
			'form'=>$passwordform)); ?>
</div>
<?php if(isset($profile)) 
		$this->renderPartial('/profile/_form', array('profile' => $profile)); ?>

<div class="row">
<?php echo $form->label($model, 'superuser');
echo $form->dropDownList($model, 'superuser',YumUser::itemAlias('AdminStatus'));
echo $form->error($model, 'superuser'); ?>
</div>

<div class="row">
<?php echo $form->label($model,'status');
echo $form->dropDownList($model,'status',YumUser::itemAlias('UserStatus'));
echo $form->error($model,'status'); ?>
</div>

<div class="row buttons">
<?php echo CHtml::submitButton($model->isNewRecord
			? Yum::t('Create')
			: Yum::t('Save')); ?>
</div>

<?php $this->endWidget(); ?>
</div>
	<div style="clear:both;"></div>
