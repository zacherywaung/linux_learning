#include<stdio.h>
#include<string.h>
#include<unistd.h>
#include<sys/types.h>
#include<sys/stat.h>
#include<fcntl.h>

int main()
{
	// fopen fwrite
//	FILE* fp = fopen("log.txt", "w");
//	if(fp == NULL)
//	{
//		perror("fopen");
//		return 1;
//	}
//	char* msg = "hello world\n";
//	fwrite(msg, 1, strlen(msg), fp);
	
	// open write read
//	int fd = open("log.txt", O_CREAT|O_WRONLY|O_TRUNC, 0666);
//	if(fd < 0)
//	{
//		perror("open");
//		return 1;
//	}
//	char* msg = "hello world\n";
//	int cnt = 5;
//	while(cnt--)
//	{
//		write(fd, msg, strlen(msg));
//	}
//
//	close(fd);

//	int fd = open("log.txt", O_RDONLY);
//	if(fd < 0)
//	{
//		perror("open");
//		return 1;
//	}
//	char buf[1024];
//	while(1)
//	{
//		ssize_t res = read(fd, buf, sizeof(buf) - 1);
//		if(res > 0)
//		{
//			buf[res] = '\0';
//			printf("%s", buf);
//		}
//		else
//			break;
//	}
//	close(fd);

	// redir(dup2)
	char buf[1024] = "Start\n";
	//write(1, buf, strlen(buf));
	printf("%s", buf);
	fflush(stdout);

	int fd = open("log.txt", O_CREAT | O_WRONLY | O_TRUNC, 0666);
	int fd_backup = dup(1);
	dup2(fd, 1);
	printf("%s", "hello from file\n");
	close(fd);

	dup2(fd_backup, 1);
	printf("%s", "End\n");
	fflush(stdout);
	close(fd_backup);

	return 0;
}
