#ifndef _IO_H_
#define _IO_H_

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>

static inline void cli(void);
static inline void sti(void);
static inline uint8_t inb(uint16_t port);
static inline uint16_t inw(uint16_t port);
static inline uint8_t inb_p(uint16_t port);
static inline uint16_t inw_p(uint16_t port);
static inline void outb(uint16_t port, uint8_t val);
static inline void outw(uint16_t port, uint16_t val);
static inline void outb_p(uint16_t port, uint8_t val);
static inline void outw_p(uint16_t port, uint16_t val);
static inline void io_wait(void);

static inline void cli(void)
{
    asm volatile("cli" ::);
}

static inline void sti(void)
{
    asm volatile("sti" ::);
}

static inline uint8_t inb(uint16_t port)
{
    uint8_t ret;
    asm volatile("inb %1, %0" : "=a"(ret) : "Nd"(port));
    return ret;
}

static inline uint16_t inw(uint16_t port)
{
    uint16_t ret;
    asm volatile("inw %1, %0" : "=a"(ret) : "Nd"(port));
    return ret;
}

static inline uint8_t inb_p(uint16_t port)
{
    uint8_t ret;
    asm volatile("inb %1, %0\noutb %%al, $0x80" : "=a"(ret) : "Nd"(port));
    return ret;
}

static inline uint16_t inw_p(uint16_t port)
{
    uint16_t ret;
    asm volatile("inw %1, %0\noutb %%al, $0x80" : "=a"(ret) : "Nd"(port));
    return ret;
}

static inline void outb(uint16_t port, uint8_t val)
{
    asm volatile("outb %0, %1" : : "a"(val), "Nd"(port));
}

static inline void outw(uint16_t port, uint16_t val)
{
    asm volatile("outw %0, %1" : : "a"(val), "Nd"(port));
}

static inline void outb_p(uint16_t port, uint8_t val)
{
    asm volatile("outb %0, %1\noutb %%al,$0x80" : : "a"(val), "Nd"(port));
}

static inline void outw_p(uint16_t port, uint16_t val)
{
    asm volatile("outw %0, %1\noutb %%al,$0x80" : : "a"(val), "Nd"(port));
}


static inline void io_wait(void)
{
    asm volatile("outb %%al, $0x80" : : "a"(0));
}

#ifdef __cplusplus
}
#endif

#endif
