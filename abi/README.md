# ABI conformance checks

Copyright (c) 2026 Nicholas Martin. All rights reserved.

An extension of posixcheck that checks a system against the System V psABI
instead of POSIX: what a process starts with, how data is laid out, how a
function is called and returns, what a new thread and a signal handler see.
It reuses the driver (`posixcheck.sh`), the result codes and the reporting;
the tests are C17 plus, for the few things C cannot express, inline assembly
kept in one header per architecture.

First batch: **i386** (the i386 psABI). x86-64 comes next; see STATUS.md.

## Run

    sh abi/run.sh i386 --results /tmp/abi-i386-1       # whole batch
    sh abi/run.sh i386 --only ABI.i386.calling.017 --results /tmp/x
    sh abi/run.sh i386 i386/entry i386/types --results /tmp/x   # some modules
    sh abi/control.sh                                  # do the checks catch faults?

`run.sh ARCH [--cc CMD] [driver options]` makes a one-line compiler script
(the system compiler plus the flag that selects `ARCH`, `-m32` for i386; `--cc`
or `$CC` name the compiler, default `cc`), points the driver at `abi/modules`
only, and passes everything else on. The driver itself was changed in one
generic place: an assertion ID `ABI.<arch>.<area>.NNN` maps to the module
`<arch>/<area>` of the tree given with `--root` (needed for `--only` and
`uses`). Nothing changes for the POSIX modules.

Results are the usual ones. In addition:

- **NOTINUSE**: the test is for an architecture other than the one the
  compiler builds for (every i386 test, built for x86-64, says so).
- **INSPECT**: a `disputed` assertion failed. Used where the two published
  revisions of the psABI differ or where only the older one states the rule (below).
- **UNTESTED**: an `untestable` assertion (needs control of the program's first
  instruction), listed so the gap is visible.

Assertions whose rule rests on the older revision alone (the x87 stack depth
at a return, calling.017-.019 and .025) or on an inference about where the
state is observed (entry.010, read in `main` after constructors) are `disputed`.

A test needs no privilege; as an ordinary user the result is the same.

## Layout

    abi/README.md, STATUS.md, known-deviations.md
    abi/run.sh            run the suite for one architecture
    abi/control.sh        the control: wrong routines and wrong compilers must be caught
    abi/arch/i386.h       the architecture primitives (the only inline assembly of the suite)
    abi/arch/i386_control.h   deliberately wrong routines, for the control only
    abi/check.h           the checks, in generic C, shared by tests and control
    abi/libcalls.h        the four library calls the calling-sequence tests make
    abi/sigtest.h         signal-test support (handler, interrupted loop, sender thread)
    abi/control/control.c the control program
    abi/modules/i386/<area>/   MODULE, <area>.asr, t/NNN-name.c

Areas of batch 1 (assertion IDs `ABI.i386.<area>.NNN`):

| area | what | assertions |
|---|---|---|
| entry | initial stack, argv/envp/auxv, DF, x87 and MXCSR control state in `main` | 13 (2 untestable) |
| types | sizes, alignments, structure/union layout, older revision's bit-fields | 16 |
| calling | callee-saved registers (library calls and compiled code), DF and control words across library calls, stack alignment at entry, argument layout, every kind of return value, `_Bool` arguments | 27 |
| threads | a new thread's entry state, inherited and private FP state | 7 |
| tls | thread-local storage; **not a psABI group** (the C language is the oracle) | 4 |
| signals | a handler's entry state, and the interrupted code's state afterwards | 7 |

## The sources and the two revisions

Public specifications only, read from the user's own copies and never copied
into the repository: the i386 psABI source (`low-level-sys-info.tex` for data
types, registers, calling sequence, return values, process initialisation and
thread state), and the older published revision `abi386-4`. Assertions are
written in our own words with an anchor naming the section or table label.

The revisions differ, and the suite says which one each check follows:

- stack alignment at a function entry: 16 bytes (current) against word (older);
- x87 precision control at process entry: 11/extended (current) against a table
  that labels the same bits 53-bit (older);
- the older revision states that x87 registers are empty at entry and exit, that
  a returning signal handler leaves registers restored, and that bit-fields are
  allocated right to left; the current one does not;
- the current revision adds `long long`, complex types, `_Bool`, MXCSR and
  `max_align_t`; the older one has none of them.

Every assertion whose rule differs between them, or that only the older revision
states, is `disputed` (a FAIL is reported as INSPECT) and its `note` says which
revision the test follows. Where both agree, or where the current revision states
a rule for something the older one predates (the types and registers listed
above), the assertion is `required`; for the stack alignment the weaker rule
both share (word aligned) is a separate `required` assertion.

