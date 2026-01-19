#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
int gval = 100;
int main()
{
	fprintf(stdout, "Parent process start, pid = %d\n", getpid());
	pid_t id = fork();
	
	if(id < 0)
	{
		perror("fork fail\n");
		return 1;
	}
	else if(id == 0)
	{
		// child
		while(1)
		{
			sleep(3);
			fprintf(stdout, "I'm a child process, my pid is %d, my parent pid is %d.\n", getpid(), getppid());
			fprintf(stdout, "change gval %d->%d\n", gval, gval + 1);
			++gval;
		}
	}
	else
	{
		// parent
		while(1)
		{
			sleep(3);
			fprintf(stdout, "I'm a parent process, my pid is %d, my parent pid is %d. gval = %d\n", getpid(), getppid(), gval);
		}
	}
	
	//chdir("/home/zachery");
//	while(1)
//	{
//		sleep(2);
//		fprintf(stdout, "I'm a process! my pid is %d! my parent pid is %d!\n", getpid(), getppid());
//	}
}
