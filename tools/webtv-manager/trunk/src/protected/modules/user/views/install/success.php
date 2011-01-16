<p> Congratulations, you have Installed the Yii User Management Module ! </p>

<p> Don't forget to look in the Documentation in the docs/ directory to 
see the module specific options that can be set in your Application 
Configuration (for example, language).

Since yum 0.8 you can define the Module Behavior in the administration backend,
 if enabled in the installation. </p>

<strong> Please change the Administrator Password to something better than 
the default password. </strong>

<p><?php 
	echo CHtml::link("Administrate your Users",array('/user/login')); 
?></p>
