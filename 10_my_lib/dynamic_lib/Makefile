libmyc.so:mystdio.o mystring.o
	gcc -shared -o $@ $^
mystdio.o:mystdio.c
	@gcc -fPIC -c $<
	@echo "compiling $< to $@ done"
mystring.o:mystring.c
	@gcc -fPIC -c $<
	@echo "compiling $< to $@ done"

.PHONY:clean
clean:
	rm -rf *.o *.so

.PHONY:output
output:
	@mkdir -p lib/include
	@mkdir -p lib/mylib
	@cp mystdio.h lib/include
	@cp mystring.h lib/include
	@cp libmyc.so lib/mylib
	@tar -czf lib.tgz lib
	@echo "output lib done"
