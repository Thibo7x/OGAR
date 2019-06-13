#!/bin/bash

read -r -p 'Nombre de bots jaune : ' max_yellow
read -r -p 'IP serveur 192.168.130.XXX : ' Filler
read -r -p 'Port de connection du serveur (2006 ou 1443) : ' Port
let "length = 0"


while [ $length -lt $max_yellow ]; do
	./sock -p $Port -n yellow 192.168.130.$Filler &
	let "length++"
done

	./sock -p $Port -n blue 192.168.130.$Filler &
	./spectacle -s -p $Port -n blue 192.168.130.$Filler
