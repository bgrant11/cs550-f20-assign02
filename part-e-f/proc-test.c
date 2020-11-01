
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include "proc-list-dev/proc_list.h"



void print_proc(process_info * proc);

int main(int argc, char **argv){
	int fd;
	// open device
	if( ( fd = open("/dev/process_list", O_RDONLY)) < 0) {
		printf("Error opening /dev/process_list\n");
		return -1;
	}
	char *buffer = malloc(BUF_SIZE);
	process_info * p_info;
	int bytes_read = 1;
	while( bytes_read != 0){
		bytes_read = read(fd, buffer, BUF_SIZE);
		if(bytes_read < 0){
			printf("Error reading proc list\n");
			return -1;
		}
		p_info = (process_info*)buffer;
		print_proc(p_info);
	}
	close(fd);


	return 0;
}

void print_proc(process_info * process){
	printf("PID: %ld\t", process->pid);
	printf("PPID: %ld\t", process->ppid);
	printf("CPU: %ld\t", process->cpu);
	printf("STATE: ");
	if(process->state & 0x0000) printf("TASK_RUNNING, ");
	if(process->state & 0x0001) printf("TASK_INTERRUPTIBLE, ");
	if(process->state & 0x0002) printf("TASK_UNINTERRUPTIBLE, ");
	if(process->state & 0x0004) printf("__TASK_STOPPED, ");
	if(process->state & 0x0008) printf("__TASK_TRACED, ");
	if(process->state & 0x0010) printf("EXIT_DEAD, ");
	if(process->state & 0x0020) printf("EXIT_ZOMBIE, ");
	if(process->state & 0x0040) printf("TASK_PARKED, ");
	if(process->state & 0x0080) printf("TASK_DEAD, ");
	if(process->state & 0x0100) printf("TASK_WAKEKILL, ");
	if(process->state & 0x0200) printf("TASK_WAKING, ");
	if(process->state & 0x0400) printf("TASK_NOLOAD, ");
	if(process->state & 0x0800) printf("TASK_NEW, ");
	if(process->state & 0x1000) printf("TASK_STATE_MAX, ");
	printf("\n");

}
