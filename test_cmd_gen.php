<?php
/*
 * Generates random read/write comments.
 *
 * These commands later consumed by ring buffer implementations and then
 * their results are compared.
 *
 * php test_cmd_get.php RINGBUF_SIZE NUM_OF_COMMANDS
 */
srand();
$rbuf_size = rand();
$loops = 100000;
if($argc > 1)
	$rbuf_size = $argv[1];

if($argc > 2)
	$loops = $argv[2];
echo 'Generating '.$loops.' commands for ring buffer size '.$rbuf_size."\n";
$fh = fopen('test_commands.txt', 'w');

fwrite($fh, 'i:'.$rbuf_size.":init\n");
for($i = 0; $i < $loops; $i++)
{
	if(rand(0,1) == 0)
	{    
		$characters = '0123456789abcdefghijklmnopqrstuvwxyz';
		$string = '';    
		$len = rand(0, $rbuf_size * 1.5);
		for ($p = 0; $p < $len; $p++) {
			$string .= $characters{rand(0, strlen($characters)-1)};
		}
		if($len == 0)
			$string = 'dummy';
		fwrite($fh, 'w:'.$len.':'.$string."\n");
	}
	else
	{
		fwrite($fh, 'r:'.rand(0, $rbuf_size * 1.5).":read\n");
	}
}
?>