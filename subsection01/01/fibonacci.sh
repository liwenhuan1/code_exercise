#!/bin/bash
fb0=0
fb1=1
for (( i=1; i <= 45; i++))
do
    fileName=$(printf "%04d.bin" $i)

    if [ x$1 = x"--clean" ]
    then
        rm -rf $fileName
    else
        fb2=$[ $fb0 + $fb1 ]
        fb0=$fb1
        fb1=$fb2
        dd if=/dev/zero of=$fileName bs=$fb2 count=1
    fi
done
