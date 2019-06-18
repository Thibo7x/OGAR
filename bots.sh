#!/bin/bash
read -r -p 'Multi (2) ou Solo (1) ? : ' mode
if [ $mode = 2 ]; then
	read -r -p 'Droite (2) ou Gauche (1) ? : ' side
fi
read -r -p 'Nombre de bots jaune : ' yellow
read -r -p 'Nombre de bots bleu : ' blue
read -r -p 'Nombre de bots cyan : ' cyan
read -r -p 'IP serveur 192.168.130.XXX : ' Filler
read -r -p 'Port de connection du serveur (2006 ou 1443) : ' Port


for color in {"yellow","blue","cyan"}
do
	let "max = $color"
	let "length = 0"
	if [ $mode = 2 ]; then
		while [ $length -lt $max ]; do
			./sock -p $Port -n $color -m $mode -S $side 192.168.130.$Filler &
			let "length++"
		done
	fi

	if [ $mode = 1 ]; then
		while [ $length -lt $max ]; do
			./sock -p $Port -n $color -m $mode -S 1 192.168.130.$Filler &
			let "length++"
		done
	fi

done

if [ $mode = 1 ]; then
	./spectacle -s -p $Port -n blue 192.168.130.$Filler
fi

if [ $mode = 2 ]; then
	./newDuo -s -p $Port -n blue 192.168.130.$Filler
fi
