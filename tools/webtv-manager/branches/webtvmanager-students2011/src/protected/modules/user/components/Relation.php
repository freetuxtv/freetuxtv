<?php
/*
 The Relation widget is used in forms, where the User can choose
 between a selection of model elements, that this models belongs to.

 It is able to handle BELONGS_TO, HAS_ONE and MANY_MANY Relations. The Relation 
type is detected automatically from the Model 'relations()' section. 

 The Widget has different styles in which it can render the possible choices.
 Use the 'style' option to set the appropriate style.
 
 The following example shows how to use Relation with a minimal config, 
 assuming we have a Model "Post" and "User", where one User belongs 
 to a Post:
  
  <pre>
  $this->widget('application.components.Relation', array(
   'model' => 'Post',
   'relation' => 'user'
   'fields' => 'username' // show the field "username" of the parent element
  ));
  </pre>
  
  Results in a drop down list in which the user can choose between
  all available Users in the Database. The shown field of the
  Table "User" is "username" in this example. 

	You can choose the Style of your Widget in the 'style' option.
	Note that a Many_Many Relation always gets rendered as a Listbox,
	since you can select multiple Elements.
 
  'fields' can be an array or an string.
  If you pass an array to 'fields', the Widget will display every field in
  this array. If you want to show further sub-relations, separate the values
  with '.', for example: 'fields' => 'array('parent.grandparent.description')
 
  Optional Parameters:

  You can use 'field' => 'post_userid' if the field in the model
  that represents the foreign model is called different than in the
  relation
 
  Use 'relatedPk' => 'id_of_user' if the primary Key of the Foreign
  Model differs from the one given in the relation.
 
  Normally you shouldn´t use this fields cause the Widget get the relations
  automatically from the relation.
   
  Use 'allowEmpty' to let the user be able to choose no parent. If you 
  set this to a string, this string will be displayed with the available
	choices.
 
  With 'showAddButton' => 'false' you can disable the 'create new Foreignkey'
  Button generated beside the Selectbox.
 
  Define the AddButtonString with 'addButtonString' => 'Add...'. This string
  is set default to '+'
 
  When using the '+' button you most likely want to return to where you came.
  To accomplish this, we pass a 'returnTo' parameter by $_GET.
  The Controller can send the user back to where he came from this way:
 
   <pre>
 	if($model->save())
 		if(isset($_GET['returnTo'])) 
 			$this->redirect(array(urldecode($_GET['returnTo'])));
 	</pre>
 			
  Using the 'style' option we can configure how our Widget gets rendered.
  The following styles are available:
  Selectbox (default), Listbox, Checkbox and in MANY_MANY relations 'twopane'
	The style is case insensitive so one can use dropdownlist or dropDownList.
 
  Use the option 'createAction' if the action to add additional foreign Model
  options differs from 'create'.
 
   With 'parentObjects' you can limit the Parent Elements that are being shown.
  It takes an array of elements that could be returned from an scope or
  an SQL Query.
 
  The parentObjects can be grouped, for example,  with 
  'groupParentsBy' => 'city'
 
   Use the option 'htmlOptions' to pass any html Options to the 
  Selectbox/Listbox form element.
 
  Full Example:
  <pre>
  $this->widget('application.components.Relation', array(
   'model' => 'Post',
   'field' => 'Userid',
   'style' => 'ListBox',
   'parentObjects' => Parentmodel::model()->findAll('userid = 17'),
   'groupParentsBy' => 'city',
   'relation' => 'user',
   'relatedPk' => 'id_of_user',
   'fields' => array( 'username', 'username.group.groupid' ),
   'delimiter' => ' -> ', // default: ' | '
   'returnTo' => 'model/create',
   'addButtonLink' => 'othercontroller/otheraction', // default: ''
   'showAddButton' => 'click here to add a new User', // default: ''
   'htmlOptions' => array('style' => 'width: 100px;')
  ));
  </pre>
  
 
  @author Herbert Maschke <thyseus@gmail.com>
  @version 1.0rc2
  @since 1.1
 */

class Relation extends CWidget
{
	// this Variable holds an instance of the Object
	protected $_model;

	// this Variable holds an instance of the related Object
	protected $_relatedModel;

	// draw the relation of which model?	
	public $model;

	// which relation should be rendered?
	public $relation;

	public $field;

	// the Primary Key of the foreign Model
	public $relatedPk;

	// a field or an array of fields that determine which field values
  // should be rendered in the selection
	public $fields;

	// if this is set, the User is able to select no related model
  // if this is set to a string, this string will be presented
	public $allowEmpty = 0;

