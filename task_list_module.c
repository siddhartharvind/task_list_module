#include <linux/init.h>
#include <linux/kernel.h>
#include <linux/module.h>

#include <linux/sched.h>
#include <linux/sched/signal.h>


// This function iterates through the task list
// and prints each task's PID, name and state.
static void print_procs(void)
{
	struct task_struct *task;
	for_each_process(task) {
		printk(KERN_INFO "%d %s %lu\n", \
			task->pid, task->comm, task->__state
		);
	}
}

// This function is called when the module is loaded.
int task_list_init(void)
{
	printk(KERN_INFO "Loading module...\n");

	print_procs();

	return 0;
}

// This function is called when the module is removed.
void task_list_exit(void)
{
	printk(KERN_INFO "Removing module...\n");
}

// Macros for registering module entry and exit points.
module_init(task_list_init);
module_exit(task_list_exit);

MODULE_LICENSE("GPL");
MODULE_DESCRIPTION("This is a simple Linux kernel module \
	that lists all the current tasks in a Linux system, \
	starting from the `init` task.");
MODULE_AUTHOR("Siddharth Arvind");
