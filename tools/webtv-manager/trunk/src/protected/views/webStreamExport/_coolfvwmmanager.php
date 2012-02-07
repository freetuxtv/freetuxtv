<?php
	header("Content-Type: text/plain; charset=utf-8");
	foreach($listWebStream as $stream){
		echo "\"".$stream->Name."\"+\"".$stream->Url."\"\n";
	}
?>
