#!/bin/bash

sumSize=0
M_byte=$[ 1024 * 1024 ]


fileStat(){
    if [ -d $1 ]   #目录
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
            echo $1
        fi
        sumSize=$[ $sumSize + $fileSize ]
    fi
}


if [ $# -eq 1 ]
then
    fileStat $1
    echo the all file size: $sumSize
else
    echo path error!
fi




