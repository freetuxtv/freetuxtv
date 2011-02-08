<?php

class SiteController extends Controller
{
	/**
	 * Declares class-based actions.
	 */
	public function actions()
	{
		return array(
			// captcha action renders the CAPTCHA image displayed on the contact page
			'captcha'=>array(
				'class'=>'CCaptchaAction',
				'backColor'=>0xFFFFFF,
			),
			// page action renders "static" pages stored under 'protected/views/site/pages'
			// They can be accessed via: index.php?r=site/page&view=FileName
			'page'=>array(
				'class'=>'CViewAction',
			),
		);
	}

	/**
	 * This is the default 'index' action that is invoked
	 * when an action is not explicitly requested by users.
	 */
	public function actionIndex()
	{
		$modelSearchForm = new WebStreamSearchForm;
		$modelSendForm = new WebStreamSendForm;

		// Load the last update
		$conditions = "";
		$params = array();

		$lastUpdates=new CActiveDataProvider('History', array(
			'criteria'=>array(
				'condition'=>$conditions,
				'params'=>$params,
				'limit'=>10,
				'order'=>'Date DESC',
			),
		));

		// renders the view file 'protected/views/site/index.php'
		// using the default layout 'protected/views/layouts/main.php'
		$this->render('index', array(
			'modelSearchForm'=>$modelSearchForm,
			'modelSendForm'=>$modelSendForm,
			'lastUpdates'=>$lastUpdates,
		));
	}

	/**
	 * This is the action to handle external exceptions.
	 */
	public function actionError()
	{
	    if($error=Yii::app()->errorHandler->error)
	    {
	    	if(Yii::app()->request->isAjaxRequest)
	    		echo $error['message'];
	    	else
	        	$this->render('error', $error);
	    }
	}

	/**
	 * Displays the contact page
	 */
	public function actionContact()
	{
		$model=new ContactForm;
		if(isset($_POST['ContactForm']))
		{
			$model->attributes=$_POST['ContactForm'];
			if($model->validate())
			{
				$headers="From: {$model->email}\r\nReply-To: {$model->email}";
				mail(Yii::app()->params['adminEmail'],$model->subject,$model->body,$headers);
				Yii::app()->user->setFlash('contact','Thank you for contacting us. We will respond to you as soon as possible.');
				$this->refresh();
			}
		}
		$this->render('contact',array('model'=>$model));
	}

	/**
	 * Displays the login page
	 */
	public function actionLogin()
	{
		$model=new LoginForm;

		// if it is ajax validation request
		if(isset($_POST['ajax']) && $_POST['ajax']==='login-form')
		{
			echo CActiveForm::validate($model);
			Yii::app()->end();
		}

		// collect user input data
		if(isset($_POST['LoginForm']))
		{
			$model->attributes=$_POST['LoginForm'];
			// validate user input and redirect to the previous page if valid
			if($model->validate() && $model->login()){
				$this->redirect(Yii::app()->user->returnUrl);
			}
		}
		// display the login form
		$this->render('login', array('model'=>$model));
	}


	public function actionSendWebStream()
	{
		$modelSearchForm = new WebStreamSearchForm;
		$modelSendForm = new WebStreamSendForm;

		// collect user input data
		if(isset($_GET['WebStreamSendForm']))
		{
			$modelSendForm->attributes=$_GET['WebStreamSendForm'];
			if($modelSendForm->validate()){
				$this->redirect(array("WebStream/send", "WebStreamUrl" => $modelSendForm->Url));
			}
		}
		
		$this->render('index', array(
			'modelSearchForm'=>$modelSearchForm,
			'modelSendForm'=>$modelSendForm,
		));
	}

	public function actionSearchWebStream()
	{
		$modelSearchForm = new WebStreamSearchForm;
		$modelSendForm = new WebStreamSendForm;

		// collect user input data
		if(isset($_GET['WebStreamSearchForm']))
		{
			$modelSearchForm->attributes=$_GET['WebStreamSearchForm'];
			if($modelSearchForm->validate()){
				$this->redirect($this->createUrl("WebStream/index", array('WebStreamSearchForm' => $modelSearchForm->attributes)));
			}
		}
		
		$this->render('index', array(
			'modelSearchForm'=>$modelSearchForm,
			'modelSendForm'=>$modelSendForm,
		));
	}

	/**
	 * Logs out the current user and redirect to homepage.
	 */
	public function actionLogout()
	{
		Yii::app()->user->logout();
		$this->redirect(Yii::app()->homeUrl);
	}


	public function actionSetAuth()
	{
		/*
		$auth=Yii::app()->authManager;
 
		$auth->createOperation('sendWebStream','Send a WebStream');
		$auth->createOperation('editWebStream','Edit a WebStream');
		$auth->createOperation('changeStatusWebStream','Change the status of a WebStream');
		 
		$role=$auth->createRole('guest');
		$role->addChild('sendWebStream');
		$role->addChild('editWebStream');
		 
		$role=$auth->createRole('contributor');
		$role->addChild('guest');
		 
		$role=$auth->createRole('moderator');
		$role->addChild('contributor');
		$role->addChild('changeStatusWebStream');
		
		$this->render('index');

		
		/*
		$auth->createOperation('createPost','create a post');
		$auth->createOperation('readPost','read a post');
		$auth->createOperation('updatePost','update a post');
		$auth->createOperation('deletePost','delete a post');
		 
		$bizRule='return Yii::app()->user->id==$params["post"]->authID;';
		$task=$auth->createTask('updateOwnPost','update a post by author himself',$bizRule);
		$task->addChild('updatePost');
		 
		$role=$auth->createRole('reader');
		$role->addChild('readPost');
		 
		$role=$auth->createRole('author');
		$role->addChild('reader');
		$role->addChild('createPost');
		$role->addChild('updateOwnPost');
		 
		$role=$auth->createRole('editor');
		$role->addChild('reader');
		$role->addChild('updatePost');
		 
		$role=$auth->createRole('admin');
		$role->addChild('editor');
		$role->addChild('author');
		$role->addChild('deletePost');
		 
		$auth->assign('reader','readerA');
		$auth->assign('author','authorB');
		$auth->assign('editor','editorC');
		$auth->assign('admin','adminD');
		*/
	}
}
