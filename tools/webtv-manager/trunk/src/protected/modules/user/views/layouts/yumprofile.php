<?php 
Yii::app()->clientScript->registerCssFile(
		Yii::app()->getAssetManager()->publish(
			Yii::getPathOfAlias('YumAssets').'/css/yum.css'));

$module = Yii::app()->getModule('user');
$this->beginContent($module->baseLayout); ?>

<div id="usermenu">
<?php $this->widget('MessageWidget'); ?>
<?php $this->widget('ProfileVisitWidget'); ?>
<?php $this->renderMenu(); ?>
</div>

<div id="usercontent">
<?php echo $content;  ?>
</div>

<?php $this->endContent(); ?>
