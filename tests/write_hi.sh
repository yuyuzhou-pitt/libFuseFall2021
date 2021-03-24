#!/bin/bash

takedown() {
    rm -rf hello.txt
}

regex='(write: 
	PATH: )(\/[a-zA-Z.]*)(
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

# all tests passed!
echo "0"
takedown
exit