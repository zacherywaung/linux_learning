#include"process.h"

typedef void (*callback_t)(double cur, double total);
double total = 1024.00;
double speed = 1.0;

void download(callback_t cb)
{
	double cur = 0;
	while(cur <= total)
	{
		cb(cur, total);
		usleep(2000);
		cur += speed;
	}
	fprintf(stdout, "\ndownload %.2lfMB Done\n", cur);
}

int main()
{
	download(FlushProcess);
	return 0;
}

