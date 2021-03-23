#!/bin/bash

# debug...
# set -x

if [[ !($# -eq 1 || ($# -eq 2 && $2 =~ ^.+\.sh)) ]]; then
    echo "There are two ways to use this script...";
    echo -e "\e[1;36mRun entire test suite:\e[0m $0 [mount directory]";
    echo -e "\e[1;33m  Run a specific test:\e[0m $0 [mount directory] test_name.sh";
    exit;
fi

printPass=true
printFullOutput=false # prints out everything each individual test echoed, rather than just the errcode

mount=$1
single_test=$2

numPass=0
numFail=0

execute_test() {
    cd $mount
    output=$(../$1)
    errcode="${output##*$'\n'}" # match longest string from the beginning til \n
    code=$?
    cd ..

    if [[ $errcode == 0 ]]; then
        if [ "$printPass" = true ]; then
            echo "PASSED:" $1
        fi
        let "numPass+=1"
    else
        echo -e "\e[1;31mFAILED (Error Code $errcode): $1\e[0m"

        let "numFail+=1"
    fi

    if [ "$printFullOutput" = true ]; then
        printf "Full output: \"$output\"\n\n"
    fi
}

if [[ $single_test == "" ]]; then
    printf "Running all tests...\n\n"
    # No test specified, run all tests!
    for f in ./tests/*.sh; do
        execute_test $f
    done
else
    printf "Running ${single_test/%'.sh'}...\n\n"
    # A particular test should be run...
    execute_test ./tests/$single_test
fi

echo ""
echo "NUMBER PASSED: $numPass    NUMBER FAILED: $numFail"