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
			echo CHtml::submitButton('Check file');
		?>
	</div>
	<div class="row">
		<?php
			echo $form->labelEx($modelImportForm,'OnlyNewLinks');
			echo $form->checkBox($modelImportForm, 'OnlyNewLinks');
			echo $form->error($modelImportForm,'OnlyNewLinks');
		?>
	</div>

<?php $this->endWidget(); ?>

</div>
