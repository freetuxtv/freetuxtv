<?php
if(!isset($model)) 
	$model = new YumUserLogin();
#page title
$this->pageTitle = Yii::app()->name . ' - '.Yii::t("UserModule.user", "Login");
#heading
$this->title = Yii::t("UserModule.user", "Login");
#breadcrumbs
$this->breadcrumbs=array(Yii::t("UserModule.user", "Login"));
?>

<?php if(Yii::app()->user->hasFlash('loginMessage')): ?>

<div class="success">
	<?php echo Yii::app()->user->getFlash('loginMessage'); ?>
</div>

<?php endif; ?>

<p>
<?php 
echo Yii::t("UserModule.user",
		"Please fill out the following form with your login credentials:"); ?></p>

<div class="form">
<?php echo CHtml::beginForm(); ?>

	<?php echo YumHelper::requiredFieldNote(); ?>
	<?php echo CHtml::errorSummary($model); ?>
	
	<div class="row">
		<?php echo CHtml::activeLabelEx($model,'username'); ?>
		<?php echo CHtml::activeTextField($model,'username') ?>
	</div>
	
	<div class="row">
		<?php echo CHtml::activeLabelEx($model,'password'); ?>
		<?php echo CHtml::activePasswordField($model,'password') ?>
	</div>
	
	<div class="row">
	<p class="hint">
	<?php 
	printf("%s | %s",
			CHtml::link(Yii::t("UserModule.user", "Registration"),
				$this->module->registrationUrl),
			CHtml::link(Yii::t("UserModule.user", "Lost password?"),
				$this->module->recoveryUrl));
?>
</p>
</div>
	
	<div class="row rememberMe">
		<?php echo CHtml::activeCheckBox($model,'rememberMe'); ?>
		<?php echo CHtml::activeLabelEx($model,'rememberMe'); ?>
	</div>

	<div class="row submit">
		<?php echo CHtml::submitButton(Yii::t("UserModule.user", "Login")); ?>
	</div>
	
<?php echo CHtml::endForm(); ?>
</div><!-- form -->


<?php
$form = new CForm(array(
    'elements'=>array(
        'username'=>array(
            'type'=>'text',
            'maxlength'=>32,
        ),
        'password'=>array(
            'type'=>'password',
            'maxlength'=>32,
        ),
        'rememberMe'=>array(
            'type'=>'checkbox',
        )
    ),

    'buttons'=>array(
        'login'=>array(
            'type'=>'submit',
            'label'=>'Login',
        ),
    ),
), $model);
?>
