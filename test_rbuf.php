<?php
ini_set("memory_limit","2024M");
$rbuf = '';
$size = 0;
function rb_write($str, $len)
{
	global $rbuf;
	global $size;
	$free = $size - strlen($rbuf);
	if($len > $free)
		$len = $free;
	$rbuf .= substr($str,0,$len);
	return $len;
}

function rb_read($len)
{
	global $rbuf;
	global $size;
	if($len == 0)
		return array('len'=>0, 'result'=>'');
	if($len > strlen($rbuf))
		$len = strlen($rbuf);
	$result = substr($rbuf, 0, $len);
	$rbuf = substr($rbuf, $len);
	return array('len' => $len, 'result' => $result);
}


$commands = file('test_commands.txt');
$tokens = explode(':',$commands[0]);
if($tokens[0] != 'i')
	die("no init\n");
	
echo 'Ring Buffer size: '.$tokens[1]."\n";
$size = $tokens[1];
$fh = fopen('php_output.txt', 'w');
foreach($commands as $cmd)
{
	$tokens = explode(':',$cmd);
	switch($tokens[0])
	{
		case 'w':
			$tokens[2] = trim($tokens[2]);
			fwrite($fh, 'w:'.rb_write($tokens[2], $tokens[1]).':'.$tokens[2]."\n");
		break;
		case 'r':
			$r = rb_read($tokens[1]);
			fwrite($fh, 'r:'.$r['len'].':'.$r['result']."\n");
		break;
		case 'i':
		break;
		default:
			die("unknown command\n");
	}
}

?>
