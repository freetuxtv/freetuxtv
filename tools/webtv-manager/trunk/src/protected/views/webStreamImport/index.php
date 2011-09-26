<?php $this->pageTitle=Yii::app()->name; ?>

<h1>WSLFE Import module</h1>

<p align="center" class="alert">The links have been successfully imported !</p>

<p>You can use this form to check and compare a WSLFE file to the current content of the database:</p>

<?php echo $this->renderPartial('_importform', array('modelImportForm'=>$modelImportForm)); ?>

