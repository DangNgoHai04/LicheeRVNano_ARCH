#!/usr/bin/env python3
"""Repack an existing board FIP with only the new C906 RTOS payload."""
import argparse
import importlib.util
import hashlib
from pathlib import Path
from types import SimpleNamespace

root = Path(__file__).resolve().parents[4]
p = argparse.ArgumentParser(description=__doc__)
p.add_argument('--base', type=Path, required=True, help='Known-good FIP for this exact board')
p.add_argument('--rtos', type=Path, default=root/'Doc_md/Tiny-1C/App/build/cvirtos.bin')
p.add_argument('--output', type=Path, default=root/'Doc_md/Tiny-1C/App/build/fip-thermal.bin')
a = p.parse_args()
if a.output.resolve() in (a.base.resolve(), a.rtos.resolve()):
    p.error('Output must not overwrite input')
spec = importlib.util.spec_from_file_location('fiptool', root/'fsbl/plat/cv181x/fiptool.py')
m = importlib.util.module_from_spec(spec); spec.loader.exec_module(m)
f = m.FIP(); f.read_fip(str(a.base))
if f.param2['BLCP_2ND_RUNADDR'].toint() != 0x8fe00000:
    p.error('Base FIP does not match RTOS address 0x8fe00000')
original = {k: bytes(v.content) for k,v in f.body2.items()}
rtos = a.rtos.read_bytes()
if not 0 < len(rtos) < 0x200000: p.error('Invalid RTOS image length')
f.body2['BLCP_2ND'].content = rtos
args = SimpleNamespace(BLCP_2ND_RUNADDR=0x8fe00000, MONITOR_RUNADDR=f.param2['MONITOR_RUNADDR'].toint())
a.output.parent.mkdir(parents=True, exist_ok=True)
a.output.write_bytes(f.make(args))
check = m.FIP(); check.read_fip(str(a.output))
for name in ('DDR_PARAM','MONITOR','LOADER_2ND'):
    assert bytes(check.body2[name].content) == original[name], name+' unexpectedly changed'
assert bytes(check.body1['BL2'].content) == bytes(f.body1['BL2'].content)
assert bytes(check.body2['BLCP_2ND'].content[:len(rtos)]) == rtos
print(a.output)
print('sha256', hashlib.sha256(a.output.read_bytes()).hexdigest())
print('Verified: RTOS replaced; BL2, DDR, monitor and loader preserved. Not flashed.')
