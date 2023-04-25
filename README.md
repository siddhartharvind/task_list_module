# Linux Kernel Module - Task List Module

This is a basic Linux kernel module whose job is to list all current
   tasks/processes in the system, starting with the *init* task. It
   will display the PID, name, and state of each task. It can iterate
   through the process tree in either linear (in order of PIDs;
   i.e. breadth-first search or BFS), or in depth-first search
   (DFS) order.


## Steps and Explanation:
### 1. Creating kernel module
To list all currently-loaded kernel modules:
```shell
$ lsmod
```

A basic kernel module could look like: <br/>
*task_list_module.c*
```c
#include <linux/init.h>
#include <linux/kernel.h>
#include <linux/module.h>

MODULE_LICENSE("GPL");
MODULE_DESCRIPTION("This is a task list module.")
MODULE_AUTHOR("John Doe");

// Called when the module is loaded.
int task_list_init(void) { printk(KERN_INFO "Loading module...\n");
  return 0;
}

// Called when the module is removed.
void task_list_exit(void) { printk(KERN_INFO "Removing module...\n");
}

// Macros for registering module entry and exit points
module_init(task_list_init);
module_exit(task_list_exit);

```

This module prints messages upon being loaded and removed. `printk()`
  sends output to the kernel log buffer, which can then be read
  using `$ dmesg`.

A basic Makefile to compile it: <br/>
*Makefile*
```Makefile
obj-m += task_list_module.o

all: make -C /lib/modules/$(shell uname -r)/build M=$(PWD) modules

clean: make -C /lib/modules/$(shell uname -r)/build M=$(PWD) clean

```

Running `$ make` produces various files, one of which is
  `task_list_module.ko`, representing the compiled kernel module.

<br/>

### 2. Loading and Removing Modules
The module is loaded into the kernel using the `$ insmod` command:

```shell
$ sudo insmod task_list_module.ko
```

We can then check if the module was loaded by:
```shell
$ lsmod | grep task_list_module
```

When the module has been loaded in, its module entry point function
  will have been invoked. We can check for this in the kernel
  log buffer:
```shell
$ sudo dmesg
```
The line 'Loading module...' (from the module entry point function)
  should be displayed at the end.

The loaded module can then be removed using `$ rmmod`:
```shell
$ sudo rmmod task_list_module
```
The line 'Removing module...' (from the module exit point function)
  should be displayed at the end.

<br/>

### 3. Displaying task details
For displaying the details of tasks, we can use:
- `struct task_struct`  - defined in `<linux/sched.h>`. This has
  various members, of which we are interested in:

    - `unsigned int __state`
    - `pid_t pid`
    - `char comm[TASK_COMM_LEN]`
      - `TASK_COMM_LEN` (also declared in `<linux/sched.h>`) = 16

- `for_each_process()` - macro; defined in `<linux/sched/signal.h>`
- `list_for_each()` - macro; defined in `<linux/list.h>` (not
  explicitly `#include`d)

For this module, we are only considering 5 possible states for a task:
- (R) Running / Runnable
- (S) Interruptible Sleep
- (D) Uninterruptible Sleep / Disk Sleep
- (T) Stopped
- (Z) Zombie

Each of these is associated with a flag, which are all defined in
  `<linux/sched.h>`.

| Macro                  | Flag |
|------------------------|------|
| `TASK_RUNNING`         | 0x00 |
| `TASK_INTERRUPTIBLE`   | 0x01 |
| `TASK_UNINTERRUPTIBLE` | 0x02 |
| `__TASK_STOPPED`       | 0x04 |
| `EXIT_ZOMBIE`          | 0x20 |

<br/>

#### 3A. In linear order
We can iterate through the process tree in linear (i.e. in ascending
  order of PIDs) or breadth-first search (BFS) order by using
  the `for_each_process()` macro.
```c
void print_tasks_linear(void) {
  struct task_struct *task;

  for_each_process(task) {
    printk(KERN_INFO "%u %s %u\n",
      task->pid, task->comm, task->__state);
  }
}

```

<br/>

#### 3B. In DFS order
We can iterate through the process tree in depth-first search (DFS)
   order by using the `list_for_each()` and `list_entry()` macros.
```c
void print_tasks_dfs(struct task_struct *task) {
  printk(KERN_INFO "%u %s %u\n",
    task->pid, task->comm, task->__state);

  // DFS on current task's children
  struct task_struct *child_task;
  struct list_head   *list;

  list_for_each(list, &task->children) {
    child_task = list_entry(list, struct task_struct, sibling);
    print_tasks_dfs(child_task); // recursive call
  }
}

print_tasks_dfs(init_task);

```

<br/>


### 4. Demonstration
For demonstrating the module in action, we use the script `demo.sh`.
It runs the Makefile to build the module, load it, remove it, and
   print the output from the kernel log buffer.
Additionally, before it runs `$ make`, the script spawns a process
   and stops it (`$ kill -SIGSTOP`) to create a stopped process. It
   also uses `zombie.c` to create a program that spawns a zombie
   process. Both of these should be visible in the module output
   during the demo.
The demo script is recommended for automating all of the above
   tasks. However, the steps can still be manually done through
   the Makefile.
By default, the demo script will call `make` to set the module to
   produce colourized output. This can be disabled by omitting the
   `colors=1` flag when calling `make`.


### 5. Notes
This module only checks for 5 possible states for a particular
   task. Additionally, it does not do any exhaustive checking; the
   first flag that it matches is set as the task state name. This
   means that combinations of states are not recognized.


### 6. References
- Operating System Concepts: International Student Version, 9th
  Edition, by A. Silberschatz, P. B. Galvin, and G. Gagne
  - The code used for the linear and DFS searches was
  inspired by the snippets given for Chapter 3: Process Concept,
  Project 2.

- Linux kernel source tree
  - [&lt;linux/sched.h&gt;](https://github.com/torvalds/linux/blob/master/include/linux/sched.h)
  - [&lt;linux/sched/signal.h&gt;](https://github.com/torvalds/linux/blob/master/include/linux/sched/signal.h)


### 7. Licence
The source code of the kernel module itself, `task_list_module.c`,
  is licensed under the GNU General Public License, version 2
  or later.
All other source files, excluding `task_list_module.c`, are licensed
  under the Apache 2.0 License.

Copyright (c) Siddharth Arvind, 2023.
