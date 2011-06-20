<?php
/**
 * Helper class
 * @author tomasz.suchanek@gmail.com
 * @since 0.6
 * @package Yum.core
 *
 */
class YumMenuItemHelper
{
	
	/**
	 * Creates menu item which points to user module controller/action
	 * @param string $label
	 * @param string $route
	 * @params boolean $visible default false 
	 * @param array $params
	 * @since 0.6
	 * @return array
	 */
	protected static function users($label, $route, $visible=false,array $params=null)
	{
		$p = $params === null ? array() : $params;
		$l = Yii::t('UserModule.user', $label); 
		$url = array_merge(array(YumHelper::route("{user}/{$route}")), $p);
		return array('label'=>$l,'url'=>$url,'visible'=>$visible);
	}	
	
	/**
	 * Creates menu item: manage users
	 * @param array $params
	 * @param string $label
	 * @since 0.6
	 * @return array
	 */
	public static function manageUsers(array $params=null, $label='Manage users')
	{
		return self::users($label,'admin',Yii::app()->user->isAdmin(),$params);
	}
	
	/**
	 * Creates menu item: manage my users
	 * @param array $params
	 * @param string $label
	 * @since 0.6
	 * @return array
	 */
	public static function manageMyUsers(array $params=null, $label='Manage my users')
	{
		return self::users(
			$label,'admin',
			(!Yii::app()->user->isAdmin() && ( Yii::app()->user->hasUsers() || Yii::app()->user->hasRole('UserCreation'))),
			$params
		);
	}	

	/**
	 * Creates menu item: list users
	 * @param array $params
	 * @param string $label
	 * @since 0.6
	 * @return array
	 */
	public static function listUsers(array $params=null, $label='List users')
	{
		return self::users($label,'index',!Yii::app()->user->isAdmin(),$params);		
	}
	
	/**
	 * Creates menu item: create user
	 * @param array $params
	 * @param string $label
	 * @since 0.6
	 * @return array
	 */	
	public static function createUser(array $params=null, $label='Create user')
	{
		return self::users(
			$label,
			'create',
			Yii::app()->user->isAdmin() || Yii::app()->user->hasRole('UserCreation'),
			$params
		);
	}
	
	/**
	 * Creates menu item: update user
	 * @param array $params
	 * @param string $label
	 * @since 0.6
	 * @return array
	 */		
	public static function updateUser(array $params=null, $label='Update user')
	{
		if(!isset($params['id']))
			throw new CException(Yii::t('UserModule.core','Missing user ID.'));
		return self::users($label,'update',Yii::app()->user->isAdmin(),$params);
	}
	
	/**
	 * Creates menu item: display user
	 * @param array $params
	 * @param string $label
	 * @since 0.6
	 * @return array
	 */		
	public static function viewUser(array $params=null,$label='View user')
	{
		if(!isset($params['id']))
			throw new CException(Yii::t('UserModule.core','Missing user ID.'));
		return self::users($label,'view',Yii::app()->user->isAdmin(),$params);
	}	
	
	/**
	 * Creates menu item: edit profile
	 * @param array $params
	 * @param string $label
	 * @since 0.6
	 * @return array
	 */		
	public static function updateProfile(array $params=null,$label='Edit profile')
	{
		return self::users($label,'edit',true,$params);
	}	
	
	/**
	 * Creates menu item: change user password
	 * @param array $params
	 * @param string $label
	 * @since 0.6
	 * @return array
	 */		
	public static function changePassword(array $params=null,$label='Change password')
	{
		return self::users($label,'changepassword',true,$params);
	}	
	
	/**
	 * Creates menu item: display profile
	 * @param array $params
	 * @param string $label
	 * @since 0.6
	 * @return array
	 */		
	public static function displayProfile(array $params=null,$label='Profile')
	{
		return self::users($label,'profile',true,$params);
	}	
	
