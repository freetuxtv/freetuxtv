<?php

$template = '<p> %s: %s </p>';

printf($template, Yum::t('Username'), $data->username);


if(Yum::module()->enableProfiles && isset($data->profile[0])) {
	printf($template, Yum::t('Firstname'), $data->profile[0]->firstname);
	printf($template, Yum::t('Lastname'), $data->profile[0]->lastname);
} 

printf($template, Yum::t('First visit'), date(UserModule::$dateFormat, $data->createtime)); 
printf($template, Yum::t('Last visit'), date(UserModule::$dateFormat, $data->lastvisit)); 

echo CHtml::link(Yum::t('Write a message'), array(
			'//user/message/compose', 'to_user_id' => $data->id)) . '<br />';
echo CHtml::link(Yum::t('Visit profile'), array(
			'//user/profile/view', 'id' => $data->id));




