<?php echo '<?xml version="1.0" encoding="UTF-8"?>'; ?>

<channels_groups>
<?php
	foreach($statsLangs as $lang){
		if($lang['TotalCount']-$lang[WebStream::TypeWebLink] > 0){
?>
	<language id="<?php echo $lang['LangCode']; ?>" _lang="<?php echo $lang['LangName']; ?> Web TV and Radio">
<?php
		// Add WebTV
		if($lang[WebStream::TypeWebTV] > 0){
			$playlist_link = Yii::app()->getRequest()->getHostInfo().$this->createUrl('playlists/playlist_webtv_'.$lang['LangCode'].'.m3u');
?>
		<channels_group uri="<?php echo $playlist_link ?>" _name="Web TV (<?php echo $lang['LangName']; ?>)"></channels_group>
<?php
		}
?>
<?php
		// Add WebRadio
		if($lang[WebStream::TypeWebRadio] > 0){
			$playlist_link = Yii::app()->getRequest()->getHostInfo().$this->createUrl('playlists/playlist_webradio_'.$lang['LangCode'].'.m3u');
?>
		<channels_group uri="<?php echo $playlist_link ?>" _name="Web Radio (<?php echo $lang['LangName']; ?>)"></channels_group>
<?php
		}
?>
<?php
		// Add WebProgrammes
		if($lang[WebStream::TypeWebProgram] > 0){
			$playlist_link = Yii::app()->getRequest()->getHostInfo().$this->createUrl('playlists/playlist_programmes_'.$lang['LangCode'].'.m3u');
?>
		<channels_group uri="<?php echo $playlist_link ?>" _name="Web Programmes (<?php echo $lang['LangName']; ?>)"></channels_group>
<?php
		}
?>
<?php
		// Add WebCam
		if($lang[WebStream::TypeWebCam] > 0){
			$playlist_link = Yii::app()->getRequest()->getHostInfo().$this->createUrl('playlists/playlist_webcam_'.$lang['LangCode'].'.m3u');
?>
		<channels_group uri="<?php echo $playlist_link ?>" _name="Web Cam (<?php echo $lang['LangName']; ?>)"></channels_group>
<?php
		}
?>
	</language>
<?php
		}
	}
?>
</channels_groups>
