<?php 
Yii::app()->clientScript->registerCssFile(
		Yii::app()->getAssetManager()->publish(
			Yii::getPathOfAlias('YumAssets').'/css/yum.css'));

$module = Yum::module();
$this->beginContent($module->baseLayout); ?>

<div id="usermenu">
<?php $this->renderMenu(); ?>
</div>

<div id="usercontent">
<?php printf('<h2> %s </h2>', $this->title);  ?>
<?php echo $content;  ?>
</div>

<?php $this->endContent(); ?>
