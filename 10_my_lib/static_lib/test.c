#include "mystdio.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "mystring.h"

int main()
{
	IO_FILE* fp = Fopen("log.txt", "a");
	if(fp == NULL)
	{
		printf("Fopen error\n");
		exit(1);
	}
	const char* msg = "hello world";
	
	int cnt = 3;
	while(cnt)
	{
		char message[64];
		snprintf(message, sizeof(message),"%s : %d\n", msg, cnt);
		Fwrite(message, strlen(message), fp);
		cnt--;
	}
	Fflush(fp);
	Fclose(fp);

	const char* s = "hello world";
	printf("Strlen: %d\n", Strlen(s));
	return 0;
}

