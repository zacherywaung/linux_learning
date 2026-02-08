#pragma once

#define MAX 1024

#define READ 0
#define WRITE 1
#define APPEND 2

#define FLUSH_LINE (1 << 1)
#define FLUSH_FULL (1 << 2)

typedef struct IO_FILE
{
	int fileno;  //fd
	char buffer[MAX];
	int buffersize;
	int mode;
	int flushmethod;
}IO_FILE;

IO_FILE* Fopen(const char* filename, const char* mode);
void Fclose(IO_FILE* stream);
void Fflush(IO_FILE* stream);
int Fwrite(const void* ptr, int size, IO_FILE* stream);
