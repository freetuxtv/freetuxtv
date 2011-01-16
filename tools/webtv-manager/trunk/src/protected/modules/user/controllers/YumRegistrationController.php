<?php

Yii::import('application.modules.user.controllers.YumController');

class YumRegistrationController extends YumController {
	public $defaultAction = 'registration';

	public function accessRules() {
		return array(
			array('allow',
				'actions' => array('index', 'registration', 'recovery', 'activation', 'resendactivation'),
				'users' => array('*'),
			),
			array('allow',
				'actions' => array('captcha'),
				'users' => array('*'),
			),
			array('deny', // deny all other users
				'users' => array('*'),
			),
		);
	}

	public function actions() {
		return array(
			'captcha' => array(
				'class' => 'CCaptchaAction',
				'backColor' => 0xFFFFFF,
			),
		);
	}

	/*
	 * Registration of an new User in the system.
	 */
	public function actionRegistration() {
		$registrationType = Yum::module()->registrationType;
		$loginType = Yum::module()->loginType;
		$usernameRequirements = Yum::module()->usernameRequirements;

		if ($registrationType == YumRegistration::REG_DISABLED)
			$this->redirect(Yii::app()->user->returnUrl);

		$form = new YumRegistrationForm;
		$profile = new YumProfile();

		if (isset($_POST['YumRegistrationForm'])) {
			$form->attributes = $_POST['YumRegistrationForm'];
			$form->email = $_POST['YumProfile']['email'];
			if (Yum::module()->loginType & UserModule::LOGIN_BY_EMAIL
					&& !Yum::module()->loginType & UserModule::LOGIN_BY_USERNAME
				 ) {
				$form->email = $form->username;
				$form->username = YumRegistrationForm::genRandomString($usernameRequirements['maxLen']);
			}

			if (isset($_POST['YumProfile'])) {
				$profile->attributes = $_POST['YumProfile'];
				if (Yum::module()->loginType & UserModule::LOGIN_BY_EMAIL)
					$profile->email = $form->email;
				$profile->validate();
			}

			if ($form->email != '' && $tmp_profile = YumProfile::model()->find('email = "' . $form->email . '"')) {
				if ($tmp_profile->user->status == 0) {
					Yii::app()->user->setFlash('success', Yum::t('Please verify your E-Mail address'));
					$this->redirect($this->createUrl('/user/registration/activation', array('email' => $form->email)));
				}
				$profile->addError('email', Yum::t('E-Mail already in use. If you have not registered before, please contact our System administrator.'));
			}

			if ($registrationType == YumRegistration::REG_NO_PASSWORD
					|| $registrationType == YumRegistration::REG_NO_PASSWORD_ADMIN_CONFIRMATION) {
				$form->password=YumUserChangePassword::createRandomPassword(
						Yum::module()->passwordRequirements['minLowerCase'],
						Yum::module()->passwordRequirements['minUpperCase'],
						Yum::module()->passwordRequirements['minDigits'],
						Yum::module()->passwordRequirements['minLen']); 
				$form->verifyPassword=$form->password;
			}
			
			if($form->validate() && !$profile->hasErrors())
			{
				$user = new YumUser();
				if (isset($_POST['roles']) && is_numeric($_POST['roles']))
					$user->roles = array($_POST['roles']);
				if (isset($_POST['roles']) && is_array($_POST['roles']))
					$user->roles = $_POST['roles'];
				if ($user->register($form->username, $form->password, $form->email)) {
					if (isset($_POST['YumProfile'])) {
						$profile->user_id = $user->id;
						$profile->save();
					}

					//YumActivityController::log($user, 'register');
					if ($registrationType == YumRegistration::REG_EMAIL_CONFIRMATION
							|| $registrationType == YumRegistration::REG_EMAIL_AND_ADMIN_CONFIRMATION
							|| $registrationType == YumRegistration::REG_NO_PASSWORD
							|| $registrationType == YumRegistration::REG_NO_PASSWORD_ADMIN_CONFIRMATION) {
						$success = $this->sendRegistrationEmail($user, $form->password);

						Yii::app()->user->setFlash('success', Yum::t("Thank you for your registration. Please check your email."));
						$this->actionActivate($user, $form);
						Yii::app()->end();
					} else if ($registrationType == YumRegistration::REG_SIMPLE) {
						Yii::app()->user->setFlash('success', Yum::t('Your account has been activated. Thank you for your registration'));
						$this->refresh();
					}
				} else {
					Yii::app()->user->setFlash('error', Yum::t("Your registration didn't work. Please try another E-Mail address. If this problem persists, please contact our System Administrator. "));
					$this->refresh();
				}
			} else {
				$form->addErrors($profile->getErrors());
				Yii::app()->user->setFlash('error', Yum::t("Form validation error"));
			}
		}

		if (Yum::module()->loginType & UserModule::LOGIN_BY_EMAIL)
			$form->username = $form->email;

		$this->render(Yum::module()->registrationView, array(
			'form' => $form,
			'profile' => $profile,
			'registrationtype' => $registrationType,
				)
		);
	}