	// disable this to hide the Add Button
	// set this to a string to set the String to be displayed
	public $showAddButton = true;
	public $addButtonLink = '';
	// Set this to false to generate a Link rather than a LinkButton
	// This is useful when Javascript is not available
	public $useLinkButton = true;

	// use this to set the link where the user should return to after
  // clicking the add Button
	public $returnLink;

	// How should a data row be rendered. {id} will be replaced by the id of
	// the model. You can also insert every field that is available in the
	// parent object.
	// Use {fields} to display all fields delimited by $this->delimiter
	// Use {func0} to {funcX} to evaluate user-contributed functions with the
	// $functions array. Example:
	//
	//  'functions' => array( "CHtml::checkBoxList('parent{id}', '',
	//    CHtml::listData(Othermodel::model()->findAll(), 'id', 'title'));",),
  // 'template' => '#{id} : {fields} ({title}) Allowed other Models: {func0}',
	public $template = '{fields}';

	// User-Contributed functions to be evaluated in template
	public $functions = array();

	// how should multiple fields be delimited
	public $delimiter = " | ";

  // style of the selection Widget
	public $style = "dropDownList";
	public $htmlOptions = array();
	public $parentObjects = 0;
	public $orderParentsBy = 0;
	public $groupParentsBy = 0;

	// override this for complicated MANY_MANY relations:
	public $manyManyTable = '';
	public $manyManyTableLeft = '';
	public $manyManyTableRight = '';

	public function init()
	{
		if(!is_object($this->model)) 
		{
			if(!$this->_model = new $this->model) 
				throw new CException(
						Yii::t('yii','Relation widget is not able to instantiate the given Model'));
		} 
		else 
		{
			$this->_model = $this->model;
		}

		// Instantiate Model and related Model
		foreach($this->_model->relations() as $key => $value) 
		{
			if(strcmp($this->relation,$key) == 0) 
			{
				// $key = Name of the Relation
				// $value[0] = Type of the Relation
				// $value[1] = Related Model
				// $value[2] = Related Field or Many_Many Table
				switch($value[0]) 
				{
					case 'CBelongsToRelation':
					case 'CHasOneRelation':
						$this->_relatedModel = new $value[1];
						if(!isset($this->field)) 
						{
							$this->field = $value[2];
						} 
						break;
					case 'CManyManyRelation':
						preg_match_all('/^.*\(/', $value[2], $matches);
						$this->manyManyTable = substr($matches[0][0], 0, strlen($matches[0][0]) -1);
						preg_match_all('/\(.*,/', $value[2], $matches);
						$this->manyManyTableLeft = substr($matches[0][0], 1, strlen($matches[0][0]) - 2);
						preg_match_all('/,.*\)/', $value[2], $matches);
						$this->manyManyTableRight = substr($matches[0][0], 2, strlen($matches[0][0]) - 3);

						$this->_relatedModel = new $value[1];
						break;
				}
			}
		}				

		if(!is_object($this->_relatedModel))	
			throw new CException(
					Yii::t('yii','Relation widget cannot find the given Relation('.$this->relation.')'));

					if(!isset($this->relatedPk) || $this->relatedPk == "") 
					{
					$this->relatedPk = $this->_relatedModel->tableSchema->primaryKey;
		}

		if(!isset($this->fields) || $this->fields == "" || $this->fields == array())
			throw new CException(Yii::t('yii','Widget "Relation" has been run without fields Option(string or array)'));
	}

	// Check if model-value contains '.' and generate -> directives:
	public function getModelData($model, $field) 
	{
		if(strstr($field, '.')) 
		{
			$data = explode('.', $field);
			$value = $model->getRelated($data[0])->$data[1];
		} else	
			$value = $model->$field;

		return $value;
	}

