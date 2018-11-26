#!/bin/bash
fb0=0
fb1=1
maxNameLen=4
for (( i=1; i <= 45; i++))
do
	nameLen=$(expr length "$i")
	fileName=$i.bin
	while [ $nameLen -lt $maxNameLen ]
	do
		fileName=0$fileName
		nameLen=$[ $nameLen + 1 ]
	done
	
	if [ x$1 = x"--clean" ]
	then
		rm -rf $fileName
	else 
		fb2=$[ $fb0 + $fb1 ]
		fb0=$fb1
		fb1=$fb2
		dd if=/dev/zero of=$fileName bs=$fb2 count=1
#		touch $fileName
	fi
done


