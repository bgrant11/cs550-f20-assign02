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

void delete_list(void){
	node* local_curr = head;
	node* next;
	while(local_curr != NULL){
		next = local_curr->next;
		kfree(local_curr);
		local_curr = next;
	}
}

int count_procs(int total){	
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

void test_print(void){
	node * curr = head->next;
	while(curr != NULL){	
		pr_info("PID: %d\t", curr->p_info.pid);
		pr_info("PPID: %d\t", curr->p_info.ppid);
		pr_info("CPU: %d\t", curr->p_info.cpu);
		pr_info("STATE: %d\t", curr->p_info.state);
		pr_info("\n");
		curr = curr->next;
	}
}

int gen_proc_list(void){
	struct task_struct * process;
	node * curr;	
	head = (node*)kmalloc(NODE_SIZE, GFP_KERNEL);
	head->next = NULL;	
	curr = head;
	total_process = 0;
		
	for_each_process(process){
		total_process++;
		curr->next = (struct node*)kmalloc(NODE_SIZE, GFP_KERNEL);
		curr = curr->next;
		curr->next = NULL;
		curr->p_info.pid = process->pid;
		curr->p_info.ppid = process->parent->pid;
		curr->p_info.cpu = process->cpu;
		curr->p_info.state = process->state;
	}
	
	test_print();
	return count_procs(total_process);
}

static int proc_open(struct inode *inode, struct file *file)
{
    pr_info("Opening Proc List Device\n");
	curr = head;    
	return gen_proc_list();
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
	if(len < BUF_SIZE){
		pr_info("bytes requested must be at least %ld\n", BUF_SIZE);
		return -2;
	}
	curr = curr->next;
	if(curr == NULL){
		return 0;
	}
	proc_info_char = (char*)(&(curr->p_info));
	err = copy_to_user(buf, proc_info_char, BUF_SIZE);
	if(err != 0){
		pr_info("Problem sending message to user, %d\n", err);
		return -1;
	}
	return BUF_SIZE;
	
	
		
	/*	
	int i = 0;	
	struct task_struct * process;
	for_each_process(process){
		i++;
		pr_info("PID: %d\t", process->pid);
		pr_info("PPID: %d\t", process->parent->pid);
		pr_info("CPU: %d\t", process->cpu);
		pr_info("STATE: ");
		if(process->state & 0x0000) pr_info("TASK_RUNNING, ");
		if(process->state & 0x0001) pr_info("TASK_INTERRUPTIBLE, ");
		if(process->state & 0x0002) pr_info("TASK_UNINTERRUPTIBLE, ");
		if(process->state & 0x0004) pr_info("__TASK_STOPPED, ");
		if(process->state & 0x0008) pr_info("__TASK_TRACED, ");
		if(process->state & 0x0010) pr_info("EXIT_DEAD, ");
		if(process->state & 0x0020) pr_info("EXIT_ZOMBIE, ");
		if(process->state & 0x0040) pr_info("TASK_PARKED, ");
		if(process->state & 0x0080) pr_info("TASK_DEAD, ");
		if(process->state & 0x0100) pr_info("TASK_WAKEKILL, ");
		if(process->state & 0x0200) pr_info("TASK_WAKING, ");
		if(process->state & 0x0400) pr_info("TASK_NOLOAD, ");
		if(process->state & 0x0800) pr_info("TASK_NEW, ");
		if(process->state & 0x1000) pr_info("TASK_STATE_MAX, ");
		pr_info("\n");
	}
	pr_info("there are %d processes\n", i);
	*/
	return 0;
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

