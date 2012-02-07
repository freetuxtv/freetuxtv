<?php
	header("Content-Type: audio/mpegurl; charset=utf-8");
	//header("Content-Type: text/plain; charset=utf-8");
	//header("Content-Disposition: attachment; filename=playlist.m3u");
	echo "#EXTM3U\n";
	foreach($listWebStream as $stream){
		echo "#EXTINF:0,".$stream->Name."\n";
		echo $stream->Url."\n";
	}
?>
