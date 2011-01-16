<?php
$profiles = Yii::app()->getModule('user')->enableProfiles;

$this->title = Yum::t('View user "{username}"',array(
			'{username}'=>$model->username));

$this->breadcrumbs = array(Yum::t('Users') => array('index'), $model->username);

echo Yum::renderFlash();

if(Yii::app()->user->isAdmin()) {
	$attributes = array(
			'id',
	);


	if(!Yum::module()->loginType & UserModule::LOGIN_BY_EMAIL)
		$attributes[] = 'username';

	if($profiles) {
		$profileFields = YumProfileField::model()->forOwner()->sort()->findAll();
		if ($profileFields && $model->profile) {
			foreach($profileFields as $field) {
				array_push($attributes, array(
							'label' => Yii::t('UserModule.user', $field->title),
							'type' => 'raw',
							'value' => is_array($model->profile)
							? $model->profile[0]->getAttribute($field->varname)
							: $model->profile->getAttribute($field->varname) ,
							));
			}
		}
	}

	array_push($attributes,
		'password',
		'activationKey',
		array(
			'name' => 'createtime',
			'value' => date(UserModule::$dateFormat,$model->createtime),
			),
		array(
			'name' => 'lastvisit',
			'value' => date(UserModule::$dateFormat,$model->lastvisit),
			),
		array(
			'name' => 'lastpasswordchange',
			'value' => date(UserModule::$dateFormat,$model->lastpasswordchange),
			),
		array(
			'name' => 'superuser',
			'value' => YumUser::itemAlias("AdminStatus",$model->superuser),
			),
		array(
			'name' => 'status',
			'value' => YumUser::itemAlias("UserStatus",$model->status),
			)
		);

	$this->widget('zii.widgets.CDetailView', array(
				'data'=>$model,
				'attributes'=>$attributes,
				));

} else {
	// For all users
	$attributes = array(
			'username',
			);

	if($profiles) {
		$profileFields = YumProfileField::model()->forAll()->sort()->findAll();
		if ($profileFields) {
			foreach($profileFields as $field) {
				array_push($attributes,array(
							'label' => Yii::t('UserModule.user', $field->title),
							'name' => $field->varname,
							'value' => $model->profile[0]->getAttribute($field->varname),
							));
			}
		}
	}

	array_push($attributes,
			array(
				'name' => 'createtime',
				'value' => date(UserModule::$dateFormat,$model->createtime),
				),
			array(
				'name' => 'lastvisit',
				'value' => date(UserModule::$dateFormat,$model->lastvisit),
				)
			);

	$this->widget('zii.widgets.CDetailView', array(
				'data'=>$model,
				'attributes'=>$attributes,
				));
}


if(Yii::app()->user->isAdmin()) {
	if($profiles && Yum::module()->enableProfileHistory) {
		$this->renderPartial('/profile/profile_history', array('model' => $model));
	}
	echo '<h2>'.Yum::t('This user belongs to these roles:') .'</h2>';

	if($model->roles) {
		echo "<ul>";
		foreach($model->roles as $role) {
			echo CHtml::tag('li',array(),CHtml::link(
						$role->title,array(Yum::route('role/view'),'id'=>$role->id)),true);
		}
		echo "</ul>";
	} else {
		printf('<p>%s</p>', Yum::t('None'));
	}
}

if(Yii::app()->user->isAdmin())
	echo CHtml::Button(
			Yum::t('Update User'), array(
				'submit' => array('user/update', 'id' => $model->id)));

	?>
