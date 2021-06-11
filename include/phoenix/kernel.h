#ifndef __KERNEL_H_
#define __KERNEL_H_

#ifdef __cplusplus
extern "C" {
#endif

#include <stivale2.h>
#include <stdint.h>
#include <stddef.h>

#define KERN_VERBOSE 4
#define KERN_INFO 3
#define KERN_WARN 2
#define KERN_ERROR 1
#define KERN_FATAL 0

#define INT_LENGTH 8

/* Printing and Formating */
int printk(uint8_t severity, const char* restrict format, ...);
int putchar(int ic, uint8_t severity);
int puts(uint8_t severity, const char* string);
int convert_int_to_char(int number, int base, char* buff);

/* Macros for printk */
#define verbose(...)    printk(KERN_VERBOSE, __VA_ARGS__)
#define info(...)       printk(KERN_INFO, __VA_ARGS__)
#define warn(...)       printk(KERN_WARN, __VA_ARGS__)
#define error(...)      printk(KERN_ERROR, __VA_ARGS__)

/* Basic Functions */
int abs(int x);
int atoi(const char* nptr);
char* itoa(int value, char* buffer, int base);
__attribute__((__noreturn__))
void panic(const char *msg);

/* Kernel entry points */
void init(struct stivale2_struct* hdr);
void kernel_main(void);

#ifdef __cplusplus
}
#endif

#endif /*__KERNEL_H_*/
