<?php
#heading
$this->title = Yii::t('UserModule.user','Viewing message: {messageTitle}',array('messageTitle'=>$model->title));
#breadcrumbs
$this->breadcrumbs=array('Messages'=>array('index'),$model->title);
#menu
$this->menu=array(
	YumMenuItemHelper::composeMessage(),
	array('label'=>Yii::t('UserModule.user', 'Delete message'), 'url'=>'#', 'linkOptions'=>array('submit'=>array('delete','id'=>$model->id),'confirm'=>Yii::t('App', 'Are you sure to delete this item?'))),
	YumMenuItemHelper::composeMessage(array('to_user_id'=>$model->from_user_id),'Reply to message'),
	YumMenuItemHelper::backToInbox(),
	YumMenuItemHelper::backToProfile(),
);
?>

<h2> <?php echo Yii::t('UserModule.user', 'Message from ') . 
'<em>' . $model->from_user->username . '</em>';
echo ': ' . $model->title; ?> 
</h2>

<?php echo $model->message; ?>

