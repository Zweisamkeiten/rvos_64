# This Code dervied from RVOS 
# https://github.com/plctlab/riscv-operating-system-mooc

  # size of each hart's stack is 1024 bytes
  .equ  STACK_SIZE, 2048
  .equ  MAXNUM_CPU, 8


  .global _start

  .text # 以下内容存入 .text section
_start:
  # park harts with id != 0
  csrr t0, mhartid    # read current hart id
  mv  tp, t0          # keep CPU's hartid in tp(线程寄存器) for later usage.
  bnez  t0, park      # park the hart if current hart is not hart 0

  # Setup stacks, the stack grows from bottom to top, so we put the
  # stack pointer to the very end of the stack range.
  slli  t0, t0, 11    # shift left the hart id by 2048
  la  sp, stacks + STACK_SIZE # set the initial stack pointer
                              # to the end of the first stack space
  add sp, sp, t0              # move the current hart stack pointer
                              # to its place in the stack space

  j start_kernel

park:
  wfi # 睡眠
  j park

stacks:
  .skip STACK_SIZE * MAXNUM_CPU # allocate space for all the harts stacks

  .end    # End of file

