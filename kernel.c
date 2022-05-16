#include "os.h"

extern void uart_init(void);
extern void page_init(void);
extern void page_test(void);
extern void sched_init(void);
extern void schedule(void);

void start_kernel(void)
{
  uart_init();
  uart_puts("Hello, RVOS!\n");

  page_init();
  page_test();

  sched_init();
  schedule();

  uart_puts("World not go here!\n");
  while (1) {
  }; // stop here!
}
