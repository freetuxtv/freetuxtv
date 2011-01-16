<?php 
$this->pageTitle=Yii::app()->name . ' - '.Yum::t( "Profile");
$this->breadcrumbs=array(
		Yum::t( "Profile")=>array('profile'),
		Yum::t( "Edit"));
$this->title = Yum::t( 'Edit profile');
?>

<?php if(Yii::app()->user->hasFlash('profileMessage')): ?>
<div class="success">
<?php echo Yii::app()->user->getFlash('profileMessage'); ?>
</div>
<?php endif; ?>
<div class="form">

<?php echo CHtml::beginForm(); ?>

<?php echo Yum::requiredFieldNote(); ?>

<?php echo CHtml::errorSummary(array($model, $profile)); ?>

<div class="row">
<?php echo CHtml::activeLabelEx($model,'username'); ?>
<?php echo CHtml::activeTextField($model,'username',array('size'=>20,'maxlength'=>20)); ?>
<?php echo CHtml::error($model,'username'); ?>
</div>

<?php if(isset($profile) && is_object($profile)) 
	$this->renderPartial('/profile/_form', array('profile' => $profile)); ?>

	<div class="row buttons">
	<?php echo CHtml::submitButton($model->isNewRecord 
			? Yum::t('Create') 
			: Yum::t('Save')); ?>
	</div>

	<?php echo CHtml::endForm(); ?>

	</div><!-- form -->
