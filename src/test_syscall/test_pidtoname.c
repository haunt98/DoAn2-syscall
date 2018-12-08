#include <linux/kernel.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/syscall.h>
#include <unistd.h>

#define MAX_NAME_SIZE 64
#define PIDTONAME_SYSCALL_NUMBER 356

int main()
{
	int pid, len;

	printf("Enter pid to find name process to find: ");
	scanf("%d", &pid);

	printf("Enter len of name process: ");
	scanf("%d", &len);
	if (len <= 0) {
		printf("len must be > 0");
		return 0;
	}

	char *name = (char *)malloc(sizeof(char) * len);
	int err = syscall(PIDTONAME_SYSCALL_NUMBER, pid, name, len);
	if (err == -1) {
		printf("There is error in copy to name or not found pid\n");
	} else if (err == 0) {
		printf("len is bigger than real length of name process\n");
	} else {
		printf("name process: %s\n", name);
		printf("bytes copied: %d\n", err);
	}
        free(name);

	return 0;
}
