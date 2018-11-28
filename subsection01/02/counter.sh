#!/bin/bash

sumSize=0
M_byte=$[ 1024 * 1024 ]
fileSize1MCnt=0

fileStat(){
    if [ -d $1 ]   #dir
    then
        for file in $(ls -a $1)  #$1/*
        do
            if [ x$file == x'.' ] || [ x$file == x'..' ]
            then
                continue
            else
                fileStat $1'/'$file
            fi
        done
    else
        fileSize=$(stat -c "%s" $1)
        if [ -z $fileSize ]
        then
            fileSize=0
        fi

        if [ $fileSize -gt $M_byte ]
        then
#            echo $1
            fileSize1MCnt=$[ $fileSize1MCnt + 1 ]
        fi
        sumSize=$[ $sumSize + $fileSize ]
    fi
}


if [ $# -eq 1 ]
then
    fileStat $1
    echo the all file size: $sumSize
    echo the file size greater than 1M count: $fileSize1MCnt
else
    echo path error!
fi




