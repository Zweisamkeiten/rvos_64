CROSS_COMPILE = riscv64-elf-
CFLAGS = -nostdlib -mcmodel=medany -fno-builtin -march=rv64ima -mabi=lp64 -g -Wall

QEMU = qemu-system-riscv64
QFLAGS = -nographic -smp 1 -machine virt -bios none

GDB = ${CROSS_COMPILE}gdb
CC = ${CROSS_COMPILE}gcc
OBJCOPY = ${CROSS_COMPILE}objcopy
OBJDUMP = ${CROSS_COMPILE}objdump

# path macros
BIN_PATH := bin
OBJ_PATH := obj
SRC_PATH := src

WORK_DIR  = $(shell pwd)

INC_PATH := $(WORK_DIR)/include $(INC_PATH)
INCLUDES = $(addprefix -I, $(INC_PATH))
CFLAGS := $(INCLUDES) $(CFLAGS)

SRC := $(foreach x, $(SRC_PATH), $(wildcard $(addprefix $(x)/*,.c*)))
SRC += $(foreach x, $(SRC_PATH), $(wildcard $(addprefix $(x)/*,.S*)))
OBJS := $(addprefix $(OBJ_PATH)/, $(addsuffix .o, $(notdir $(basename $(SRC)))))

MAIN_OBJ := $(OBJ_PATH)/start.o
LD_OBJS := $(filter-out $(MAIN_OBJ),$(OBJS))

default: makedir all
all: $(BIN_PATH)/os.elf

# start.o must be the first in dependency
$(BIN_PATH)/os.elf: ${OBJS} $(MAIN_OBJ)
	${CC} ${CFLAGS} -T os.ld -o $@ $(MAIN_OBJ) $(LD_OBJS)
	${OBJCOPY} -O binary $@ $(BIN_PATH)/os.bin

$(OBJ_PATH)/%.o : $(SRC_PATH)/%.c
	${CC} ${CFLAGS} -c -o $@ $<

$(OBJ_PATH)/%.o : $(SRC_PATH)/%.S
	${CC} ${CFLAGS} -c -o $@ $<

run: all
	@${QEMU} -M ? | grep virt >/dev/null || exit
	@echo "Press Ctrl-A and then X to exit QEMU"
	@echo "------------------------------------"
	@${QEMU} ${QFLAGS} -kernel $(BIN_PATH)/os.elf

.PHONY : debug
debug: all
	@echo "Press Ctrl-C and then input 'quit' to exit GDB and QEMU"
	@echo "-------------------------------------------------------"
	@${QEMU} ${QFLAGS} -kernel $(BIN_PATH)/os.elf -s -S &
	@${GDB} $(BIN_PATH)/os.elf -q -x ./gdbinit -tui

.PHONY : code
code: all
	@${OBJDUMP} -S $(BIN_PATH)/os.elf | less

.PHONY : clean
clean:
	rm -rf $(OBJ_PATH)/*.o $(BIN_PATH)/*

.PHONY: makedir
makedir:
	@mkdir -p $(BIN_PATH) $(OBJ_PATH) ./include $(SRC_PATH)
