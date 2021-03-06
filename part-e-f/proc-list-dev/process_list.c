#include <linux/miscdevice.h>
#include <linux/fs.h>
#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/uaccess.h>
#include <linux/sched.h>
#include <linux/sched/signal.h>
#include <linux/slab.h>
#include "proc_list.h"


node * head;
node * curr;
long total_process;
long total_reads;

void delete_list(void){
	node* local_curr = head;
	node* next;
	pr_info("deleting list\n");	
	while(local_curr != NULL){
		next = local_curr->next;
		kfree(local_curr->p_info);		
		kfree(local_curr);
		local_curr = next;
	}
	pr_info("deleting list end\n");	
}

int count_procs(int total){	
	int count = 0;
	node * curr = head;
	pr_info("count_procs\n");	
	while(curr->next != NULL){
			count++;
			curr = curr->next;
	}
	pr_info("count_procs end\n");
	if(count == total){
		return 0;
	} else {
		delete_list();		
		return -2;
	}
}

void test_print(void){	
	node * curr = head->next;
	pr_info("test print\n");	
	while(curr != NULL){	
		pr_info("PID: %d\t", curr->p_info->pid);
		pr_info("PPID: %d\t", curr->p_info->ppid);
		pr_info("CPU: %d\t", curr->p_info->cpu);
		pr_info("STATE: %d\t", curr->p_info->state);
		pr_info("\n");
		curr = curr->next;
	}
	pr_info("test print end\n");
}

int gen_proc_list(void){
	int ret;	
	struct task_struct * process;
	node * curr;	
	head = (node*)kmalloc(NODE_SIZE, GFP_KERNEL);
	head->p_info = NULL;
	head->next = NULL;	
	curr = head;
	total_process = 0;
	total_reads = 0;	
	pr_info("generating proc list\n");
	for_each_process(process){
		total_process++;
		curr->next = (struct node*)kmalloc(NODE_SIZE, GFP_KERNEL);
		curr = curr->next;
		curr->next = NULL;
		curr->p_info = (process_info*)kmalloc(BUF_SIZE, GFP_KERNEL);
		pr_info("%d\n", process->pid);		
		curr->p_info->pid = process->pid;
		curr->p_info->ppid = process->parent->pid;
		curr->p_info->cpu = process->cpu;
		curr->p_info->state = process->state;
	}
	
	test_print();
	ret = count_procs(total_process);
	pr_info("generating proc list end\n");
	return ret;
}

static int proc_open(struct inode *inode, struct file *file)
{
	int ret;    
	pr_info("Opening Proc List Device\n");
	ret = gen_proc_list();
	curr = head->next;
	pr_info("Opening Proc List Device end\n");	
	return ret;
}

static int proc_close(struct inode *inodep, struct file *filp)
{
    pr_info("Closing Proc List Device\n");
	delete_list();
    return 0;
}

static ssize_t proc_read(struct file *file, char __user *buf,
		       size_t len, loff_t *ppos)
{
	int err;
	char * proc_info_char;	
	pr_info("proc_read\n");	
	total_reads++;
	pr_info("%ld of %ld\n", total_reads, total_process);
	if(len < BUF_SIZE){
		pr_info("bytes requested must be at least %ld\n", BUF_SIZE);
		return -2;
	}
	if(curr == NULL){
		pr_info("end read");		
		curr = head;		
		return 0;
	}
	proc_info_char = (char*)(curr->p_info);
	err = copy_to_user(buf, proc_info_char, BUF_SIZE);
	if(err != 0){
		pr_info("Problem sending message to user, %d\n", err);
		return -1;
	}
	curr = curr->next;
	return BUF_SIZE;
}

static const struct file_operations proc_fops = {
    .owner			= THIS_MODULE,
    .open			= proc_open,
    .release		= proc_close,
    .llseek 		= no_llseek,
	.read			= proc_read,
};

struct miscdevice proc_device = {
    .minor = MISC_DYNAMIC_MINOR,
    .name = "process_list",
    .fops = &proc_fops,
};

static int __init proc_init(void)
{
    int error;

    error = misc_register(&proc_device);
    if (error) {
        pr_err("can't misc_register :(\n");
        return error;
    }

    pr_info("Proc List Device registered\n");
    return 0;
}

static void __exit proc_exit(void)
{
    misc_deregister(&proc_device);
    pr_info("Proc List Device deregistered\n");
}

module_init(proc_init)
module_exit(proc_exit)

MODULE_DESCRIPTION("Process List Device");
MODULE_AUTHOR("Brian Grant");
MODULE_LICENSE("GPL");

