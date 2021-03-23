#!/bin/bash

takedown() {
    rm -rf hello.txt
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
echo hello world > hello.txt
expected="write: 
	PATH: /hello.txt
	SIZE: 12
	OFFS: 0"
check_recent_write "1"

echo hello again >> hello.txt
expected="write: 
	PATH: /hello.txt
	SIZE: 12
	OFFS: 12"
check_recent_write "2"

echo hello yet again >> hello.txt
expected="write: 
	PATH: /hello.txt
	SIZE: 16
	OFFS: 24"
check_recent_write "3"

# all tests passed!
echo "0"
takedown
exit