<?php $this->beginContent('//layouts/main'); ?>
<div class="container">
	<?php
		if($this->menu){
	?>
		<div class="span-5 last">
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
		</div>
		<div class="span-19">
	<?php
		}
	?>
		<div id="content">
			<?php echo $content; ?>
		</div><!-- content -->
	<?php
		if($this->menu){
	?>
		</div>
	<?php
		}
	?>


	</div>
</div>
<?php $this->endContent(); ?>
