#include <phoenix/kernel.h>

__attribute__((__noreturn__))
void panic(void)
{
	printk(KERN_FATAL, "Kernel Panic !\n");
	while (1);
	__builtin_unreachable();
}
