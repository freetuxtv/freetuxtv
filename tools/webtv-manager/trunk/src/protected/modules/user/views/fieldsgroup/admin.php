<?php
$this->title = Yii::t("UserModule.user", 'Manage profile fields groups');
$this->breadcrumbs=array(
	Yii::t("UserModule.user", 'Profile fields groups')=>array('admin'),
	Yii::t("UserModule.user", 'Manage'),);

Yii::app()->clientScript->registerScript('search', "
$('.search-button').click(function(){
	$('.search-form').toggle();
	return false;
});
$('.search-form form').submit(function(){
	$.fn.yiiGridView.update('yum-profile-fields-group-grid', {
		data: $(this).serialize()
	});
	return false;
});
");
?>

<?php echo CHtml::link(Yii::t("UserModule.user",'Advanced search'),'#',array('class'=>'search-button')); ?>
<div class="search-form" style="display:none">
<?php $this->renderPartial('_search',array(
	'model'=>$model,
)); ?>
</div><!-- search-form -->

<?php $this->widget('zii.widgets.grid.CGridView', array(
	'id'=>'yum-profile-fields-group-grid',
	'dataProvider'=>$model->search(),
	'filter'=>$model,
	'columns'=>array(
		'id',
		'group_name',
		array(
			'name'=>'title',
			'value'=>'Yii::t("UserModule.user", $data->title)',
		),
		'position',
		array(
			'class'=>'CButtonColumn',
		),
	),
)); ?>
