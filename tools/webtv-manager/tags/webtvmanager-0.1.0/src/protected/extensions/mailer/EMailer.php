<?php
/**
 * EMailer class file.
 *
 * @author MetaYii
 * @version 2.2
 * @link http://www.yiiframework.com/
 * @copyright Copyright &copy; 2009 MetaYii
 *
 * Copyright (C) 2009 MetaYii.
 *
 * 	This program is free software: you can redistribute it and/or modify
 * 	it under the terms of the GNU Lesser General Public License as published by
 * 	the Free Software Foundation, either version 2.1 of the License, or
 * 	(at your option) any later version.
 *
 * 	This program is distributed in the hope that it will be useful,
 * 	but WITHOUT ANY WARRANTY; without even the implied warranty of
 * 	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * 	GNU Lesser General Public License for more details.
 *
 * 	You should have received a copy of the GNU Lesser General Public License
 * 	along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 * For third party licenses and copyrights, please see phpmailer/LICENSE
 *
 */

/**
 * Include the the PHPMailer class.
 */
require_once(dirname(__FILE__).DIRECTORY_SEPARATOR.'phpmailer'.DIRECTORY_SEPARATOR.'class.phpmailer.php');

/**
 * EMailer is a simple wrapper for the PHPMailer library.
 * @see http://phpmailer.codeworxtech.com/index.php?pg=phpmailer
 *
 * @author MetaYii
 * @package application.extensions.emailer 
 * @since 1.0
 */
class EMailer
{
   //***************************************************************************
   // Configuration
   //***************************************************************************

   /**
    * The path to the directory where the view for getView is stored. Must not
    * have ending dot.
    *
    * @var string
    */
   protected $pathViews = 'application.views.email';

   /**
    * The path to the directory where the layout for getView is stored. Must
    * not have ending dot.
    *
    * @var string
    */
   protected $pathLayouts = 'application.views.email.layouts';

   //***************************************************************************
   // Private properties
   //***************************************************************************

   /**
    * The internal PHPMailer object.
    *
    * @var object PHPMailer
    */
   private $_myMailer;

   //***************************************************************************
   // Initialization
   //***************************************************************************

   /**
    * Init method for the application component mode.
    */
   public function init() {}

   /**
    * Constructor. Here the instance of PHPMailer is created.
    */
	public function __construct()
	{
		$this->_myMailer = new PHPMailer();
	}

   //***************************************************************************
   // Setters and getters
   //***************************************************************************

   /**
    * Setter
    *
    * @param string $value pathLayouts
    */
   public function setPathLayouts($value)
   {
      if (!is_string($value) && !preg_match("/[a-z0-9\.]/i"))
         throw new CException(Yii::t('EMailer', 'pathLayouts must be a Yii alias path'));
      $this->pathLayouts = $value;
   }

   /**
    * Getter
    *
    * @return string pathLayouts
    */
   public function getPathLayouts()
   {
      return $this->pathLayouts;
   }

   /**
    * Setter
    *
    * @param string $value pathViews
    */
   public function setPathViews($value)
   {
      if (!is_string($value) && !preg_match("/[a-z0-9\.]/i"))
         throw new CException(Yii::t('EMailer', 'pathViews must be a Yii alias path'));
      $this->pathViews = $value;
   }

   /**
    * Getter
    *
    * @return string pathViews
    */
   public function getPathViews()
   {
      return $this->pathViews;
   }

   //***************************************************************************
   // Magic
   //***************************************************************************

   /**
    * Call a PHPMailer function
    *
    * @param string $method the method to call
    * @param array $params the parameters
    * @return mixed
    */
	public function __call($method, $params)
	{
		if (is_object($this->_myMailer) && get_class($this->_myMailer)==='PHPMailer') return call_user_func_array(array($this->_myMailer, $method), $params);
		else throw new CException(Yii::t('EMailer', 'Can not call a method of a non existent object'));
	}

   /**
    * Setter
    *
    * @param string $name the property name
    * @param string $value the property value
    */
	public function __set($name, $value)
	{
	   if (is_object($this->_myMailer) && get_class($this->_myMailer)==='PHPMailer') $this->_myMailer->$name = $value;
	   else throw new CException(Yii::t('EMailer', 'Can not set a property of a non existent object'));
	}

   /**
    * Getter
    *
    * @param string $name
    * @return mixed
    */
	public function __get($name)
	{
	   if (is_object($this->_myMailer) && get_class($this->_myMailer)==='PHPMailer') return $this->_myMailer->$name;
	   else throw new CException(Yii::t('EMailer', 'Can not access a property of a non existent object'));
	}

	/**
	 * Cleanup work before serializing.
	 * This is a PHP defined magic method.
	 * @return array the names of instance-variables to serialize.
	 */
	public function __sleep()
	{
	}

	/**
	 * This method will be automatically called when unserialization happens.
	 * This is a PHP defined magic method.
	 */
	public function __wakeup()
	{
	}

   //***************************************************************************
   // Utilities
   //***************************************************************************

   /**
    * Displays an e-mail in preview mode. 
    *
    * @param string $view the class
    * @param array $vars
    * @param string $layout
    */
   public function getView($view, $vars = array(), $layout = null)
   {
      $body = Yii::app()->controller->renderPartial($this->pathViews.'.'.$view, array_merge($vars, array('content'=>$this->_myMailer)), true);
      if ($layout === null) {
         $this->_myMailer->Body = $body;
      }
      else {
         $this->_myMailer->Body = Yii::app()->controller->renderPartial($this->pathLayouts.'.'.$layout, array('content'=>$body), true);
      }
   }
}