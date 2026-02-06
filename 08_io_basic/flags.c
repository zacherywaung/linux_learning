#include<stdio.h>

#define FLAGONE (1<<0)
#define FLAGTWO (1<<1)
#define FLAGTHREE (1<<2)
#define FLAGFOUR (1<<3)

void func(int flags)
{
	if(flags & FLAGONE)
	{
		printf("FLAG is one\n");
	}
	if(flags & FLAGTWO)
	{
		printf("FLAG is two\n");
	}
	if(flags & FLAGTHREE)
	{
		printf("FLAG is three\n");
	}
	if(flags & FLAGFOUR)
	{
		printf("FLAG is four\n");
	}
}

int main()
{
	func(FLAGTWO | FLAGFOUR);
	return 0;
}
