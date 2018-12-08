#include <linux/init.h>
#include <linux/mm.h>
#include <linux/mman.h>
#include <linux/module.h>
#include <linux/sched.h>
#include <linux/syscalls.h>
#include <linux/unistd.h>

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Nguyen Tran Hau - Nguyen Chi Thuc");
MODULE_DESCRIPTION("Hook open and write syscall");

// raw access to syscall table
unsigned long **syscall_table;

asmlinkage int (*old_open)(const char __user *, int, mode_t);
asmlinkage int (*old_write)(unsigned int, const char __user *, size_t);

asmlinkage int new_open(const char __user *pathname, int flags, mode_t mode)
{
	printk(KERN_INFO "HOOK open syscall\n");
	printk(KERN_INFO "Process name: %s\n", current->comm);
	printk(KERN_INFO "File open: %s\n", pathname);

	return (*old_open)(pathname, flags, mode);
}

asmlinkage int new_write(unsigned int fd, const char __user *buf, size_t count)
{
	int bytes_write;

	printk(KERN_INFO "HOOK write syscall\n");
	printk(KERN_INFO "Process name: %s\n", current->comm);

	bytes_write = (*old_write)(fd, buf, count);
	printk(KERN_INFO "Bytes write: %d\n, ", bytes_write);

	return bytes_write;
}

static void get_syscall_table(void)
{
	unsigned long int offset = PAGE_OFFSET;

	while (offset < ULLONG_MAX) {
		unsigned long **temp_syscall_table = (unsigned long **)offset;
		// found system syscall table
		if (temp_syscall_table[__NR_close] ==
		    (unsigned long *)sys_close) {
			syscall_table = temp_syscall_table;
			return;
		}

		offset += sizeof(void *);
	}
	syscall_table = NULL;
}

static void enable_write(void)
{
	write_cr0(read_cr0() & ~0x10000);
}

static void disable_write(void)
{
	write_cr0(read_cr0() | 0x10000);
}

static int init_hook(void)
{
	printk(KERN_INFO "HOOK module init\n");

	get_syscall_table();
	if (syscall_table == NULL) {
		printk(KERN_ERR "HOOK not found syscall table\n");
		return -1;
	}
	// backup syscall
	old_open = (void *)syscall_table[__NR_open];
	old_write = (void *)syscall_table[__NR_write];

	enable_write();

	// override syscall
	syscall_table[__NR_open] = (unsigned long *)new_open;
	syscall_table[__NR_write] = (unsigned long *)new_write;

	disable_write();
	return 0;
}

static void exit_hook(void)
{
	printk(KERN_INFO "HOOK module exit\n");

	// tim duoc syscall table
	if (syscall_table != NULL) {
		enable_write();

		// restore syscall
		syscall_table[__NR_open] = (unsigned long *)old_open;
		syscall_table[__NR_write] = (unsigned long *)old_write;

		disable_write();
	}
}

module_init(init_hook);
module_exit(exit_hook);
