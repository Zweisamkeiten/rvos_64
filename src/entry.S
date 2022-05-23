# 
# RV64I 定义了 64 位计算机体系结构，其中寄存器为 64 位宽（因此 RV64）——其指令也是 32 位宽。 
# RV32 指令仍然有效，并且还有一些额外的指令可以同时适应 32 位和 64 位操作。 
# 例如， lw仍然加载一个 32 位字（尽管现在符号扩展以填充 64 位寄存器），因此使用了一条新指令 ld加载一个 64 位字。
# add仍然添加两个寄存器和第三个目标，但这相同 add现在正在执行 64 位加法而不是 32 位加法，因为 RV64 中的寄存器是 64 位的。
# 一条新指令 addw做 32 位加法，以防这就是你想要的。 
#

# 将当前所有的通用寄存器内容保存至任务的上下文
# struct context *base = &ctx_task;
# base->ra = ra;
# ......
.macro reg_save base
	sd ra, 0(\base)
	sd sp, 8(\base)
	sd gp, 16(\base)
	sd tp, 24(\base)
	sd t0, 32(\base)
	sd t1, 40(\base)
	sd t2, 48(\base)
	sd s0, 56(\base)
	sd s1, 64(\base)
	sd a0, 72(\base)
	sd a1, 80(\base)
	sd a2, 88(\base)
	sd a3, 96(\base)
	sd a4, 104(\base)
	sd a5, 112(\base)
	sd a6, 120(\base)
	sd a7, 128(\base)
	sd s2, 136(\base)
	sd s3, 144(\base)
	sd s4, 152(\base)
	sd s5, 160(\base)
	sd s6, 168(\base)
	sd s7, 176(\base)
	sd s8, 186(\base)
	sd s9, 194(\base)
	sd s10, 202(\base)
	sd s11, 210(\base)
	sd t3, 218(\base)
	sd t4, 224(\base)
	sd t5, 232(\base)
.endm

# 从任务上下文中读入所有通用寄存器的值
# struct context *base = &ctx_task;
# ra = base->ra;
# ......
.macro reg_restore base
	ld ra, 0(\base)
	ld sp, 8(\base)
	ld gp, 16(\base)
	ld tp, 24(\base)
	ld t0, 32(\base)
	ld t1, 40(\base)
	ld t2, 48(\base)
	ld s0, 56(\base)
	ld s1, 64(\base)
	ld a0, 72(\base)
	ld a1, 80(\base)
	ld a2, 88(\base)
	ld a3, 96(\base)
	ld a4, 104(\base)
	ld a5, 112(\base)
	ld a6, 120(\base)
	ld a7, 128(\base)
	ld s2, 136(\base)
	ld s3, 144(\base)
	ld s4, 152(\base)
	ld s5, 160(\base)
	ld s6, 168(\base)
	ld s7, 176(\base)
	ld s8, 186(\base)
	ld s9, 194(\base)
	ld s10, 202(\base)
	ld s11, 210(\base)
	ld t3, 218(\base)
	ld t4, 224(\base)
	ld t5, 232(\base)
	ld t6, 240(\base)
.endm

# Something to note about save/restore:
# - We use mscratch to hold a pointer to context of previous task
# - We use t6 as the 'base' for reg_save/reg_restore, because it is the
#   very bottom register (x31) and would not be overwritten during loading.

.text

# interrupts and exceptions while in machine mode come here.
.globl trap_vector
# the trap vector base address must always be aligned on a 4-byte boundary
.align 4
trap_vector:
	# save context(registers).
	csrrw	t6, mscratch, t6	# swap t6 and mscratch
	reg_save t6

	# Save the actual t6 register, which we swapped into
	# mscratch
	mv	t5, t6		# t5 points to the context of current task
	csrr	t6, mscratch	# read t6 back from mscratch
	sd	t6, 240(t5)	# save t6 with t5 as base

	# save mepc to context of current task
	csrr	a0, mepc
	sd	a0, 248(t5)

	# Restore the context pointer into mscratch
	csrw	mscratch, t5

	# 32call the C trap handler in trap.c
	csrr	a0, mepc
	csrr	a1, mcause
	call	trap_handler

	# trap_handler will return the return address via a0.
	csrw	mepc, a0

	# restore context(registers).
	csrr	t6, mscratch
	reg_restore t6

	# return to whatever we were doing before trap.
	mret


# void switch_to(struct context *next);
# a0: pointer to the context of the next task
.globl switch_to
.align 4
switch_to:
	# switch mscratch to point to the context of the next task
	csrw	mscratch, a0

	# set mepc to the pc of the next task
	ld	a1, 248(a0)
	csrw	mepc, a1

	# Restore all GP registers
	# Use t6 to point to the context of the new task
	mv	t6, a0
	reg_restore t6

	# Do actual context switching.
	mret

.end

