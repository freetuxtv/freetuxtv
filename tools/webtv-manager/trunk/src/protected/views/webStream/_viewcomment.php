<h3>WebStream Comment</h3>

<?php

	if(Yii::app()->user->checkAccess('modoHideComment')) {
	// Display a grid for the comment with delete, edit power
	$this->widget('zii.widgets.grid.CGridView', array(
		'dataProvider'=>$dataComment,
		'enablePagination'=>false,
		'enableSorting'=>false,
		'columns'=>array(
			array(
		        'name'=>'Date',
				'htmlOptions'=>array('width'=>'180'),
		    ),
		    array(
				'header'=>'User',
				'type'=>'html',
		        'value'=>'"<font>".($data->Username ? "<b>".$data->Username."</b>" : "Anonyme" )."</font>"',
				'htmlOptions'=>array('width'=>'150'),
		    ),
			 array(
				'name'=>'Comment',
				'value'=>'$data->Content',
			),
			 array(
		        'name'=>'Statut',
				'type'=>'html',
				'value'=>'$data->getCommentStatut()',
				'htmlOptions'=>array('width'=>'20'),
			),
				array(            // display a column with "validComment" and "hideComment" buttons
					'class'=>'CButtonColumn',
					'template'=>'{hideComment},{validComment}',
					'buttons'=>array(
						'hideComment' => array
						(
							'label'=>'Hide this comment',
							'imageUrl'=>Yii::app()->request->baseUrl.'/assets/6f904512/gridview/delete.png',
							'url'=>'Yii::app()->createUrl("webStream/hideComment/", array("id"=>$data->id))',
						),
						
						'validComment' => array
						(
							'label'=>'Valid this comment',
							'imageUrl'=>Yii::app()->request->baseUrl.'/assets/6f904512/gridview/update.png',
							'url'=>'Yii::app()->createUrl("webStream/validComment/", array("id"=>$data->id))',
						),
						
					),
				),
		),
	));
	}
	else {
			// Display a grid for the comment without delete, edit power
		$this->widget('zii.widgets.grid.CGridView', array(
			'dataProvider'=>$dataComment,
			'enablePagination'=>false,
			'enableSorting'=>false,
			'columns'=>array(
				array(
					'name'=>'Date',
					'htmlOptions'=>array('width'=>'180'),
				),
				array(
					'header'=>'User',
					'type'=>'html',
					'value'=>'"<font>".($data->Username ? "<b>".$data->Username."</b>" : "Anonyme" )."</font>"',
					'htmlOptions'=>array('width'=>'150'),
				),
				 array(
					'name'=>'Comment',
					'value'=>'$data->Content',
					//'htmlOptions'=>array('width'=>'40'),
				),
			),
		));
	}
?>
