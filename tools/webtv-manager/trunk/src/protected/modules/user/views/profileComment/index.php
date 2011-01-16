<?php if($model->allow_comments) { ?>
<h1> <?php echo Yum::t('Profile Comments'); ?> </h1>

<?php 
$dataProvider = new CActiveDataProvider('YumProfileComment', array(
			'criteria'=>array(
				'condition'=>'profile_id = :profile_id',
				'params' => array(':profile_id' => $model->profile_id),
				'order'=>'createtime DESC')
			)
		);

$this->renderPartial('/profileComment/create', array(
			'comment' => new YumProfileComment,
			'profile' => $model));

$this->widget('zii.widgets.CListView', array(
			'dataProvider'=>$dataProvider,
			'itemView'=>Yum::module()->profileCommentView,
			));  

}


?>


