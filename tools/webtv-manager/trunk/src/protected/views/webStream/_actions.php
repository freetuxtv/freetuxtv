<h3>Actions</h3>

<script type="text/javascript">
	function loadPage() {
		document.getElementById("edit-request-form").style.display = 'none';
	}
	
	function showForm(button) {

		var description = '';
		var action = '';

		switch(button) {
		case 'bt_reportdeadlink':
			description = "You are going to report this link as dead.";
			action = 'ReportDeadLink';
			break;
		}

		document.getElementById('edit-request-action').value = action;
		document.getElementById('description').innerHTML = description;
		document.getElementById(button).style.display = 'none';
		document.getElementById("edit-request-form").style.display = 'block';
	}
</script>

<p align="right">
<?php
	if($model->StreamStatusCode == StreamStatus::STREAM_STATUS_WORKING){
?>
		<button type="button" onclick="JavaScript:showForm(this.id)" id="bt_reportdeadlink">Report as dead link</button>
<?php
	}
?>
</p>

<div class="form">

<?php
	echo CHtml::beginForm(Yii::app()->createUrl("WebStream/editrequest", array("id" => $model->Id)), 'POST', array('id'=>'edit-request-form')); 
?>

	<p align="left">
		<span id="description"></span><br/>
		You may add a comment related to this report :
	</p>

	<?php echo CHtml::hiddenField('Action', '', array('id'=>'edit-request-action')); ?>

	<div class="row">
		<?php echo CHtml::label('Comments', 'Comments'); ?>
		<?php echo CHtml::textArea('Comments', '', array('cols'=>80,'rows'=>10)); ?>
	</div>


	<div class="row buttons">
		<?php echo CHtml::submitButton('Submit change request'); ?>
	</div>

<?php
     echo CHtml::endForm();
?>
</div>

<script type="text/javascript">
	loadPage();
</script>
