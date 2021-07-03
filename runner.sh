#!/bin/bash

g++ -o pool_tester -Wall thread/thread_pool_tester.cpp -lpthread -O3

while true ; do
    ./pool_tester
    if [ $? != 0 ] ; then
        echo "EXITING!"
        break
    fi
    
    sleep .3
done