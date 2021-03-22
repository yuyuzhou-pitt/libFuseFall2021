#!/bin/bash

# debug...
# set -x
if [ $# -ne 2 ];then
    echo "Usage: $0 [base directory] [mount directory]";
    exit;
fi

printPass=true

base=$1
mount=$2

numPass=0
numFail=0
printf "Running tests...\n\n"
for f in ./tests/*.sh; do
    cd $mount
    output=$(../$f)
    errcode="${output##*$'\n'}" # match longest string from the beginning til \n
    code=$?
    cd ..

    if [[ $errcode == 0 ]]; then
        if [ "$printPass" = true ]; then
            echo "PASSED:" $f
        fi
        let "numPass+=1"
    else
        echo -e "\e[1;31mFAILED (Error Code $errcode): $f\e[0m"

        let "numFail+=1"
    fi
done

echo ""
echo "NUMBER PASSED: $numPass    NUMBER FAILED: $numFail"