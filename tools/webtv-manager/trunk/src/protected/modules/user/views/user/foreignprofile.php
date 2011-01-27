<?php
#page title 
$this->pageTitle=Yii::app()->name . ' - '.Yii::t('UserModule.user', "Profile");
#heading
$this->title = Yii::t('UserModule.user', 'Profile of ') . $model->username;
#breadcrumbs
$this->breadcrumbs = array(Yii::t('UserModule.user', "Profile"), $model->username); 
#menu
$this->menu = array(
	YumMenuItemHelper::backToProfile(),
	YumMenuItemHelper::logout(),
	YumMenuItemHelper::composeMessage(array('to_user_id'=>$model->id),'Send a message to this user'));
?> 

<?php if(Yii::app()->user->hasFlash('profileMessage')): ?>
<div class="success">
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
		$profileFields=YumProfileField::model()->forOwner()->sort()->findAll();
		if ($profileFields) {
			foreach($profileFields as $field) {
			?>
<tr>
	<th class="label"><?php echo CHtml::encode(Yii::t('UserModule.user', $field->title)); ?>
</th>
    <td><?php echo CHtml::encode($profile->getAttribute($field->varname)); ?>
</td>
</tr>
			<?php
			}
		}
?>
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
</table>
