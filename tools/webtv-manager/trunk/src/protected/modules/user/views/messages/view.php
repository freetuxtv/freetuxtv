<?php
$this->title = $model->title;
$this->breadcrumbs=array(Yum::t('Messages')=>array('index'),$model->title);
?>

<h2> <?php echo Yum::t('Message from') .  ' <em>' . $model->from_user->username . '</em>';

echo ': ' . $model->title; ?> 
</h2>

<hr />

<div class="message">
<?php echo $model->message; ?>
</div>

<hr />
<?php
if(Yii::app()->user->id != $model->from_user_id)
 echo CHtml::Button(Yum::t('Reply to message'), array(
			'submit' => array(
				'//user/messages/compose',
				'to_user_id' => $model->from_user_id)));
?>
