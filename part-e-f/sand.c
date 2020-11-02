#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "proc-list-dev/proc_list.h"

node * head;
node * curr;
long total_process;



void print_proc(process_info * process){
	printf("PID: %ld\t", process->pid);
	printf("PPID: %ld\t", process->ppid);
	printf("CPU: %ld\t", process->cpu);
	printf("STATE: %ld\t", process->state);

	printf("\n");

}

void delete_list(void){
	node* local_curr = head;
	node* next;
	while(local_curr != NULL){
		next = local_curr->next;
		free(local_curr);
		local_curr = next;
	}
}

int count_procs(int total){
	printf("total %d\n", total);	
	int count = 0;
	node * curr = head;
	while(curr->next != NULL){
			count++;
			curr = curr->next;
	}
	if(count == total){
		return 0;
	} else {
		delete_list();		
		return -2;
	}
}


int gen_proc_list(void){
	struct task_struct * process;
	head = (node*)malloc(NODE_SIZE);
	head->next = NULL;	
	node * curr = head;
	total_process = 0;
		
	for(int i =0; i < 20; i++){
		total_process++;
		curr->next = (node*)malloc(NODE_SIZE);
		curr = curr->next;		
		curr->next = NULL;
		curr->p_info.pid = 1 * i;
		curr->p_info.ppid = 2 * i;
		curr->p_info.cpu = 3 * i;
		curr->p_info.state = 4 * i;
	}
	return count_procs(total_process);
}

int main(int argc, char **argv){
	gen_proc_list();
	node * curr = head->next;
	while(curr != NULL){
		printf("YO");		
		print_proc(&(curr->p_info));
		curr = curr->next;
	}	
	delete_list();
	return 0;
}
