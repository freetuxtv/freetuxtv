<?php

    if(empty($tabularIdx))
    {
        #title
        $this->title=Yii::t("UserModule.user", 'Update user')." ".$model->id;
        #breadcrumbs
        $this->breadcrumbs = array(
            Yii::t("UserModule.user", 'Users')=>array('index'),
            $model->username=>array('view','id'=>$model->id),
            Yii::t("UserModule.user", 'Update'));
        #menu
        $this->menu = array(
            YumMenuItemHelper::manageUsers(),
            YumMenuItemHelper::listUsers(),
            YumMenuItemHelper::createUser(),
            YumMenuItemHelper::viewUser(array('id'=>$model->id)),
            YumMenuItemHelper::manageRoles(),
            YumMenuItemHelper::updateProfile(array('id'=>$model->id),'Manage this profile'),
            YumMenuItemHelper::manageFields());
    }

    echo $this->renderPartial('_form', array(
		'model'=>$model,
        'profile'=>$profile,
		'tabularIdx'=>$tabularIdx));
?>
