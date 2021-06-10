#include <phoenix/kernel.h>

__attribute__((__noreturn__))
void panic(char *msg)
{
	printk(KERN_FATAL, "Kernel Panic !\n");
    printk(KERN_FATAL, "%s", msg);
	while (1);
	__builtin_unreachable();
}
