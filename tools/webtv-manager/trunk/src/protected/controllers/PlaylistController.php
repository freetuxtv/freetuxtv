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
		$conditions .= " StreamStatusCode NOT IN (SELECT Code FROM wtvmT_StreamStatus WHERE Searchable=0) "; // Remove forbidden link
		$conditions .= " AND TypeStream NOT IN (5) "; // Remove WebLink

		if(isset($_GET['name'])){
			$conditions .= " AND Name LIKE :WebStream";
			$params[':WebStream'] = '%'.$_GET['name'].'%';
		}
		if(isset($_GET['type'])){
			$typeId = -1;
			if($_GET['type'] != ""){
				if(is_numeric($_GET['type'])){
					$typeId = $_GET['type'];
				}else{
					if($_GET['type'] != "all"){
						$typeId = WebStream::getPlaylistTypeStreamByName($_GET['type']);
					}
				}
			}
			if($typeId > 0 ){
				$conditions .= " AND TypeStream=:WebStreamType";
				$params[':WebStreamType'] = $typeId;
			}
		}
		if(isset($_GET['status'])){
			if($_GET['status'] != ""){
				$conditions .= " AND StreamStatusCode=:WebStreamStatus";
				$params[':WebStreamStatus'] = $_GET['status'];
			}
		}else{
			$conditions .= " AND StreamStatusCode=:WebStreamStatus";
			$params[':WebStreamStatus'] = WebStream::WEBSTREAM_STATUS_WORKING;
		}
		if(isset($_GET['lngstrem'])){
			if($_GET['lngstrem'] != ""){
				if($_GET['lngstrem'] != "all"){
					$conditions .= " AND LangStream=:WebStreamLang";
					$params[':WebStreamLang'] = $_GET['lngstrem'];
				}
			}
		}
		if(isset($_GET['lngcode'])){
			if($_GET['lngcode'] != ""){
				if($_GET['lngcode'] != "all"){
					$conditions .= " AND LangCode=:WebLangCode";
					$params[':WebLangCode'] = $_GET['lngcode'];
				}
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
