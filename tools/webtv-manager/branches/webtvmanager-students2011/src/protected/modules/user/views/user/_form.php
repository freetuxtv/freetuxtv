<?php

    if(empty($tabularIdx))
    {
        #dont display when used in multiform
        echo CHtml::openTag('div',array('class'=>'form'));
        echo CHtml::beginForm();
        echo YumHelper::requiredFieldNote();
    }

    echo CHtml::errorSummary($model); echo CHtml::errorSummary($profile);

	$attribute = !empty($tabularIdx) ? "[{$tabularIdx}]username" : "username";
    echo CHtml::openTag('div',array('class'=>'row'));
    echo CHtml::activeLabelEx($model,$attribute);
	echo CHtml::activeTextField($model,$attribute,array('size'=>20,'maxlength'=>20));
	echo CHtml::error($model,$attribute);
    echo CHtml::closeTag('div');

	$attribute = !empty($tabularIdx) ? "[{$tabularIdx}]password" : "password";
    echo CHtml::openTag('div',array('class'=>'row'));
    echo CHtml::activeLabelEx($model,$attribute);
	echo CHtml::activePasswordField($model,$attribute,array('size'=>60,'maxlength'=>128));
	echo CHtml::error($model,$attribute);
    echo CHtml::closeTag('div');

	$attribute = !empty($tabularIdx) ? "[{$tabularIdx}]superuser" : "superuser";
    echo CHtml::openTag('div',array('class'=>'row'));
    echo CHtml::activeLabelEx($model,$attribute);
	echo CHtml::activeDropDownList($model,$attribute,YumUser::itemAlias('AdminStatus'));
	echo CHtml::error($model,$attribute);
    echo CHtml::closeTag('div');

	$attribute = !empty($tabularIdx) ? "[{$tabularIdx}]status" : "status";
    echo CHtml::openTag('div',array('class'=>'row'));
	echo CHtml::activeLabelEx($model,$attribute);
	echo CHtml::activeDropDownList($model,$attribute,YumUser::itemAlias('UserStatus'));
	echo CHtml::error($model,$attribute);
    echo CHtml::closeTag('div');
    
    foreach($profile->loadProfileFields() as $field)
    {
        echo CHtml::openTag('div',array('class'=>'row'));
        $attribute = !empty($tabularIdx) ? "[{$tabularIdx}]{$field->varname}" : $field->varname;
        echo CHtml::activeLabelEx($profile,$attribute);
        if ($field->field_type=="TEXT")
            echo CHtml::activeTextArea($profile,$attribute,array('rows'=>6, 'cols'=>50));
        else
            echo CHtml::activeTextField($profile,$attribute,array('size'=>60,'maxlength'=>(($field->field_size)?$field->field_size:255)));
        echo CHtml::error($profile,$attribute);
        if($field->hint)
            echo CHtml::tag('div',array('class'=>'hint'),$field->hint,true);
        echo CHtml::closeTag('div');
    }

    if($this->module->hasModule('role'))
    {
        echo CHtml::openTag('div',array('class'=>'row'));
        echo CHtml::tag('p',array(),Yii::t('UserModule.user', 'User belongs to these roles'),true);
        $this->widget('YumModule.components.Relation',
			array('model' => $model,
			'relation' => 'roles',
			'style' => 'dropdownlist',
			'fields' => 'title',
			'showAddButton' => false
		));
        echo CHtml::closeTag('div');

        if($model->users)
        {
           echo CHtml::openTag('div',array('class'=>'row'));
           echo CHtml::tag('p',array(),Yii::t('UserModule.user', 'This user can administrate this users'),true);
           echo CHtml::closeTag('div');
           $this->widget('YumModule.components.Relation',
               array('model' => $model,
                   'relation' => 'users',
                   'style' => 'listbox',
                   'fields' => 'username',
                   'showAddButton' => false
            ));
        }
    }

    if(empty($tabularIdx))
    {
        echo CHtml::openTag('div',array('class'=>'row buttons'));
        echo CHtml::submitButton($model->isNewRecord
            ? Yii::t('UserModule.user', 'Create')
            : Yii::t('UserModule.user', 'Save'));
        echo CHtml::closeTag('div');

        echo CHtml::endForm();
        echo CHtml::closeTag('div');
    }
?>
