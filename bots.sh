#!/bin/bash

let "max = 10"
let "length = 0"
let "pas = 1"

while [ $length -lt $max ]; do
	./sock -p 2006 -n purple 192.168.130.114 &
	$length += 1
done  
