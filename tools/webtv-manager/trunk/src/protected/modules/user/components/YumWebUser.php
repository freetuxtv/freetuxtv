<?php

Yii::import('application.modules.user.models.*');
Yii::setPathOfAlias( 'YumModule' , dirname(__FILE__) . '/../');

class YumWebUser extends CWebUser
{
	public $loginUrl=array('//user/user/login');

	public function init()
	{
		parent::init();
		$this->loginUrl=$this->loginUrl;
	}

	public function data() {
		if($model = YumUser::model()->findByPk($this->id))
			return $model;
		else
			return new YumUser();
	}

	/**
	 * Performs access check for this user.
	 * @param string the name of the role that need access check.
	 * @param array name-value pairs that would be passed to business rules associated
	 * with the tasks and roles assigned to the user.
	 * @param boolean whether to allow caching the result of access checki.
	 * This parameter has been available since version 1.0.5. When this parameter
	 * is true (default), if the access check of an operation was performed before,
	 * its result will be directly returned when calling this method to check the same operation.
	 * If this parameter is false, this method will always call {@link CAuthManager::checkAccess}
	 * to obtain the up-to-date access result. Note that this caching is effective
	 * only within the same request.
	 * @return boolean whether the operations can be performed by this user.
	 */
	public function checkAccess($operation, $params=array(), $allowCaching=true)
	{
		if(Yum::module()->useYiiCheckAccess) 
			return parent::checkAccess();

		return $this->hasRole($operation);	
	}

	/**
	 * Checks if this (non-admin) User can administrate some users
	 */
	public static function hasUsers($uid = 0)
	{
		if($uid == 0)
			$uid = Yii::app()->user->getId();

		$user = YumUser::model()->findByPk($uid);

		return isset($user->users) && $user->users !== array();
	}

	/**
	 * Checks if this (non-admin) User can administrate some users of a specific role
	 */
	public static function hasRoles($uid = 0)
	{
		if($uid == 0)
			$uid = Yii::app()->user->id;

		$user = YumUser::model()->findByPk($uid);

		$flag = false;
		if(isset($user->roles))
			foreach($user->roles as $role) 
				if (isset($role->roles) && $role->roles !== array())
					$flag = true;

		return $flag;
	}

	/**
	 * Checks if this (non-admin) User can administrate the given user
	 */
	public static function hasUser($username, $uid = 0)
	{
		if($uid == 0)
			$uid = Yii::app()->user->getId();

		// Every user can modify himself
		if($username == $uid)
			return true;

		$user = YumUser::model()->findByPk($uid);

		if(!is_array($username))
			$username = array ($username);

		if(isset($user->users)) 
			foreach($user->users as $userobj) 
			{
				if(in_array($userobj->username, $username) ||
						in_array($userobj->id, $username))
					return true;
			}
		return false;
	}

	/**
	 * Checks if this (non-admin) User can administrate the given user of a specific role
	 */
	public static function hasRoleOfUser($username, $uid = 0)
	{
		if($uid == 0)
			$uid = Yii::app()->user->getId();

		$user = YumUser::model()->findByPk($uid);

		if(!is_array($username))
			$username = array ($username);

		if(isset($user->roles)) 
			foreach($user->roles as $roleobj) {
				if(isset($roleobj->roles))
					foreach($roleobj->roles as $administerable_role) {
						if($administerable_role->users)
							foreach($administerable_role->users as $userobj) {
								if(in_array($userobj->username, $username) ||
										in_array($userobj->id, $username))
									return true;
							}
					}
			}

		return false;
	}



	/**
	 * Checks if the user has the given Role
	 * @mixed Role string or array of strings that should be checked
	 * @int (optional) id of the user that should be checked 
	 * @return bool Return value tells if the User has access or hasn't access.
	 */
	public static function hasRole($role, $uid = 0) {
		if($uid == 0)
			$uid = Yii::app()->user->getId();

		if(!is_array($role))
			$role = array ($role);

		if($user = YumUser::model()->findByPk($uid)) {

			// Check if a user has a active membership and, if so, add this
			// to the roles
			$roles = array_merge($user->roles, $user->getActiveMemberships());

			if(isset($roles)) 
				foreach($roles as $roleobj) {
					if(in_array($roleobj->title, $role) ||
							in_array($roleobj->id, $role))
						return true;
				}

		}

		return false;
	}

	/**
	 * Return admin status.
	 * @return boolean
	 */
	public function isAdmin() {
		if($this->isGuest)
			return false;
		else {
			if(YumUser::model()->active()->superuser()->findbyPk(Yii::app()->user->id))
				return true;
			else
				return false;
		}
	}
}
?>
