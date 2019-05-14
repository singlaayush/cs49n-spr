/*
 * General functions we use.  These could be broken into multiple small
 * header files, but that's kind of annoying to context-switch through,
 * so we put all the main ones here.
 */
#ifndef __RPI_H__
#define __RPI_H__

// We are running without an OS, but these will get pulled from gcc's include's,
// not your laptops.
// 
// however, we don't want to do this too much, since unfortunately header files
// have a bunch of code we cannot run, which can lead to problems.
#include <stddef.h>
#include <stdint.h>

#include <string.h>
#if 0
/*
 * Not sure if we should include these so you don't have to mess up 
 * the namespace since string.h includes all sortas of stuff.
 */
int strcmp(const char *_p, const char *q);
void *memset(void *_p, int c, size_t n) ;
int memcmp(const void *_s1, const void *_s2, size_t nbytes);
void *memcpy(void *dst, const void *src, size_t nbytes);
int strncmp(const char* _s1, const char* _s2, size_t n);

char *strcat (char *dest, const char *src);
size_t strlen(const char *p);
char *strcpy(char * s1, const char * s2);
#endif

/*****************************************************************************
 * standard libc like functions for the pi.
 */
int rpi_putchar(int c);

int printk(const char *format, ...);
int snprintk(char *buf, size_t n, const char *fmt, ...);
int putk(const char *msg);

int uart_hex(unsigned h);

// a not very good rand()
unsigned short rpi_rand(void);

/*****************************************************************************
 * common device functions
 */

// uart functions
void uart_init(void);
int uart_getc(void);
void uart_putc(unsigned c);

/***************************************************************************
 * simple timer functions.
 */

// delays for <ticks> (each tick = a few cycles)
void delay_cycles(unsigned ticks) ;
// delay for <us> microseconds.
void delay_us(unsigned us) ;
// delay for <ms> milliseconds
void delay_ms(unsigned ms) ;

// returns time in usec.
// NOTE: this can wrap around!   do not do direct comparisons.
unsigned timer_get_usec(void) ;
 
// returns time in nanosec.
// NOTE: this can wrap around!   do not do direct comparisons.
unsigned timer_get_ns(void) ;

/****************************************************************************
 * Reboot the pi smoothly.
 */

// reboot the pi.
void rpi_reboot(void) __attribute__((noreturn));

// reboot after printing out a string to cause the unix my-install to shut down.
void clean_reboot(void) __attribute__((noreturn));


/*******************************************************************************
 * simple memory allocation: no free, just have to reboot().
 */

// returns 0-filled memory.
void *kmalloc(unsigned nbytes) ;

// currently no-ops.
void kfree(void *p);
void kfree_all(void);

// set where the heap starts.
void kmalloc_set_start(unsigned _addr);

/*****************************************************************************
 * memory related helpers
 */

// memory barrier.
void dmb(void);
// sort-of write memory barrier (more thorough).  dsb() >> dmb().
void dsb(void);

// use this if you need a device memory barrier.
void dev_barrier(void);
#define mb dev_barrier

// cache enable
void enable_cache(void) ;
void disable_cache(void) ;


/*****************************************************************************
 * Low-level code: you could do in C, but these are in assembly to defeat
 * the compiler.
 */
// *(unsigned *)addr = v;
void PUT32(unsigned addr, unsigned v);
void put32(volatile void *addr, unsigned v);

// *(unsigned char *)addr = v;
void PUT16(unsigned addr, unsigned short v);
void put16(volatile void *addr, unsigned short v);

// *(unsigned char *)addr = v;
void PUT8(unsigned addr, unsigned char v);
void put8(volatile void *addr, unsigned char v);

// *(unsigned *)addr
unsigned GET32(unsigned addr);
unsigned get32(const volatile void *addr);

// *(unsigned short *)addr
unsigned short GET16(unsigned addr);
unsigned short get16(const volatile void *addr);

// *(unsigned char *)addr
unsigned char GET8(unsigned addr);
unsigned char get8(const volatile void *addr);

// jump to <addr>
void BRANCHTO(unsigned addr);

// a no-op routine called to defeat the compiler.
void dummy(unsigned);

#include "cs49n-src/gpio.h"
#include "cs49n-libc/assert.h"

#endif
