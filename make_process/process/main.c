#include"process.h"

double total = 1024.00;
double speed = 1.0;

void download()
{
	double cur = 0;
	while(cur <= total)
	{
		FlushProcess(cur, total);
		usleep(2000);
		cur += speed;
	}
	fprintf(stdout, "\ndownload %.2lfMB Done\n", cur);
}

int main()
{
	download();
	return 0;
}

