typedef struct process_info {
	int pid;
	int ppid;
	int cpu;
	int state;
} process_info;

typedef struct node {
	process_info p_info;
	struct node * next;
} node;

#define BUF_SIZE sizeof(process_info)
#define NODE_SIZE sizeof(node)
