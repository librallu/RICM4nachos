#!/bin/bash
declare -a tests=("putchar" "getchar" "putint" "getint" "putstring" "getstring" "userpages0" "userpages1" "userpages2")
for i in "${tests[@]}"
do
    echo ""
    echo "################ Running test : " ${i} "##################";
    read -p "Press enter to continue ..."
    echo ""
    ./nachos-step2 -x $i ;
done
