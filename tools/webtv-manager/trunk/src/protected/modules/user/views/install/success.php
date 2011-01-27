<p> Congratulations, you have Installed the Yii User Management Module ! </p>

<p> Don't forget to look in the Documentation in the docs/ directory to 
see the module specific options that can be set in your Application 
Configuration (for example, language). </p>

<strong> Please change the Administrator Password to something better than 
the default password. </strong>

<p><?php 
	$route=YumHelper::route('{user}/login'); 
	echo CHtml::link("Administrate your Users (use {$route})",array($route)); 
?></p>
