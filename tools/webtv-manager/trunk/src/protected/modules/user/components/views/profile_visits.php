<?php
echo Yum::t('This users have visited your profile recently') . ': <br />';

if($visits) {
	echo '<ul>';
	foreach($visits as $visit)
		printf('<li>%s</li>', CHtml::link($visit->visitor->username, array(
						'//user/profile/view', 'id' => $visit->visitor_id)));
	echo '</ul>';
} else
echo Yum::t('Nobody has visited your profile yet');
?>
