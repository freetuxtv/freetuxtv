<?php
#page title 
$this->pageTitle=Yii::app()->name . ' - ' . Yii::t("UserModule.user", "Profile");
#breadcrumbs
$this->breadcrumbs=array(Yii::t("UserModule.user", "Profile"));
#heading
$this->title = Yii::t("UserModule.user", 'Your profile');
#menu
$this->menu=array(
	YumMenuItemHelper::manageMyUsers(),
	YumMenuItemHelper::manageUsers(),
	YumMenuItemHelper::listUsers(),
	YumMenuItemHelper::updateProfile(),
	YumMenuItemHelper::manageFields(),
	YumMenuItemHelper::manageRoles(),
	YumMenuItemHelper::listMessages(),
	YumMenuItemHelper::composeMessage(),
	YumMenuItemHelper::changePassword(),
	YumMenuItemHelper::deleteAccount(),
	YumMenuItemHelper::logout());
?>

<?php
if($this->module->hasModule('messages')) $this->renderPartial('newMessages');?>

<?php if(Yii::app()->user->hasFlash('profileMessage')): ?>
<div class="errorSummary">
<?php echo Yii::app()->user->getFlash('profileMessage'); ?>
</div>
<?php endif; ?>
<table class="dataGrid">
<tr>
	<th class="label"><?php echo CHtml::encode($model->getAttributeLabel('username')); ?>
</th>
    <td><?php echo CHtml::encode($model->username); ?>
</td>
</tr>
<?php 
		$profileFields=YumProfileField::model()->forOwner()->sort()->with('group')->together()->findAll();
		if ($profileFields) {
			foreach($profileFields as $field) {
			?>
<tr>
	<th class="label"><?php echo CHtml::encode(Yii::t("UserModule.user", $field->title)); ?>
</th>
    <td><?php echo CHtml::encode($profile->getAttribute($field->varname)); ?>
</td>
</tr>
			<?php
			}
		}
?>
<tr>
	<th class="label"><?php echo CHtml::encode($model->getAttributeLabel('password')); ?>
</th>
    <td><?php echo CHtml::link(Yii::t("UserModule.user", "Change password"),array(YumHelper::route('{user}/changepassword'))); ?>
</td>
</tr>
<tr>
	<th class="label"><?php echo CHtml::encode($model->getAttributeLabel('createtime')); ?>
</th>
    <td><?php echo date(UserModule::$dateFormat,$model->createtime); ?>
</td>
</tr>
<tr>
	<th class="label"><?php echo CHtml::encode($model->getAttributeLabel('lastvisit')); ?>
</th>
    <td><?php echo date(UserModule::$dateFormat,$model->lastvisit); ?>
</td>
</tr>
<tr>
	<th class="label"><?php echo CHtml::encode($model->getAttributeLabel('status')); ?>
</th>
    <td><?php echo CHtml::encode(YumUser::itemAlias("UserStatus",$model->status));
    ?>
</td>
</tr>
</table>
