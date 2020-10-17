#include <linux/miscdevice.h>
#include <linux/fs.h>
#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/uaccess.h>
#include <linux/sched.h>
#include <linux/sched/signal.h>

static int proc_open(struct inode *inode, struct file *file)
{
    pr_info("Opening Proc List Device\n");
    return 0;
}

static int proc_close(struct inode *inodep, struct file *filp)
{
    pr_info("Closing Proc List Device\n");
    return 0;
}

static ssize_t proc_read(struct file *file, char __user *buf,
		       size_t len, loff_t *ppos)
{
	int i = 0;	
	struct task_struct * process;
	for_each_process(process){
		i++;
		pr_info("pid: %d\n", process->pid);
	}
	pr_info("there are %d processes\n", i);
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
    .name = "proc_list",
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

