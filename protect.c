#include <stdio.h>
#include <unistd.h>
#include <sys/mman.h>
#include <errno.h>
#include <string.h>
#include <signal.h>

extern int errno;

void sighdl(int s) {
	printf("Handling signal.\n");
	_exit(1);
}

#define PAGE_SIZE (0xffffffff << 12)
int function(int f) {
	return f + 7;
}

int function2(int f) {
	return f + 7;
}

int main() {
	int (*newf)(int) = NULL;
	int function_size = ((unsigned long int)&function2) - ((unsigned long int)&function);
	signal(SIGSEGV, sighdl);

	newf = (int (*)(int))memalign(4096, function_size);
	memcpy(newf, &function, function_size);

	printf("&sighdl: %p\n", &sighdl);
	printf("&function: %p\n", &function);
	printf("PAGE_SIZE: 0x%x\n", PAGE_SIZE);
	printf("&function: %p\n", (void*)(((unsigned long int)&function) & (PAGE_SIZE)));
	if (-1 == mprotect(newf,
		//(void*)(((unsigned long int)&function) & (PAGE_SIZE)),
		((unsigned long int)&function2) - ((unsigned long int)&function),
		PROT_NONE)) {
		printf("mmap: %s\n", strerror(errno));
	}
	printf("function(7): %d\n", (*newf)(7));
	return 0;
}
