#include "os.h"

extern void schedule(void);

/* interval ~= 1s */
#define TIMER_INTERVAL CLINT_TIMEBASE_FREQ
#define MAX_TIMER 10
static timer timer_list[MAX_TIMER];
static timer* list_start = &(timer_list[0]);
static timer* list_end = &(timer_list[0]);

static uint32_t _tick = 0;

/* load timer interval(in ticks) for next timer interrupt.*/
void timer_load(int interval)
{
	/* each CPU has a separate source of timer interrupts. */
	int id = r_mhartid();
	
	*(uint64_t*)CLINT_MTIMECMP(id) = *(uint64_t*)CLINT_MTIME + interval;
}

void timer_init()
{
	timer *t = list_start;
	for (int i = 0; i < MAX_TIMER; i++) {
		t->func = NULL;
		t->arg = NULL;
		t++;
	}
	/*
	 * On reset, mtime is cleared to zero, but the mtimecmp registers 
	 * are not reset. So we have to init the mtimecmp manually.
	 */
	timer_load(TIMER_INTERVAL);

	/* enable machine-mode timer interrupts. */
	w_mie(r_mie() | MIE_MTIE);

}

timer *timer_create(void (*handler)(void *arg), void *arg, uint32_t timeout)
{
	if (NULL == handler || 0 == timeout)
	{
		return NULL;
	}

	// 在多任务系统中, 使用自旋锁来保护共享的计时器列表
	spin_lock();

	timer *t = list_start;
	if (list_end == NULL)
	{
		return NULL;
	}
	list_end->func = handler;
	list_end->arg = arg;
	list_end->timeout_tick = _tick + timeout;
	while (t != list_end)
	{
		if (list_end->timeout_tick < t->timeout_tick && t == list_start)
		{
			list_end->next = list_start;
			list_start = list_end;
			break;
		}
		else if (t->next == NULL)
		{
			t->next = list_end;
			break;
		}
		else if (list_end->timeout_tick < t->next->timeout_tick)
		{
			list_end->next = t->next;
			t->next = list_end;
			break;
		}
		t = t->next;
	}
	list_end++;

	spin_unlock();

	return t;
}

void timer_delete(timer *thetimer)
{
	spin_lock();

	timer *t = list_start;
	for (int i = 0; i < MAX_TIMER; i++)
	{
		if (t == thetimer)
		{
			t->func = NULL;
			t->arg = NULL;

			break;
		}
		t++;
	}
}

static inline void timer_check()
{
	timer *t = list_start;
	while (t != NULL)
	{
		if (NULL != t->func)
		{
			if (_tick >= t->timeout_tick)
			{
				t->func(t->arg);

				t->func = NULL;
				t->arg = NULL;

				break;
			}
		}
		t = t->next;
	}
}

void timer_handler() 
{
	_tick++;
	int hour, minute, second;
	second = _tick % 60;
	minute = _tick / 60 % 60;
	hour = _tick / 60 / 60 % 60;
	printf("\r%02d:%02d:%02d\n", hour, minute, second);

	timer_check();

	timer_load(TIMER_INTERVAL);
	schedule();
}
