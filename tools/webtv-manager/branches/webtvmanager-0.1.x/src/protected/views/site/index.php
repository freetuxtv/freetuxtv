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

<?php
	}
?>

</div><!-- web-stream-send-form -->


<p>This webstite is part of the FreetuxTV project : <a href="http://code.google.com/p/freetuxtv/">http://code.google.com/p/freetuxtv/</a>.
