#!/bin/bash

takedown() {
    rm -rf hello.txt
}

w_regex='(write: 
	PATH: )(\/(.*))(
	SIZE: )([0-9]+)(
	OFFS: )([0-9]+)'
db_regex='([0-9]{20}),([0-9]{20}),([0-9]{20}),([0-9]{20}),([0-9]{20})'

: '
Database records:
user_id,             byte_total,          byte_quota,          file_total,          file_quota
00000000000000001000,00000000000000000005,00000000000001048576,00000000000000000000,00000000000000000100
'

check_recent_write() {
    logtext=$(cat ../log.txt) # the entire logs file...
    recent_logtext="write:${logtext##*$'write:'}" # ...except logs before the most recent write are removed...
    csv=$(cat ../database.csv)

    if [[ $recent_logtext =~ $w_regex ]]; then # ...because bash matches only the first instance of regex
        actual=${BASH_REMATCH[0]}
        write_size=${BASH_REMATCH[5]}
    fi
    if [[ $csv =~ $db_regex ]]; then
        # trim off leading zeroes
        user_id=$((10#${BASH_REMATCH[1]}))
        byte_total=$((10#${BASH_REMATCH[2]}))
        byte_quota=$((10#${BASH_REMATCH[3]}))
        file_total=$((10#${BASH_REMATCH[4]}))
        file_quota=$((10#${BASH_REMATCH[5]}))
    fi
    
    if [[ $actual != $expected ]]; then
        echo $1
        takedown
        exit
    fi
    # Assumes we start with an empty mount
    if [[ $write_size -ne $byte_total ]]; then
        echo -$1
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