typedef struct process_info {
	long pid;
	long ppid;
	long cpu;
	long state;
} process_info;

typedef struct node {
	process_info p_info;
	struct node * next;
} node;

#define BUF_SIZE sizeof(process_info)
#define NODE_SIZE sizeof(node)
