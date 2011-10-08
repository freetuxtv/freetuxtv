<?php $this->pageTitle=Yii::app()->name; ?>

<h1>WSLFE Import module</h1>

<?php
	if(isset($_GET["error"])){
		if($_GET["error"] == 0){
?>
<p align="center" class="alert"><?php echo $_GET["success"] ?> links have been successfully imported !</p>
<?php
		}else if($_GET["error"] > 0){
?>
<p align="center" class="alert"><?php echo $_GET["error"] ?> error(s) have been encountered while importing link !</p>
<?php
		}else{
?>
<p align="center" class="alert">Error(s) have been encountered while importing link !</p>
<?php
		}
	}
?>

<p>You can use this form to check and compare a WSLFE file to the current content of the database:</p>

<?php echo $this->renderPartial('_importform', array('modelImportForm'=>$modelImportForm)); ?>

<br/>

<p>
The format WSLFE for "Web Stream Link File Exchange" used by the importer, in an XML file that look the following example :
<br/>
<?php
	$xmlDesc = '
<?xml version="1.0" encoding="UTF-8"?>
<webstream_link_exchange xmlns:wslfe="wslfe">
	<wslfe:sequence source="datasource_name">
		<wslfe:stream name="Channel 1">mms://mywebsite.com/channel1.wsx</wslfe:stream>
		<wslfe:stream name="Channel 2">rtsp://channel2.com/live</wslfe:stream>
		<wslfe:stream name="Channel International">http://192.168.0.1/stream</wslfe:stream>
	</wslfe:sequence>
</webstream_link_exchange>';

echo str_replace("\t", "&nbsp;&nbsp;", str_replace("\n", "<br/>", htmlentities($xmlDesc)));
?>

<br/>
<br/>
If you have a link database and compare it to our database you can just create a WSLFE file, put it on a Web Server and pass the link to the import module.
You will get the list of links and see if the links are already present in the database. If not, you can send us your WSLFE file (or URL), and we can import all the link in the database. 
</p>

