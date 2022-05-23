#include "os.h"

extern void schedule(void);

/* interval ~= 1s */
#define TIMER_INTERVAL CLINT_TIMEBASE_FREQ
#define MAX_TIMER 10
static struct timer timer_list[MAX_TIMER];

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
	struct timer *t = &(timer_list[0]);
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

struct timer *timer_create(void (*handler)(void *arg), void *arg, uint32_t timeout)
{
	if (NULL == handler || 0 == timeout)
	{
		return NULL;
	}

	// 在多任务系统中, 使用自旋锁来保护共享的计时器列表
	spin_lock();

	struct timer *t = &(timer_list[0]);
	for (int i = 0; i < MAX_TIMER; i++)
	{
		if (NULL == t->func) {
			break;
		}
		t++;
	}
	if (NULL != t->func) {
		spin_unlock();
		return NULL;
	}

	t->func = handler;
	t->arg = arg;
	t->timeout_tick = _tick + timeout;

	spin_unlock();

	return t;
}

void timer_delete(struct timer *timer)
{
	spin_lock();

	struct timer *t = &(timer_list[0]);
	for (int i = 0; i < MAX_TIMER; i++)
	{
		if (t == timer)
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
	struct timer *t = &(timer_list[0]);
	for (int i = 0; i < MAX_TIMER; i++)
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
		t++;
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
