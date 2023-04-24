#!/usr/bin/bash


# Create `zombie.out` executable
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

# To run the module demo
make demo colors=1

# Remove the `zombie.out` executable
rm ./zombie.out
