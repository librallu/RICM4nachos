#!/bin/bash
declare -a tests=("putchar" "putstring" "putint" "userpages0" "userpages1")
for i in "${tests[@]}"
do
    echo ""
    echo "################ Running test : " ${i} "##################";
    echo ""
    ./nachos-step2 -x $i ;
done
