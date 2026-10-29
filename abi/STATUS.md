# ABI suite status

Copyright (c) 2026 Nicholas Martin. All rights reserved.

Running log of the ABI extension of posixcheck (branch abi/sysv). Read
README.md for the design.

## What exists

- Driver reuse: `abi/run.sh ARCH`; one generic change in `posixcheck.sh` (an
  `ABI.<arch>.<area>.NNN` ID maps to the module `<arch>/<area>`), and `ci/lint.py`
  / `ci/catalog.py` know the architecture directories (no `.cov` file for them).
  `python3 ci/lint.py --modules abi/modules` is clean (6 modules, 74 assertions,
  0 problems). The POSIX tree's lint output is unchanged by this work.
- i386, batch 1, 6 modules, 74 assertions: entry 13, types 16, calling 27,
  threads 7, tls 4 (non-psABI), signals 7. 72 have tests, 2 are `untestable` (EDX and EBP at the
  first instruction).
- Primitives: `abi/arch/i386.h` (the only inline assembly): state readers
  (EFLAGS, x87 control/status/tag, MXCSR, CPUID SSE), `abi_call_raw` (call
  with sentinels, aligned or deliberately misaligned stack, records registers,
  EAX/EDX, flags, control words, ESP, x87 values), `abi_spin` (loop that can be
  interrupted by a signal), `ABI_ENTRY_TRAMPOLINE` (records ESP, EFLAGS, x87
  control word, MXCSR at a function's entry, then jumps on), a handler that
  clobbers state on purpose, small asm callees.
- Control: `abi/control.sh`, 32 faults detected, 0 missed.

## Results (this machine, run 1)

72 PASS, 0 other, 2 UNTESTED; unprivileged identical; `-O2` identical; clang
69 PASS and 1 FAIL (types.008, recorded in known-deviations.md). x86-64
build: all NOTINUSE.

## Notes

- The RULES.md 10 prediction (expected result written before the first run) was
  kept in each assertion's text (class, note) rather than in a separate list;
  all `required` assertions were expected to hold, and the `disputed` ones were
  expected to hold too, with INSPECT as the fallback.
- No blind second reading (docs/methodology.md gate 2) has been done for this
  batch; it should be before the batch is trusted.
- The unprivileged run used `runuser -u nobody` with the results directory
  writable by that user.

## Deferred

- x86-64 phase (`abi/arch/x86_64.h`, psABI sources already at hand).
- Dynamic linking, TLS models in shared objects (general-dynamic, local-dynamic,
  initial-exec, local-exec, descriptors), procedure linkage tables.
- Vector-state context-switch stress (x87/MMX/SSE/AVX contents across
  preemption and signals; the batch only checks control words).
- Start-up code of our own, to observe EDX and EBP at the first instruction.
- AArch64, RISC-V.
- Optional types and vector argument passing; varargs-specific rules; DWARF and
  unwind tables.
- A planted fault for the thread-local-storage tests.

## Untestable from C

- EDX and EBP at the first instruction (entry.012, entry.013).
- EFLAGS at process entry other than DF (CF, PF, AF, ZF, SF, OF = 0): the
  flags are changed by the instructions that run before `main` and by the
  tests' own arithmetic, and a status flag cannot be read without an
  instruction that itself alters it; only a start-up file of our own could see
  them at the first instruction. DF is testable because nothing in between sets it.
