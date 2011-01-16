<?php

class YumStatisticsController extends YumController {
	const PAGE_SIZE=10;

	public function accessRules()
	{
		return array(
				array('allow',
					'actions'=>array('index'),
					'users'=>array(Yii::app()->user->name ),
					'expression' => 'Yii::app()->user->isAdmin()'
					),
				array('deny', 'users'=>array('*')));
	}

	public function actions()
	{
		return array('index');
	}

	public function actionIndex()
	{
		$this->render('statistics', array(
					'active_users' => YumUser::model()->count('status = 1'),
					'inactive_users' => YumUser::model()->count('status = 0'),
					'banned_users' => YumUser::model()->count('status = -1'),
					'admin_users' => YumUser::model()->count('superuser = 1'),
					'roles' => YumRole::model()->count(),
					'profiles' => YumProfile::model()->count(),
					'profile_fields' => YumProfileField::model()->count(),
					'profile_field_groups' => YumProfileFieldsGroup::model()->count(),
					'profile_views' => YumProfileVisit::model()->count(),
					'messages' => YumMessage::model()->count(),
					'logins_today' => $this->loginsToday(),
					));
	}

	public function getStartOfDay($time = 0)
	{
		if($time == 0)
			$time = time();
		$hours = date("G", $time);
		$minutes = date("i", $time);
		$seconds = date("s", $time);

		$temp = $time;
		$temp -= ($hours * 3600);
		$temp -= ($minutes * 60);
		$temp -= $seconds;

		$today = $temp;
		$first = $today;

		return $first;
	}


	public function loginsToday()
	{
		$day = $this->getStartOfDay(time());
		return YumUser::model()->count('lastvisit > :begin and lastvisit < :end', array(
					':begin' => $day,
					':end' => $day + 86400));
	}

}
