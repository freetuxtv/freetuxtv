<?php

class YumMembershipController extends YumController {
	public function accessRules() {
		return array(
				array('allow', 
					'actions'=>array('index', 'order'),
					'users'=>array('@'),
					),
				array('allow', 
					'actions'=>array('admin','delete', 'update', 'view', 'orders'),
					'users'=>array('admin'),
					),
				array('deny', 
					'users'=>array('*'),
					),
				);
	}

	public function actionView()
	{
		return false;
	}

	public function actionUpdate() {
		if(isset($_GET['id']))
			$model = YumMembership::model()->findByPk($_GET['id']);


		if(isset($_POST['YumMembership'])) {
			$model = YumMembership::model()->findByPk($_POST['YumMembership']['id']); 
			$model->attributes = $_POST['YumMembership'];
			$model->payment_date = time();
			$model->end_date = $model->payment_date + ($model->role->duration * 86400);

			if($model->save()) {
				YumMessage::write($model->user, 1,
						Yum::t('Payment arrived'),
						YumTextSettings::getText('text_payment_arrived', array(
								'{payment_date}' => date(Yum::module()->dateTimeFormat, $model->payment_date),
								'{id}' => $model->id,
								)));

				$this->redirect(array('admin'));
			}
		}

		$this->render('update',array(
					'model'=>$model,
					));
	}

	public function actionOrder()
	{
		$model = new YumMembership;

		if(isset($_POST['YumMembership'])) {
			$model->attributes = $_POST['YumMembership'];
			if($model->save()) {
				$this->redirect(array('index'));
			}
		}

		$this->render('order',array( 'model'=>$model));
	}

	public function actionDelete()
	{
		if(Yii::app()->request->isPostRequest)
		{
			$this->loadModel()->delete();

			if(!isset($_GET['ajax']))
			{
				if(isset($_POST['returnUrl']))
					$this->redirect($_POST['returnUrl']); 
				else
					$this->redirect(array('admin'));
			}
		}
		else
			throw new CHttpException(400,
					Yii::t('app', 'Invalid request. Please do not repeat this request again.'));
	}

	public function actionIndex()
	{
		$dataProvider=new CActiveDataProvider('YumMembership', array(
					'criteria' => array(
						'condition' => 'user_id = '.Yii::app()->user->id),
					));

		$this->render('index',array(
					'dataProvider'=>$dataProvider,
					));
	}

	public function actionOrders()
	{
		$model=new YumMembership('search');
		$model->unsetAttributes();

		if(isset($_GET['YumMembership']))
			$model->attributes = $_GET['YumMembership'];

		$this->render('orders',array(
					'model'=>$model,
					));
	}

	public function actionAdmin() {
		$model=new YumMembership('search');
		$model->unsetAttributes();

		if(isset($_GET['YumMembership']))
			$model->attributes = $_GET['YumMembership'];

		$this->render('admin',array(
					'model'=>$model,
					));
	}

}
