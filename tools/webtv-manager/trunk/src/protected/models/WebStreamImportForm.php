<?php

/**
 * WebStreamImportForm class.
 * WebStreamImportForm is the data structure for keeping
 * webstream form data when importing webstream list.
 */
class WebStreamImportForm extends CFormModel
{
	public $UrlWSLFEData;
	public $OnlyNewLinks;

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
			array('OnlyNewLinks', 'type', 'type'=>'boolean'),
		);
	}

	/**
	 * Declares attribute labels.
	 */
	public function attributeLabels()
	{
		return array(
			'UrlWSLFEData'=>'URL to WSLFE XML File',
			'OnlyNewLinks'=>'Show only new links',
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
