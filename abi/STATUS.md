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
- i386, batch 1, 6 modules, 76 assertions: entry 15, types 16, calling 27,
  threads 7, tls 4 (non-psABI), signals 7. 75 have tests, 1 is `untestable` (EBP at the first instruction).
- Primitives: `abi/arch/i386.h` (the only inline assembly): state readers
  (EFLAGS, x87 control/status/tag, MXCSR, CPUID SSE), `abi_call_raw` (call
  with sentinels, aligned or deliberately misaligned stack, records registers,
  EAX/EDX, flags, control words, ESP, x87 values), `abi_spin` (loop that can be
  interrupted by a signal), `ABI_ENTRY_TRAMPOLINE` (records ESP, EFLAGS, x87
  control word, MXCSR at a function's entry, then jumps on), a handler that
  clobbers state on purpose, small asm callees.
- Control: `abi/control.sh`, 35 faults detected, 0 missed (three of them for the
  entry stub).
- Entry stub: per-test link options (`xflags` in the assertion file, read by
  `posixcheck.sh`), `ABI_ENTRY_STATE_STUB` in `arch/i386.h`; entry.012 (EDX),
  entry.014 (ESP at the first instruction), both linked with `-Wl,-e,abi_entry_stub`.

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
- AArch64, RISC-V.
- Optional types and vector argument passing; varargs-specific rules; DWARF and
  unwind tables.
- A planted fault for the thread-local-storage tests.

## Untestable from C

- EBP at the first instruction (entry.013). The text calls the register
  unspecified and asks the *user code* to zero the frame pointer, which is a
  duty of the program's own start-up, not something the system promises; the
  stub can read EBP but no value would be right or wrong.
- EDX when it is zero (entry.012 reports NOTINUSE): the text says EDX is a
  function to register but gives zero no meaning, so nothing is judged. On the
  development machine a dynamically linked program gets a non-zero EDX and a
  static one gets zero.
- EFLAGS at process entry other than DF (CF, PF, AF, ZF, SF, OF = 0): the
  flags are changed by the instructions that run before `main` and by the
  tests' own arithmetic, and a status flag cannot be read without an
  instruction that itself alters it; only a start-up file of our own could see
  them at the first instruction. DF is testable because nothing in between sets it.

## The program's own file (entry.015) and what else it could give

The older revision (not the current source, whose object-file chapter has no
header section) states for the file header: 32-bit class, little-endian data
encoding, machine value 3, flags zero. entry.015 reads the program's file by
its absolute argv[0] with a small reader and checks these (`disputed`, since
only one revision states them). Nothing else in the file is a psABI-checkable
fact:

- the entry address: the text gives no rule for it (any address in loaded code
  is fine), and the entry-point override of entry.012 changes it on purpose;
- the program interpreter: the older revision names one file path as the
  valid interpreter, which is a system convention that a conforming system may
  place elsewhere, and the current source states none; asserting it would
  encode a system, so it is left out;
- segment alignment and file-offset congruence: a generic object-file rule
  and not specific to the i386 psABI text in the source set;
- relocation types and the unwind table section: the current source lists
  them, but checking them needs the dynamic-linking side (deferred).
