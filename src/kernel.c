#include "os.h"

extern void uart_init(void);
extern void page_init(void);
extern void page_test(void);
extern void sched_init(void);
extern void schedule(void);
extern void user_init(void);
extern void trap_init(void);
extern void plic_init(void);
extern void timer_init(void);

void start_kernel(void)
{
  uart_init();
  uart_puts("Hello, RVOS!\n");

  page_init();
  /* page_test(); */

  trap_init();

  plic_init();

  timer_init();

  sched_init();

  user_init();

  schedule();
  /* while (1) { */
  /*   uart_puts("OS: Activate next task\n"); */
  /*   schedule(); */
  /*   uart_puts("OS: Back to OS\n"); */
  /*   uart_puts("\n"); */
  /* } */

  uart_puts("World not go here!\n");
  while (1) {
  }; // stop here!
}