	public function actionActivate($user=null, $form=null) {
		if (!isset($user) && isset($_POST['YumProfile']['email'])) {
			$email = $_POST['YumProfile']['email'];
			$profile = YumProfile::model()->findAll($condition = 'email = :email', array(':email' => $email));
			$user = $profile[0]->user;
		} else {
			$user = new YumUser;
		}
		if (!isset($form)) {
			$form = new YumRegistrationForm;
		}

		$this->render(Yum::module()->activateView, array(
			'form' => $form,
			'user' => $user,
			'activateFromWeb' => Yum::module()->activateFromWeb,
		));
	}

	public function actionResendActivation() {

		if (isset($_POST['email'])) {
			$email = $_POST['email'];
			$registrationType = Yum::module()->registrationType;
			$password = null;
			$profile = YumProfile::model()->findAll($condition = 'email = :email', array(':email' => $email));
			$user = $profile[0]->user;
			$user->activationKey = $user->generateActivationKey();
			if ($registrationType == YumRegistration::REG_NO_PASSWORD || $registrationType == YumRegistration::REG_NO_PASSWORD_ADMIN_CONFIRMATION) {
				$password = YumUserChangePassword::createRandomPassword(Yum::module()->passwordRequirements['minLowerCase'], Yum::module()->passwordRequirements['minUpperCase'], Yum::module()->passwordRequirements['minDigits'], Yum::module()->passwordRequirements['minLen']);
				$user->password = YumUser::model()->encrypt($password);
				$this->sendRegistrationEmail($user);
			}
			$user->save();
		} else {
			if (!isset($user) && !isset($_POST['email']))
				$user = new YumUser;
		}
		$form = new YumRegistrationForm;
		$this->render(Yum::module()->activateView, array(
			'form' => $form,
			'user' => $user,
				)
		);
	}

	// Send the Email to the given user object. $user->email needs to be set.
	public function sendRegistrationEmail($user, $password=null) {
		if (!isset($user->profile[0]->email)) {
			throw new CException(Yum::t('Email is not set when trying to send Registration Email'));
		}
		$registrationType = Yum::module()->registrationType;

		$activation_url = $this->createAbsoluteUrl('registration/activation', array(
					'activationKey' => $user->activationKey,
					'email' => $user->profile[0]->email)
		);

		$content = YumTextSettings::model()->find('language = :lang', array(
					'lang' => Yii::app()->language));
		$sent = null;

		if (is_object($content)) {
			if ($registrationType == YumRegistration::REG_NO_PASSWORD || $registrationType == YumRegistration::REG_NO_PASSWORD_ADMIN_CONFIRMATION)
				$body = strtr($content->text_email_registration . "\n\nYour Activation Key is $user->activationKey ,\n\n Your temporary password is $password,", array('{activation_url}' => $activation_url));
			else
				$body = strtr($content->text_email_registration, array('{activation_url}' => $activation_url));

			$mail = array(
				'from' => Yum::module()->registrationEmail,
				'to' => $user->profile[0]->email,
				'subject' => $content->subject_email_registration,
				'body' => $body,
			);
			$sent = YumMailer::send($mail);
		}
		else {
			throw new CException(Yum::t('The messages for your application language are not defined.'));
		}

		return $sent;
	}

