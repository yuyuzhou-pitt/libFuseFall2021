#!/bin/bash

takedown() {
    rm -rf hello.txt
}

regex='(write: 
	PATH: )(\/(.*))(
	SIZE: )([0-9]+)(
	OFFS: )([0-9]+)'

check_recent_write() {
    logtext=$(cat ../log.txt) # the entire logs file...
    recent_logtext="write:${logtext##*$'write:'}" # ...except logs before the most recent write are removed...

    if [[ $recent_logtext =~ $regex ]]; then # ...because bash matches only the first instance of regex
        actual=${BASH_REMATCH[0]}
    fi
    
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