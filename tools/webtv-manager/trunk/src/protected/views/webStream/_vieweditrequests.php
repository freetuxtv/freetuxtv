<h3>Edit requests</h3>

<?php
	// Display a grid for the historic for webstream
	$this->widget('zii.widgets.grid.CGridView', array(
		'dataProvider'=>$dataEditRequests,
		'enablePagination'=>false,
		'enableSorting'=>false,
		'columns'=>array(
		    array(
				'header'=>'Edit request',
				'type'=>'html',
		        'value'=>'"<font><b>".$data->Date."</b> by ".($data->UserId ? "<b>".$data->User->Username."</b>" : (Yii::app()->user->isGuest ? "Anonyme" : $data->RemoteAddr))."</font>"."<font><br/><b>Actions :</b> ".str_replace("\n","<br>",$data->ActionDetails)."</font>".($data->Comments ? "<br/><br/><b>Comment :</b><br/>".$data->Comments->Comment : "")',
		    ),
		    array(
		        'name'=>'Status',
				'type'=>'html',
				'htmlOptions' => array('style'=>'text-align:center'),
		        'value'=>'"<font>".($data->EditRequest ? $data->EditRequest[0]->getStatusName() : "")."</font>"',
		    ),
		),
	));
?>
