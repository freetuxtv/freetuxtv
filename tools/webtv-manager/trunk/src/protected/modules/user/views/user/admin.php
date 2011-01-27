<?php
// Header
$this->title = Yii::t('UserModule.user', 'Manage users');

// Breadcrumbs
$this->breadcrumbs = array(
	Yii::t("UserModule.user", 'Users') => array('index'),
	Yii::t("UserModule.user", 'Manage'));
#menu
$this->menu = array(
	YumMenuItemHelper::listUsers(),
	YumMenuItemHelper::createUser(),
	YumMenuItemHelper::manageRoles(),
	YumMenuItemHelper::manageFields(),
	YumMenuItemHelper::displayProfile(),
	YumMenuItemHelper::logout()
	);

if(Yii::app()->user->hasFlash('adminMessage')) 
	printf('<div class="errorSummary">%s</div>', Yii::app()->user->getFlash('adminMessage')); 

$this->widget('zii.widgets.grid.CGridView', array(
	'dataProvider'=>$dataProvider,
		'columns'=>array(
			array(
				'name'=>'id',
				'type'=>'raw',
				'value'=>'CHtml::link(CHtml::encode($data->id),
				array(YumHelper::route("{user}/update"),"id"=>$data->id))',
			),
			array(
				'name'=>'username',
				'type'=>'raw',
				'value'=>'CHtml::link(CHtml::encode($data->username),
				array(YumHelper::route("{user}/view"),"id"=>$data->id))',
			),
			array(
				'name'=>'createtime',
				'value'=>'date(UserModule::$dateFormat,$data->createtime)',
			),
			array(
				'name'=>'lastvisit',
				'value'=>'date(UserModule::$dateFormat,$data->lastvisit)',
			),
			array(
				'name'=>'status',
				'value'=>'YumUser::itemAlias("UserStatus",$data->status)',
			),
			array(
				'name'=>'superuser',
				'value'=>'YumUser::itemAlias("AdminStatus",$data->superuser)',
			),
			array(
				'class'=>'CButtonColumn',
			),
))); ?>
