<?php
/* this is stupid, please don't actually use it */

function parse_args($args)
{
    $out = ["*rest" => []];
    foreach($args as $arg)
    {
        if(substr($arg, 0, 2) == '--')
        {
            $eqPos = strpos($arg, '=');
            if($eqPos === false)
            {
                $key = substr($arg, 2);
                $out[$key] = isset($out[$key]) ? $out[$key] : true;
            }
            else
            {
                $key = substr($arg, 2, $eqPos - 2);
                $out[$key] = substr($arg, $eqPos + 1);
            }
        }
        else if(substr($arg, 0, 1) == '-')
        {
            if(substr($arg, 2, 1) == '=')
            {
                $key = substr($arg, 1, 1);
                $out[$key] = substr($arg, 3);
            }
            else
            {
                $chars = str_split(substr($arg, 1));
                foreach($chars as $char)
                {
                    $key = $char;
                    $out[$key] = isset($out[$key]) ? $out[$key] : true;
                }
            }
        }
        else
        {
            $out["*rest"][] = $arg;
        }
    }
    return $out;
}

array_shift($argv);
print_r(parse_args($argv));
