<?php
$this->breadcrumbs=array(
		'Memberships'=>array('index'),
		Yii::t('app', 'Update'),
		);

?>

<h1> <?php echo Yum::t('Membership'); ?> </h1>
<?php
$this->widget('zii.widgets.CDetailView', array(
			'data'=>$model,
			'attributes'=>array(
				'user.username',
						'role.price',
		'payment.title',

				),
			)); 

echo 'Ordered at: ' .date("Y. m. d G:i:s", $model->order_date) . '<br />';
echo 'Ends at: '. date("Y. m. d G:i:s", $model->end_date) . '<br />';
echo 'Payment Date: '. date("Y. m. d G:i:s", $model->payment_date) . '<br />';

echo CHtml::beginForm(array('//user/membership/update'));
echo CHtml::hiddenField('YumMembership[id]', $model->id);
echo CHtml::submitButton(Yum::t('Set payment date to today'));
echo CHtml::endForm();
?>
