<?php
$this->title = Yii::t('UserModule.user', 'User Management settings configuration'); 
$this->breadcrumbs=array(
		Yum::t('User administration')=>array('//user/user/admin'),
		Yum::t('Manage'),
		);

?>

<?php $this->renderPartial('/settings/choose_active_profile', array(
			'returnTo' => '/settings/index',
					'model' => $model)); ?>
		<?php $this->widget('zii.widgets.grid.CGridView', array(
	'id'=>'yum-settings-grid',
	'dataProvider'=>$model->search(),
	'filter'=>$model,
	'columns'=>array(
		'id',
		'title',
		array(
			'name'=>'is_active',
			'value'=>'$data->is_active?Yii::t("UserModule.user","Yes"):Yii::t("UserModule.user","No")',
			'filter' => false,
			),
		array(
			'class'=>'CButtonColumn',
			'template' => '{update}{delete}',
		),
	),
)); ?>
