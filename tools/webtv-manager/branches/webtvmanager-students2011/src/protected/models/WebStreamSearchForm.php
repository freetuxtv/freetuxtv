<?php

/**
 * WebStreamSearchForm class.
 * WebStreamSearchForm is the data structure for searching webstream.
 */
class WebStreamSearchForm extends CFormModel
{
	public $Name;
	public $Type;
	public $Language;
	public $Country;
	public $Status;

	/**
	 * Declares the validation rules.
	 * The rules state that username and password are required,
	 * and password needs to be authenticated.
	 */
	public function rules()
	{
		return array(
			array('Name', 'length', 'max'=>'255'),
			array('Type', 'numerical', 'integerOnly'=>true),
			array('Language', 'length', 'max'=>'2'),
			array('Country', 'length', 'max'=>'2'),
			array('Status', 'numerical', 'integerOnly'=>true),
		);
	}

	/**
	 * Declares attribute labels.
	 */
	public function attributeLabels()
	{
		return array(
			'Name'=>'Name',
			'Type'=>'Type',
			'Language'=>'Language',
			'Country'=>'Country',
			'Status'=>'Status',
		);
	}

}
