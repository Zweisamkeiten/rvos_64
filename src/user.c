#include "os.h"

#define DELAY 4000

/* #define USE_LOCK */
struct userdata {
	int counter;
	char *str;
};

struct userdata person1 = {0, "A"};
struct userdata person2 = {0, "B"};
struct userdata person3 = {0, "C"};

void timer_func(void *arg)
{
	if (NULL == arg)
		return;

	struct userdata *param = (struct userdata *)arg;

	param->counter++;
	printf("=====> TIMEOUT: %s: %d\n", param->str, param->counter);
}

extern void trap_test(void);

void os_kernel() {
  task_os();
}

void user_task0(void* param)
{
  uart_puts("Task 0: Created!\n");
	timer *t1 = timer_create(timer_func, &person1, 5);
	if (NULL == t1) {
		printf("timer_create() failed!\n");
	}
	timer *t2 = timer_create(timer_func, &person2, 3);
	if (NULL == t2) {
		printf("timer_create() failed!\n");
	}
	timer *t3 = timer_create(timer_func, &person3, 7);
	if (NULL == t3) {
		printf("timer_create() failed!\n");
	}
  while (1) {
#ifdef USE_LOCK
		spin_lock();
#endif
		uart_puts("Task 0: Running ... \n");
		task_delay(DELAY);
		/* uart_puts("Task 0: Begin ... \n"); */
		/* for (int i = 0; i < 5; i++) { */
		/* 	uart_puts("Task 0: Running... \n"); */
		/* 	task_delay(DELAY); */
		/* } */
		/* uart_puts("Task 0: End ... \n"); */
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
		/* uart_puts("Task 2: Begin ... \n"); */
		/* for (int i = 0; i < 5; i++) { */
			uart_puts("Task 2: Running... \n");
			task_delay(DELAY);
		/* } */
		/* uart_puts("Task 2: End ... \n"); */
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

