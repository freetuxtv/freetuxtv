<?php 
// Page title
$this->pageTitle = Yii::app()->name . ' - '.Yii::t("user", "Registration");

// Header
$this->title = Yii::t("UserModule.user", "Registration");

// Breadcrumbs
$this->breadcrumbs = array(Yii::t("UserModule.user", "Registration"));
?>

<?php if(Yii::app()->user->hasFlash('registration')): ?>
<div class="success">
<?php echo Yii::app()->user->getFlash('registration'); ?>
</div>
<?php else: ?>

<div class="form">
<?php echo CHtml::beginForm(); ?>

	<?php echo YumHelper::requiredFieldNote(); ?>
	<?php echo CHtml::errorSummary($form); ?>
	
	<div class="row">
	<?php echo CHtml::activeLabelEx($form,'username'); ?>
	<?php echo CHtml::activeTextField($form,'username'); ?>
	</div>
	
	<div class="row">
	<?php echo CHtml::activeLabelEx($form,'password'); ?>
	<?php echo CHtml::activePasswordField($form,'password'); ?>
	<p class="hint">
	<?php echo Yii::t("UserModule.user", "Minimal password length is 4 symbols."); ?>
	</p>
	</div>
	
	<div class="row">
	<?php echo CHtml::activeLabelEx($form,'verifyPassword'); ?>
	<?php echo CHtml::activePasswordField($form,'verifyPassword'); ?>
	</div>

	<?php 
	$profileFields=YumProfileField::model()->forRegistration()->sort()->findAll();

if ($profileFields) 
{
	if(!isset($profile))
		$profile = new YumProfile();

	foreach($profileFields as $field) 
	{
		?>
			<div class="row">
			<?php echo CHtml::activeLabelEx($profile, $field->varname); ?>
			<?php 
			if ($field->range) 
			{
				echo CHtml::activeDropDownList($profile,
						$field->varname,
						YumProfile::range($field->range));
			}
		elseif ($field->field_type == "TEXT") 
		{
			echo CHtml::activeTextArea($profile,
					$field->varname, 
					array('rows'=>6, 'cols'=>50));
		}
			else 
			{
				echo CHtml::activeTextField($profile,
						$field->varname,
						array(
							'size'=>60,
							'maxlength'=>(($field->field_size)?$field->field_size:255)));
			}
		?>
			<?php echo CHtml::error($profile,$field->varname); ?>
			</div>  
			<?php
	}
}
?>

	<?php if(extension_loaded('gd') && Yii::app()->controller->module->allowCaptcha): ?>
	<div class="row">
		<?php echo CHtml::activeLabelEx($form,'verifyCode'); ?>
		<div>
		<?php $this->widget('CCaptcha'); ?>
		<?php echo CHtml::activeTextField($form,'verifyCode'); ?>
		</div>
		<p class="hint"><?php echo Yii::t("UserModule.user","Please enter the letters as they are shown in the image above."); ?>
		<br/><?php echo Yii::t("UserModule.user","Letters are not case-sensitive."); ?></p>
	</div>
	<?php endif; ?>
	
	<div class="row submit">
		<?php echo CHtml::submitButton(Yii::t("UserModule.user", "Registration")); ?>
	</div>

<?php echo CHtml::endForm(); ?>
</div><!-- form -->
<?php endif; ?>
