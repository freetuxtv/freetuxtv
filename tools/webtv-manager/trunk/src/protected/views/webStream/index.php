<?php
$this->breadcrumbs=array(
	'Web Streams',
);

$this->menu=array(
	array('label'=>'Create WebStream', 'url'=>array('create')),
	array('label'=>'Manage WebStream', 'url'=>array('admin')),
);

?>

<h1>Web Streams</h1>

<?php
	$playlist_link = $this->createUrl("Playlist/", $playlist_params);
?>

Playlist corresponding to the search : <br/>
=> <?php echo CHtml::link($playlist_link, $playlist_link); ?><br/><br/>

<?php
	
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
            'value'=>'"images/lang/languageicons/flags/".strtolower($data->LangCode).".png"',
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
			'class'=>'CButtonColumn',
            'header'=>'Edit',
			//'deleteButtonOptions' => array('style'=>'display:none'),
			//'updateButtonOptions' => array('style'=>'display:none'),
        ),
    ),
)); ?>
