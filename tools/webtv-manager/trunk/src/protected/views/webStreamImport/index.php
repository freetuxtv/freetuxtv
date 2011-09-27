<?php $this->pageTitle=Yii::app()->name; ?>

<h1>WSLFE Import module</h1>

<?php
	if(isset($_GET["success"])){
?>
<p align="center" class="alert">The links have been successfully imported !</p>
<?php
	}
?>

<p>You can use this form to check and compare a WSLFE file to the current content of the database:</p>

<?php echo $this->renderPartial('_importform', array('modelImportForm'=>$modelImportForm)); ?>

