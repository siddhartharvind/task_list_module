#!/usr/bin/bash


# Usage
if [ $# -ne 1 ] || ( [ $1 != "linear" ] && [ $1 != "dfs" ] ); then
	printf "Usage: %s [linear|dfs]\n" $0
	exit 1
fi

mode=$1

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

# Run the module `demo`
# To use linear/BFS order: mode=0
#	(default; chosen for any value other than 1)
#
# To use DFS order: mode=1

if [ ${mode} = "linear" ]; then
	make demo colors=1 mode=0

elif [ ${mode} = "dfs" ]; then
	make demo colors=1 mode=1

fi

exit 0
