<?php $this->pageTitle=Yii::app()->name; ?>

<h1>WSLFE Import module</h1>

<p>Here you can see the result of the comparaison to from the WSLFE file :</p>

<?php echo $this->renderPartial('_importform', array('modelImportForm'=>$modelImportForm)); ?>

<div class="form">

<?php
	echo CHtml::beginForm(
		Yii::app()->createUrl("WebStreamImport/import"),
		'POST'
	);

	echo CHtml::hiddenField('WebStreamImportForm[UrlWSLFEData]', $modelImportForm->UrlWSLFEData);	
?>

<p align="center">The import module has found <?php echo count($webStreamList) ?> links.</p>

<?php
	$dataProvider=new CArrayDataProvider($webStreamList, array(
		'id'=>'webstream_list',
		'pagination'=>array(
			'pageSize'=>count($webStreamList),
		),
	));

	$this->widget('zii.widgets.grid.CGridView', array(
		'dataProvider'=>$dataProvider,
		'enablePagination'=>false,
		'summaryText'=>'',
		'columns'=>array(
			array(
				'name'=>'#',
				'type'=>'html',
				'htmlOptions' => array('style'=>'text-align:center'),
				'value'=>'($row+1)',
			),
			array(
				'name'=>'Stream to import',
				'type'=>'html',
				'htmlOptions' => array('style'=>'text-align:left'),
				'value'=>'(isset($data["Name"]) ? $data["Name"]."<br/>" : "")."=> <a href=\"".$data["URL"]."\">".(strlen($data["URL"])<100?$data["URL"]:substr($data["URL"], 0,100)."...")."</a>"',
			),
			array(
				'name'=>'Related WebStream',
				'type'=>'html',
				'htmlOptions' => array('style'=>'text-align:left'),
				'value'=>'"<font>".(isset($data["WebStream"]) ? $data["WebStream"]->Name."<br/>".$data["WebStream"]->getTypeStreamName() : "")."</font>"',
			),
			array(
				'name'=>'Status',
				'type'=>'html',
				'htmlOptions' => array('style'=>'text-align:center'),
				'value'=>'(isset($data["WebStream"]) ? "<font color=\"".$data["WebStream"]->StreamStatus->Color."\">".$data["WebStream"]->StreamStatus->Label."</font>" : "/")',
			),
			array(
				'name'=>'Actions',
				'type'=>'raw',
				'htmlOptions' => array('style'=>'text-align:center'),
				'value'=>'(!isset($data["WebStream"]) ? CHtml::checkBox("Import[".$row."][Edit]", false).CHtml::hiddenField("Import[".$row."][Name]", $data["Name"]).CHtml::hiddenField("Import[".$row."][URL]", $data["URL"]) : "")',
			),
		),
	));
?>

<?php
	if(Yii::app()->user->checkAccess('importWebStream')) {
?>

<div class="wide form">
	<div class="row">
		<?php echo CHtml::activeLabelEx($modelWebStreamData,'TypeStream'); ?>
		<?php
			$dropList = WebStream::getTypeStreamList();
			echo CHtml::activeDropDownList($modelWebStreamData, 'TypeStream', $dropList, array ('empty' => '-- Unknown --'));
			echo CHtml::errorSummary($modelWebStreamData,'TypeStream');
		?>
	</div>

<?php
	if(Yii::app()->user->checkAccess('changeStatusWebStream')) {
?>
	<div class="row">
		<?php echo CHtml::activeLabelEx($modelWebStreamData,'StreamStatusCode'); ?>
		<?php
			$dropList = CHtml::listData(StreamStatus::model()->findAll(), 'Code', 'Label');
			echo CHtml::activeDropDownList($modelWebStreamData, 'StreamStatusCode', $dropList);
			echo CHtml::errorSummary($modelWebStreamData,'StreamStatusCode');
		?>
	</div>
<?php
	}
?>

	<div class="row">
		<?php echo CHtml::activeLabelEx($modelWebStreamData,'LangCode'); ?>
		<?php
			$dropList = CHtml::listData(Lang::model()->findAll(array('order'=>'Label')), 'Code', 'Label');
			echo CHtml::activeDropDownList($modelWebStreamData, 'LangCode', $dropList, array ('empty' => '-- None --'));
			echo CHtml::errorSummary($modelWebStreamData,'LangCode');
		?>
	</div>

	<div class="row">
		<?php echo CHtml::activeLabelEx($modelWebStreamData,'CountryCode'); ?>
		<?php
			$dropList = CHtml::listData(Country::model()->findAll(array('order'=>'Label')), 'Code', 'Label');
			echo CHtml::activeDropDownList($modelWebStreamData, 'CountryCode', $dropList, array ('empty' => '-- None --'));
			echo CHtml::errorSummary($modelWebStreamData,'CountryCode');
		?>
	</div>
</div>

<?php

	echo CHtml::submitButton('Import');
?>

<?php
	}
?>

<?php echo CHtml::endForm(); ?>

</div>

