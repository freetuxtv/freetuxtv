<h3>WebStream History</h3>

<?php
	// Display a grid for the historic for webstream
	$this->widget('zii.widgets.grid.CGridView', array(
		'dataProvider'=>$dataHistory,
		'enablePagination'=>false,
		'enableSorting'=>false,
		'columns'=>array(
		    array(
		        'name'=>'Date',
		    ),
		    array(
				'header'=>'User',
				'type'=>'html',
		        'value'=>'"<font>".($data->UserId ? $data->User->Username : "Anonyme")."</font>"',
		    ),
		    array(
		        'name'=>'Action',
				'type'=>'html',
		        'value'=>'"<font>".$data->getHistoryActionName()."</font>"',
		    ),
		    array(
		        'name'=>'Details',
				'type'=>'html',
		        'value'=>'"<font>".str_replace("\n","<br>",$data->ActionDetails)."</font>"',
		    ),/*
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
		    ),*/
		),
	));
?>
