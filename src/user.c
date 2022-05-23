#include "os.h"

#define DELAY 1000

extern void trap_test(void);

void os_kernel() {
  task_os();
}

void user_task0(void* param)
{
  uart_puts("Task 0: Created!\n");
  uart_puts("Task 0: Now, return to kernel mode!\n");
  os_kernel();
  while (1) {
    uart_puts("Task 0: Running...\n");

    /* trap_test(); */

    task_delay(DELAY);
    os_kernel();
  }
}

void user_task1(void* param)
{
  uart_puts("Task 1: Created!\n");
  uart_puts("Task 1: Now, return to kernel mode!\n");
  os_kernel();
  while (1) {
    uart_puts("Task 1: Running...\n");
    task_delay(DELAY);
    os_kernel();
  }
}

void user_task2(void* param)
{
  uart_puts("Task 2: Created!\n");
  uart_puts("Task 2: Now, return to kernel mode!\n");
  os_kernel();
  while (1) {
    uart_puts("Task 2: Running...\n");
    task_delay(DELAY);
    uart_puts("Task 2: Exiting...\n");
    task_exit();
  }
}

void user_task3(void* param)
{
  uart_puts("Task 3: Created!\n");
  uart_puts("Task 3: Now, return to kernel mode!\n");
  os_kernel();
  while (1) {
    uart_puts("Task 3: Running...\n");
    task_delay(DELAY);
    uart_puts("Task 3: Exiting...\n");
    task_exit();
  }
}

/* NOTICE: DON'T LOOP INFINITELY IN main() */
void user_init(void)
{
  task_create(user_task0, NULL, 1);
  task_create(user_task1, NULL, 1);
  task_create(user_task2, NULL, 1);
  task_create(user_task3, NULL, 1);
}

