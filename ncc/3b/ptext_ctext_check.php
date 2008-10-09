<?php
$a = file_get_contents("3b_ctext");
$b = file_get_contents("3b_ptext");

$c = Array();                      
for ($i = 0; $i < strlen($a); $i++)
{
  $d = $a{$i};
  $e = $b{$i};

  $d = trim($d);
  $e = trim($e);

  if (strlen($d) > 0 && strlen($e) > 0)
  {
    if (!isset($c[$e]))   $c[$e] = $d;
    else if ($c[$e] != $d)  echo "Contradiction $e => $d or ".$c[$e]."\n";
  }
}     

ksort($c);

foreach ($c as $e => $d)
{
  $f = base_convert($d, 36, 10) - 10;
  $g = base_convert($e, 36, 10) - 10;


  echo "    [$e $g] => $d $f\n";
}

?>
