<?php
    if(empty($tabularIdx))
    {
    #heading
    $this->title = Yii::t('UserModule.user', "Create user");
    #breadcrumbs
    $this->breadcrumbs = array(
        Yii::t('UserModule.user', 'Users') => array('index'),
    Yii::t('UserModule.user', 'Create'));
    #menu
    $this->menu = array(
        YumMenuItemHelper::listUsers(),
        YumMenuItemHelper::manageUsers(),
        YumMenuItemHelper::manageFields());
    }

    echo $this->renderPartial('_form', array(
		'model'=>$model,
		'profile'=>$profile,
		'tabularIdx'=>$tabularIdx));
?>
