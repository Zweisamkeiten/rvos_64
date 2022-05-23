#include "os.h"

void plic_init(void)
{
	int hart = r_tp();

	// Set priority for UART0
	*(uint32_t*)PLIC_PRIORITY(UART0_IRQ) = 1;

	// Enable UART0
	*(uint32_t*)PLIC_MENABLE(hart) = (1 << UART0_IRQ);

	// Set priority threshold for UART0
	*(uint32_t*)PLIC_MTHRESHOLD(hart) = 0;

	// enable machine-mode external interrupts
	w_mie(r_mie() | MIE_MEIE);

	// enable machine-mode global interrupts
	w_mstatus(r_mstatus() | MSTATUS_MIE);
}

int plic_claim(void)
{
	int hart = r_tp();
	int irq = *(uint32_t*)PLIC_MCLAIM(hart);
	return irq;
}

void plic_complete(int irq)
{
	int hart = r_tp();
	*(uint32_t*)PLIC_MCOMPLETE(hart) = irq;
}