Where the psABI leaves something open nothing is asserted: the upper bits of a
`char`/`short` return value (the test checks that the compiler's own caller does
not depend on them), `wchar_t` and `ptrdiff_t` (not in the ABI), the contents of
padding. Observations about the first instructions of a process are inferred,
and the assertion note says so (`argv - 4` is the initial stack pointer; state
read in `main` is taken as the state at entry).

Thread-local storage is a non-psABI group, kept in its own area `tls`
(`ABI.i386.tls.NNN`) so the ids show it: the psABI refers to a separate document
for TLS that was not in the source set. Those assertions say so in their notes
and take the C language's thread storage duration as the oracle (own copy per thread, initial value or zero, the
alignment asked for), which is what the TLS layout implements.

## Design rules

- Tests are generic C. Architecture-specific primitives (read or set a control
  word, call a function with chosen registers and record what came back, record
  the state at a function's entry) live in `abi/arch/<arch>.h`, selected with
  `#if defined(__i386__)`. Inline assembly is allowed only there and in
  `i386_control.h`. The assembly is position independent.
- The check functions (`abi/check.h`) contain no assembly and no judgement hidden
  in a primitive; the same function is applied by a test to library code and
  by the control to a wrong routine.
- On another architecture the guard leaves `ABI_ARCH_I386` undefined and every
  i386 test builds a tiny `main` that reports NOTINUSE; nothing fails to compile.
- No test, assertion or note names an operating system or a C library. A
  difference between systems is a finding (`known-deviations.md`), never a
  condition in a test.
- Tests are ephemeral and standalone: POSIX sh and C17 plus the guarded asm, no
  files left behind, one assertion per test, header comment naming it.

## The control

`abi/control.sh` does what the planted faults of the POSIX suite do. Part A
applies the checks to hand-written wrong routines (clobbers EBX/ESI/EDI/EBP,
leaves DF set, misaligns the stack by 4 before a call, calls with DF set,
returns a structure in registers or without popping the hidden argument, returns
no or two floating-point values, changes the x87 control word or MXCSR and does
not restore it, leaves a value on the x87 stack, does not pop a returned
`double`, returns half a 64-bit value, a signal handler that damages the context
it resumes) and requires FAIL, and requires PASS from each routine's
conforming twin. Part B runs real tests with a compiler told to break the ABI
(`-malign-double`, `-mlong-double-64`, `-freg-struct-return`,
`-mpreferred-stack-boundary=2`, `-mno-fp-ret-in-387`) and requires FAIL or
INSPECT there and PASS with the normal flags. Part B also builds the compiled-code callee-saved test (calling.026) with
`-fcall-used-esi -fcall-used-edi -fcall-used-ebx`, which must FAIL. The entry,
threads and tls tests have no planted fault (the first two use the same
entry detectors that the control exercises; the tls ones none).

## Add a test

1. Pick or add an area `abi/modules/<arch>/<area>/` with a `MODULE` file (copy
   one; `libs -lpthread` if it needs it; `buildfail FAIL` when a compile
   error is itself the verdict, as for layout checks).
2. Add an assertion to `<area>.asr`: `assertion ABI.<arch>.<area>.NNN`,
   `anchor` (section title or label of the psABI), `class`, `says` (your own
   words), `note` (which revision, what is inferred), `test t/NNN-name.c`,
   and a `nomutant` reason (the POSIX mutant machinery does not apply).
3. Write `t/NNN-name.c` with a header comment containing `ASSERTION <id>`,
   `#include "../../../../check.h"`, the real body under
   `#ifdef ABI_ARCH_I386` and `ABI_NOT_THIS_ARCH("<id>")` otherwise. Use the
   primitives and checks; add a primitive to `abi/arch/<arch>.h` only for
   something C cannot say, and a check to `abi/check.h` for any new judgement.
4. If the check is new, add a scenario to `abi/control/control.c` (and a wrong
   routine to `i386_control.h`) and a line to `abi/control.sh`.
5. `python3 ci/lint.py --modules abi/modules`, `sh abi/run.sh i386`,
   `sh abi/control.sh`, and the driver self-test (`./posixcheck.sh --only
   XBD.limits.h.017`).

## Clean room

Written from the public psABI texts only. No text of them is copied (the
assertions paraphrase and cite a section or label); no other test suite, for a
compiler or a C library, was read or used as evidence; no expectation comes from
what a system does (the first run found no deviation, and the failures seen
while writing were mistakes in the tests, recorded in known-deviations.md).

## Deferred

x86-64 (the psABI sources are in place; same layout, `abi/arch/x86_64.h`); the
dynamic-linking side (procedure linkage, TLS models in shared objects, symbol
versioning); stress of vector state across context switches (x87/SSE/AVX
registers kept intact across preemption and signals, not only control words);
a start-up file of our own to see EDX and EBP at the first instruction;
AArch64 and RISC-V; the optional types (`__float128`, `__m64`, `__m128`,
`_Decimal*`, `_BitInt`); argument passing of vector types; DWARF and unwind
tables.
