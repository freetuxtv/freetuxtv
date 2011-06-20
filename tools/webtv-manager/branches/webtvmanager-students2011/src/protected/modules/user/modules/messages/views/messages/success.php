<?php 
#heading
$this->title = Yii::t('UserModule.user', 'Your message has been sent.');
#breadcrumbs
$this->breadcrumbs=array(
	Yii::t('UserModule.user', 'Messages')=>array('index'),
	Yii::t('UserModule.user', 'Success'));
#menu
$this->menu = array(
	YumMenuItemHelper::composeMessage(array(),'Write another message'),
	YumMenuItemHelper::backToInbox());
?>

<p> <?php echo CHtml::link(Yii::t('UserModule.user', 'Back to inbox'), 
array('index')); ?> </p>

<p> <?php echo CHtml::link(Yii::t('UserModule.user', 'Write another message'), 
array('compose')); ?> </p>
