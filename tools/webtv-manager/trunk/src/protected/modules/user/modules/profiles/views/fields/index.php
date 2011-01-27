<?php
#heading
$this->title = Yii::t("UserModule.user", 'List profile field');
#breadcrumb
$this->breadcrumbs=array(Yii::t("UserModule.user", 'Profile fields'));
#menu
$this->menu = array(
	YumMenuItemHelper::createField(),
	YumMenuItemHelper::manageFields(),
	YumMenuItemHelper::manageFieldsGroups());
?>

<?php $this->widget('zii.widgets.CListView', array(
	'dataProvider'=>$dataProvider,
	'itemView'=>'_view',
)); ?>
