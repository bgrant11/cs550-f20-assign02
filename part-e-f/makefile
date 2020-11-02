
all: proc-test

proc-test: proc-test.c
	gcc -Wall -g -o proc-test proc-test.c


test: proc-test
	sudo ./proc-test

val: proc-test
	sudo valgrind -v --leak-check=full --show-leak-kinds=all ./proc-test

clean:
	rm -f proc-test sand


sand: sand.c
	gcc -Wall -g -o sand sand.c

sandt:
	./sand

sandv: sand
	valgrind -v --leak-check=full --show-leak-kinds=all ./sand


