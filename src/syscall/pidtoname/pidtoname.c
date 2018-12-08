#include <linux/syscalls.h>
#include <linux/kernel.h>
#include <linux/sched.h>
#include <linux/init.h>
#include <linux/string.h>
#include <linux/uaccess.h> // for function copy_to_user()
#include "pidtoname.h"

// get pid from name procress
// -1   error or not found
// 0    len is bigger than real length of name process
// n    n is real len of name procress
asmlinkage int sys_pidtoname(int pid, char *buf, int len)
{
	struct task_struct *task;

	// iterate all task
	for_each_process (task) {
		if (task_pid_nr(task) == pid) {
			if (len > strlen(task->comm)) {
				return 0;
			}
			// return bytes could not be copied
			// success return 0
			int bytes_not_copied =
				copy_to_user(buf, task->comm, len);
                        // if success, len bytes is copied
			if (bytes_not_copied == 0) {
				return len;
			} else {
				return -1;
			}
		}
	}
	return -1;
}
