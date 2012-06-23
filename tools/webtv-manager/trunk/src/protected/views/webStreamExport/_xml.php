<?php
	header("Content-Type: application/xml; charset=utf-8");

	$writer = new XMLWriter();
	$writer->openURI('php://output');
	$writer->startDocument("1.0", "UTF-8");
	$writer->startElementNS("wslfe", "webstream_link_exchange", "http://database.freetuxtv.net/wslfe");
	$writer->text("\n\t");
	$writer->startElementNS("wslfe", "sequence", null);
	$writer->writeAttribute("source", "freetuxtv_database");

	foreach($listWebStream as $stream){
		$writer->text("\n\t\t");
		$writer->startElementNS("wslfe", "stream", null);
		$writer->writeAttribute("name", $stream->Name);
		$writer->text($stream->Url);
		$writer->endElement();
	}

	$writer->text("\n\t");
	$writer->endElement();
	$writer->text("\n");
	$writer->endElement();
	
	$writer->endDocument();
	
	// Write the content
	$writer->flush();
?>









