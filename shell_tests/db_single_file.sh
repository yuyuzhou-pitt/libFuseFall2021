#!/bin/bash

takedown() {
    rm -rf hello.txt
}

regex='([0-9]{20}),([0-9]{20}),([0-9]{20}),([0-9]{20}),([0-9]{20})'

get_db_entry() {
    dbtext=$(cat ../database.csv)
    uid_topmost="$1${dbtext##*$'$1'}" 
    # the entry we want is at the beginning of the text so regex can match it

    if [[ $uid_topmost =~ $regex ]]; then
        # trim off leading zeroes and populate variables
        user_id=$((10#${BASH_REMATCH[1]}))
        byte_total=$((10#${BASH_REMATCH[2]}))
        byte_quota=$((10#${BASH_REMATCH[3]}))
        file_total=$((10#${BASH_REMATCH[4]}))
        file_quota=$((10#${BASH_REMATCH[5]}))
    fi
}

# test
current_user_id=$(printf "%020d" $UID)

# get quota usage before altering
get_db_entry $current_user_id
initial_byte_total=$byte_total

# add to size
echo hello world > hello.txt
expected_byte_change=12
get_db_entry $current_user_id
altered_byte_total=$byte_total

if [[ $(expr $altered_byte_total - $initial_byte_total) -ne $expected_byte_change ]]; then
    echo "1"
    takedown
    exit
fi

# remove from size
rm -rf hello.txt
get_db_entry $current_user_id
altered_byte_total=$byte_total

if [[ $altered_byte_total -ne $initial_byte_total ]]; then
    echo "2"
    takedown
    exit
fi

# all tests passed!
echo "0"
takedown
exit