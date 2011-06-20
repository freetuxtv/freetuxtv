<?php 
#page title
$this->pageTitle=Yii::app()->name . ' - '.Yii::t('UserModule.user', "Change password");
#heading
$this->title = Yii::t('UserModule.user', "Change password");
#breadcrumbs
$this->breadcrumbs = array(
	Yii::t('UserModule.user', "Profile") => array('profile'),
	Yii::t('UserModule.user', "Change password"));
#menu
$this->menu = array(YumMenuItemHelper::backToProfile());
?>

<div class="form">
<?php echo CHtml::beginForm(); ?>

	<?php echo YumHelper::requiredFieldNote(); ?>
	<?php echo CHtml::errorSummary($form); ?>
	
	<div class="row">
	<?php echo CHtml::activeLabelEx($form,'password'); ?>
	<?php echo CHtml::activePasswordField($form,'password'); ?>
	<p class="hint">
	<?php echo Yii::t('UserModule.user', "Minimal password length is 4 symbols."); ?>
	</p>
	</div>
	
	<div class="row">
	<?php echo CHtml::activeLabelEx($form,'verifyPassword'); ?>
	<?php echo CHtml::activePasswordField($form,'verifyPassword'); ?>
	</div>
	
	
	<div class="row submit">
	<?php echo CHtml::submitButton(Yii::t('UserModule.user', "Save")); ?>
	</div>

<?php echo CHtml::endForm(); ?>
</div><!-- form -->
