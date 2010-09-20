<?php

class WebStreamController extends Controller
{
	/**
	 * @var string the default layout for the views. Defaults to '//layouts/column2', meaning
	 * using two-column layout. See 'protected/views/layouts/column2.php'.
	 */
	public $layout='//layouts/column1';

	/**
	 * @var CActiveRecord the currently loaded data model instance.
	 */
	private $_model;

	/**
	 * @return array action filters
	 */
	public function filters()
	{
		return array(
			'accessControl', // perform access control for CRUD operations
		);
	}

	/**
	 * Specifies the access control rules.
	 * This method is used by the 'accessControl' filter.
	 * @return array access control rules
	 */
	public function accessRules()
	{
		return array(
			array('allow',  // allow all users to perform 'index' and 'view' actions
				'actions'=>array('index','view','send', 'add'),
				'users'=>array('*'),
			),
			array('allow', // allow authenticated user to perform 'create' and 'update' actions
				'actions'=>array('create'),
				'users'=>array('@'),
			),
			array('allow', // allow admin user to perform 'admin' and 'delete' actions
				'actions'=>array('admin','delete','update'),
				'users'=>array('admin'),
			),
			array('deny',  // deny all users
				'users'=>array('*'),
			),
		);
	}

	/**
	 * Displays a particular model.
	 */
	public function actionView()
	{
		$this->render('view',array(
			'model'=>$this->loadModel(),
		));
	}

	/**
	 * Creates a new model.
	 * If creation is successful, the browser will be redirected to the 'view' page.
	 */
	public function actionAdd()
	{
		$model=new WebStream;

		// Uncomment the following line if AJAX validation is needed
		// $this->performAjaxValidation($model);

		if(isset($_POST['WebStream']))
		{
			$model->attributes=$_POST['WebStream'];
			if($model->LangCode==""){
				$model->LangCode=null;
			}
			if($model->save()){
				$this->redirect(array('view','id'=>$model->Id));
			}
		}
		
	}

	/**
	 * Creates a new model.
	 * If creation is successful, the browser will be redirected to the 'view' page.
	 */
	public function actionCreate()
	{
		$model=new WebStream;

		// Uncomment the following line if AJAX validation is needed
		// $this->performAjaxValidation($model);

		if(isset($_POST['WebStream']))
		{
			$model->attributes=$_POST['WebStream'];
			if($model->save())
				$this->redirect(array('view','id'=>$model->Id));
		}

		$this->render('create',array(
			'model'=>$model,
		));
	}

	/**
	 * Updates a particular model.
	 * If update is successful, the browser will be redirected to the 'view' page.
	 */
	public function actionUpdate()
	{
		$model=$this->loadModel();

		// Uncomment the following line if AJAX validation is needed
		// $this->performAjaxValidation($model);

		if(isset($_POST['WebStream']))
		{
			$model->attributes=$_POST['WebStream'];
			if($model->save())
				$this->redirect(array('view','id'=>$model->Id));
		}

		$this->render('update',array(
			'model'=>$model,
		));
	}

	/**
	 * Deletes a particular model.
	 * If deletion is successful, the browser will be redirected to the 'index' page.
	 */
	public function actionDelete()
	{
		if(Yii::app()->request->isPostRequest)
		{
			// we only allow deletion via POST request
			$this->loadModel()->delete();

			// if AJAX request (triggered by deletion via admin grid view), we should not redirect the browser
			if(!isset($_GET['ajax']))
				$this->redirect(array('index'));
		}
		else
			throw new CHttpException(400,'Invalid request. Please do not repeat this request again.');
	}

	/**
	 * Lists all models.
	 */
	public function actionIndex()
	{
		$conditions = "";
		$params = array();
		if(isset($_GET['WebStreamName'])){
			$conditions = "Name LIKE :WebStreamName";
			$params[':WebStreamName'] = '%'.$_GET['WebStreamName'].'%';
		}
		if(isset($_GET['WebStreamStatus'])){
			if($_GET['WebStreamStatus'] != "all" && $_GET['WebStreamStatus'] != ""){
				if($conditions != ""){
					$conditions .= " AND ";
				}
				$conditions .= " StreamStatusCode=:WebStreamStatus";
				$params[':WebStreamStatus'] = $_GET['WebStreamStatus'];
			}
		}
		if(isset($_GET['WebStreamLang'])){
			if($_GET['WebStreamLang'] != "all" && $_GET['WebStreamLang'] != ""){
				if($conditions != ""){
					$conditions .= " AND ";
				}
				$conditions .= " LangCode=:WebStreamLang";
				$params[':WebStreamLang'] = $_GET['WebStreamLang'];
			}
		}

		$dataProvider=new CActiveDataProvider('WebStream',array(
			'criteria'=>array(
				'condition'=>$conditions,
				'params'=>$params,
				'order'=>'Name',
			),
			'pagination'=>array(
				'pageSize'=>20,
			),
		));

		$this->render('index',array(
			'dataProvider'=>$dataProvider,
		));
	}

	/**
	 * Send a new URL.
	 */
	public function actionSend()
	{
		$conditions = "";
		$params = array();
		$conditions = "Url=:WebStreamUrl";
		$params[':WebStreamUrl'] = $_GET['WebStreamUrl'];

		$dataProvider=new CActiveDataProvider('WebStream',array(
			'criteria'=>array(
				'condition'=>$conditions,
				'params'=>$params,
				'order'=>'Name DESC',
			),
			'pagination'=>array(
				'pageSize'=>20,
			),
		));

		$this->render('send',array(
			'dataProvider'=>$dataProvider,
		));
	}

	/**
	 * Manages all models.
	 */
	public function actionAdmin()
	{
		$model=new WebStream('search');
		$model->unsetAttributes();  // clear any default values
		if(isset($_GET['WebStream']))
			$model->attributes=$_GET['WebStream'];

		$this->render('admin',array(
			'model'=>$model,
		));
	}

	/**
	 * Returns the data model based on the primary key given in the GET variable.
	 * If the data model is not found, an HTTP exception will be raised.
	 */
	public function loadModel()
	{
		if($this->_model===null)
		{
			if(isset($_GET['id']))
				$this->_model=WebStream::model()->findbyPk($_GET['id']);
			if($this->_model===null)
				throw new CHttpException(404,'The requested page does not exist.');
		}
		return $this->_model;
	}

	/**
	 * Performs the AJAX validation.
	 * @param CModel the model to be validated
	 */
	protected function performAjaxValidation($model)
	{
		if(isset($_POST['ajax']) && $_POST['ajax']==='web-stream-form')
		{
			echo CActiveForm::validate($model);
			Yii::app()->end();
		}
	}
}
