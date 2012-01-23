<h3>Relations</h3>

<?php
	// Display a grid for the historic for webstream
	$this->widget('zii.widgets.grid.CGridView', array(
		'dataProvider'=>$dataRelations,
		'enablePagination'=>false,
		'enableSorting'=>false,
		'columns'=>array(
		    array(
				'header'=>'Relation type',
				'type'=>'html',
				'htmlOptions' => array('style'=>'text-align:center'),
		        'value'=>'"<font><b>".$data->RelationType->Description."</b></font>"',
		    ),
		    array(
				'header'=>'WebStream',
				'type'=>'html',
		        'value'=>'"<font>".$data->RelatedWebStream->Name."</font>"',
		    ),
		    array(
		        'name'=>'Language',
				'type'=>'image',
				'htmlOptions' => array('style'=>'text-align:center', 'class' => 'flag'),
		        'value'=>'Yii::app()->request->baseUrl."/images/lang/languageicons/flags/".strtolower($data->RelatedWebStream->LangCode).".png"',
		    ),
		    array(
		        'name'=>'Country',
				'type'=>'image',
				'htmlOptions' => array('style'=>'text-align:center', 'class' => 'flag'),
		        'value'=>'Yii::app()->request->baseUrl."/images/country/softclean/".strtoupper($data->RelatedWebStream->CountryCode).".png"',
		    ),
		    array(
		        'name'=>'Status',
				'type'=>'html',
				'htmlOptions' => array('style'=>'text-align:center'),
		        'value'=>'"<font color=\"".$data->RelatedWebStream->StreamStatus->Color."\">".$data->RelatedWebStream->StreamStatus->Label."</font>"',
		    ),
		    array(
				'class'=>'CLinkColumn',
		        'header'=>'Details',
				'label' => 'View details',
				'urlExpression'=>'Yii::app()->createUrl("WebStream/view", array("id" => $data->RelatedWebStream->Id))',
				'htmlOptions' => array('style'=>'text-align:center'),
		    ),
		),
	));
?>
