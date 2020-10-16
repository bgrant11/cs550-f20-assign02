
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>


int main(int argc, char **argv){
	int fd;
	// open device
	if( ( fd = open("/dev/misc_sample", O_RDWR)) < 0) {
		printf("Error opening /dev/misc_sample\n");
	}
	// test write
	char str[100] = "This is a string";
	ssize_t ret = write(fd, str, 100);
	if(ret < 0){
		printf("Error writing\n");
	} else {
		printf("Write successful\n");
	}
	// test read
	char str2[100];
	ret = read(fd, str2, 100);
	if(ret < 0){
		printf("Error reading\n");
	} else {
		printf("Read successful\n");
	}	
	printf("read: %s\n", str2);

	
	close(fd);
	return 0;
}
