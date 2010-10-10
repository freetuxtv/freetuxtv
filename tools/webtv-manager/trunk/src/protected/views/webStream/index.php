<?php
$this->breadcrumbs=array(
	'Web Streams',
);

/*
$this->menu=array(
	array('label'=>'Create WebStream', 'url'=>array('create')),
	array('label'=>'Manage WebStream', 'url'=>array('admin')),
);
*/

?>

<h1>Web Streams</h1>

<?php
	$playlist_link = $this->createUrl("Playlist/", $playlist_params);
?>

Playlist corresponding to the search : <br/>
=> <?php echo CHtml::link($playlist_link, $playlist_link); ?><br/><br/>

<?php echo $this->renderPartial('_results', array('dataProvider'=>$dataProvider)); ?>
