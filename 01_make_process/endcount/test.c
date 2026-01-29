#include<stdio.h>
#include<unistd.h>

int main()
{
	int i = 10;
	while(i >= 0)
	{
		fprintf(stdout, "%-2d\r", i);
		fflush(stdout);
		i--;
		sleep(1);
	}
	fprintf(stdout, "\n");
	return 0;
}

