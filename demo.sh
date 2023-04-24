#!/usr/bin/bash


# Create `zombie.out` executable if it does not exist
if [ ! -e zombie.out ]; then
	gcc zombie.c -o zombie.out
fi

# For Stopped (T) process
sleep 60 & \
ps aux > /dev/null 2>&1 ;\
kill -SIGSTOP $!

echo;

# For Zombie (Z) process
ps aux > /dev/null 2>&1 ;
./zombie.out & ps aux > /dev/null 2>&1 ;

# Run the module demo
# To use DFS order: mode=1
make demo colors=1 mode=1
