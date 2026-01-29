#include"process.h"

#define NUM 101
#define STYLE '#'

void FlushProcess(double cur, double total)
{

	char buffer[NUM];
	memset(buffer, 0, sizeof(buffer));
	const char* lable = "|/-\\";
	int len = strlen(lable);

	int num = (int)(cur*100 / total);
	for(int i = 0; i < num; i++)
	{
		buffer[i] = STYLE;
	}
	double rate = cur / total;
	static int count = 0;
	count %= len;

	fprintf(stdout, "[%-100s][%.2lf%%][%c]\r", buffer, rate*100, lable[count]);
	count++;
	fflush(stdout);
}


void process()
{
	char buffer[NUM];
	memset(buffer, 0, sizeof(buffer));
	const char* lable = "|/-\\";
	int len = strlen(lable);

	int count = 0;
	while(count <= 100)
	{
		fprintf(stdout, "[%-100s][%d%%][%c]\r", buffer, count, lable[count%len]);
		fflush(stdout);
		buffer[count] = STYLE;
		count++;
		usleep(100000);
	}
	fprintf(stdout, "\n");

}

