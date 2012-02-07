<?php

class WebStreamExportController extends Controller
{
	/**
	 * @var string the default layout for the views. Defaults to '//layouts/webstreamexport', meaning
	 * using two-column layout. See 'protected/views/layouts/webstreamexport.php'.
	 */
	public $layout='//layouts/webstreamexport';

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
		if(isset($_GET['lng'])){
			if($_GET['lng'] != ""){
				if($_GET['lng'] == "none"){
					$conditions .= " AND LangCode IS NULL";
				}elseif($_GET['lng'] != "all"){
					$conditions .= " AND LangCode=:WebStreamLang";
					$params[':WebStreamLang'] = $_GET['lng'];
				}
			}
		}
		if(isset($_GET['country'])){
			if($_GET['country'] != ""){
				if($_GET['country'] == "none"){
					$conditions .= " AND CountryCode IS NULL";
				}elseif($_GET['country'] != "all"){
					$conditions .= " AND CountryCode=:WebStreamCountryCode";
					$params[':WebStreamCountryCode'] = $_GET['country'];
				}
			}
		}
		if(isset($_GET['isp'])){
			if($_GET['isp'] != ""){
				if($_GET['isp'] != "all"){
					$conditions .= " AND RequiredISP=:WebStreamRequiredISP";
					$params[':WebStreamRequiredISP'] = $_GET['isp'];
				}
			}else{
				$conditions .= " AND RequiredISP IS NULL";
			}
		}else{
			$conditions .= " AND RequiredISP IS NULL";
		}

		$format = null;
		if(isset($_GET['format'])){
			$format = $_GET['format'];
		}

		$criteria=new CDbCriteria;
		$criteria->condition=$conditions;
		$criteria->params=$params;
		$criteria->order='Name';

		$data=WebStream::model()->findAll($criteria);

		$this->render('index',array(
			'format'=>$format,
			'listWebStream'=>$data,
		));
	}
}
