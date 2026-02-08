libmyc.a:mystdio.o mystring.o
	@ar -rc $@ $^
	@echo "archive $^ to $@ done"
mystdio.o:mystdio.c
	@gcc -c $<
	@echo "compiling $< to $@ done"
mystring.o:mystring.c
	@gcc -c $<
	@echo "compiling $< to $@ done"

.PHONY:clean
clean:
	rm -rf *.o *.a

.PHONY:output
output:
	@mkdir -p lib/include
	@mkdir -p lib/mylib
	@cp mystdio.h lib/include
	@cp mystring.h lib/include
	@cp libmyc.a lib/mylib
	@tar -czf lib.tgz lib
	@echo "output lib done"
