#include <stdio.h>
#include <unistd.h>
#include <errno.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <stdlib.h>
#include <string.h>

int main()
{
	pid_t id = fork();
	if(id < 0)
	{
		printf("fork fail, errno:%d, %s", errno, strerror(errno));
		return 1;
	}
	else if(id == 0)
	{
		int cnt = 5;
		while(cnt)
		{
			printf("child process, pid:%d, ppid:%d\n", getpid(), getppid());
			cnt--;
			sleep(2);
			//error
			printf("about to crash\n");
			int* p = 0;
			*p = 100;
		}
		exit(20);
	}
	else
	{
		int status;
		//pid_t ret = waitpid(id, &status, 0);
		pid_t ret;
		do{
			ret = waitpid(id, &status, WNOHANG);
			printf("waitpid ret = %d\n", ret);
			if(ret == 0)
			{
				printf("child running\n");
			}
			sleep(1);
			// do sth.
		}while(ret == 0);

		if(WIFEXITED(status))
		{
			printf("wait child process success, return code:%d, signal:%d\n",
					WEXITSTATUS(status), status & 0x7F);
					// (status>>8) & 0xFF
		}
		else
		{
			printf("wait fail, singal: %d\n", status & 0x7F);
			return 1;
		}
	}
	return 0;
}
