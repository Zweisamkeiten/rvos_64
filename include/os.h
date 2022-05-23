#ifndef __OS_H__
#define __OS_H__

#include "types.h"
#include "platform.h"
#include "riscv.h"

#include <stddef.h>
#include <stdarg.h>

/* uart */
extern int uart_putc(char ch);
extern void uart_puts(char *s);
extern int uart_getc(void);

/* printf */
extern int  printf(const char* s, ...);
extern void panic(char *s);

/* memory management */
extern void *malloc(size_t size);
extern void free(void *ptr);

struct task {
	uint8_t priority;
	char state;
};

extern void task_os();
extern void task_go(int n);
extern int  task_create(void (*task)(void* param), void *param, uint8_t priority);
extern void task_delay(volatile int count);
extern void task_exit(void);

extern int plic_claim(void);
extern void plic_complete(int irq);

#endif /* __OS_H__ */