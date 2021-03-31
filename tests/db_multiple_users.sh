#!/bin/bash

: '
This test fails presumably because multithreading doesnt work yet.
For Example:

sudo -H -u root bash -c 'echo "I am $USER, with uid $UID"' > root.txt
    > I am root, with uid 0
    This adds bytes to my (cas380 = UID 1000) quota rather than (root = UID 0) quota

Also, not sure how exactly this happened, but I had some files in mount. 
Upon deleting them, the quota increased.
They were all created through this script, so multithreading doesnt delete properly.
'

#$(sudo -H -u root bash -c 'cmd')
#$(cmd)
#set -x

takedown() {
    rm -rf user.txt
    rm -rf root.txt
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

# get the IDs
user_id=$(printf "%020d" $UID)
root_id=$(printf "%020d" 0)

# get quota usage before altering
get_db_entry $user_id
initial_user_bytes=$byte_total
get_db_entry $root_id
initial_root_bytes=$byte_total

# add to size
expected_byte_change=12
user_echo=$(echo "hello world" > user.txt)
root_echo=$(sudo -H -u root bash -c 'echo "hello world"' > root.txt)

get_db_entry $user_id
altered_user_bytes=$byte_total
get_db_entry $root_id
altered_root_bytes=$byte_total

if [[ $(expr $altered_user_bytes - $initial_user_bytes) -ne $expected_byte_change ]]; then
    echo "1"
    takedown
    exit
fi
if [[ $(expr $altered_root_bytes - $initial_root_bytes) -ne $expected_byte_change ]]; then
    echo "2"
    takedown
    exit
fi

# remove from size
takedown
get_db_entry $user_id
altered_user_bytes=$byte_total
get_db_entry $root_id
altered_root_bytes=$byte_total

if [[ $altered_root_bytes -ne $initial_root_bytes ]]; then
    echo "3"
    takedown
    exit
fi
if [[ $altered_user_bytes -ne $initial_user_bytes ]]; then
    echo "4"
    takedown
    exit
fi

# all tests passed!
echo "0"
takedown
exit