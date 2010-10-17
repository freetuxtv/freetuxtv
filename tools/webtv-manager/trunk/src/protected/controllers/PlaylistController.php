<?php

class PlaylistController extends Controller
{
	/**
	 * @var string the default layout for the views. Defaults to '//layouts/playlistm3u', meaning
	 * using two-column layout. See 'protected/views/layouts/playlistm3u.php'.
	 */
	public $layout='//layouts/playlistm3u';

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
				'actions'=>array('index'),
				'users'=>array('*'),
			),
			array('deny',  // deny all users
				'users'=>array('*'),
			),
		);
	}

	/**
	 * Generate playlist from the search models.
	 */
	public function actionIndex()
	{
		$conditions = "";
		$params = array();

		// Security
		$conditions .= " StreamStatusCode NOT IN (SELECT * FROM wtvmT_StreamStatus WHERE Searchable=0) "; // Remove forbidden link
		$conditions .= " AND TypeStream NOT IN (5) "; // Remove WebLink

		if(isset($_GET['name'])){
			$conditions = "Name LIKE :WebStream";
			$params[':WebStream'] = '%'.$_GET['name'].'%';
		}
		if(isset($_GET['type'])){
			if($_GET['type'] != ""){
				$conditions .= " AND TypeStream=:WebStreamType";
				$params[':WebStreamType'] = $_GET['type'];
			}
		}
		if(isset($_GET['status'])){
			if($_GET['status'] != ""){
				$conditions .= " AND StreamStatusCode=:WebStreamStatus";
				$params[':WebStreamStatus'] = $_GET['status'];
			}
		}
		if(isset($_GET['lng'])){
			if($_GET['lng'] != ""){
				$conditions .= " AND LangCode=:WebStreamLang";
				$params[':WebStreamLang'] = $_GET['lng'];
			}
		}

		$criteria=new CDbCriteria;
		$criteria->condition=$conditions;
		$criteria->params=$params;
		$criteria->order='Name';

		$data=WebStream::model()->findAll($criteria);

		$this->render('index',array(
			'data'=>$data,
		));
	}
}
