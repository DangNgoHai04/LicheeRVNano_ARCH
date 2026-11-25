#!/usr/bin/env bash
set -euo pipefail
ROOT=$(cd "$(dirname "$0")/../../../.." && pwd)
TOP="$ROOT/freertos/cvitek"
export PATH="$ROOT/host-tools/gcc/riscv64-elf-x86_64/bin:$PATH"
export PROJECT_FULLNAME=sg2002_licheervnano_sd
export DDR_64MB_SIZE=n
OUT="$ROOT/Doc_md/Tiny-1C/App/build"
I2C_HZ="${THERMAL_I2C_FREQ_HZ:-100000}"
mkdir -p "$OUT"
for part in arch kernel common hal driver task; do
    source_dir="$TOP/$part"
    if [[ "$part" == hal ]]; then source_dir="$source_dir/cv181x"; fi
    args=(-G Ninja -DCHIP=cv181x -DRUN_ARCH=riscv64 -DRUN_TYPE=CVIRTOS
          -DTOP_DIR="$TOP" -DBUILD_ENV_PATH="$ROOT/build" -DBOARD_FPGA=n
          -DCMAKE_TOOLCHAIN_FILE="$TOP/scripts/toolchain-riscv64-elf.cmake")
    if [[ "$part" == task ]]; then args+=(-DTHERMAL_TINY1C=ON -DTHERMAL_I2C_FREQ_HZ="$I2C_HZ"); fi
    cmake -S "$source_dir" -B "$TOP/build/$part" "${args[@]}" > "$OUT/configure-$part.log" 2>&1
    cmake --build "$TOP/build/$part" --target install > "$OUT/build-$part.log" 2>&1 || {
        tail -60 "$OUT/build-$part.log"; exit 1;
    }
done
riscv64-unknown-elf-objcopy -O binary "$TOP/install/bin/cvirtos.elf" "$OUT/cvirtos.bin"
cp "$TOP/install/bin/cvirtos.elf" "$OUT/cvirtos.elf"
cp "$TOP/install/bin/cvirtos.map" "$OUT/cvirtos.map"
riscv64-unknown-elf-nm "$OUT/cvirtos.elf" > "$OUT/cvirtos.symbols"
grep -q ' T thermal_service_task$' "$OUT/cvirtos.symbols"
grep -q ' T spi_transfer_one$' "$OUT/cvirtos.symbols"
grep -q ' T soft_i2c_read_reg16$' "$OUT/cvirtos.symbols"
echo "Firmware ready: $OUT/cvirtos.bin (I2C $I2C_HZ Hz; not flashed)"
