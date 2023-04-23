#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>

int main()
{
	pid_t pid = fork();

	if (pid > 0) {
		// Parent
		// printf("zombie.c: In parent\n");
		sleep(100);
	}
	else if (pid == 0) {
		// Child
		// printf("zombie.c: In child\n");
		exit(0);
	}
	else {
		// Fork failed
		return 1;
	}
	return 0;
}