	/**
	 * Creates menu item: display logout action
	 * @param array $params
	 * @param string $label
	 * @since 0.6
	 * @return array
	 */			
	public static function logout(array $params=null,$label='Logout')
	{
		return self::users($label,'logout',!Yii::app()->user->isGuest,$params);		
	}
	
	/**
	 * Creates menu item: back to profile
	 * @param array $params
	 * @param string $label
	 * @since 0.6
	 * @return array
	 */		
	public static function backToProfile(array $params=null,$label='Back to profile')
	{
		return self::displayProfile($params,$label);
	}	
	
	/**
	 * Creates menu item which points to profiles controller/action
	 * Menu item is visible only in case when module 'profiles' is installed
	 * @param string $label
	 * @param string $route
	 * @param boolean $visible, default to false
	 * @param array $params
	 * @since 0.6
	 * @return array
	 */
	protected static function profiles($label,$route,$visible=false,array $params=null)
	{
		$p=$params===null ? array() : $params;
		$l=Yii::t('UserModule.user',$label); 
		$url=array_merge(array(YumHelper::route("{profiles}/{$route}")),$p);
		$vis=$visible && YumWebModule::yum()->hasModule('profiles');
		return array('label'=>$l,'url'=>$url,'visible'=>$vis);			
	}
	
	/**
	 * Creates menu item: manage profile fields
	 * @param array $params
	 * @param string $label
	 * @since 0.6
	 * @return array
	 */
	public static function manageFields(array $params=null,$label='Manage profile fields')
	{
		return self::profiles($label,'fields/admin',Yii::app()->user->isAdmin(),$params);
	}	
	
	/**
	 * Creates menu item: create profile field
	 * @param array $params
	 * @param string $label
	 * @since 0.6
	 * @return array
	 */
	public static function createField(array $params=null,$label='Create profile field')
	{
		return self::profiles($label,'fields/create',Yii::app()->user->isAdmin(),$params);
	}
	
	/**
	 * Creates menu item: update profile field
	 * @param array $params
	 * @param string $label
	 * @since 0.6
	 * @return array
	 */
	public static function updateField(array $params=null,$label='Edit profile field')
	{
		return self::profiles($label,'fields/update',Yii::app()->user->isAdmin(),$params);
	}

	/**
	 * Creates menu item: list profile fields
	 * @param array $params
	 * @param string $label
	 * @since 0.6
	 * @return array
	 */
	public static function listFields(array $params=null,$label='List profile field')
	{
		return self::profiles($label,'fields/index',Yii::app()->user->isAdmin(),$params);
	}	
	
	/**
	 * Creates menu item: manage profile fields groups
	 * @param array $params
	 * @param string $label
	 * @since 0.6
	 * @return array
	 */
	public static function manageFieldsGroups(array $params=null,$label='Manage profile fields groups')
	{
		return self::profiles($label,'fieldsGroup/admin',Yii::app()->user->isAdmin(),$params);
	}	
	
	/**
	 * Creates menu item: create profile fields group
	 * @param array $params
	 * @param string $label
	 * @since 0.6
	 * @return array
	 */
	public static function createFieldsGroup(array $params=null,$label='Create profile fields group')
	{
		return self::profiles($label,'fieldsGroup/create',Yii::app()->user->isAdmin(),$params);
	}
	
	/**
	 * Creates menu item: update profile fields group
	 * @param array $params
	 * @param string $label
	 * @since 0.6
	 * @return array
	 */
	public static function updateFieldsGroup(array $params=null,$label='Edit profile fields group')
	{
		return self::profiles($label,'fieldsGroup/update',Yii::app()->user->isAdmin(),$params);
	}	

	/**
	 * Creates menu item which points to messages controller/action
	 * Menu item is visible only in case module 'messages' is installed
	 * @param string $label
	 * @param string $route
	 * @param boolean $visible, defualt false
	 * @param array $params
	 * @since 0.6
	 * @return array
	 */
	protected static function messages($label,$route,$visible=false,array $params=null)
	{
		$p=$params===null ? array() : $params;
		$l=Yii::t('UserModule.user',$label); 
		$url=array_merge(array(YumHelper::route("{messages}/".$route)),$p);
		$vis=$visible && YumWebModule::yum()->hasModule('messages');
		return array('label'=>$l,'url'=>$url,'visible'=>$vis);
	}	

