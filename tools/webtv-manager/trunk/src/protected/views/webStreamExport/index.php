<?php

	$page = "_m3u";
	if($format != null){
		if($format == "xml"){
			$page = "_xml";
		}
		if($format == "coolfvwmmanager"){
			$page = "_coolfvwmmanager";
		}
	}

	echo $this->renderPartial($page,
		array('listWebStream'=>$listWebStream)
	);
?>
