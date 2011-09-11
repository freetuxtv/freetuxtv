<?php $this->pageTitle=Yii::app()->name; ?>

<h1>Welcome to <i><?php echo CHtml::encode(Yii::app()->name); ?></i></h1>

<p>This website is a free database of WebTV and Web Radio.</p>

<p>You can look for a channel by using the search form :</p>

<div class="wide form">

<?php
	$form=$this->beginWidget('CActiveForm', array(
		'action'=>Yii::app()->createUrl("site/SearchWebStream"),
		'method'=>'get',
		'id'=>'web-stream-search-form',
		'enableAjaxValidation'=>false,
		));
?>

	<div class="row">
		<?php
			echo $form->labelEx($modelSearchForm,'Name');
			echo $form->textField($modelSearchForm,'Name',array('size'=>20,'maxlength'=>20));
			echo $form->error($modelSearchForm,'Name');
		?>
	</div>

	<div class="row">
		<?php
			echo $form->labelEx($modelSearchForm, 'Type');
			$dropList = WebStream::getTypeStreamList();
			echo $form->dropDownList($modelSearchForm, 'Type', $dropList, array ('empty' => '-- All --'));
			echo $form->error($modelSearchForm, 'Type');
		?>
	</div>

	<div class="row">
		<?php
			echo $form->labelEx($modelSearchForm, 'Language');
			$dropList = CHtml::listData(Lang::model()->findAll(array('order'=>'Label')), 'Code', 'Label');
			echo $form->dropDownList($modelSearchForm, 'Language', $dropList, array ('empty' => '-- All --'));
			echo $form->error($modelSearchForm, 'Language');
		?>
	</div>

	<div class="row">
		<?php
			echo $form->labelEx($modelSearchForm, 'Status');
			$dropList = CHtml::listData(StreamStatus::model()->findAll('Searchable=TRUE'), 'Code', 'Label');
			echo $form->dropDownList($modelSearchForm, 'Status', $dropList, array (
				'empty' => '-- All --',
				'options' => array(WebStream::WEBSTREAM_STATUS_WORKING => array('selected' => true))
			));
			echo $form->error($modelSearchForm, 'Status');
		?>
	</div>

	<div class="row buttons">
		<?php echo CHtml::submitButton('Search'); ?>
	</div>

<?php $this->endWidget(); ?>

</div><!-- search-form -->

<?php
	if(Yii::app()->user->checkAccess('sendWebStream')) {
?>

<p>Or you can send to the team a new URL :</p>

<div class="wide form">

<?php
	$form=$this->beginWidget('CActiveForm', array(
		'action'=>Yii::app()->createUrl("site/SendWebStream"),
		'method'=>'get',
		'id'=>'web-stream-search-form',
		'enableAjaxValidation'=>false,
		));
?>

	<div class="row">
		<?php echo $form->labelEx($modelSendForm,'Url'); ?>
		<?php echo $form->textField($modelSendForm,'Url',array('size'=>60,'maxlength'=>255)); ?>
		<?php echo CHtml::submitButton('Confirm'); ?>
		<?php echo $form->error($modelSendForm,'Url'); ?>
	</div>

<?php $this->endWidget(); ?>

</div><!-- web-stream-send-form -->

<?php
	}
?>

<br/>

<p>Here you can see overview of the working URL added the WebTV Manager database :</p>

<?php
	//print_r($statsLangs);
	// Display a grid for the lang stats list

	$columns = array();
	$columns[] = array(
	    'name'=>'Language',
		'type'=>'html',
		'htmlOptions' => array('style'=>'text-align:left'),
	    'value'=>'($data["LangCode"] ? "<img src=\"'.Yii::app()->request->baseUrl.'/images/lang/languageicons/flags/'.'".strtolower($data["LangCode"])."'.'.png'.'\" alt=\"\">" : $data["LangCode"])."&nbsp;<i>".$data["LangName"]."</i>"',
	);

	$streamTypes = WebStream::getTypeStreamList();

	foreach ($streamTypes as $key => $value) {
		$columns[] = array(
			'name'=>$value,
			'type'=>'html',
			'htmlOptions' => array('style'=>'text-align:center'),
			'value'=>'"<font>".($data['.$key.'] != 0 ? "<a href=\"".Yii::app()->createUrl("WebStream/index", array("WebStreamSearchForm[Type]"=>'.$key.', "WebStreamSearchForm[Language]"=>$data["LangCode"], "WebStreamSearchForm[Status]"=>'.WebStream::WEBSTREAM_STATUS_WORKING.'))."\">".$data['.$key.']."</a>" : "-")."</font>"',
		);
	}

	$columns[] = array(
	    'name'=>'Total count',
		'type'=>'html',
		'htmlOptions' => array('style'=>'text-align:center'),
		'value'=>'"<font><a href=\"".Yii::app()->createUrl("WebStream/index", array("WebStreamSearchForm[Language]"=>$data["LangCode"], "WebStreamSearchForm[Status]"=>'.WebStream::WEBSTREAM_STATUS_WORKING.'))."\">".$data["TotalCount"]."</a></font>"',
	);

	$this->widget('zii.widgets.grid.CGridView', array(
		'dataProvider'=>$statsLangs,
		'enablePagination'=>false,
		'enableSorting' => true,
		'summaryText'=>'',
		'columns'=>$columns,
	));
?>
<p>Last adds/updates :</p>

<table border="0">
<tr>
	<td width="50%" style="vertical-align:top;">

	<div style="width:80%">
	<?php
		// Display a grid for the adds list
		$this->widget('zii.widgets.grid.CGridView', array(
		'dataProvider'=>$lastAdds,
		'enablePagination'=>false,
		'summaryText'=>'',
		'columns'=>array(
		    array(
		        'name'=>'Last adds',
				'type'=>'html',
				'htmlOptions' => array('style'=>'text-align:left'),
		        'value'=>'"<font><b>".$data->getHistoryActionName()."</b> ".CHtml::link($data->WebStream->Name, array("WebStream/view", "id"=>$data->EntityId))
					."<br/>".$data->getTimeAgo()." by ".($data->User ? $data->User->Username : "Anonyme")
					."<br/>".$data->ActionDetails
					."</font>"',
		    ),
		),
	)); ?>
	</div>

	</td>
	<td width="50%" style="vertical-align:top">

	<div style="width:80%">
	<?php
		// Display a grid for the updates list
		$this->widget('zii.widgets.grid.CGridView', array(
		'dataProvider'=>$lastUpdates,
		'enablePagination'=>false,
		'summaryText'=>'',
		'columns'=>array(
		    array(
		        'name'=>'Last updates',
				'type'=>'html',
				'htmlOptions' => array('style'=>'text-align:left'),
		        'value'=>'"<font><b>".$data->getHistoryActionName()."</b> ".CHtml::link($data->WebStream->Name, array("WebStream/view", "id"=>$data->EntityId))
					."<br/>".$data->getTimeAgo()." by ".($data->User ? $data->User->Username : "Anonyme")
					."<br/>".$data->ActionDetails
					."</font>"',
		    ),
		),
	)); ?>
	</div>

	</td>
</tr>
</table>

<p>This webstite is part of the FreetuxTV project : <a href="http://code.google.com/p/freetuxtv/">http://code.google.com/p/freetuxtv/</a>.
