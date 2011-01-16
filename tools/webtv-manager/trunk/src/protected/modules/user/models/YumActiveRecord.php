<?php
/**
 * Base class for all active records
 * @author tomasz.suchanek
 * @since 0.6
 * @package Yum.core
 *
 */
abstract class YumActiveRecord extends CActiveRecord {
	protected $_tableName;

	/**
	 * @return array
	 */
	public function behaviors() {
		return array( 'CAdvancedArBehavior' );
	}	

	/**
	 * @return CActiveRecordMetaData the meta for this AR class.
	 */	
	public function getMetaData( )
	{
		$md = parent::getMetaData( );
		if($this->getScenario()==='search')
		{
			$md->attributeDefaults  = array ();
		}

		return $md;
	}

}
?>
