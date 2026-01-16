BIN=mycode
SRC=$(shell ls *.c)
OBJ=$(SRC:.c=.o)

$(BIN):$(OBJ)
	@gcc -o $@ $^
	@echo "linking ... $^ to $@"
%.o:%.c
	@gcc -c $< -g
	@echo "compiling ... $< to $@"
.PHONY:clean
clean:
	@rm -rf $(BIN) $(OBJ)
