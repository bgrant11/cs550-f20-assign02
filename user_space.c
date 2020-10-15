
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>


int main(int argc, char **argv){
	int fd;
	char str[100] = "This is a string";
	if( ( fd = open("/dev/misc_sample", O_RDWR)) < 0) {
		printf("Error opening /dev/misc_sample\n");
	}
	ssize_t ret = write(fd, str, 100);
	if(ret < 0){
		printf("Error writing\n");
	} else {
		printf("Write successful\n");
	}
	close(fd);
	return 0;
}
