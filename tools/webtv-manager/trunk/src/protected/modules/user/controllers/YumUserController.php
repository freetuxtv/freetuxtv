<?php

class YumUserController extends YumController
{
	const PAGE_SIZE=10;
	private $_model;
	
	public function accessRules()
	{
		return array(
			array('allow',
				'actions'=>array('index','view','registration','login', 'recovery', 'activation'),
				'users'=>array('*'),
			),
			array('allow',
				'actions'=>array('captcha'),
				'users'=>array('*'),
				'expression'=>'Yii::app()->controller->module->allowCaptcha',
			),
			array('allow',
				'actions'=>array('profile', 'edit', 'logout', 'changepassword'),
				'users'=>array('@'),
			),
			array('allow',
				'actions'=>array('admin','delete','create','update', 'list', 'assign'),
				'users'=>array(Yii::app()->user->name ),
                'expression' => 'Yii::app()->user->isAdmin()'
			),
			array('allow',
				'actions' => array('admin'),
				'expression' => "Yii::app()->user->hasUsers()",
			),
			array('allow',
				'actions' => array('update'),
				'expression' => 'Yii::app()->user->hasUser($_GET[\'id\'])',
			),
			array('deny',  // deny all other users
				'users'=>array('*'),
				),
			);
	}

	public function actions()
	{
		return Yii::app()->controller->module->allowCaptcha 
			? array(
				'captcha'=>array(
					'class'=>'CCaptchaAction',
					'backColor'=>0xFFFFFF,
				),
			)
			: array();
	}

	public function actionIndex()
	{
		if(Yii::app()->user->isGuest)
			$this->actionLogin();
		else if(isset($_GET['id']) || isset ($_GET['user_id']))
			$this->actionProfile();
		else
			$this->actionList();
	}

	/* 
	Registration of an new User in the system.
	Depending on whether $enableEmailRegistration is set, an confirmation Email
	will be sent to the Email address.
	*/
	public function actionRegistration()
	{
		$form = new YumRegistrationForm;
		$profile = new YumProfile();

		if(isset($_POST['YumProfile']))
			$profile->attributes = $_POST['YumProfile'];

		// User is already logged in?
		if (($uid = Yii::app()->user->id) === true)
		{
			$this->redirect(Yii::app()->homeUrl);
		}
		else
		{
			if(isset($_POST['YumRegistrationForm']))
			{
				$form->attributes = $_POST['YumRegistrationForm'];
				$form->email = $_POST['YumProfile']['email'];

				if($form->validate())
				{
					$user = new YumUser();

					if ($user->register($form->username, $form->password, $form->email))
					{
						if(isset($_POST['YumProfile']))
						{
							$profile->attributes = $_POST['YumProfile'];
							$profile->user_id = $user->id;
							$profile->save();
							$user->email = $profile->attributes['email'];
						}

						if(Yii::app()->controller->module->enableEmailActivation)
						{
							$this->sendRegistrationEmail($user);
						} 
						else 
						{
							Yii::app()->user->setFlash('registration',
									Yii::t("UserModule.user",
								"Your account has been activated. Thank you for your registration."));
							$this->refresh();
						}

						if (UserModule::$allowInactiveAcctLogin)
						{
							if (Yii::app()->user->allowAutoLogin)
							{
								$identity = new YumUserIdentity($model->username,$sourcePassword);
								$identity->authenticate();
								Yii::app()->user->login($identity, 0);
								$this->redirect(Yii::app()->controller->module->returnUrl);
							}
							else
							{
								Yii::app()->user->setFlash('registration',
									Yii::t("UserModule.user",
										"Thank you for your registration. Please check your email or login."));
								$this->refresh();
							}
						}
						else
						{
							Yii::app()->user->setFlash('registration',
								Yii::t("UserModule.user",
									"Thank you for your registration. Please check your email."));
							$this->refresh();
						}
					}
					else
					{
						Yii::app()->user->setFlash('registration',
							Yii::t("UserModule.user",
								"Your registration didn't work. Please contact our System Administrator."));
						$this->refresh();

					}
				}
			}
			$this->render('/user/registration', array(
						'form' => $form,
						'profile' => $profile
						)
					);
		}
	}

