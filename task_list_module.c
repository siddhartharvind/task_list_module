#include <linux/init.h>
#include <linux/kernel.h>
#include <linux/module.h>

#include <linux/sched.h>
#include <linux/sched/signal.h>


MODULE_LICENSE("GPL");
MODULE_DESCRIPTION("This is a simple Linux kernel module \
	that lists all the current tasks in a Linux system, \
	starting from the `init` task.");
MODULE_AUTHOR("Siddharth Arvind");

#define MODULE_NAME THIS_MODULE->name

// Color codes for printk()
#define PRINTK_COLORS
#include "printk_colors.h"


// This function takes a task's state and returns
// its meaning using the macros defined in <linux/sched.h>
static char *get_task_state(unsigned int state, 
	unsigned int exit_state)
{
	if (!state) 						return GRN("Running (R)");
	if (state & TASK_INTERRUPTIBLE) 	return CYN("Sleeping (S)");
	if (state & TASK_UNINTERRUPTIBLE) 	return YEL("Disk Sleep (D)");
	if (state & __TASK_STOPPED) 		return RED("Stopped (T)");
	if (exit_state & EXIT_ZOMBIE) 		return REDBG("Zombie (Z)");
	return MAG("Unknown");
}


// This function linearly iterates through the task list
// and prints each task's PID, name and state.
static void print_procs(void)
{
	struct task_struct *task;
	char *fmt;
	fmt = KERN_INFO "%s: " BLUBG("%s") "\t    " BLUBG("%s") \
			"\t\t   " BLUBG("%s") "\n";	
	printk(fmt, MODULE_NAME, "[  PID  ]", "[ NAME ]", "[ STATE ]");

	fmt = KERN_INFO "%s: [ %5d ]" WHT("%21s") "\t\t%15s\n";
	for_each_process(task) {
		printk(fmt, MODULE_NAME, task->pid, \
			task->comm, get_task_state(task->__state, \
			task->exit_state));
	}
}


// This function is called when the module is loaded.
int task_list_init(void)
{
	printk(KERN_INFO "%s: Loading module...\n\n", \
		MODULE_NAME);

	print_procs();
	printk("\n");

	return 0;
}


// This function is called when the module is removed.
void task_list_exit(void)
{
	printk(KERN_INFO "%s: Removing module...\n\n", \
		MODULE_NAME);
}


// Macros for registering module entry and exit points.
module_init(task_list_init);
module_exit(task_list_exit);
