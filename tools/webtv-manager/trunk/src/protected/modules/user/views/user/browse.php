<?php
$this->title = Yum::t('Browse users');
$this->breadcrumbs=array(Yum::t('Browse'));

Yum::register('js/tooltip.min.js');
Yum::register('css/yum.css'); 


?>
<div class="search_options">

<?php echo CHtml::beginForm(); ?>

<div style="float: left;">
<?php
echo CHtml::label(Yum::t('Search for username'), 'search_username') . '<br />';
echo CHtml::textField('search_username',
		$search_username, array(
			'submit' => array('//user/user/browse')));
?>

</div>

<?php
echo CHtml::label(Yum::t('Having'), 'search_role') . '<br />';
echo CHtml::dropDownList('search_role', $role,
		CHtml::listData(YumRole::model()->searchable()->findAll(), 'id', 'description'), array(
			'submit' => array('//user/user/browse'),
			'empty' => ' - ',
			));
echo CHtml::endForm();

$this->widget('zii.widgets.CListView', array(
    'dataProvider'=>$dataProvider,
    'itemView'=>'_view', 
    'sortableAttributes'=>array(
        'username',
        'lastvisit',
    ),
));

?>

<div style="clear: both;"> </div>


