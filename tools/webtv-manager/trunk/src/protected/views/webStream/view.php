<?php
$this->breadcrumbs=array(
	'Web Streams'=>array('index'),
	$model->Name,
);
?>

<?php
	$this->menu=array();
	$this->menu[] = array('label'=>'View', 'url'=>array('view', 'id'=>$model->Id));
	if(Yii::app()->user->checkAccess('editWebStream')) {
		$this->menu[] = array('label'=>'Edit', 'url'=>array('update', 'id'=>$model->Id));
	}
	if(Yii::app()->user->checkAccess('changeStatusWebStream')) {
		$this->menu[] = array('label'=>'Change status', 'url'=>array('changestatus', 'id'=>$model->Id));
	}
/*
$this->menu=array(
	array('label'=>'List WebStream', 'url'=>array('index')),
	array('label'=>'Create WebStream', 'url'=>array('create')),
	array('label'=>'Update WebStream', 'url'=>array('update', 'id'=>$model->Id)),
	array('label'=>'Delete WebStream', 'url'=>'#', 'linkOptions'=>array('submit'=>array('delete','id'=>$model->Id),'confirm'=>'Are you sure you want to delete this item?')),
	array('label'=>'Manage WebStream', 'url'=>array('admin')),
);*/

?>

<h1>WebStream : <?php echo $model->Name; ?></h1>

Here you can see the detail of the channel <?php echo $model->Name; ?> :

<br><br>

<?php $this->widget('zii.widgets.CDetailView', array(
	'data'=>$model,
	'attributes'=>array(
		array(
            'label'=>'Id',
			'name'=>'Id',
            'type'=>'text',
        ),
		'Name',
        array(
            'label'=>'Type',
			'type'=>'html',
            'value'=>"<font>".$model->getTypeStreamName()."</font>",
        ),
		array(
            'label'=>'Url',
			'name'=>'Url',
            'type'=>'url',
        ),
		'RequiredIsp',
		array(
            'label'=>'Language',
			'type'=>'html',
            'value'=>($model->LangCode ? '<img class="flag" src="'.Yii::app()->request->baseUrl.'/images/lang/languageicons/flags/'.strtolower($model->LangCode).'.png'.'"> '.$model->Lang->Label : $model->LangCode),
        ),
		array(
            'label'=>'Country',
			'type'=>'html',
            'value'=>($model->CountryCode ? '<img class="flag" src="'.Yii::app()->request->baseUrl.'/images/country/softclean/'.strtoupper($model->CountryCode).'.png'.'"> '.$model->Country->Label : $model->CountryCode),
        ),
        array(
            'label'=>'Status',
			'type'=>'html',
            'value'=>'<font color="'.$model->StreamStatus->Color.'">'.$model->StreamStatus->Label.'</font> : <i>'.$model->StreamStatus->Description.'</i>',
        ),
		'SubmissionDate',
        array(
            'label'=>'Comments',
			'name'=>'Comments',
            'type'=>'text',
        ),
	),
)); ?>

<br/>

<?php
	if($model->StreamStatusCode != StreamStatus::STREAM_STATUS_WORKING){
		if(Yii::app()->user->checkAccess('changeStatusWebStream')){
?>
<h3>Change status:</h3>
<?php
			echo $this->renderPartial('_changestatus', array('model'=>$model, 'nofield'=>true));
		}
	}
?>

<h3>Play the stream:</h3>

<p align="left">
	You can try this link with the following commands on GNU/Linux:
	<ul>
		<li>vlc --open "<?php echo $model->Url ?>"</li>
		<li>mplayer "<?php echo $model->Url ?>"</li>
	</ul>
</p>

<br/>

<?php echo $this->renderPartial('_actions', array('model'=>$model)); ?>

<?php
	if($model->TypeStream == WebStream::TypeWebLink){
?>
		<p align="center">
			<font color="red">WARNING :<br/>This link have been marked as WebLink, that means it's not a video/audio stream and cannot be played in FreetuxTV.<br/>This is just a link containing a stream that can be played in your browser (usually with flashplayer), so don't be surprised if you don't find this link in FreetuxTV.<br/><br/>If you're courageous you can try to find the real stream URL, by analysing the HTML source code of the targeted URL, but sometimes, this is not possible. Nevertheless, if you find it, please send us the new link.</font>
		</p>
<?php
	}
?>

<br/>

<?php
	$rawData = $model->RelatedWebStream;
	$dataRelations=new CArrayDataProvider($rawData, array(
		'id'=>'dataRelations',
		'keyField'=>'WebStreamId2',/*
		'sort'=>array(
		    'attributes'=>array(
		         'WebStreamId1',
		    ),
		),*/
	));

	echo $this->renderPartial('_viewrelations', array('dataRelations'=>$dataRelations));
?>

<br/>

<?php echo $this->renderPartial('_viewhistory', array('dataHistory'=>$dataHistory)); ?>

<br/>

<?php echo $this->renderPartial('_vieweditrequests', array('dataEditRequests'=>$dataEditRequests)); ?>
