#include<iostream>
#include<cstdio>
#include<unistd.h>
#include<cstring>
#include<sys/types.h>
#include<sys/wait.h>
#include<string>

#define PROMPTFORMAT "[%s@%s %s]# "
#define PROMPTSIZE 1024
#define COMMANDSIZE 1024
#define MAXARGC 100

char* g_argv[MAXARGC];
int g_argc = 0;

char* getUser(){return getenv("USER");}	

char* getHostName(){return getenv("HOSTNAME");}	

char* getPwd(){return getenv("PWD");}

#define SLASH "/"
std::string getDirname(char* pwd)
{
	std::string ret = pwd;
	if(ret == SLASH) return ret;
	int pos = ret.rfind(SLASH);
	return ret.substr(pos + 1);	
}

void makePrompt(char* prompt, int size)
{
	//snprintf(prompt, size, PROMPTFORMAT, getUser(), getHostName(), getPwd());
	snprintf(prompt, size, PROMPTFORMAT, getUser(), getHostName(), getDirname(getPwd()).c_str());
}

void showCommandPrompt()
{
	// [zachery@login-students myshell]$ 
	char prompt[PROMPTSIZE];
	makePrompt(prompt, sizeof(prompt));
	printf("%s", prompt);
	fflush(stdout);
}

bool getCommand(char* command, int size)
{
	char* ret = fgets(command, size, stdin);
	if(ret == NULL) return false;
	command[strlen(command)-1] = '\0';
	if(strlen(command) == 0) return false;
	return true;	
}

#define SPLIT " "
void analyzeCommand(char* command)
{

	g_argc = 0;
	g_argv[g_argc++] = strtok(command, SPLIT);
	char* piece;
	while((piece = strtok(nullptr, SPLIT)) != NULL)
	{
		g_argv[g_argc++] = piece;
	}
	g_argv[g_argc] = NULL;
	return;
}

void testAnalyzeCommand()
{
	for(int i = 0; i < g_argc; i++)
	{
		printf("%s ", g_argv[i]);
	}
	printf("\n");
}

int excuteCommand()
{
	pid_t id = fork();
	if(id == 0)
	{
		execvp(g_argv[0], g_argv);
		exit(1);
	}
	else
	{
		pid_t res = waitpid(id, nullptr, 0);
		(void)res;
	}
	return 0;
}

int main()
{
	while(1)
	{
		// show prompt
		showCommandPrompt();
		
		// get command input
		char command[COMMANDSIZE];
		if(!getCommand(command, sizeof(command)))
			continue;
		
		// analyze command
		analyzeCommand(command);
		//testAnalyzeCommand();
		
		// excute command
		excuteCommand();
	}


	return 0;
}
