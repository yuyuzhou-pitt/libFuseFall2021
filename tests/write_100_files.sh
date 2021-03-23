#!/bin/bash

takedown() {
    for n in {1..100}
    do
        rm -rf "file_$n.txt"
    done
}

check_recent_write() {
    logtext=$(cat ../log.txt)
    actual="write:${logtext##*$'write:'}"
    
    if [[ $actual != $expected ]]; then
        echo $1
        takedown
        exit
    fi
}

# test
for n in {1..100}
do
    echo "this is file #$n" > "file_$n.txt"
    
    if [ $n -lt 10 ]; then
    expected="write: 
	PATH: /file_$n.txt
	SIZE: 16
	OFFS: 0"
    elif [ $n -lt 100 ]; then
    expected="write: 
	PATH: /file_$n.txt
	SIZE: 17
	OFFS: 0"
    else
    expected="write: 
	PATH: /file_$n.txt
	SIZE: 18
	OFFS: 0"
    fi

    check_recent_write $n
done

# all tests passed!
echo "0"