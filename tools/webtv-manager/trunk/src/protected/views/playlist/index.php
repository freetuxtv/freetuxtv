<?php
	foreach($data as $stream){
		echo "#EXTINF:0,".$stream->Name."\n";
		echo $stream->Url."\n";
	}
?>
