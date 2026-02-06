#include "mystdio.h"
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>

IO_FILE* newfile(int fd, int mode)
{
	IO_FILE* newfile = (IO_FILE*)malloc(sizeof(IO_FILE));
	if(newfile == NULL) return NULL;
	newfile->fileno = fd;
	newfile->buffersize = 0;
	newfile->mode = mode;
	if(isatty(fd))
	{
		newfile->flushmethod = FLUSH_LINE;
	}
	else
		newfile->flushmethod = FLUSH_FULL;

	memset(newfile->buffer, 0, newfile->buffersize);
	return newfile;
}


IO_FILE* Fopen(const char* filename, const char* mode)
{
	int fd = -1;
	int flag;
	if(strcmp(mode, "r") == 0)
	{
		flag = READ;
		fd = open(filename, O_RDONLY);
	}
	else if(strcmp(mode, "w") == 0)
	{
		flag = WRITE;
		fd = open(filename, O_CREAT | O_WRONLY | O_TRUNC, 0666);
	}
	else if(strcmp(mode, "a") == 0)
	{
		flag = APPEND;
		fd = open(filename, O_CREAT | O_WRONLY | O_APPEND, 0666);
	}
	if(fd < 0) return NULL;
	return newfile(fd, flag);
}

void Fflush(IO_FILE* stream)
{
	if(!stream->buffersize) return;
	write(stream->fileno, stream->buffer, stream->buffersize);
	//fsync(stream->fileno);
	stream->buffersize = 0;
}

int Fwrite(const void* ptr, int size, IO_FILE* stream)
{	
	// copy
	memcpy(stream->buffer + stream->buffersize, ptr, size);
	stream->buffersize += size;
	// line flush
	if(stream->flushmethod == FLUSH_LINE &&
			stream->buffer[stream->buffersize - 1] == '\n')
	{
		Fflush(stream);
	}
}

void Fclose(IO_FILE* stream)
{
	if(!stream) return;
	if(stream->buffersize) Fflush(stream);
	close(stream->fileno);
	free(stream);
	stream = NULL;
}


