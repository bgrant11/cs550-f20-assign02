
all: user_space

user_space: user_space.c
	gcc -Wall -o user_space user_space.c

test: user_space
	sudo ./user_space

clean:
	rm user_space
