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
#define MAX_ENVS 100

// argument vector
char* g_argv[MAXARGC];
int g_argc = 0;

// environment vector 
char *g_env[MAX_ENVS];
int g_envs = 0;

// last exit code
int last_code = 0;

char buff[1024];
char oldpwdenv[1024];

char* getUser(){return getenv("USER");}	

char* getHostName(){return getenv("HOSTNAME");}	

char* getPwd()
{
	char* pwd = getcwd(buff, sizeof(buff));
	if(pwd != NULL)
	{
		char cwd[1024];
		snprintf(cwd, sizeof(cwd), "PWD=%s", buff);
		putenv(cwd);
	}
	return buff;
}

char* getHome(){return getenv("HOME");}

char* getOldPwd(){return getenv("OLDPWD");}

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

void excuteCd()
{
	int ret = 1;
	char* curpwd = getPwd();
	if(g_argc == 1)
	{
		char* home = getHome();
		ret = chdir(home);
	}
	else if(g_argc == 2)
	{
		std::string where = g_argv[1];
		if(where == "~")
		{
			std::string home = getHome();
			if(home.empty())
			{
				last_code = 1;
				return;
			}
		        ret = chdir(home.c_str());
		}
		else if(where == "-")
		{	
			char* oldpwd = getOldPwd();
			ret = chdir(oldpwd);			
		}
		else
		{
			ret = chdir(where.c_str());
		}
	}
	if(ret == 0)
	{
		last_code = 0;
		// change OLDPWD
		snprintf(oldpwdenv, sizeof(oldpwdenv), "OLDPWD=%s", curpwd);
		putenv(oldpwdenv);
	}
	else
		last_code = 1;
	return;
}

void excuteEcho()
{
	int ret = 1;
	if(g_argc == 2)
	{
		std::string which = g_argv[1];
		if(which == "$?")
		{
			std::cout << last_code << std::endl;
			ret = 0;
		}	
		else if(which[0] == '$')
		{
			char* res = getenv((which.substr(1)).c_str());
			if(res != NULL)
			{
				std::cout << res <<std::endl;
				ret = 0;
			}
			else
				ret = 1;
		}
		else
		{
			std::cout << which << std::endl;
			ret = 0;
		}
	}
	if(ret == 0)
	{
		last_code = 0;
	}
	else
		last_code = 1;

	return;
}


bool checkAndExcuteBuiltIn()
{
	std::string which = g_argv[0];
	if(which == "cd")
	{
		excuteCd();
		return true;
	}
	if(which == "echo")
	{
		excuteEcho();
		return true;
	}

	return false;
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
		int status = 0;
		pid_t res = waitpid(id, &status, 0);
		if(res > 0)
		{
			last_code = WEXITSTATUS(status);
		}
	}
	return 0;
}

void InitEnv()
{
	extern char** environ;
	memset(g_env, 0, sizeof(g_env));
	g_envs = 0;
	for(int i = 0; environ[i]; i++)
	{
		g_env[i] = (char*)malloc(strlen(environ[i]) + 1);
		strcpy(g_env[i], environ[i]);
		g_envs++;
	}
	g_env[g_envs] = NULL;
}

int main()
{
	InitEnv();
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
		
		// excute builtin command
		if(checkAndExcuteBuiltIn())
		       continue;	

		// excute command
		excuteCommand();
	}
	for(int i = 0; g_env[i]; i++)
	{
		free(g_env[i]);
		g_env[i] = nullptr;
	}

	return 0;
}
