<?php

/**
 * WebStreamImportForm class.
 * WebStreamImportForm is the data structure for keeping
 * webstream form data when importing webstream list.
 */
class WebStreamImportForm extends CFormModel
{
	public $UrlWSLFEData;

	/**
	 * Declares the validation rules.
	 * The rules state that username and password are required,
	 * and password needs to be authenticated.
	 */
	public function rules()
	{
		return array(
			array('UrlWSLFEData', 'required'),
			array('UrlWSLFEData', 'length', 'max' => 255),
		);
	}

	/**
	 * Declares attribute labels.
	 */
	public function attributeLabels()
	{
		return array(
			'UrlWSLFEData'=>'Url to WSLFE XML File',
		);
	}
	
	protected function beforeValidate()
	{
		if(parent::beforeValidate())
		{
			return true;
		}
		return false;
	}

}
