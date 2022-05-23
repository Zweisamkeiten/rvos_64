#include "os.h"

#define DELAY 4000

#define USE_LOCK

extern void trap_test(void);

void os_kernel() {
  task_os();
}

void user_task0(void* param)
{
  uart_puts("Task 0: Created!\n");
  /* uart_puts("Task 0: Now, return to kernel mode!\n"); */
  /* os_kernel(); */
  while (1) {
#ifdef USE_LOCK
		spin_lock();
#endif
		uart_puts("Task 0: Begin ... \n");
		for (int i = 0; i < 5; i++) {
			uart_puts("Task 0: Running... \n");
			task_delay(DELAY);
		}
		uart_puts("Task 0: End ... \n");
#ifdef USE_LOCK
		spin_unlock();
#endif
  }
}

void user_task1(void* param)
{
  uart_puts("Task 1: Created!\n");
  while (1) {
		uart_puts("Task 1: Begin ... \n");
		for (int i = 0; i < 5; i++) {
			uart_puts("Task 1: Running... \n");
			task_delay(DELAY);
		}
		uart_puts("Task 1: End ... \n");
  }
}

void user_task2(void* param)
{
  uart_puts("Task 2: Created!\n");
  while (1) {
		uart_puts("Task 2: Begin ... \n");
		for (int i = 0; i < 5; i++) {
			uart_puts("Task 2: Running... \n");
			task_delay(DELAY);
		}
		uart_puts("Task 2: End ... \n");
  }
}

void user_task3(void* param)
{
  uart_puts("Task 3: Created!\n");
  while (1) {
		uart_puts("Task 3: Begin ... \n");
		for (int i = 0; i < 5; i++) {
			uart_puts("Task 3: Running... \n");
			task_delay(DELAY);
		}
		uart_puts("Task 3: End ... \n");
  }
}

/* NOTICE: DON'T LOOP INFINITELY IN main() */
void user_init(void)
{
	task_create(user_task0, NULL, 1, 1);
	task_create(user_task1, NULL, 1, 2);
	task_create(user_task2, NULL, 2, 2);
	task_create(user_task3, NULL, 2, 4);
}

