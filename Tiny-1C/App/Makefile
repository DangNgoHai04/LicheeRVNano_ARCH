ROOT := $(abspath ../../..)
CC ?= cc
CFLAGS := -std=c11 -O2 -Wall -Wextra -Werror -Iinclude
LINUX_CC ?= $(ROOT)/host-tools/gcc/riscv64-linux-musl-x86_64/bin/riscv64-unknown-linux-musl-gcc
.PHONY: all linux test
all: build/thermal_bridge_host
build:
	mkdir -p $@
build/thermal_bridge_host: linux/thermal_bridge.c include/thermal_protocol.h | build
	$(CC) $(CFLAGS) $< -o $@
linux: build/thermal_bridge
build/thermal_bridge: linux/thermal_bridge.c include/thermal_protocol.h | build
	$(LINUX_CC) $(CFLAGS) -static $< -o $@
test: all
	python3 -m unittest discover -s tests -v
