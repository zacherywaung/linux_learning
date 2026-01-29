BIN=process
SRC=$(shell ls *.c)
OBJ=$(SRC:.c=.o)

$(BIN):$(OBJ)
	@gcc -o $@ $^
	@echo "linking ... $^ to $@"
%.o:%.c
	@gcc -c $<
	@echo "compling ... $< to $@"

.PHONY:clean
clean:
	rm -rf $(OBJ) $(BIN)
