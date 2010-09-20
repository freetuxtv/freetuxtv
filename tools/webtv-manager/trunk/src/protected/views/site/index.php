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
		<?php echo CHtml::label('Language', 'WebStreamLang'); ?>
		<?php
			$dropList["all"] = "-- All --";
			$dropListTmp = CHtml::listData(Lang::model()->findAll(array('order'=>'Label')), 'Code', 'Label');
			$dropList = array_merge($dropList, $dropListTmp);
			echo CHtml::dropDownList('WebStreamLang', 'all', $dropList, array ('key'=>'all'));
		?>
	</div>

	<div class="row">
		<?php echo CHtml::label('Status', 'WebStreamStatus'); ?>
		<?php
			$dropList["all"] = "-- All --";
			$dropListTmp = CHtml::listData(StreamStatus::model()->findAll('Searchable=TRUE'), 'Code', 'Label');
			$dropList = array_merge($dropList, $dropListTmp);
			echo CHtml::dropDownList('WebStreamStatus', 'all', $dropList, array ('key'=>'all'));
		?>
	</div>

	<div class="row buttons">
		<?php echo CHtml::submitButton('Search'); ?>
	</div>

<?php echo CHtml::endForm(); ?>

</div><!-- search-form -->

<p>Or you can send to the team a new URL :</p>

<div class="wide form">

<?php echo CHtml::form(Yii::app()->createUrl("WebStream/send"), 'get'); ?>

	<div class="row">
		<?php echo CHtml::label('Url', 'WebStreamUrl'); ?>
		<?php echo CHtml::textField('WebStreamUrl', '', array('size'=>60,'maxlength'=>255)); ?>
		<?php echo CHtml::submitButton('Confirm'); ?>
	</div>

<?php echo CHtml::endForm(); ?>

</div><!-- search-form -->


<p>This webstite is part of the FreetuxTV project : <a href="http://code.google.com/p/freetuxtv/">http://code.google.com/p/freetuxtv/</a>.
