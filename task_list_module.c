#include <linux/init.h>
#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/sched.h>
#include <linux/sched/signal.h>


MODULE_LICENSE("GPL");
MODULE_DESCRIPTION(
    "This is a simple Linux kernel module "
    "that lists all the current tasks in a Linux system, "
    "starting from the `init` process."
);
MODULE_AUTHOR("Siddharth Arvind");
#define MODULE_NAME THIS_MODULE->name


// Color codes for printk()
#include "printk_colors.h"


// This function takes a task's state and returns
// its meaning using the macros defined in <linux/sched.h>
static
char *get_task_state_name(unsigned int state, unsigned int exit_state)
{
    if (     state == TASK_RUNNING        )  return   GRN( "Running (R)"    );
    if (     state  & TASK_INTERRUPTIBLE  )  return   CYN( "Sleeping (S)"   );
    if (     state  & TASK_UNINTERRUPTIBLE)  return   YEL( "Disk Sleep (D)" );
    if (     state  & __TASK_STOPPED      )  return   RED( "Stopped (T)"    );
    if (exit_state  & EXIT_ZOMBIE         )  return REDBG( "Zombie (Z)"     );

    return MAG("Unknown");
}



// This function prints out the header row for the task
// details to be printed below it.
static
void print_task_header_row(void)
{
    static const char *const fmt = KERN_INFO    \
        "%s: " BLUBG("%s") "\t    " BLUBG("%s") \
        "\t\t   " BLUBG("%s") "\n";

    printk(fmt, MODULE_NAME,
        "[  PID  ]",
        "[ NAME ]",
        "[ STATE ]"
    );
}


// This function prints out the details of a given task:
// PID, name, and state.
static
void print_task_details(struct task_struct *task)
{
    static const char *const fmt = KERN_INFO \
        "%s: [ %5d ]" WHT("%21s") "\t\t%15s\n";

    printk(fmt, MODULE_NAME,
        task->pid,
        task->comm,
        get_task_state_name(task->__state, task->exit_state)
    );
}



#if defined TASK_LIST_MODE_DFS


// This is a recursive helper function that iterates through
// the child tasks of a given task using depth-first search (DFS).
static
void print_processes_dfs_helper(struct task_struct *task)
{
    print_task_details(task);

    // DFS on current task's children
    struct task_struct *child_task;
    struct list_head   *list;

    list_for_each(list, &task->children) {
        child_task = list_entry(list, struct task_struct, sibling);
        print_processes_dfs_helper(child_task);
    }
}


// This function iterates through the task list using depth-first search
// (DFS) and prints each task's PID, name and state.
static
void print_processes_dfs(void)
{
    print_task_header_row();

    // Print process tree
    print_processes_dfs_helper(&init_task);
}


#else // if ! defined TASK_LIST_MODE_DFS


// This function linearly iterates through the task list
// and prints each task's PID, name and state.
static
void print_processes_linear(void)
{
    print_task_header_row();

    struct task_struct *task;
    for_each_process(task) {
        print_task_details(task);
    }
}


#endif // if defined TASK_LIST_MODE_DFS



// This function is called when the module is loaded.
static
int task_list_init(void)
{
    printk(KERN_INFO "%s: Loading module...\n\n", \
        MODULE_NAME);

    // Default behaviour: print in linear order.
    // If TASK_LIST_MODE_DFS is defined,
    // print in DFS order instead.

    #if defined TASK_LIST_MODE_DFS
        print_processes_dfs();

    #else // if ! defined TASK_LIST_MODE_DFS
        print_processes_linear();

    #endif

    printk("\n");

    return 0;
}


// This function is called when the module is removed.
static
void task_list_exit(void)
{
    printk(KERN_INFO "%s: Removing module...\n\n", \
        MODULE_NAME);
}


// Macros for registering module entry and exit points.
module_init(task_list_init);
module_exit(task_list_exit);
