<div class="wide form">

<?php
	$form=$this->beginWidget('CActiveForm', array(
		'action'=>Yii::app()->createUrl("WebStreamImport/checkimport"),
		'method'=>'GET',
		'id'=>'web-stream-import-form',
		'enableAjaxValidation'=>false,
		));
?>

	<div class="row">
		<?php
			echo $form->labelEx($modelImportForm,'UrlWSLFEData');
			echo $form->textField($modelImportForm, 'UrlWSLFEData', array('size'=>60,'maxlength'=>255));
			echo $form->error($modelImportForm,'UrlWSLFEData');
		?>
	</div>

	<div class="row buttons">
		<?php echo CHtml::submitButton('Check'); ?>
	</div>

<?php $this->endWidget(); ?>

</div>
