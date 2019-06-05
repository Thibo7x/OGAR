#!/bin/bash

read -r -p 'Nombre de bots: ' max
let "length = 0"


while [ $length -lt $max ]; do
	./sock -p 2005 -n yellow 192.168.130.114 &
	let "length++"
done
