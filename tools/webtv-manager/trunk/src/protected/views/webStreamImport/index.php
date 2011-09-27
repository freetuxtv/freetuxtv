<?php $this->pageTitle=Yii::app()->name; ?>

<h1>WSLFE Import module</h1>

<?php
	if(isset($_GET["error"])){
		if($_GET["error"] == 0){
?>
<p align="center" class="alert"><?php echo $_GET["success"] ?> links have been successfully imported !</p>
<?php
		}else if($_GET["error"] > 0){
?>
<p align="center" class="alert"><?php echo $_GET["error"] ?> error(s) have been encountered while importing link !</p>
<?php
		}else{
?>
<p align="center" class="alert">Error(s) have been encountered while importing link !</p>
<?php
		}
	}
?>

<p>You can use this form to check and compare a WSLFE file to the current content of the database:</p>

<?php echo $this->renderPartial('_importform', array('modelImportForm'=>$modelImportForm)); ?>

