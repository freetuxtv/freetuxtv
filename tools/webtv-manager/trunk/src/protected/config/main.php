<?php

// uncomment the following to define a path alias
// Yii::setPathOfAlias('local','path/to/local-folder');

// This is the main Web application configuration. Any writable
// CWebApplication properties can be configured here.
return array(
	'basePath'=>dirname(__FILE__).DIRECTORY_SEPARATOR.'..',
	'name'=>'FreetuxTV WebTV Manager',

	// preloading 'log' component
	'preload'=>array('log'),

	// autoloading model and component classes
	'import'=>array(
		'application.models.*',
		'application.components.*',
	),

	// application components
	'components'=>array(
		'user'=>array(
			// enable cookie-based authentication
			'allowAutoLogin'=>true,
		),
		// uncomment the following to enable URLs in path-format
		'urlManager'=>array(
			'urlFormat'=>'path',
			'rules'=>array(
				'playlists/playlist\.m3u'=>'Playlist/index',
				'playlists/playlist_<type:\w+>_<lng:\w+>\.m3u'=>'Playlist/index',
				'playlists/list'=>'Playlist/list',
				'<controller:\w+>/<id:\d+>'=>'<controller>/view',
				'<controller:\w+>/<action:\w+>/<id:\d+>'=>'<controller>/<action>',
				'<controller:\w+>/<action:\w+>'=>'<controller>/<action>',
			),
			'showScriptName'=>false,
		),
		/*
		'db'=>array(
			'connectionString' => 'sqlite:'.dirname(__FILE__).'/../data/testdrive.db',
		),*/
		// uncomment the following to use a MySQL database
		'db'=>array(
			'connectionString' => 'mysql:host=localhost;dbname=freetuxtv',
			'emulatePrepare' => true,
			'username' => 'root',
			'password' => 'mysql',
			'charset' => 'utf8',
		),
		'authManager'=>array(
			'class'=>'CDbAuthManager',
			'connectionID'=>'db',
			'itemTable'=>'wtvmT_AuthItem',
			'itemChildTable'=>'wtvmT_AuthItemChild',
			'assignmentTable'=>'wtvmT_AuthAssignment',
			'defaultRoles'=>array('guest'),
        ),
		'errorHandler'=>array(
			// use 'site/error' action to display errors
            'errorAction'=>'site/error',
        ),
		'log'=>array(
			'class'=>'CLogRouter',
			'routes'=>array(
				array(
					'class'=>'CFileLogRoute',
					'levels'=>'error, warning',
				),
				// uncomment the following to show log messages on web pages
				/*
				array(
					'class'=>'CWebLogRoute',
				),
				*/
			),
		),
		'mailer' => array(
      		'class' => 'application.extensions.mailer.EMailer',
      		'pathViews' => 'application.views.email',
      		'pathLayouts' => 'application.views.email.layouts',
   ),
	),

	// application-level parameters that can be accessed
	// using Yii::app()->params['paramName']
	'params'=>array(
		// this is used in contact page
		'adminEmail'=>'admin@mail.com',
		'appAuthor'=>'FreetuxTV WebTV Manager',
		'appEmail'=>'sender@mail.com',
		'SMTPHost'=>'smtp.mail.com',
		'SMTPUsername'=>'sender@mail.com',
		'SMTPPassword'=>'password',
	),
);
