<?php

// uncomment the following to define a path alias
// Yii::setPathOfAlias('local','path/to/local-folder');

// This is the main Web application configuration. Any writable
// CWebApplication properties can be configured here.
return array(
	'basePath'=>dirname(__FILE__).DIRECTORY_SEPARATOR.'..',
	'name'=>'FreetuxTV WebTV Manager',
	'language' => 'en',
	
	// preloading 'log' component
	'preload'=>array('log'),

	// autoloading model and component classes
	'import'=>array(
		'application.models.*',
		'application.components.*',
		'application.modules.user.models.*',
        'application.modules.user.components.*',
		'application.modules.user.controllers.*',
	),

	// application components
	'components'=>array(
		'user'=>array(
			'class' => 'application.modules.user.components.YumWebUser',
      		'loginUrl' => array('/user/login'),
      		// enable cookie-based authentication
			'allowAutoLogin'=>true,
		),
		// uncomment the following to enable URLs in path-format
		'urlManager'=>array(
			'urlFormat'=>'path',
			'rules'=>array(
				'playlists/playlist\.m3u'=>'Playlist/index',
				'playlists/playlist_<type:\w+>_<lng:\w+>\.m3u'=>'Playlist/index',
				'<controller:\w+>/<id:\d+>'=>'<controller>/view',
				'<controller:\w+>/<action:\w+>/<id:\d+>'=>'<controller>/<action>',
				'<controller:\w+>/<action:\w+>'=>'<controller>/<action>',
			),
			'showScriptName'=>true,
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

	'modules' => array(
		'user' => array(
			'debug' => true, // Set it to true to install YUM
			// Set table name for the module
			'usersTable' => 'wtvmT_YumUsers',
		//	'privacySettingTable' => 'wtvmT_YumPrivacySetting',
		//	'settingsTable' => 'wtvmT_YumSettings',
		//	'textSettingsTable' => 'wtvmT_YumTextSettings',
			'messagesTable' => 'wtvmT_YumMessages',
		//	'usergroupTable' => 'wtvmT_YumUsergroup',
		//	'userUsergroupTable' => 'wtvmT_YumUserHasUsergroup',
			'profileFieldsTable' => 'wtvmT_YumProfileFields',
			'profileFieldsGroupTable' => 'wtvmT_YumProfileFieldsGroup',
			'profileTable' => 'wtvmT_YumProfiles',
		//	'profileCommentTable' => 'wtvmT_YumProfileComment',
		//	'profileVisitTable' => 'wtvmT_YumProfileVisit',
			'rolesTable' => 'wtvmT_YumRoles',
		//	'membershipTable' => 'wtvmT_YumMembership',
		//	'paymentTable' => 'wtvmT_YumPayment',
		//	'friendshipTable' => 'wtvmT_YumFriendship',
		//	'permissionTable' => 'wtvmT_YumPermission',
		//	'actionTable' => 'wtvmT_YumAction',
			'userRoleTable' => 'wtvmT_YumUserHasRole',
		//	'activityTable' => 'wtvmT_YumActivities',
		//	'activitiesTable' => 'wtvmT_YumActivities',
		)
	),	

	// application-level parameters that can be accessed
	// using Yii::app()->params['paramName']
	'params'=>array(
		// this is used in contact page
		'adminEmail'=>'administrateur@PROVID.com',
		'appAuthor'=>'FreetuxTV WebTV Manager',
		'appEmail'=>'USER@mail.com',
		'UseSMTP'=>false,
		'SMTPHost'=>'smtp.mail.com',
		'SMTPUsername'=>'USER@mail.com',
		'SMTPPassword'=>'password',
	),
);