	/*
	Send the Email to the given user object. $user->email needs to be set.
	*/
	public function sendRegistrationEmail($user)
	{
		if(!isset($user->email))
		{
			throw new CException(Yii::t('UserModule.user', 'Email is not set when trying to send Registration Email'));	
		}

		$headers = "From: " . Yii::app()->params['adminEmail']."\r\nReply-To: ".Yii::app()->params['adminEmail'];
		$activation_url = 'http://' .
			$_SERVER['HTTP_HOST'] .
			$this->createUrl('user/activation',array(
						"activationKey" => $user->activationKey, "email" => $user->email)
					);
		mail($user->email,"You registered from " . Yii::app()->name,"Please activate your account go to $activation_url.",$headers);

		return true;
	}

	public function actionLogin()
	{
		$model=new YumUserLogin;
		// collect user input data
		if(isset($_POST['YumUserLogin']))
		{
			$model->attributes=$_POST['YumUserLogin'];
			// validate user input and redirect to previous page if valid
			if($model->validate())
			{
				$lastVisit = YumUser::model()->findByPk(Yii::app()->user->id);
				$lastVisit->lastvisit = time();
				$lastVisit->save();
				$this->redirect(Yii::app()->controller->module->returnUrl);
			}
		}
		// display the login form
		$this->render('/user/login',array('model'=>$model,));
	}

	public function actionLogout()
	{
		Yii::app()->user->logout();
    	$this->redirect(Yii::app()->controller->module->returnLogoutUrl);
	}

	/**
	 * Activation of an user account
	 */
	public function actionActivation ()
	{
		if(YumUser::activate($_GET['email'], $_GET['activationKey']))
		{
			$this->render('message', array(
				'title'=>Yii::t("UserModule.user", "User activation"),
				'content'=>Yii::t("UserModule.user", "Your account has been activated.")));
		}
		else
		{
			$this->render('message',array(
				'title'=>Yii::t("UserModule.user", "User activation"),
				'content'=>Yii::t("UserModule.user", "Incorrect activation URL.")));
		}
	}

	/**
	 * Change password
	 */
	public function actionChangepassword() {
		$form = new YumUserChangePassword;
		if (isset(Yii::app()->user->id))
		{
			if(isset($_POST['YumUserChangePassword']))
			{
				$form->attributes = $_POST['YumUserChangePassword'];
				if($form->validate())
				{
					$new_password = YumUser::model()->findByPk(Yii::app()->user->id);
					$new_password->password = YumUser::encrypt($form->password);
					$new_password->activationKey = YumUser::encrypt(microtime().$form->password);

					if($new_password->save())
					{

						Yii::app()->user->setFlash('profileMessage',
							Yii::t("UserModule.user", "Your new password has been saved."));
						$this->redirect(array("user/profile"));
					}
					else
					{
						Yii::app()->user->setFlash('profileMessage',
							Yii::t("UserModule.user", "There was an error saving your password."));
						$this->redirect(array("user/profile"));
					}
				}
			}
			$this->render('changepassword',array('form'=>$form));
		} else {
			// No id was set. An error has occured. (should never get here)
			$this->redirect(Yii::app()->controller->module->returnUrl);
		}
	}


	/**
	 * Recover password
	 */
	public function actionRecovery () {
		$form = new YumUserRecoveryForm;

		// User is already logged in
		if (($uid = Yii::app()->user->id) === true)
		{
			$this->redirect(Yii::app()->controller->module->returnUrl);
		}
		else
		{
			if (isset($_GET['email']) && isset($_GET['activationKey'])) {
				$email = $_GET['email'];
				$activationKey = $_GET['activationKey'];
				$form2 = new YumUserChangePassword;
				$find = YumUser::model()->findByAttributes(array('email'=>$email));
				if($find->activationKey==$activationKey)
				{
					if(isset($_POST['YumUserChangePassword']))
					{
						$form2->attributes=$_POST['YumUserChangePassword'];
						if($form2->validate())
						{
							$find->password=YumUser::encrypt($form2->password);
							$find->activationKey=YumUser::encrypt(microtime().$form2->password);
							$find->save();
							Yii::app()->user->setFlash('loginMessage',Yii::t("user", "Your new password has been saved."));
							$this->redirect(Yii::app()->controller->module->loginUrl);
						}
					}
					$this->render('changepassword',array('form'=>$form2));
				}
				else
				{
					Yii::app()->user->setFlash('recoveryMessage',Yii::t("user", "Incorrect recovery link."));
					$this->redirect('http://' . $_SERVER['HTTP_HOST'].$this->createUrl('user/recovery'));
				}
			}
			else
			{
				if(isset($_POST['YumUserRecoveryForm']))
				{
					$form->attributes=$_POST['YumUserRecoveryForm'];

					if($form->validate())
					{
						$user = YumUser::model()->findbyPk($form->user_id);
						$headers="From: ".Yii::app()->params['adminEmail']."\r\nReply-To: ".Yii::app()->params['adminEmail'];

						$activation_url = 'http://' . $_SERVER['HTTP_HOST'].$this->createUrl('user/recovery',array(
							"activationKey" => $user->activationKey,
							"email" => $user->email));

						mail($user->email,"You have requested to be reset. To receive a new password, go to $activation_url.",$headers);

						Yii::app()->user->setFlash('loginMessage',
								Yii::t("UserModule.user",
									"Instructions have been sent to you. Please check your email."));

						$this->redirect(array('/user/user/login'));
					}
				}
				$this->render('recovery',array('form'=>$form));
			}
		}
	}

