<?php
$this->breadcrumbs=array(
	'Web Streams', 'Send a Web Stream'
);
?>

<?php
	$itemCount = $dataProvider->getTotalItemCount();
?>

<h1>Send a Web Streams</h1>

<?php
	// If we have results, we show them to the user
	if($itemCount != 0){
?>

The link you are sending already exists in our database. You are not authorized to send it as duplicate link.
<br/><br/>
You can view the details of existing links by choosing one in the following list :

<?php echo $this->renderPartial('_results', array('dataProvider'=>$dataProvider)); ?>

You can try to send another link :

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

<?php
	// If we don't have any result we can have some result
	if($itemCount == 0){
?>

The link you are sending doesn't exits in our database.
<br/><br/>
Please complete the following information about the link before sending it :

<br/><br/>
<div class="warning">
	<b>WARNING!</b> Before sumitting a link, please be sure it is working. To be accepted by moderator, the link must be work with VLC on GNU/Linux. You can test this link with the following command:<br/>
	&nbsp;&nbsp;&nbsp;&nbsp;<i>vlc --open "<?php echo $model->Url ?>"</i>
</div>

<div class="wide form">

<?php
	 $form=$this->beginWidget('CActiveForm', array(
	'action'=>Yii::app()->createUrl("WebStream/send"),
	'id'=>'web-stream-form',
	'enableAjaxValidation'=>false,
)); ?>

	<br/>

	<p class="note">Fields with <span class="required">*</span> are required.</p>

	<?php echo $form->errorSummary($model); ?>

	<div class="row">
		<?php echo $form->labelEx($model,'Name'); ?>
		<?php echo $form->textField($model,'Name',array('size'=>60,'maxlength'=>100)); ?>
		<?php echo $form->error($model,'Name'); ?>
	</div>

	<div class="row">
		<?php echo $form->labelEx($model,'Url'); ?>
		<?php echo $form->textField($model,'Url',array('size'=>60,'maxlength'=>255,'readonly'=>1)); ?>
		<?php echo $form->error($model,'Url'); ?>
	</div>

	<div class="row">
		<?php echo $form->labelEx($model,'TypeStream'); ?>
		<?php
			$dropList = WebStream::getTypeStreamList();
			echo $form->dropDownList($model, 'TypeStream', $dropList, array ('empty' => '-- Unknown --'));
			echo $form->error($model,'TypeStream');
		?>
	</div>
<?php
	if(Yii::app()->user->checkAccess('changeStatusWebStream')) {
?>
	<div class="row">
		<?php echo $form->labelEx($model,'StreamStatusCode'); ?>
		<?php
			$dropList = CHtml::listData(StreamStatus::model()->findAll(), 'Code', 'Label');
			echo $form->dropDownList($model, 'StreamStatusCode', $dropList);
			echo $form->error($model,'StreamStatusCode');
		?>
	</div>
<?php
	}
?>
	<div class="row">
		<?php echo $form->labelEx($model,'RequiredIsp'); ?>
		<?php echo $form->textField($model,'RequiredIsp',array('size'=>20,'maxlength'=>20)); ?>
		<?php echo $form->error($model,'RequiredIsp'); ?>
	</div>

	<div class="row">
		<?php echo $form->labelEx($model,'LangCode'); ?>
		<?php
			$dropList = CHtml::listData(Lang::model()->findAll(array('order'=>'Label')), 'Code', 'Label');
			echo $form->dropDownList($model, 'LangCode', $dropList, array ('empty' => '-- None --'));
			echo $form->error($model,'LangCode');
		?>
	</div>

	<div class="row">
		<?php echo $form->labelEx($model,'CountryCode'); ?>
		<?php
			$dropList = CHtml::listData(Country::model()->findAll(array('order'=>'Label')), 'Code', 'Label');
			echo $form->dropDownList($model, 'CountryCode', $dropList, array ('empty' => '-- None --'));
			echo $form->error($model,'CountryCode');
		?>
	</div>

	<?php
		if(Yii::app()->user->isGuest){
	?>
		<p class="hint">
			This link will be send as user "Anonyme" from IP adress <?php echo Yii::app()->getRequest()->getUserHostAddress(); ?>.
		</p>
	<?php
		}
	?>

	<div class="row buttons">
		<?php echo CHtml::submitButton('Send'); ?>
	</div>

<?php $this->endWidget(); ?>

<?php
	}
?>

</div><!-- form -->
