<?php

Yii::import('zii.widgets.CPortlet');

class AdminMenu extends CPortlet {
	public function init() {
		$this->title = sprintf('%s <br /> %s: %s',
				Yum::t('Usermenu'),
				Yum::t('Logged in as'),
				Yii::app()->user->data()->username);
		$this->contentCssClass = 'menucontent';
		return parent::init();
	}

	public function run() {
		$this->widget('YumMenu', array(
					'items' => $this->getMenuItems()
					));

		parent::run();
	}

	public function getMenuItems() {
		return array( 
				array('label'=>'Users', 'items' => array(
						array('label'=> 'Statistics', 'url'=>array('//user/statistics/index')), 
						array('label' => 'Administration', 'url' => array('//user/user/admin')),
						array('label' => 'Create new user', 'url' => array('//user/user/create')),
						array('label' => 'Browse user activities', 'url' => array('//user/activities/index')),
						array('label' => 'Generate Demo Data', 'url' => array('//user/user/generateData'), 'visible' => Yum::module()->debug),
						)
					),
				array('label'=>'Access control', 'visible' => Yum::module()->enableRoles, 'items' => array(
						array('label' => 'Roles', 'url' => array('//user/role/admin')),
						array('label' => 'Create new role', 'url' => array('//user/role/create')),
						array('label' => 'Permissions', 'url' => array('//user/permission/admin')),
						array('label' => 'Grant permission', 'url' => array('//user/permission/create')),
						array('label' => 'Actions', 'url' => array('//user/action/admin')),
						array('label' => 'Create new action', 'url' => array('//user/action/create')),
						)
					),
				array('label'=>'Profiles', 'visible' => Yum::module()->enableProfiles, 'items' => array(
						array('label' => 'Manage profiles', 'url' => array('//user/profile/admin')),
						array('label' => 'Show profile visits', 'url' => array('//user/profile/visits')),
						array('label' => 'Manage profile fields', 'url' => array('//user/fields/admin')),
						array('label' => 'Create profile field', 'url' => array('//user/fields/create')),
						array('label' => 'Manage field groups', 'url' => array('//user/fieldsgroup/admin')),
						array('label' => 'Create new field group', 'url' => array('//user/fieldsgroup/create')),
						)
					),
				array('label' => 'Messages', 'visible' => Yum::module()->messageSystem != 'None', 'items' => array ( 
							array('label' => 'Admin inbox', 'url' => array('/user/messages/index')),
							array('label' => 'Sent messages', 'url' => array('/user/messages/sent')),
							array('label' => 'Write a message', 'url' => array('/user/messages/compose')),
							array('label' => 'Send message notifier emails', 'url' => array('/user/messages/sendDigest')),
							),
						),
				array('label'=>'Module settings', 'items' => array(
							array('label' => 'User module settings', 'url' => array('//user/yumSettings/update')),
							array('label' => 'New settings profile', 'url' => array('//user/yumSettings/create')),
							array('label' => 'Module text settings', 'url' => array('//user/yumTextSettings/admin')),
							array('label' => 'Settings profiles', 'url' => array('//user/yumTextSettings/index')),
							)
						),
				array('label' => 'Misc', 'items' => array(
							array('label' => 'Change admin Password', 'url' => array('//user/user/changePassword')),
							array('label' => 'Logout', 'url' => array('//user/user/logout')),
							)
						),
				);

	}
}
?>






