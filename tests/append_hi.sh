#!/bin/bash

# setup
rm ../log.txt

# test
echo hello world > hello.txt
echo hello again >> hello.txt
echo hello yet again >> hello.txt

expected="write: 
	PATH: /hello.txt
	SIZE: 12
	OFFS: 0
write: 
	PATH: /hello.txt
	SIZE: 12
	OFFS: 12
write: 
	PATH: /hello.txt
	SIZE: 16
	OFFS: 24"
actual=$(cat ../log.txt)

if [[ $actual == $expected ]]; then
    echo "0"
else
    echo "1"
fi

# takedown
rm hello.txt