	public function actionAssign()
	{
		Relation::handleAjaxRequest($_POST);
	}


	public function actionProfile()
	{
		// Display my own profile:
		if(!isset($_GET['id']) || $_GET['id'] == Yii::app()->user->id)
		{
			if (Yii::app()->user->id)
			{
				$model = $this->loadUser(Yii::app()->user->id);

				$this->render('myprofile',array(
					'model'=>$model,
					'profile'=>$model->profile,
				));
			}
		}
		else
		{
			// Display a foreign profile:

			$model = $this->loadUser($uid = $_GET['id']);

			if($this->module->forceProtectedProfiles == true ||
				$model->profile->privacy == 'protected' ||
				$model->profile->privacy == 'private')
			{
				$this->render('profilenotallowed');
			}
			else
			{
				$this->render('foreignprofile',array(
					'model'=>$model,
					'profile'=>$model->profile,
				));
			}
		}

	}

	/**
	 * Edits a User profile.
	 */
	public function actionEdit()
	{

		if($this->module->readOnlyProfiles == true)
		{
			Yii::app()->user->setFlash('profileMessage',
				Yii::t("UserModule.user",
					"You are not allowed to edit your own profile. Please contact your System Administrator."));

			$this->redirect(array('profile', 'id'=>$model->id));
		}

		$model=YumUser::model()->findByPk(Yii::app()->user->id);
		$profile = $model->profile;

		if(isset($_POST['YumUser']))
		{
			$model->attributes=$_POST['YumUser'];
      		if($this->module->profileHistory == true)
				$profile = new YumProfile();

			if(isset($_POST['YumProfile']))
			{
				$profile->attributes=$_POST['YumProfile'];
				$profile->timestamp = time();
				$profile->privacy = $_POST['YumProfile']['privacy'];
				$profile->user_id = $model->id;
			}

				if($model->save() && $profile->save() )
					Yii::app()->user->setFlash('profileMessage',
						Yii::t("UserModule.user", "Your changes have been saved"));
				else
					Yii::app()->user->setFlash('profileMessage',
						Yii::t("UserModule.user", "An error occured while saving your changes"));
				$this->redirect(array('profile', 'id'=>$model->id));
		}

		$this->render('profile-edit',array(
			'model'=>$model,
			'profile'=>$profile,
		));

	}

	/**
	 * Displays a User
	 */
	public function actionView()
	{
		$model = $this->loadUser();
		$this->render('view',array(
			'model'=>$model,
		));
	}

	/**
	 * Creates a new User.
	 */
	public function actionCreate()
	{
		$this->layout = YumWebModule::yum()->adminLayout;
		$model=new YumUser;
		$profile=new YumProfile;

		if(isset($_POST['YumUser']))
		{
			$model->attributes=$_POST['YumUser'];


			if($this->module->hasModule('role'))
			{
				$model->roles = Relation::retrieveValues($_POST, 'YumRole');
			}
			$model->activationKey=YumUser::encrypt(microtime().$model->password);
			$model->createtime=time();
			$model->lastvisit=time();

			if( isset($_POST['YumProfile']) )
				$profile->attributes=$_POST['YumProfile'];
			$profile->user_id = 0;
			if($model->validate() && $profile->validate()) {
				$model->password=YumUser::encrypt($model->password);
				if($model->save())
				{
					$profile->user_id=$model->id;
					$profile->save();
				}
				$this->redirect(array('view','id'=>$model->id));
			}
		}

		$this->render('create',array(
			'model'=>$model,
			'profile'=>$profile,
			'tabularIdx'=>null,
		));

	}

