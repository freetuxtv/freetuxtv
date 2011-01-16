<div id="profile">
<?php
$this->pageTitle = Yii::app()->name . ' - ' . Yum::t('Profile');
$this->breadcrumbs = array(Yum::t('Profile'), $model->username);
$this->title = Yum::t('Profile');
Yum::renderFlash(); 
echo $model->getAvatar(); 

if($model->id == Yii::app()->user->id)
	$this->renderPartial('/messages/new_messages');

?>

<table class="dataGrid">
<?php if(!Yum::module()->loginType & UserModule::LOGIN_BY_EMAIL) {?>
<tr>
<th class="label"><?php echo CHtml::activeLabel($model,'username'); ?>
</th>
<td><?php echo CHtml::encode($model->username); ?>
</td>
</tr>
<?php 
}
$profileFields = YumProfileField::model()->forOwner()->sort()->with('group')->together()->findAll();
if ($profileFields) {
	foreach($profileFields as $field) {
		if($field->field_type == 'DROPDOWNLIST') {
			?>
			<tr>
				<th class="label"><?php echo CHtml::encode(Yum::t($field->title)); ?>
				</th>
				<td><?php
				if(isset($model->profile[0]))
					echo CHtml::encode($model->profile[0]->{ucfirst($field->varname)}->{$field->related_field_name}); ?>
				</td>
				</tr>
				<?php
		} else {
			?>
				<tr>
				<th class="label"><?php echo CHtml::encode(Yum::t($field->title)); ?>
				</th>
				<td><?php 
				if(isset($model->profile[0]))
					echo CHtml::encode($model->profile[0]->getAttribute($field->varname)); ?>
						</td>
				</tr>
				<?php
		}
	}
}
?>
<tr>
<th class="label"><?php echo Yum::t('First visit'); ?>
</th>
<td><?php echo date(UserModule::$dateFormat, $model->createtime); ?>
</td>
</tr>
<tr>
	<th class="label"><?php echo Yum::t('Last visit'); ?>
</th>
    <td><?php echo date(UserModule::$dateFormat,$model->lastvisit); ?>
</td>
</tr>
</table>

<?php $this->renderPartial('/friendship/friends', array('model' => $model)); ?> 
<br /> 
<?php $this->renderPartial('/messages/write_a_message', array('model' => $model)); ?> 
<br /> 
<?php
 if(isset($model->profile[0]))
	$this->renderPartial('/profileComment/index', array('model' => $model->profile[0])); ?> 
</div>
