#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <stdlib.h>

int main()
{
	pid_t id = fork();
	if(id == 0)
	{
		printf("program start replace\n");
		//execl("/usr/bin/ls", "ls", "-l", "-a", NULL);
		//execlp("ls", "ls", "-l", "-a", NULL);
	//	extern char** environ;
	//	char* const argv[] = {
	//	"ls", "-l", "-a", NULL
	//	};
	//	char* const env[] = {
	//	"MYVAL=1234", "MYVALUE=9876", NULL
	//	};
	//	int i = 0;
	//	for(; env[i]; i++)
	//	{
	//		putenv(env[i]);
	//	}
	//	execve("/usr/bin/ls", argv, environ);
		execl("./other", "other", NULL);

	}
	else
	{
		pid_t res = waitpid(id, NULL, 0);
		printf("excute finish\n");
	}
	return 0;
}
