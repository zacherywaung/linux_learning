#include<iostream>
#include<cstdio>
#include<unistd.h>
#include<cstring>
#include<sys/types.h>
#include<sys/stat.h>
#include<sys/wait.h>
#include<string>
#include<fcntl.h>

#define PROMPTFORMAT "[%s@%s %s]# "
#define PROMPTSIZE 1024
#define COMMANDSIZE 1024
#define MAXARGC 100
#define MAX_ENVS 100
#define BASICSIZE 1024
#define BASICSIZE2 100

// argument vector
char* g_argv[MAXARGC];
int g_argc = 0;

// environment vector 
char *g_env[MAX_ENVS];
int g_envs = 0;

// last exit code
int last_code = 0;

char buff[BASICSIZE];
char oldpwdenv[BASICSIZE];

// redir
#define NODIR 0
#define INPUT_REDIR 1
#define OUTPUT_REDIR 2
#define APPEND_REDIR 3
int redir = NODIR;
std::string filename;

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
	for(int i = 1; i < g_argc; i++)
	{
		std::string which = g_argv[i];
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
			std::cout << which;
			ret = 0;
		}
		if(i < g_argc - 1)
		{
			std::cout << " ";
		}
	}
	std::cout << std::endl;
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
	int fd_backup = -1;
	if(redir == INPUT_REDIR)
	{
		fd_backup = dup(0);
		int fd = open(filename.c_str(), O_RDONLY);
		dup2(fd, 0);
		close(fd);
	}
	else if(redir == OUTPUT_REDIR)
	{
		fd_backup = dup(1);
		int fd = open(filename.c_str(), O_CREAT | O_WRONLY | O_TRUNC, 0664);
		dup2(fd, 1);
		close(fd);
	}
	else if(redir == APPEND_REDIR)
	{
		fd_backup = dup(1);
		int fd = open(filename.c_str(), O_CREAT | O_WRONLY | O_APPEND, 0664);
		dup2(fd, 1);
		close(fd);
	}

	bool ret = false;
	if(which == "cd")
	{
		excuteCd();
		ret = true;
	}
	if(which == "echo")
	{
		excuteEcho();
		ret = true;
	}

	if(fd_backup != -1)
	{
		if(redir == INPUT_REDIR)
		{
			dup2(fd_backup, 0);
		}
		else
		{
			dup2(fd_backup, 1);
		}
		close(fd_backup);
	}

	return ret;
}


int excuteCommand()
{
	pid_t id = fork();
	if(id == 0)
	{
		if(redir == INPUT_REDIR)
		{
			int fd = open(filename.c_str(), O_RDONLY);
			dup2(fd, 0);
			close(fd);
		}
		else if(redir == OUTPUT_REDIR)
		{
			int fd = open(filename.c_str(), O_CREAT | O_WRONLY | O_TRUNC, 0666);
			dup2(fd, 1);
			close(fd);
		}
		else if(redir == APPEND_REDIR)
		{

			int fd = open(filename.c_str(), O_CREAT | O_WRONLY | O_APPEND, 0666);
			dup2(fd, 1);
			close(fd);
		}
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

void trimSpace(char* cmd, int& end)
{
	while(isspace(cmd[end]))
	{
		end++;
	}
}

void checkRedir(char* cmd)
{
	filename.clear();
	redir = NODIR;
	int start = 0;
	int end = strlen(cmd) - 1;
	while(end > start)
	{
		if(cmd[end] == '<')
		{
			redir = INPUT_REDIR;
			cmd[end++] = '\0';
			trimSpace(cmd, end);
			filename = cmd + end;
			break;
		}
		else if(cmd[end] == '>')
		{
			if(cmd[end - 1] == '>')
			{
				redir = APPEND_REDIR;
				cmd[end - 1] = '\0';
			}
			else{
				redir = OUTPUT_REDIR;
			}
		        cmd[end++] = '\0';
		        trimSpace(cmd, end);
		        filename = cmd + end;
			break;
		}
		end--;
	}	
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
		
		// check redir
		checkRedir(command);

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
