
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>


int main(int argc, char **argv){
	int fd;
	// open device
	if( ( fd = open("/dev/proc-list", O_RDONLY)) < 0) {
		printf("Error opening /dev/proc-list\n");
		return -1;
	}
	char *buffer;
	//allocate mem for buffer
	int bytes_read = read(fd, buffer, buffer_len);
	if(bytes_read < 0){
		printf("Error reading proc list\n");
		return -1;
	}



	return 0;
}