	/**
	* This function fetches all needed data of the related Object and returns them
	* in an array that is prepared for use in ListData.
	*/
	public function getRelatedData() 
	{
		/* At first we determine, if we want to display all parent Objects, or
		 * if the User supplied an list of Objects */
		if(is_object($this->parentObjects)) // a single Element
		{
 			$parentobjects = array($this->parentObjects);
		}	
		else if(is_array($this->parentObjects)) // Only show this elements
		{
			$parentobjects = $this->parentObjects;
		} 
		else // Show all Parent elements
		{ 
			$parentobjects = CActiveRecord::model(get_class($this->_relatedModel))->findAll();
		} 

		if($this->allowEmpty)
			if(is_string($this->allowEmpty))
				$dataArray[0] = $this->allowEmpty;
			else
				$dataArray[0] = Yii::t('app', 'None');

		foreach($parentobjects as $obj)	
		{
			if(!is_array($this->fields))
				$this->fields = array($this->fields);

			$fields = '';
			foreach($this->fields as $field)
			{
				$rule = sprintf('{%s}',$field);
				$rules[$rule] = $obj->$field;
				$fields .= $this->getModelData($obj, $field);
				if(count($this->fields) >1)  
					$fields .= $this->delimiter;
			}

			$defaultrules = array(
					'{fields}' => $fields,
					'{id}' => $obj->id);

			// Look for user-contributed functions and evaluate them
			if($this->functions != array()) 
			{
				foreach($this->functions as $key => $function) 
				{
					$funcrules[sprintf('{func%d}', $key)] = CComponent::evaluateExpression(
							strtr($function, $defaultrules));
				}
			}

			// Merge the evaluated rules, if exist
			if(isset($funcrules))
				$rules = array_merge($rules, $funcrules);

			// Merge the default rules into our ruleset
			$rules = array_merge($rules, $defaultrules);

			// Apply the rules to the template
			$value = strtr($this->template, $rules);

			if($this->groupParentsBy != '') 
			{
				$dataArray[$obj->{$this->groupParentsBy}][$obj->{$this->relatedPk}] = $value;
			}
			else 
			{
				$dataArray[$obj->{$this->relatedPk}] = $value;
			}	
		}

		if(!isset($dataArray) || !is_array($dataArray))
			$dataArray = array();

		return $dataArray;
	}


	/**
	 * Retrieves the Assigned Objects of the MANY_MANY related Table
	 */
	public function getAssignedObjects() 
	{
		if(!$this->_model->id)
			return array();

		$sql = sprintf("select * from %s where %s = %s",
			$this->manyManyTable,
			$this->manyManyTableLeft,
			$this->_model->{$this->_model->tableSchema->primaryKey});

		$result = Yii::app()->db->createCommand($sql)->queryAll();

		foreach($result as $foreignObject) {
			$id = $foreignObject[$this->manyManyTableRight];
			$objects[$id] = $this->_relatedModel->findByPk($id); 
		}

		return isset($objects) ? $objects : array();
	}

	/**
	 * Retrieves the not Assigned Objects of the MANY_MANY related Table
	 * This is used in the two-pane style view.
	 */
	public function getNotAssignedObjects() 
	{
		foreach($this->getRelatedData() as $key => $value) 
		{
			if(!array_key_exists($key, $this->getAssignedObjects())) 
			{
				$objects[$key] = $this->_relatedModel->findByPk($key);
			}
		}

		return $objects ? $objects : array();
	}

	/**
	 * Gets the Values of the given Object or Objects depending on the
	 * $this->fields the widget requests
	 */
	public function	getObjectValues($objects)
	{
		if(is_array($objects)) { 
			foreach($objects as $object) 
			{
				$attributeValues[$object->primaryKey] = $object->{$this->fields};
			}
		}
		else if(is_object($objects)) 
		{
			$attributeValues[$object->primaryKey] = $objects->{$this->fields};
		}

		return isset($attributeValues) ? $attributeValues : array();
	}

	/*
	 * How will the Listbox of the MANY_MANY Assignment be called? 
	 */
	public function getListBoxName($ajax = false) 
	{
		if($ajax) 
		{
			return	sprintf('%s_%s',
				get_class($this->_model),
				get_class($this->_relatedModel)
			);  
		}
		else 
		{
			return	sprintf('%s[%s]',
				get_class($this->_model),
				get_class($this->_relatedModel)
			);  
		}
	}

	public function renderBelongsToSelection() {
		if(strcasecmp($this->style, "dropDownList") == 0) 
			echo CHtml::ActiveDropDownList(
					$this->_model, 
					$this->field, 
					$this->getRelatedData(), 
					$this->htmlOptions);
		else if(strcasecmp($this->style, "listbox") == 0)
			echo CHtml::ActiveListBox(
					$this->_model, 
					$this->field, 
					$this->getRelatedData(), 
					$this->htmlOptions);
		else if(strcasecmp($this->style, "checkbox") == 0)
			echo CHtml::ActiveCheckBoxList(
					$this->_model,
					$this->field, 
					$this->getRelatedData(), 
					$this->htmlOptions);

	}

	public function renderManyManySelection() {
		if(strcasecmp($this->style, 'twopane') == 0) 
			$this->renderTwoPaneSelection();
		else if(strcasecmp($this->style, 'checkbox') == 0)
			$this->renderCheckBoxListSelection();
		else if(strcasecmp($this->style, 'dropDownList') == 0)
			$this->renderManyManyDropDownListSelection();
		else
			$this->renderOnePaneSelection();
	}


