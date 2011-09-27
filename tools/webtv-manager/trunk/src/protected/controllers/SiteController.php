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

		// Load the last updates
		$conditions = "";
		$params = array();

		$conditions = "EntityType=:EntityType AND (ActionType=:ActionType1 OR ActionType=:ActionType2)";
		$params = array(":EntityType"=>History::ENTITYTYPE_WEBSTREAM,
			":ActionType1"=>History::ACTIONTYPE_WEBSTREAM_ADD, ":ActionType2"=>History::ACTIONTYPE_WEBSTREAM_IMPORT);
		$lastAdds=new CActiveDataProvider('History', array(
			'criteria'=>array(
				'condition'=>$conditions,
				'params'=>$params,
				'limit'=>10,
				'order'=>'Date DESC',
			),
		));

		// Load the last adds
		$conditions = "EntityType=:EntityType AND (ActionType=:ActionType1 OR ActionType=:ActionType2)";
		$params = array(":EntityType"=>History::ENTITYTYPE_WEBSTREAM,
			":ActionType1"=>History::ACTIONTYPE_WEBSTREAM_EDIT, ":ActionType2"=>History::ACTIONTYPE_WEBSTREAM_CHANGESTATUS);
		$lastUpdates=new CActiveDataProvider('History', array(
			'criteria'=>array(
				'condition'=>$conditions,
				'params'=>$params,
				'limit'=>10,
				'order'=>'Date DESC',
			),
		));

		// Load stats
		$query = "SELECT wtvmT_WebStream.LangCode AS LangCode, wtvmT_Lang.Label As LangName, TypeStream, Count(*) AS StreamCount, CountLang
			FROM wtvmT_WebStream LEFT JOIN wtvmT_Lang ON wtvmT_WebStream.LangCode=wtvmT_Lang.Code, (SELECT LangCode, Count(*) AS CountLang FROM wtvmT_WebStream GROUP BY LangCode) AS TmpCountLang
			WHERE StreamStatusCode = :StreamStatusCode
			AND (TmpCountLang.LangCode = wtvmT_WebStream.LangCode OR (TmpCountLang.LangCode IS NULL AND wtvmT_WebStream.LangCode IS NULL))
			GROUP BY wtvmT_WebStream.LangCode, TypeStream
			ORDER BY CountLang DESC, wtvmT_WebStream.LangCode";
		$params = array(":StreamStatusCode" => WebStream::WEBSTREAM_STATUS_WORKING);
		$command = Yii::app()->db->createCommand($query);
		$rows = $command->queryAll(true, $params);

		$streamTypes = WebStream::getTypeStreamList();

		$statsTypes = WebStream::getTypeStreamList();
		foreach($statsTypes as $key => $value){
			$statsTypes[$key] = 0;
		}

		$rawData = array();
		$curLang = null;
		$rawLine = null;
		foreach($rows as $row) {
			$realLang = $row['LangCode'];
			$realLangName = $row['LangName'];
			if($realLang == null || $realLang == ""){
				$realLang = "none";
				$realLangName = "No lang";
			}

			if($curLang == null || $curLang != $realLang){

				$curLang = $realLang;
				if($rawLine != null){
					$rawData[] = $rawLine;
				}

				$rawLine = array('LangCode'=>$realLang, 'id'=>$realLang, 'LangName'=>$realLangName);
				foreach($streamTypes as $key => $value){
					$rawLine[$key] = 0;
				}
				$rawLine['TotalCount'] = 0;
			}
			$rawLine[$row['TypeStream']] = $row['StreamCount'];
			$rawLine['TotalCount'] = $rawLine['TotalCount'] + $row['StreamCount'];
			$statsTypes[$row['TypeStream']] = $statsTypes[$row['TypeStream']] + $row['StreamCount'];
		}
		if($rawLine != null){
			$rawData[] = $rawLine;
		}
		$statsLangs = new CArrayDataProvider($rawData, array(
			'id'=>'LangCode',
			'sort'=>array(
				'attributes'=>array(
				     'LangCode', 'TotalCount',
				),
			),
			'pagination'=>array(
				'pageSize'=>count($rawData),
			),
		));
		
		// Render page
		$this->render('index', array(
			'modelSearchForm'=>$modelSearchForm,
			'modelSendForm'=>$modelSendForm,
			'lastAdds'=>$lastAdds,
			'lastUpdates'=>$lastUpdates,
			'statsLangs' => $statsLangs,
			'statsTypes' => $statsTypes,
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
