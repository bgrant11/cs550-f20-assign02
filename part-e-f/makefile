
all: proc-test

proc-test: proc-test.c
	gcc -Wall -o proc-test proc-test.c

test: proc-test
	sudo ./proc-test

clean:
	rm proc-test
