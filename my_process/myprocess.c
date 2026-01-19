#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
int main()
{
	while(1)
	{
		sleep(2);
		fprintf(stdout, "I'm a process! my pid is %d! my parent pid is %d!\n", getpid(), getppid());
	}
}
