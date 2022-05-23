#ifndef __PLATFORM_H__
#define _PLATFORM_H__

/*
 * QEMU RISC-V Virt machine with 16550a UART and VirtIO MMIO
 */

/*
 * maximum number of CPUs
 * see https://github.com/qemu/qemu/blob/master/include/hw/riscv/virt.h
 * #define VIRT_CPUS_MAX 8
 */
#define MAXNUM_CPU 8

/*
 * MemoryMap
 * see https://github.com/qemu/qemu/blob/master/hw/riscv/virt.c, virt_memmap[]
 * 0x00001000 -- boot ROM, provided by qemu
 * 0x02000000 -- CLINT
 * 0x0C000000 -- PLIC
 * 0x10000000 -- UART0
 * 0x10001000 -- virtio disk
 * 0x80000000 -- boot ROM jumps here in machine mode, where we load our kernel
 */

/* This machine puts UART registers here in physical memory. */
#define UART0 0x10000000L

typedef uint64_t reg_t; // RISCV64: register is 64bits
/* task management */
struct context {
  /* ignore x0 */
  reg_t ra;
  reg_t sp;
  reg_t gp;
  reg_t tp;
  reg_t t0;
  reg_t t1;
  reg_t t2;
  reg_t s0;
  reg_t s1;
  reg_t a0;
  reg_t a1;
  reg_t a2;
  reg_t a3;
  reg_t a4;
  reg_t a5;
  reg_t a6;
  reg_t a7;
  reg_t s2;
  reg_t s3;
  reg_t s4;
  reg_t s5;
  reg_t s6;
  reg_t s7;
  reg_t s8;
  reg_t s9;
  reg_t s10;
  reg_t s11;
  reg_t t3;
  reg_t t4;
  reg_t t5;
  reg_t t6;
};

/*
 * UART0 interrupt source
 * see https://github.com/qemu/qemu/blob/master/include/hw/riscv/virt.h
 * enum {
 *     UART0_IRQ = 10,
 *     ......
 * };
 */
#define UART0_IRQ 10

/*
 * This machine puts platform-level interrupt controller (PLIC) here.
 * Here only list PLIC registers in Machine mode.
 * see https://github.com/qemu/qemu/blob/master/include/hw/riscv/virt.h
 * #define VIRT_PLIC_HART_CONFIG "MS"
 * #define VIRT_PLIC_NUM_SOURCES 127
 * #define VIRT_PLIC_NUM_PRIORITIES 7
 * #define VIRT_PLIC_PRIORITY_BASE 0x04
 * #define VIRT_PLIC_PENDING_BASE 0x1000
 * #define VIRT_PLIC_ENABLE_BASE 0x2000
 * #define VIRT_PLIC_ENABLE_STRIDE 0x80
 * #define VIRT_PLIC_CONTEXT_BASE 0x200000
 * #define VIRT_PLIC_CONTEXT_STRIDE 0x1000
 * #define VIRT_PLIC_SIZE(__num_context) \
 *     (VIRT_PLIC_CONTEXT_BASE + (__num_context) * VIRT_PLIC_CONTEXT_STRIDE)
 */
/*
 * PLIC 地址基址为 0x0c000000
 * 每个 PLIC 中断源对应一个寄存器, 用于配置该中断源的优先级
 * 每个 PLIC包含 2 个 32位 Pending 寄存器, 每个 bit 对应一个中断源, 1 表示该中断源发生中断
 * id / 32 整除得到第一还是第二寄存器
 * 每个 Hart 有 2 个 Enable 寄存器, 用于针对该 Hart 启动或关闭某路中断源
 * 每个 Hart 有一个 Threshold 寄存器用于设置中断优先级的阈值
 * Claim 和 Complete 是同一个寄存器, 每个 Hart 一个
 */
#define PLIC_BASE 0x0c000000L
#define PLIC_PRIORITY(id) (PLIC_BASE + (id) * 4)
#define PLIC_PENDING(id) (PLIC_BASE + 0x1000 + ((id) / 32) * 4)
#define PLIC_MENABLE(hart) (PLIC_BASE + 0x2000 + (hart) * 0x80)
#define PLIC_MTHRESHOLD(hart) (PLIC_BASE + 0x200000 + (hart) * 0x1000)
#define PLIC_MCLAIM(hart) (PLIC_BASE + 0x200004 + (hart) * 0x1000)
#define PLIC_MCOMPLETE(hart) (PLIC_BASE + 0x200004 + (hart) * 0x1000)

 /*
  * The Core Local INTerruptor (CLINT) block holds memory-mapped control and
  * status registers associated with software and timer interrupts.
  * QEMU-virt reuses sifive configuration for CLINT.
  * see https://gitee.com/qemu/qemu/blob/master/include/hw/riscv/sifive_clint.h
  * enum {
  * 	SIFIVE_SIP_BASE     = 0x0,
  * 	SIFIVE_TIMECMP_BASE = 0x4000,
  * 	SIFIVE_TIME_BASE    = 0xBFF8
  * };
  *
  * enum {
  * 	SIFIVE_CLINT_TIMEBASE_FREQ = 10000000
  * };
  *
  * Notice:
  * The machine-level MSIP bit of mip register are written by accesses to
  * memory-mapped control registers, which are used by remote harts to provide
  * machine-mode interprocessor interrupts.
  * For QEMU-virt machine, Each msip register is a 32-bit wide WARL register
  * where the upper 31 bits are tied to 0. The least significant bit is
  * reflected in the MSIP bit of the mip CSR. We can write msip to generate
  * machine-mode software interrupts. A pending machine-level software
  * interrupt can be cleared by writing 0 to the MSIP bit in mip.
  * On reset, each msip register is cleared to zero.
  */
#define CLINT_BASE 0x2000000L
#define CLINT_MSIP(hartid) (CLINT_BASE + 4 * (hartid))
#define CLINT_MTIMECMP(hartid) (CLINT_BASE + 0x4000 + 8 * (hartid))
#define CLINT_MTIME (CLINT_BASE + 0xBFF8) // cycles since boot.

/* 10000000 ticks per-second */
#define CLINT_TIMEBASE_FREQ 10000000

#endif /* __PLATFORM_H__ */
