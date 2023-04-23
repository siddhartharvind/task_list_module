#!/usr/bin/bash

# For Stopped (T) process
sleep 60 & \
ps aux > /dev/null 2>&1 ;\
kill -SIGSTOP $!

echo;

# For Zombie (Z) process
ps aux > /dev/null 2>&1 ;
./zombie.out & ps aux > /dev/null 2>&1 ;

# To run the module demo
make demo colors=1