	/**
	 * Activation of an user account
	 */
	public function actionActivation($email=null, $key=null) {
		if (!Yii::app()->user->isGuest)
			$this->redirect(Yii::app()->user->returnUrl);

		if (isset($_POST['YumUserChangePassword'])) {
			//FIXME: Ugly hack to pass email and key on $_POST
			$email = $_POST['email'];
			$key = $_POST['activationKey'];

			if (YumUser::activate($email, $key)) {
				$form = new YumUserChangePassword;
				$form->attributes = $_POST['YumUserChangePassword'];

				if ($form->validate()) {
					$profile = YumProfile::model()->findByAttributes(array('email'=> $email ));
					$user = $profile->user;
					$user->password = YumUser::encrypt($form->password);
					$user->save();

					// handle the login task
					$this->doLogin($user->username, $form->password);
				} else {
					Yii::app()->user->setFlash('error', Yum::t("Cannot set password. Try again."));

					// Renders the change password form
					$this->renderPasswordForm(array(
						'title' => Yum::t("User activation"),
						'content' => Yum::t("Cannot set password. Try again."),
							), array(
						'email' => $email,
						'key' => $key,
					));

					Yii::app()->end();
				}
			}
		}

		if (isset($_GET['email']) && isset($_GET['activationKey'])) {
			$email = $_GET['email'];
			$key = $_GET['activationKey'];
			$profile = YumProfile::model()->findByAttributes(array('email' => $email));
			if ($profile !== null) {
				if ($profile->user->status == 1) {
					$this->redirect(Yii::app()->user->returnUrl);
				}
			} else {
				$this->redirect($this->createUrl('/user/registration'));
			}
		}

		if (YumUser::activate($email, $key)) {
			// Renders the change password form
			$this->renderPasswordForm(array(
				'title' => Yum::t("User activation"),
				'content' => Yum::t("Your account has been activated."),
					), array(
				'email' => $email,
				'key' => $key,
					)
			);
		} else {
			$this->actionActivate();
			/*
			  $this->render('/user/message',array(
			  'title'=>Yum::t("User activation"),
			  'content'=>Yum::t("Incorrect activation URL")));
			 */
		}
	}

	/**
	 * Password recovery routine. The User will receive an email with an
	 * activation link. If clicked, he will be prompted to enter his new
	 * password.
	 */
	public function actionRecovery() {
		$form = new YumUserRecoveryForm;

		if (isset($_GET['email']) && isset($_GET['activationKey'])) {
			$email = $_GET['email'];
			$key = $_GET['activationKey'];

			$profile = YumProfile::model()->findByAttributes(array('email' => $_GET['email']));
			if ($profile !== null) {
				$user = $profile->user;
				if ($user->activationKey != $key)
					$this->render(Yum::module()->recoveryView, array('form' => new YumUserRecoveryForm));
			} else {
				$this->render(Yum::module()->recoveryView, array('form' => new YumUserRecoveryForm));
			}

			$registrationType = Yum::module()->registrationType;
			$passwordform = new YumUserChangePassword;

			if ($registrationType == YumRegistration::REG_NO_PASSWORD
					|| $registrationType == YumRegistration::REG_NO_PASSWORD_ADMIN_CONFIRMATION) {
				if (Yum::module()->recoveryFromWeb) {
					if (isset($_POST['YumUserChangePassword'])) {
						$passwordform->attributes = $_POST['YumUserChangePassword'];
						if ($passwordform->validate()) {
							$user->password = YumUser::encrypt($passwordform->password);
							$user->save();
							$username = (Yum::module()->loginType & UserModule::LOGIN_BY_EMAIL) ? $email : $user->username;
							$identity = new YumUserIdentity($username, $passwordform->password);

							// handle the login task
							$this->doLogin($username, $passwordform->password);
						}
						else
						{
							Yii::app()->user->setFlash('error', Yum::t('Passwords do not match'));
						}
					}
					// Renders the change password form
					$this->renderPasswordForm(array(
						'title' => Yum::t("Password recovery"),
						'content' => Yum::t("Your request succeeded. Please enter below your new password:"),
							), array(
						'email' => $email,
						'key' => $key,
							)
					);
					return;
				}
				$password = YumUserChangePassword::createRandomPassword(Yum::module()->passwordRequirements['minLowerCase'], Yum::module()->passwordRequirements['minUpperCase'], Yum::module()->passwordRequirements['minDigits'], Yum::module()->passwordRequirements['minLen']);
				$user->password = YumUser::encrypt($password);
				$user->save();

				$mail = array(
					'from' => Yii::app()->params['adminEmail'],
					'to' => $user->profile[0]->email,
					'subject' => Yum::t('Password recovery'),
					'body' => sprintf('You have requested to reset your Password. Your new password, is %s', $password),
				);
				$sent = YumMailer::send($mail);

				Yii::app()->user->setFlash('loginMessage', Yum::t('Instructions have been sent to you. Please check your email.'));
			}

			if ($user->activationKey == $_GET['activationKey']) {
				if (isset($_POST['YumUserChangePassword'])) {
					$passwordform->attributes = $_POST['YumUserChangePassword'];
					if ($passwordform->validate()) {

						$user->password = YumUser::encrypt($passwordform->password);
						$user->activationKey = YumUser::encrypt(microtime() . $passwordform->password);
						$user->save();
						Yii::app()->user->setFlash('loginMessage', Yum::t("Your new password has been saved."));
						$this->redirect(Yum::module()->loginUrl);
					}
				}
				if ($registrationType == YumRegistration::REG_NO_PASSWORD || $registrationType == YumRegistration::REG_NO_PASSWORD_ADMIN_CONFIRMATION) {
					$this->redirect(array('/user/user/login'));
				} else {
					$this->render(Yum::module()->recoveryChangePasswordView, array('form' => $passwordform));
				}
			} else {
				Yii::app()->user->setFlash('recoveryMessage', Yum::t("Incorrect recovery link."));
				$this->redirect($this->createAbsoluteUrl('registration/recovery'));
			}
		} else {
			if (isset($_POST['YumUserRecoveryForm'])) {
				$form->attributes = $_POST['YumUserRecoveryForm'];

				if ($form->validate()) {
					$user = YumUser::model()->findbyPk($form->user_id);



					$activation_url = $this->createAbsoluteUrl('registration/recovery', array(
								'activationKey' => $user->activationKey,
								'email' => $user->profile[0]->email));
					if (Yum::module()->enableLogging == true)
						YumActivityController::logActivity($user, 'recovery');
					Yii::app()->user->setFlash('loginMessage', Yum::t('Instructions have been sent to you. Please check your email.'));

					$content = YumTextSettings::model()->find('language = :lang', array('lang' => Yii::app()->language));
					$sent = null;

					if (is_object($content)) {
						$mail = array(
							'from' => Yii::app()->params['adminEmail'],
							'to' => $user->profile[0]->email,
							'subject' => $content->subject_email_registration,
							'body' => strtr($content->text_email_recovery, array('{activation_url}' => $activation_url)),
						);
						$sent = YumMailer::send($mail);
					} else {
						throw new CException(Yum::t('The messages for your application language are not defined.'));
					}
				}
			}
			$this->render(Yum::module()->recoveryView, array('form' => $form));
		}
	}

