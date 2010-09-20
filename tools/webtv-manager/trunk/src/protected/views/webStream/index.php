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
	$playlist_link = "./playlist/index.php";
?>

Playlist corresponding to the search : <a href="<?php echo $playlist_link; ?>"><?php echo $playlist_link; ?></a>

<?php $this->widget('zii.widgets.grid.CGridView', array(
	'dataProvider'=>$dataProvider,
	'columns'=>array(
        array(
            'name'=>'WebStream',
			'type'=>'html',
			'value'=>'$data->Name.($data->RequiredIsp? " (Only for <b>".$data->RequiredIsp."</b> provider)" : "")."<br/>"."=> <a href=\"".$data->Url."\">".$data->Url."</a>"',
        ),
        array(
            'name'=>'Language',
			'type'=>'image',
			'htmlOptions' => array('style'=>'text-align:center'),
            'value'=>'"images/lang/softclean/".strtoupper($data->LangCode).".png"',
        ),
        array(
            'name'=>'Status',
			'type'=>'html',
            'value'=>'"<font color=\"".$data->StreamStatus->Color."\">".$data->StreamStatus->Label."</font>"',
        ),
        array(
			'class'=>'CButtonColumn',
            'header'=>'Edit',
        ),
    ),
)); ?>
