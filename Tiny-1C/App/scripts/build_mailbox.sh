#!/usr/bin/env bash
set -euo pipefail
ROOT=$(cd "$(dirname "$0")/../../../.." && pwd)
export PATH="$ROOT/host-tools/gcc/riscv64-linux-musl-x86_64/bin:$PATH"
MODULE="$ROOT/osdrv/interdrv/rtos_cmdqu"
OUT="$ROOT/Doc_md/Tiny-1C/App/build"
mkdir -p "$OUT"
make -C "$ROOT/linux_5.10/build/sg2002_licheervnano_sd" \
    M="$MODULE" PWD="$MODULE" ARCH=riscv CROSS_COMPILE=riscv64-unknown-linux-musl- \
    CVIARCH=CV181X modules > "$OUT/build-mailbox.log" 2>&1
cp "$MODULE/soph_rtos_cmdqu.ko" "$OUT/"
echo "Mailbox module ready: $OUT/soph_rtos_cmdqu.ko (requires matching board kernel)"
