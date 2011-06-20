<?php

/**
 * WebStreamSendForm class.
 * WebStreamSendForm is the data structure for keeping
 * webstream form data when sending new webstream.
 */
class WebStreamSendForm extends CFormModel
{
	public $Url;

	/**
	 * Declares the validation rules.
	 * The rules state that username and password are required,
	 * and password needs to be authenticated.
	 */
	public function rules()
	{
		return array(
			// username and password are required
			array('Url', 'required'),
			array('Url', 'length', 'max'=>255),
		);
	}

	/**
	 * Declares attribute labels.
	 */
	public function attributeLabels()
	{
		return array(
			'Url'=>'Url',
		);
	}
	
	protected function beforeValidate()
	{
		if(parent::beforeValidate())
		{
			if($this->Url){
				$this->Url = trim($this->Url);
			}

			return true;
		}
		return false;
	}

}
