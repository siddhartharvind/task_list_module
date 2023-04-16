#include <linux/init.h>
#include <linux/kernel.h>
#include <linux/module.h>

#include <linux/sched.h>
#include <linux/sched/signal.h>


// This function takes a task's state and returns
// its meaning using the macros defined in
// <linux/sched.h>
static char *get_task_state(unsigned int state, 
	unsigned int exit_state)
{
	if (!state) 						return "Running (R)";
	if (state & TASK_INTERRUPTIBLE) 	return "Sleeping (S)";
	if (state & TASK_UNINTERRUPTIBLE) 	return "Disk Sleep (D)";
	if (state & __TASK_STOPPED) 		return "Stopped (T)";
	if (exit_state & EXIT_ZOMBIE) 		return "Zombie (Z)";
	return "Unknown";
}

// This function iterates through the task list
// and prints each task's PID, name and state.
static void print_procs(void)
{
	printk(KERN_INFO "%s: [ %5s ]\t[ %15s ]\t[ %14s ]\n", \
		THIS_MODULE->name, "PID", "NAME", "STATE");
	struct task_struct *task;
	for_each_process(task) {
		printk(KERN_INFO "%s: [ %5d ]\t %16s\t %15s\n", \
			THIS_MODULE->name, task->pid, task->comm, \
			get_task_state(task->__state, task->exit_state)
		);
	}
}

// This function is called when the module is loaded.
int task_list_init(void)
{
	printk(KERN_INFO "%s: Loading module...\n\n", \
		THIS_MODULE->name);

	print_procs();
	printk("\n");

	return 0;
}

// This function is called when the module is removed.
void task_list_exit(void)
{
	printk(KERN_INFO "%s: Removing module...\n\n", \
		THIS_MODULE->name);
}

// Macros for registering module entry and exit points.
module_init(task_list_init);
module_exit(task_list_exit);

MODULE_LICENSE("GPL");
MODULE_DESCRIPTION("This is a simple Linux kernel module \
	that lists all the current tasks in a Linux system, \
	starting from the `init` task.");
MODULE_AUTHOR("Siddharth Arvind");