	/**
	 * doLogin does the login task, whether is autologin or simply show the
	 * login page. This is an attemp to DRY YumRegistrationController.
	 * @param string $username The user's username
	 * @param string $password The user's password
	 * @return None. It just redirects to the proper page.
	 */
	public function doLogin($username=null, $password=null) {
		if (Yum::module()->loginType & UserModule::LOGIN_BY_EMAIL && strpos($username, '@')) {
			$username = YumProfile::model()->findByAttributes(array('email' => $username))->user->username;
		}

		if (Yum::module()->autoLogin) {
			$identity = new YumUserIdentity($username, $password);
			$identity->authenticate();
			if ($identity->errorCode == UserIdentity::ERROR_NONE) {
				$duration = 3600 * 24 * 30; // 30 days
				Yii::app()->user->login($identity, $duration);
				$this->redirect(Yum::module()->returnUrl);
			}
		} else {
			Yii::app()->user->setFlash('success', Yum::t("Please log in into the application."));
			$this->redirect(Yum::module()->loginUrl);
		}
	}

	/**
	 * renderPasswordForm shows the password change form. This is an attemp to DRY
	 * YumRegistrationController.
	 * @param array $vars Array that contains the 'title' and 'contents'
	 * key=>value parameters.
	 */
	public function renderPasswordForm($vars=array(), $params=array()) {
		if ($vars['title'] == '' || $vars['title'] == null)
			$vars['title'] == Yum::t('Message');
		if ($vars['content'] == null)
			$vars['content'] == '';

		$partial = array();
		if (Yum::module()->activationPasswordSet || $this->action - id == 'activation')
			$params = array_merge(array('form' => new YumUserChangePassword), $params);
		$partial = array(
			array('view' => Yum::module()->passwordFormView, 'params' => $params));
		$this->render(Yum::module()->messageView, array(
			'title' => $vars['title'],
			'content' => $vars['content'],
			'partial' => $partial,
				)
		);
	}

}
