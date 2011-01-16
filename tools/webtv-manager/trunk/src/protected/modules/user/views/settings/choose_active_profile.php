<?php
echo CHtml::beginForm(array('yumSettings/setActive'));
echo CHtml::hiddenField('returnTo', $returnTo);
echo CHtml::label(Yii::t('UserModule.user', 'Active setting profile'), 'active_profile');
echo ':&nbsp;';
echo CHtml::dropDownList('active_profile',
		$model->getActive(),
		CHtml::listData(YumSettings::model()->findAll(), 'id', 'title'));
echo CHtml::submitButton(Yii::t('UserModule.user', '<- Save'));
echo CHtml::Button(Yii::t('UserModule.user', 'Edit active profile settings'), array('submit' => array('//user/yumSettings/update', 'id' => 0)));
echo CHtml::endForm();

?>