	/**
	 * Creates menu item: list messages
	 * @param array $params
	 * @param string $label
	 * @since 0.6
	 * @return array
	 */
	public static function listMessages(array $params=null,$label='My inbox')
	{
		return self::messages($label,'messages/index',true,$params);
	}	
	
	/**
	 * Creates menu item: back to inbox
	 * For more details see {@link listMessages}
	 * @param array $params
	 * @param string $label
	 * @since 0.6
	 * @return array
	 */
	public static function backToInbox(array $params=null,$label='Back to inbox')
	{
		return self::listMessages($params,$label);		
	}
	
	/**
	 * Creates menu item: compose new message
	 * @param array $params
	 * @param string $label
	 * @since 0.6
	 * @return array
	 */
	public static function composeMessage(array $params=null,$label='Compose new message')
	{
		return self::messages($label,'messages/compose',true,$params);
	}
	
	/**
	 * Creates menu item: view message
	 * @param array $params
	 * @param string $label
	 * @since 0.6
	 * @return array
	 */
	public static function viewMessage(array $params=null,$label='View message')
	{
		return self::messages($label,'messages/view',Yii::app()->user->isAdmin(),$params);
	}	
	
	/**
	 * Creates menu item: update message
	 * @param array $params
	 * @param string $label
	 * @since 0.6
	 * @return array
	 */
	public static function updateMessage(array $params=null,$label='Update message')
	{
		if(!isset($params['id']))
			throw new CException(Yii::t('UserModule.user','Missing message ID'));
		return self::messages($label,'messages/update',Yii::app()->user->isAdmin(),$params);
	}	

	/**
	 * Creates menu item which points to roles module controller/action
	 * Menu item is visible only in case module 'roles' is installed
	 * @param string $label
	 * @param string $route
	 * @param boolean $visible, default to false
	 * @param array $params
	 * @since 0.6
	 * @return array
	 */	
	 
	protected static function roles($label,$route,$visible=false,array $params=null)
	{
		$p=$params===null ? array() : $params;
		$l=Yii::t('UserModule.user',$label); 
		$url=array_merge(array(YumHelper::route("{roles}/".$route)),$p);
		$vis=$visible && YumWebModule::yum()->hasModule('role');
		return array('label'=>$l,'url'=>$url,'visible'=>$vis);		
	}

	/**
	 * Creates menu item: manage roles
	 * @param array $params
	 * @param string $label
	 * @since 0.6
	 * @return array
	 */
	public static function manageRoles(array $params=null,$label='Manage roles')
	{
		return self::roles($label,'role/admin',Yii::app()->user->isAdmin(),$params);
	}	
	
	/**
	 * Creates menu item: create role
	 * @param array $params
	 * @param string $label
	 * @since 0.6
	 * @return array
	 */
	public static function createRole(array $params=null,$label='Create role')
	{
		return self::roles($label,'role/create',Yii::app()->user->isAdmin(),$params);
	}
	
	/**
	 * Creates menu item: update role
	 * @param array $params
	 * @param string $label
	 * @since 0.6
	 * @return array
	 */
	public static function updateRole(array $params=null,$label='Edit this role')
	{
		if(!isset($params['id']))
			throw new CException(Yii::t('UserModule.user','Missing role ID'));		
		return self::roles($label,'role/update',Yii::app()->user->isAdmin(),$params);
	}
	
	
		
		/**
	 * Creates menu item: Delete Account
	 * @param array $params
	 * @param string $label
	 * @since 0.6
	 * @return array
	 */
		public static function deleteAccount(array $params=null,$label='Delete Account')
	{
		return self::users($label,'delete',true,$params);
	}
}
?>
