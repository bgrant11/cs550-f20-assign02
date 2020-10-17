
all: proc-test

proc-test: proc-test.c
	gcc -Wall -o user_space user_space.c

test: proc-test
	sudo ./proc-test

clean:
	rm proc-test
