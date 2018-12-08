#include <linux/syscalls.h>
#include <linux/kernel.h>
#include <linux/sched.h>
#include <linux/init.h>
#include <linux/string.h>
#include "pnametoid.h"

// get pid from name process
// -1   not found
// otherwise return first pid founded
asmlinkage int sys_pnametoid(char *name)
{
	struct task_struct *task;

	// iterate all task
	for_each_process (task) {
		// compare name
		if (strcmp(task->comm, name) == 0) {
			return task_pid_nr(task);
		}
	}
	return -1;
}
