#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>

int main()
{
	//int i;
	//scanf("%d\n", &i);

	//while(1)
	//{
	//	fprintf(stdout, "hello world\n");
	//}

	// while :; do ps ajx | grep myprocess | grep -v grep; sleep 2; done
	// check zombie state
		
	pid_t id = fork();
	if(id == 0)
	{
		//child
		int count = 5;
		while(count)
		{
			fprintf(stdout, "I'm child process, running:%d\n", count);
			sleep(2);
			count--;	
		}
	}
	else{
		//parent
		while(1)
		{
			fprintf(stdout, "I'm parent process, running\n");
			sleep(2);
		}
	}


	return 0;
}
