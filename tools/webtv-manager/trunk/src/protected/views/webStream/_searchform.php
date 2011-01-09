<div class="table">
	<div class="caption">Search a WebStream :</div>


	<div class="tbody">
<?php
	$form=$this->beginWidget('CActiveForm', array(
		'method'=>'get',
		'id'=>'web-stream-search-form',
		'enableAjaxValidation'=>false,
		));
?>
		<div class="tr">
			<div class="td">
				<?php
					echo $form->labelEx($modelSearchForm,'Name');
					echo $form->textField($modelSearchForm,'Name',array('size'=>20,'maxlength'=>20));
					echo $form->error($modelSearchForm,'Name');
				?>
			</div>

			<div class="td">
				<?php
					echo $form->labelEx($modelSearchForm, 'Type');
					$dropList = WebStream::getTypeStreamList();
					echo $form->dropDownList($modelSearchForm, 'Type', $dropList, array ('empty' => '-- All --'));
					echo $form->error($modelSearchForm, 'Type');
				?>
			</div>

			<div class="td">
				<?php
					echo $form->labelEx($modelSearchForm, 'Language');
					$dropList = CHtml::listData(Lang::model()->findAll(array('order'=>'Label')), 'Code', 'Label');
					echo $form->dropDownList($modelSearchForm, 'Language', $dropList, array ('empty' => '-- All --'));
					echo $form->error($modelSearchForm, 'Language');
				?>
			</div>
					
			<div class="td">
				<?php
					echo $form->labelEx($modelSearchForm, 'Country');
					$dropList = CHtml::listData(Lang::model()->findAll(array('order'=>'Label')), 'Code', 'Label');
					echo $form->dropDownList($modelSearchForm, 'Country', $dropList, array ('empty' => '-- All --'));
					echo $form->error($modelSearchForm, 'Country');
				?>
			</div>

			<div class="td">
				<?php
					echo $form->labelEx($modelSearchForm, 'Status');
					$dropList = CHtml::listData(StreamStatus::model()->findAll('Searchable=TRUE'), 'Code', 'Label');
					echo $form->dropDownList($modelSearchForm, 'Status', $dropList, array ('empty' => '-- All --'));
					echo $form->error($modelSearchForm, 'Status');
				?>
			</div>

			<div class="td">
				<br/>
				<?php
					echo CHtml::submitButton('Search');
				?>
			</div>
		</div>
<?php $this->endWidget(); ?>

	</div>

</div><!-- search-form -->
