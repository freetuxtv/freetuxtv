<?php 
#page title
$this->pageTitle=Yii::app()->name . ' - '.Yii::t("UserModule.user", "Profile");
#breadcrumbs
$this->breadcrumbs=array(
	Yii::t("UserModule.user", "Profile")=>array('profile'),
	Yii::t("UserModule.user", "Edit"));
#title
$this->title = Yii::t("UserModule.user", 'Edit profile');
#menu
$this->menu = array(
	YumMenuItemHelper::manageUsers(),
	YumMenuItemHelper::listUsers(),
	YumMenuItemHelper::manageRoles(),
	YumMenuItemHelper::backToProfile(),
	YumMenuItemHelper::changePassword(),
	YumMenuItemHelper::logout());
?>

<?php if(Yii::app()->user->hasFlash('profileMessage')): ?>
<div class="success">
<?php echo Yii::app()->user->getFlash('profileMessage'); ?>
</div>
<?php endif; ?>
<div class="form">

<?php echo CHtml::beginForm(); ?>

<?php echo YumHelper::requiredFieldNote(); ?>

<?php echo CHtml::errorSummary($model);
  echo CHtml::errorSummary($profile); ?>
<?php 
$profileFields=YumProfileField::model()->forOwner()->sort()->with('group')->together()->findAll();

if ($profileFields) 
{
	foreach($profileFields as $field) 
	{
			?>
	<div class="row">
	<?php echo CHtml::activeLabelEx($profile,$field->varname);
			if ($field->field_type=="TEXT") {
				echo CHtml::activeTextArea($profile,
						$field->varname,
						array('rows'=>6, 'cols'=>50));
			} 
			else 
			{
				echo CHtml::activeTextField($profile,
						$field->varname,
						array('size'=>60,'maxlength'=>(($field->field_size)?$field->field_size:255)));
			}
			echo CHtml::error($profile,$field->varname); ?>
	</div>	
			<?php
			}
		}
?>
	<div class="row">
		<?php echo CHtml::activeLabelEx($model,'username'); ?>
		<?php echo CHtml::activeTextField($model,'username',array('size'=>20,'maxlength'=>20)); ?>
		<?php echo CHtml::error($model,'username'); ?>
	</div>

	<div class="row">
		<?php 
		echo CHtml::activeLabelEx($profile, 'privacy'); 
		echo CHtml::activeDropDownList($profile, 'privacy',
				array(
					'protected' => Yii::t('UserModule.user', 'protected'),
					'private' => Yii::t('UserModule.user', 'private'),
					'public' => Yii::t('UserModule.user', 'public'),
					)
); 
				echo CHtml::error($profile,'privacy'); 
?>
		</div>



	<div class="row buttons">
		<?php echo CHtml::submitButton($model->isNewRecord ? Yii::t("UserModule.user", 'Create') : Yii::t("UserModule.user", 'Save')); ?>
	</div>

<?php echo CHtml::endForm(); ?>

</div><!-- form -->
