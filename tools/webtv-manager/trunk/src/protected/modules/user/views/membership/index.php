<?php
$this->breadcrumbs = array(
	Yum::t('Membership'),
	Yum::t('My memberships'),
);

$this->title = Yum::t('My memberships');

?>


<?php $this->widget('zii.widgets.CListView', array(
	'dataProvider'=>$dataProvider,
	'itemView'=>'_view',
)); ?>

<div style="clear: both;"></div>
