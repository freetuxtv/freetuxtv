<?php

class WebStreamTest extends WebTestCase
{
	public $fixtures=array(
		'webStreams'=>'WebStream',
	);

	public function testShow()
	{
		$this->open('?r=webStream/view&id=1');
	}

	public function testCreate()
	{
		$this->open('?r=webStream/create');
	}

	public function testUpdate()
	{
		$this->open('?r=webStream/update&id=1');
	}

	public function testDelete()
	{
		$this->open('?r=webStream/view&id=1');
	}

	public function testList()
	{
		$this->open('?r=webStream/index');
	}

	public function testAdmin()
	{
		$this->open('?r=webStream/admin');
	}
}
