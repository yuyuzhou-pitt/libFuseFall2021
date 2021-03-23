#!/bin/bash

# setup
rm ../log.txt

# test
echo hello world > hello.txt
expected="write: 
	PATH: /hello.txt
	SIZE: 12
	OFFS: 0" # these linebreaks are important!
actual=$(cat ../log.txt)

if [[ $actual == $expected ]]; then
    echo "0"
else
    echo "1"
fi

# takedown
rm hello.txt