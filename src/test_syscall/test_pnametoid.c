#include <linux/kernel.h>
#include <stdio.h>
#include <string.h>
#include <sys/syscall.h>
#include <unistd.h>

#define MAX_NAME_SIZE 64
#define PNAMETOID_SYSCALL_NUMBER 355

int main()
{
	printf("Enter name process to find pid: ");

	char name[MAX_NAME_SIZE];
	fgets(name, MAX_NAME_SIZE, stdin);
	// remove \n of name
	int len_name = strlen(name);
	if (len_name > 0 && name[len_name - 1] == '\n') {
		name[len_name - 1] = '\0';
	}

	int pid = syscall(PNAMETOID_SYSCALL_NUMBER, name);
	printf("pid: %d\n", pid);

	return 0;
}