	public function actionUpdate()
	{
		$this->layout = YumWebModule::yum()->adminLayout;
		$model = $this->loadUser();
		$model->password = '';

		if(($profile = $model->profile) === false)
			$profile = new YumProfile();

		if(isset($_POST['YumUser']))
		{
			$model->attributes = $_POST['YumUser'];

			if($this->module->hasModule('role'))
			{
				// Assign the roles and slave Users to the model
				if(!isset($_POST['YumUser']['YumRole']))
					$_POST['YumUser']['YumRole'] = array();

				if(!isset($_POST['YumUser']['YumUser']))
					$_POST['YumUser']['YumUser'] = array();

				$model->roles = Relation::retrieveValues($_POST, 'YumRole');
				$model->users = $_POST['YumUser']['YumUser'];
			}

			if(isset($_POST['YumProfile']))
				$profile->attributes = $_POST['YumProfile'];

			if($model->validate() && $profile->validate())
			{
				$old_password = YumUser::model()->findByPk($model->id)->password;
				if ($model->password != '')
				{
					$model->password = YumUser::encrypt($model->password);
					$model->activationKey = YumUser::encrypt(microtime().$model->password);
				}
				else
				{
					$model->password = $old_password;
				}

				$model->save();
				$profile->save();
				$this->redirect(array('view','id'=>$model->id));
			}
		}

		$this->render('update',array(
			'model'=>$model,
			'profile'=>$profile,
			'tabularIdx'=>null,
		));
	}


	/**
	 * Deletes a User, and if profile History is deactivated, deletes all
   * profiles.
	 */
	public function actionDelete()
	{
		if(Yii::app()->request->isPostRequest)
		if(Yii::app()->user->isAdmin())
		{
			$this->layout = YumWebModule::yum()->adminLayout;
			Yii::app()->user->setFlash('adminMessage',
					Yii::t("UserModule.user",
						"Admin Users can not be deleted!"));
			$this->redirect('admin');
		}
		else
		{
			$this->layout = YumWebModule::yum()->layout;
			$model = $this->loadUser(Yii::app()->user->id);

			if(isset($_POST['confirmPassword'])) 
			{
				if($model->encrypt($_POST['confirmPassword']) == $model->password)
				{
					if(Yii::app()->controller->module->profileHistory == false) 
					{
			if(is_array($model->profile))
			{
				foreach($model->profile as $profile)
				{
					$profile->delete();
				}
			}
			else if (is_object($model->profile))
			{
				$model->profile->delete();
			}
					}
			$model->delete();
					$this->actionLogout();
				} 
				else 
				{
					Yii::app()->user->setFlash('profileMessage',
							sprintf('%s. (%s)', 
								Yii::t('UserModule.user', 'Wrong password confirmation! Account was not deleted'),
								CHtml::link(Yii::t('UserModule.user', 'Try again'), array('delete'))
								)
							);
					$this->redirect('profile');
		}
			}
		else
			{
				$this->render('confirmDeletion', array('model' => $model));
	}
		}
	}



	public function actionList()
	{
		$this->layout = YumWebModule::yum()->adminLayout;
		$dataProvider=new CActiveDataProvider('YumUser', array(
			'pagination'=>array(
				'pageSize'=>self::PAGE_SIZE,
		)));

		$this->render('index',array(
			'dataProvider'=>$dataProvider,
		));
	}

	public function actionAdmin()
	{
		$this->layout = YumWebModule::yum()->adminLayout;
		$dataProvider=new CActiveDataProvider('YumUser', array(
			'pagination'=>array(
				'pageSize'=>self::PAGE_SIZE,
		)));

		$this->render('admin',array(
			'dataProvider'=>$dataProvider,
		));

	}

	/**
	 * Loads the User Object instance
	 * @return YumUser
	 */
	public function loadUser($uid = 0)
	{
		if($this->_model === null)
		{
			if($uid != 0)
				$this->_model = YumUser::model()->findByPk($uid);
			elseif(isset($_GET['id']))
				$this->_model = YumUser::model()->findByPk($_GET['id']);
			if($this->_model === null)
				throw new CHttpException(404,'The requested User does not exist.');
		}
		return $this->_model;
	}

}
