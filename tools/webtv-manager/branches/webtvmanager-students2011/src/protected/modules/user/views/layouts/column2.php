<?php $this->beginContent('application.views.layouts.main'); ?>
<div class="container">
	<div class="span-19">
		<div id="content">
			<h1><?php echo $this->title; ?></h1>		
			<?php if(Yii::app()->controller->module->debug===true){
				echo CHtml::openTag('div',array('class'=>'hint'));
				echo Yii::t('UserModule.core','You are running the Yii User Management Module {version} in Debug Mode!',array('{version}'=>Yii::app()->controller->module->version));
				echo CHtml::closeTag('div');
			}?>
			<?php echo $content; ?>
		</div><!-- content -->
	</div>
	<div class="span-5 last">
		<?php if(!empty($this->menu)) : ?>
		<div id="sidebar">
		<?php
			$this->beginWidget('zii.widgets.CPortlet', array(
				'title'=>'Operations',
			));
			$this->widget('zii.widgets.CMenu', array(
				'items'=>$this->menu,
				'htmlOptions'=>array('class'=>'operations'),
			));
			$this->endWidget();
		?>
		</div><!-- sidebar -->
		<?php endif; ?>
	</div>
</div>
<?php $this->endContent(); ?>
