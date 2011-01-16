<?php
if(Yum::module()->rtepath != false)
Yii::app()->clientScript-> registerScriptFile(Yii::app()->getModule('user')->rtepath);                                                                         
if(Yum::module()->rteadapter != false)
Yii::app()->clientScript-> registerScriptFile(Yii::app()->getModule('user')->rteadapter); 

foreach($profile->loadProfileFields() as $field) {
	echo CHtml::openTag('div',array('class'=>'row'));


	if($field->hint)
		echo CHtml::tag('div',array('class'=>'hint'),$field->hint,true);

	echo CHtml::activeLabelEx($profile,$field->varname);
	if ($field->field_type=="TEXT") {
		echo CHtml::activeTextArea($profile,
				$field->varname,
				array('rows'=>6, 'cols'=>50));
		if(Yii::app()->getModule('user')->rtepath != false)
			Yii::app()->clientScript->registerScript("ckeditor", "$('#YumProfile_".$field->varname."').ckeditor();"); 
	} 
	else if($field->field_type == "DROPDOWNLIST") {
		echo CHtml::activeDropDownList($profile,
				$field->varname, 
				CHtml::listData(CActiveRecord::model(ucfirst($field->varname))->findAll(),
					'id',
					$field->related_field_name));

	} else {
		echo CHtml::activeTextField($profile,
				$field->varname,
				array('size'=>60,'maxlength'=>(($field->field_size)?$field->field_size:255)));
	}
	echo CHtml::error($profile,$field->varname); 

	echo CHtml::closeTag('div');
}
?>
