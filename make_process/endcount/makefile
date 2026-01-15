BIN=test
CC=gcc
SRC=$(shell ls *.c)
OBJ=$(SRC:.c=.o)
LFLAGS=-o
FLAGS=-c
RM=rm -rf

$(BIN):$(OBJ)
	@$(CC) $(LFLAGS) $@ $^
	@echo "linking ... $^ to $@"

%.o:%.c
	@$(CC) $(FLAGS) $<
	@echo "compling ... $< to $@"

.PHONY:clean
clean:
	$(RM) $(OBJ) $(BIN)







# test:test.c
# 	gcc test.c -o test
# .PHONY:clean
# clean:
# 	rm -rf test
