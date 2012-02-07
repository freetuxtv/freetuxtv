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

<?php echo $this->renderPartial('_searchform', array('modelSearchForm'=>$modelSearchForm)); ?>

<br/><br/>

<?php
	$querystring="";
	$lng="all";
	$type="all";
	foreach ($playlist_params as $key => $val)
	{
		if($key == "lng"){
			$lng = $val;
		}else if($key == "type"){
			$type = WebStream::getPlaylistTypeStreamNameById($val);
		}else if($key == "status" && $val == WebStream::WEBSTREAM_STATUS_WORKING){
			// We do nothing
		}else{
			if($querystring==""){
				$querystring.="?";
			}else{
				$querystring.="&";
			}
			$querystring.=$key."=".$val;
		}
	}
	if($lng == "all" && $type == "all"){
		$playlist_link = $this->createUrl("playlists/playlist.m3u".$querystring);
	}else{
		$playlist_link = $this->createUrl("playlists/playlist_".$type."_".$lng.".m3u".$querystring);
	}
  	$params = array_merge(array("format"=>""), $playlist_params);
?>

<b>Export for the current search:</b>
<?php
	// Export for M3U format
	$params["format"] = "m3u";
	$link = $this->createUrl("WebStreamExport/index", $params);
	echo CHtml::link("M3U", $link);
	echo " - ";
	// Export for XML format
	$params["format"] = "xml";
	$link = $this->createUrl("WebStreamExport/index", $params);
	echo CHtml::link("XML", $link);
	echo " - ";
	// Export for coolfvwmmanager format
	$params["format"] = "coolfvwmmanager";
	$link = $this->createUrl("WebStreamExport/index", $params);
	echo CHtml::link("Coolfvwmmanager", $link);
?>

<?php echo $this->renderPartial('_results', array('dataProvider'=>$dataProvider)); ?>
