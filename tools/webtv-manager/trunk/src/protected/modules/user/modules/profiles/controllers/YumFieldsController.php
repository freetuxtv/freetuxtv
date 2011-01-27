<?php

class YumFieldsController extends YumController
{
	const PAGE_SIZE=10;

	private $_model;

	public function accessRules()
	{
		return array(
			array('allow',  
				'actions'=>array('*'),
				'users'=>array('*'),
			),
			array('allow', 
				'actions'=>array('*'),
				'users'=>array('@'),
			),
			array('allow', 
				'actions'=>array('index', 'create','update','view','admin','delete'),
				'users'=>array(Yii::app()->user->name ),
                'expression' => 'Yii::app()->user->isAdmin()'
			),
			array('deny',  // deny all users
				'users'=>array('*'),
			),
		);
	}

	public function actionView()
	{
		$this->layout = YumWebModule::yum()->adminLayout;
		$this->render('view',array(
			'model'=>$this->loadModel(),
		));
	}

	public function actionCreate()
	{
		$this->layout = YumWebModule::yum()->adminLayout;
		$model=new YumProfileField;
		#add to group?
		if(isset($_GET['in_group']))
			$model->field_group_id=$_GET['in_group'];
		if(isset($_POST['YumProfileField']))
		{
			$model->attributes=$_POST['YumProfileField'];

			if($model->validate()) 
			{
				$sql = 'ALTER TABLE '.YumProfile::model()->tableName().' ADD `'.$model->varname.'` ';
				$sql .= $model->field_type;
				if ($model->field_type!='TEXT'&&$model->field_type!='DATE')
					$sql .= '('.$model->field_size.')';
				$sql .= ' NOT NULL ';
				if ($model->default)
					$sql .= " DEFAULT '".$model->default."'";
				else
					$sql .= (($model->field_type=='TEXT'||$model->field_type=='VARCHAR')?" DEFAULT ''":" DEFAULT 0");

				$model->dbConnection->createCommand($sql)->execute();
				$model->save();
				$this->redirect(array('view','id'=>$model->id));
			}
		}

		$this->render('create',array(
			'model'=>$model,
		));
	}

	public function actionUpdate()
	{
		$this->layout = YumWebModule::yum()->adminLayout;
		$model=$this->loadModel();
		if(isset($_POST['YumProfileField']))
		{
			$model->attributes=$_POST['YumProfileField'];
			
			// ALTER TABLE `test` CHANGE `profiles` `field` INT( 10 ) NOT NULL 
			// ALTER TABLE `test` CHANGE `profiles` `description` INT( 1 ) NOT NULL DEFAULT '0'
			if($model->save())
				$this->redirect(array('view','id'=>$model->id));
		}

		$this->render('update',array(
			'model'=>$model,
		));
	}

	public function actionDelete()
	{
		$this->layout = YumWebModule::yum()->adminLayout;
		if(Yii::app()->request->isPostRequest)
		{
			// we only allow deletion via POST request
			$model = $this->loadModel();
			$sql = 'ALTER TABLE '.YumProfile::model()->tableName().' DROP `'.$model->varname.'`';
			if ($model->dbConnection->createCommand($sql)->execute()) {
				$model->delete();
			}

			if(!isset($_POST['ajax']))
				$this->redirect(array('index'));
		}
		else
			throw new CHttpException(400,'Invalid request. Please do not repeat this request again.');
	}

	public function actionIndex()
	{
		$this->layout = YumWebModule::yum()->adminLayout;
		$dataProvider=new CActiveDataProvider('YumProfileField', array(
			'pagination'=>array(
				'pageSize'=>self::PAGE_SIZE,
			),
			'sort'=>array(
				'defaultOrder'=>'position',
			),
		));

		$this->render('index',array(
			'dataProvider'=>$dataProvider,
		));
	}

	public function actionAdmin()
	{
		$this->layout = YumWebModule::yum()->adminLayout;
		$dataProvider=new CActiveDataProvider('YumProfileField', array(
			'pagination'=>array(
				'pageSize'=>self::PAGE_SIZE,
			),
			'sort'=>array(
				'defaultOrder'=>'position',
			),
		));

		$this->render('admin',array(
			'dataProvider'=>$dataProvider,
		));
	}

	/**
	 * @return YumProfileField
	 */
	public function loadModel()
	{
		if($this->_model===null)
		{
			if(isset($_GET['id']))
				$this->_model=YumProfileField::model()->findbyPk($_GET['id']);
			if($this->_model===null)
				throw new CHttpException(404,'The requested page does not exist.');
		}
		return $this->_model;
	}
}
