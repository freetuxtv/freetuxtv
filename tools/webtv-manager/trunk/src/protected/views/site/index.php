<?php $this->pageTitle=Yii::app()->name; ?>

<h1>Welcome to <i><?php echo CHtml::encode(Yii::app()->name); ?></i></h1>

<p>This website is a free database of WebTV and Web Radio.</p>

<p>You can look for a channel by using the search form :</p>

<div class="wide form">

<?php echo CHtml::form(Yii::app()->createUrl("WebStream"), 'get'); ?>

	<div class="row">
		<?php echo CHtml::label('Name', 'WebStreamName'); ?>
		<?php echo CHtml::textField('WebStreamName', '', array('size'=>20,'maxlength'=>20)); ?>
	</div>

	<div class="row">
		<?php echo CHtml::label('Type', 'WebStreamType'); ?>
		<?php
			$dropList = WebStream::getTypeStreamList();
			echo CHtml::dropDownList('WebStreamType', '', $dropList, array ('empty' => '-- All --'));
		?>
	</div>

	<div class="row">
		<?php echo CHtml::label('Language', 'WebStreamLang'); ?>
		<?php
			$dropList = CHtml::listData(Lang::model()->findAll(array('order'=>'Label')), 'Code', 'Label');
			echo CHtml::dropDownList('WebStreamLang', '', $dropList, array ('empty' => '-- All --'));
		?>
	</div>

	<div class="row">
		<?php echo CHtml::label('Status', 'WebStreamStatus'); ?>
		<?php
			$dropList = CHtml::listData(StreamStatus::model()->findAll('Searchable=TRUE'), 'Code', 'Label');
			echo CHtml::dropDownList('WebStreamStatus', '', $dropList, array ('empty' => '-- All --'));
		?>
	</div>

	<div class="row buttons">
		<?php echo CHtml::submitButton('Search'); ?>
	</div>

<?php echo CHtml::endForm(); ?>

</div><!-- search-form -->

<?php
	if(Yii::app()->user->checkAccess('sendWebStream')) {
?>

<p>Or you can send to the team a new URL :</p>

<div class="wide form">

<?php
	if(!isset($model)){
		$model = new WebStreamSendForm;
	}
	$form=$this->beginWidget('CActiveForm', array(
		'action'=>Yii::app()->createUrl("site/send"),
		'method'=>'get',
		'id'=>'web-stream-send-form',
		'enableAjaxValidation'=>false,
		));
?>

	<div class="row">
		<?php echo $form->labelEx($model,'Url'); ?>
		<?php echo $form->textField($model,'Url',array('size'=>60,'maxlength'=>255)); ?>
		<?php echo CHtml::submitButton('Confirm'); ?>
		<?php echo $form->error($model,'Url'); ?>
	</div>

<?php $this->endWidget(); ?>

<?php
	}
?>


<p>This webstite is part of the FreetuxTV project : <a href="http://code.google.com/p/freetuxtv/">http://code.google.com/p/freetuxtv/</a>.
