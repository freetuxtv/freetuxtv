<?php
	// Display a grid for the webstream list

	$this->widget('zii.widgets.grid.CGridView', array(
	'dataProvider'=>$dataProvider,
	'columns'=>array(
        array(
            'name'=>'WebStream',
			'type'=>'html',
			'value'=>'$data->Name.($data->RequiredIsp? " (Only for <b>".$data->RequiredIsp."</b> provider)" : "")."<br/>"."=> <a href=\"".$data->Url."\">".(strlen($data->Url)<100?$data->Url:substr($data->Url, 0,100)."...")."</a>"',
        ),
        array(
            'name'=>'Language',
			'type'=>'image',
			'htmlOptions' => array('style'=>'text-align:center'),
            'value'=>'Yii::app()->request->baseUrl."/images/lang/languageicons/flags/".strtolower($data->LangCode).".png"',
        ),
		
        array(
            'name'=>'Country',
			'type'=>'image',
			'htmlOptions' => array('style'=>'text-align:center'),
            'value'=>'Yii::app()->request->baseUrl."/images/lang/languageicons/flags/".strtolower($data->CountryCode).".png"',
        ),
        array(
            'name'=>'Type',
			'type'=>'html',
			'htmlOptions' => array('style'=>'text-align:center'),
            'value'=>'"<font>".$data->getTypeStreamName()."</font>"',
        ),
        array(
            'name'=>'Status',
			'type'=>'html',
			'htmlOptions' => array('style'=>'text-align:center'),
            'value'=>'"<font color=\"".$data->StreamStatus->Color."\">".$data->StreamStatus->Label."</font>"',
        ),
        array(
			'class'=>'CLinkColumn',
            'header'=>'Details',
			'label' => 'View details',
			'urlExpression'=>'Yii::app()->createUrl("WebStream/view", array("id" => $data->Id))',
			'htmlOptions' => array('style'=>'text-align:center'),
        ),
    ),
)); ?>
