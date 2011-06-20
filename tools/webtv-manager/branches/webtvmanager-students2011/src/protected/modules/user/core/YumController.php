<?php
/**
 * Base controller class
 * @author tomasz.suchanek
 * @since 0.6
 * @package Yum.core
 *
 */
abstract class YumController extends CController
{
	/**
	 * @var array
	 */
	public $breadcrumbs;
	
	/**
	 * @var array
	 */
	public $menu;
	
	/**
	 * @var string
	 */
	public $title='Change me!';
 	
	/**
	 * Filters aplied to all Yum controllers
	 * @return array
	 */
	public function filters()
	{
		return array(
			'accessControl',
		);
	}	
	
}
?>
