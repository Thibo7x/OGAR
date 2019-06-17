#!/bin/bash
read -r -p 'Multi (2) ou Solo (1) ? : ' mode
read -r -p 'Nombre de bots jaune : ' yellow
read -r -p 'Nombre de bots bleu : ' blue
read -r -p 'Nombre de bots cyan : ' cyan
read -r -p 'Nombre de bots violet : ' purple
read -r -p 'IP serveur 192.168.130.XXX : ' Filler
read -r -p 'Port de connection du serveur (2006 ou 1443) : ' Port


for color in {"yellow","blue","cyan","purple"}
do
	let "max = $color"
	echo $max
	let "length = 0"
	while [ $length -lt $max ]; do
		./sock -p $Port -n $color 192.168.130.$Filler &
		let "length++"
	done
done

if [ $mode = 1 ]; then
	./spectacle -s -p $Port -n blue 192.168.130.$Filler
fi
if [ $mode = 2 ]; then
	./duo -s -p $Port -n blue 192.168.130.$Filler
fi
