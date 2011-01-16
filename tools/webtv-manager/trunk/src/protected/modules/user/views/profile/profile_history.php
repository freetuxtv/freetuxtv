<?php
	printf('<h2>%s</h2>', Yii::t('UserModule.user', 'Profile history'));

	$cmp_attributes = YumProfileField::model()->findAll();

	if(!is_array($model->profile))
		$model->profile = array($model->profile);

	$i = 0;
	printf ('<table><tr><th>%s</th><th>%s</th><th>%s</th></tr>',
			Yii::t('UserModule.user', 'Profile number'),
			Yii::t('UserModule.user', 'Date'),
			Yii::t('UserModule.user', 'Changes'));

foreach($model->profile as $profile) {
	$data = $model->profile[$i]->compare(
			isset($model->profile[$i + 1]) 
				? $model->profile[$i + 1] : $model->profile[$i]);
		$i++;
		printf ('<tr><td> %s </td> <td> %s </td> <td> ',
				$profile->profile_id,
				$profile->timestamp);

		if(count($data) > 2) {
			printf('<table><tr><th>%s</th><th>%s</th><th>%s</th></tr>', 
					Yii::t('UserModule.user', 'Field'),
					Yii::t('UserModule.user', 'Old value'),
					Yii::t('UserModule.user', 'New value')) ;
			foreach($cmp_attributes as $field) {
				if(isset($data[$field->varname])) {
					printf('<tr> <td> %s </td> <td> %s </td> <td> %s <td> </tr>',
							Yii::t('app', $field->varname),
							$data[$field->varname]['new'],
							$data[$field->varname]['old']); 
				}
			}
		} else {
			printf('<td> <em> %s </em> </td> </tr>', Yii::t('UserModule.user', 'No profile changes were made')); 
break;
		}

		printf('</table></td></tr>');
	}
echo '</table>';
echo '<br />';
?>
