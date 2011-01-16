<?php
$this->title = Yum::t('Upload avatar');

$this->breadcrumbs = array('Avatar upload');

if($model->avatar) {
	echo Yum::t('Your Avatar image');
	echo '<br />';
	echo $model->getAvatar();
}
else
	echo Yum::t('You do not have set an avatar image yet');

	echo '<br />';

	echo '<p>' . Yum::t('The image should have at least 50px and a maximum of 200px in width and height. Supported filetypes are .jpg, .gif and .png') . '</p>';
	echo CHtml::errorSummary($model);
	echo CHtml::beginForm(array('//user/avatar/editAvatar'), 'POST', array(
				'enctype' => 'multipart/form-data'));
	echo '<div class="row">';
	echo CHtml::activeLabelEx($model, 'avatar');
	echo CHtml::activeFileField($model, 'avatar');
	echo CHtml::error($model, 'avatar');
	echo '</div>';
	echo CHtml::Button(Yum::t('Remove Avatar'), array(
				'submit' => array(
					'avatar/removeAvatar')));
	echo CHtml::submitButton(Yum::t('Upload Avatar'));
	echo CHtml::endForm();

