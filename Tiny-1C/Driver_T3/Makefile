# Tầng 3: Makefile kiểm thử độc lập trên Host (x86_64) và kiểm tra biên dịch chéo RISC-V với T0-T1-T2.
CC          ?= gcc
CFLAGS      := -Wall -Wextra -Werror -O2 -g -I./include -pthread

RISCV_ROOT  ?= /home/duong/Workspace/LicheeSG-Nano-Build
RISCV_CC    := $(RISCV_ROOT)/host-tools/gcc/riscv64-elf-x86_64/bin/riscv64-unknown-elf-gcc
FREERTOS_DIR:= $(RISCV_ROOT)/freertos
CVITEK_DIR  := $(FREERTOS_DIR)/cvitek

RISCV_INC   := -I./include \
               -I$(CVITEK_DIR)/driver/spi/include \
               -I$(CVITEK_DIR)/driver/i2c/include \
               -I$(CVITEK_DIR)/hal/cv181x/spi/include \
               -I$(CVITEK_DIR)/hal/cv181x/i2c/include \
               -I$(CVITEK_DIR)/driver/rtos_cmdqu/include \
               -I$(CVITEK_DIR)/driver/common/include \
               -I$(CVITEK_DIR)/common/include \
               -I$(CVITEK_DIR)/common/include/riscv64 \
               -I$(CVITEK_DIR)/hal/cv181x/config \
               -I$(CVITEK_DIR)/driver/pinmux/include \
               -I$(CVITEK_DIR)/hal/cv181x/pinmux/include \
               -I$(CVITEK_DIR)/kernel/include/riscv64 \
               -I$(CVITEK_DIR)/arch/riscv64/include \
               -I$(CVITEK_DIR)/scripts/Only_C906_1 \
               -I$(FREERTOS_DIR)/Source/include \
               -I$(FREERTOS_DIR)/Source/portable/GCC/RISC-V \
               -I$(FREERTOS_DIR)/Source/portable/GCC/RISC-V/chip_specific_extensions/thead_c906_rv64imafdc \
               -I$(FREERTOS_DIR)/Tracealyzer/include \
               -I$(FREERTOS_DIR)/Tracealyzer/config

RISCV_FLAGS := -march=rv64imafdc -mabi=lp64d -mcmodel=medany \
               -Wall -Wextra -Werror -O2 -ffunction-sections -fdata-sections \
               $(RISCV_INC)

SRCS        := $(wildcard src/*.c)
OBJS        := $(SRCS:.c=.o)
RISCV_OBJS  := $(SRCS:src/%.c=build_riscv/%.o)
TARGET      := thermal_test

.PHONY: all clean test riscv-check

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $@ $^

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

test: $(TARGET)
	./$(TARGET)

build_riscv/%.o: src/%.c
	@mkdir -p build_riscv
	$(RISCV_CC) $(RISCV_FLAGS) -c $< -o $@

riscv-check: $(RISCV_OBJS)
	@echo "[OK] Bien dich cheo RISC-V Tang 3 ket noi voi T0-T1-T2 trong freertos thanh cong!"

clean:
	rm -rf $(OBJS) $(TARGET) build_riscv
