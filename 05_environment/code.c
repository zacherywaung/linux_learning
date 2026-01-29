#include <stdio.h>
#include <string.h>
#include <stdlib.h>

int main(int argc, char* argv[], char* env[])
{
	(void)argc;
	(void)argv;
	(void)env;
	// use getenv
	char* who = getenv("USER");
	if(who == NULL)
	{
		return 1;
	}
	if(strcmp(who, "zachery") == 0)
	{
		printf("valid excute\n");
	}
	else{
		printf("illegal excute, not zachery\n");
	}

	
//	// get environment variables 1
//	fprintf(stdout, "PATH:%s\n", getenv("PATH"));	



//	// get environment variables 2
//	extern char** environ;
//	int i = 0;
//	for(i = 0; environ[i]; i++)
//	{
//		fprintf(stdout, "environ[%d]->%s\n", i, environ[i]);
//	}

	
//	// get environment variables 3
//	int i = 0;
//	for(i = 0; env[i]; i++)
//	{
//		fprintf(stdout, "env[%d]->%s\n", i, env[i]);
//	}


//	// stimulate command line arguments
//	if(argc != 2)
//	{
//		fprintf(stdout, "Usage: %s [-a|-b|-c]\n", argv[0]);
//		return 1;
//	}
//	if(strcmp(argv[1], "-a") == 0)
//	{
//		fprintf(stdout, "module -a\n");
//	}
//	else if(strcmp(argv[1], "-b") == 0)
//	{
//		fprintf(stdout, "module -b\n");
//	}
//	else if(strcmp(argv[1], "-c") == 0)
//	{
//		fprintf(stdout, "module -c\n");
//	}
//	else{
//		fprintf(stdout, "Usage: %s [-a|-b|-c]\n", argv[0]);
//		return 1;
//	}
	return 0;

}
