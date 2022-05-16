#ifndef __OS_H__
#define __OS_H__

#include "types.h"
#include "platform.h"

#include <stddef.h>
#include <stdarg.h>

/* uart */
extern int uart_putc(char ch);
extern void uart_puts(char *s);

/* printf */
extern int  printf(const char* s, ...);
extern void panic(char *s);

/* memory management */
extern void *malloc(size_t size);
extern void free(void *ptr);

extern int  task_create(void (*task)(void));
extern void task_delay(volatile int count);

#endif /* __OS_H__ */