	/* 
	 * Renders one dropDownList per selectable related Element.
   * The users can add additional entries with the + and remove entries
   * with the - Button .
   */
	public function renderManyManyDropDownListSelection()
	{
		$i = 0;
		foreach($this->_relatedModel->findAll() as $obj)
		{ 
			$i++;
			$isAssigned = $this->isAssigned($obj->id);

			echo CHtml::openTag('div', array(
						'id' => 'div' . $i,
						'style' => $isAssigned ? '' : 'display:none;',
						));
			echo CHtml::dropDownList('rel-' . $obj->id . "-" . $this->getListBoxName(),
					$isAssigned ? $obj->id : 0,
					CHtml::listData(
						array_merge(
							array('0' => $this->allowEmpty),
							$this->_relatedModel->findAll()),
						$this->relatedPk,
						$this->fields
						)
					);
			echo CHtml::closeTag('div');
		}

		$jsadd = '
			i = 1;
		maxi = '.$i.';
		$(\'#add\').click(function() {
$(\'#div\' + i).show();
if(i <= maxi) ++i;
});
';

		$jssub = '
$(\'#sub\').click(function() {
if(i > 2) --i;
$(\'#div\' + i).hide();
});
';

		Yii::app()->clientScript->registerScript('addbutton', $jsadd); 
		Yii::app()->clientScript->registerScript('subbutton', $jssub); 

		echo CHtml::button('+', array('id' => 'add'));
		echo '&nbsp;';
		echo CHtml::button('-', array('id' => 'sub'));
		echo '&nbsp;';
		}

	public function isAssigned($id) 
	{
		return in_array($id, array_keys($this->getAssignedObjects()));
	}

	public static function retrieveValues($data, $field) 
	{
		$returnArray = array();

		foreach($data as $key => $value) 
		{
			if(strpos($key, 'rel') !== false)
			{
				if($value[$field] != "")
				$returnArray[] = $value[$field];
			}
		}
	
		return $returnArray;
	}


public function renderCheckBoxListSelection()
{
	$keys =	array_keys($this->getAssignedObjects());

		echo CHtml::CheckBoxList($this->getListBoxName(),
					$keys,
					$this->getRelatedData(),
				 	$this->htmlOptions);
	}


	public function renderOnePaneSelection() 
	{
		$keys =	array_keys($this->getAssignedObjects());

		echo CHtml::ListBox($this->getListBoxName(), 
					$keys,
					$this->getRelatedData(),
					array('multiple' => 'multiple'));
	}

	public function handleAjaxRequest($_POST) {
		print_r($_POST);
	}

	public function renderTwoPaneSelection() 
	{
		echo CHtml::ListBox($this->getListBoxName(),
			array(),
			$this->getObjectValues($this->getAssignedObjects()),
			array('multiple' => 'multiple'));

		$ajax =
			array(
				'type'=>'POST',
				'data'=>array('yeah'),
				'update'=>'#' . $this->getListBoxName(true),
			);

		echo CHtml::ajaxSubmitButton('<<',
			array('assign'),
			$ajax
		);

		$ajax =
			array(
				'type'=>'POST',
				'update'=>'#not_'.$this->getListBoxName(true)
			);

		echo  CHtml::ajaxSubmitButton('>>',
			array('assign','revoke'=>1),
			$ajax);//,
			//$data['revoke']); 


		echo CHtml::ListBox('not_' . $this->getListBoxName(),
			array(),
			$this->getObjectValues($this->getNotAssignedObjects()), 
			array('multiple' => 'multiple'));
	}

public function run()
{
	if($this->manyManyTable != '')
		$this->renderManyManySelection();
	else
		$this->renderBelongsToSelection();

	if($this->showAddButton !== false) 
	{
		$this->renderAddButton();
	}
}
protected function renderAddButton() 
{
	if(!isset($this->returnLink) or $this->returnLink == "")
		$this->returnLink = $this->model->tableSchema->name . "/create";

	if($this->addButtonLink != '')
		$link = $this->addButtonLink;
	else
		$link = array(
				$this->_relatedModel->tableSchema->name . "/create", 
				'returnTo' => $this->returnLink); 


	if(!$this->useLinkButton)
	{
		echo CHtml::Link(
				is_string($this->showAddButton) ? $this->showAddButton : 'New', $link 
				);  
	}
	else
	{
		echo CHtml::LinkButton(
				is_string($this->showAddButton) ? $this->showAddButton : 'New',
				array('submit' => $link));
	}

}
}

