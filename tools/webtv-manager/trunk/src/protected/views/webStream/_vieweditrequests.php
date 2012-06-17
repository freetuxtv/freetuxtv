<h3>Edit requests</h3>

<?php
	$columns = array(
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
	);

	if(Yii::app()->user->checkAccess('editWebStream')){
		$columns[] = array(
			'header'=>'Actions',
			'class'=>'CButtonColumn',
			'buttons'=>array(
				'EditAccept' => array(
					'label'=>'Accept',
					'url'=>'Yii::app()->createUrl("WebStream/updateEditRequest", array("id" => $data->WebStream->Id, "edit_id" => $data->EditRequest[0]->Id, "status" => "accept"))',
					'visible'=>'($data->EditRequest[0]->Status == EditRequest::FIELD_STATUS_SUBMITTED ? true : false)',
					'click'=>'function(){return confirm("Are you sure you want to accept the change?");}',
				),
				'EditReject' => array(
					'label'=>'Reject',
					'url'=>'Yii::app()->createUrl("WebStream/updateEditRequest", array("id" => $data->WebStream->Id, "edit_id" => $data->EditRequest[0]->Id, "status" => "reject"))',
					'visible'=>'($data->EditRequest[0]->Status == EditRequest::FIELD_STATUS_SUBMITTED ? true : false)',
					'click'=>'function(){return confirm("Are you sure you want to reject the change?");}',
				),
			),
			'template'=>'{EditAccept} {EditReject}',
		);
	}

	// Display a grid for the historic for webstream
	$this->widget('zii.widgets.grid.CGridView', array(
		'dataProvider'=>$dataEditRequests,
		'enablePagination'=>false,
		'enableSorting'=>false,
		'columns'=> $columns,
	));
?>