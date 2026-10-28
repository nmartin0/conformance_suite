# Negative control: the null system

Copyright (c) 2026 Nicholas Martin. All rights reserved.

## What it is

A test that gives PASS on a system that conforms to nothing is probably
vacuous or wrong. The null system is that system: every standard function
fails, every utility does nothing, the shell does nothing. The suite is run
against it, and every assertion that still does not FAIL (or end
UNRESOLVED, UNSUPPORTED, NOTINUSE or UNINITIATED) is listed as suspicious.
It is development tooling; the driver (`posixcheck.sh`) knows nothing of it.

Needs a system with a C compiler, `python3` (the generator only), the
spec model (`build/spec/model.json`, from `gen/specmodel.py`) and dynamic
linking with `LD_PRELOAD` and `dlsym(RTLD_NEXT, ...)`. It is for
development machines, like `ci/mutate.sh`. The calling-convention note at
the end of this page says which machines.

## How it is built

`ci/nullsys.sh build DIR` makes in DIR:

- `libnullsys.so`: a stub for every function the spec model lists
  (`gen/nullsys.py` writes `nullsys.c`; `gen/nullsys_rt.c` is the runtime).
  A stub sets errno to `ENOSYS` and returns the failure value of its return
  kind: a null pointer, -1 for any integer (`ssize_t`, `size_t`, `pid_t`,
  ...), NaN for floating results (both parts for complex ones), a zeroed
  structure for `div`, `ldiv`, `lldiv`, `imaxdiv` and the `ndbm` datum, and
  nothing for `void`. A function that cannot return (`exit`, `_exit`,
  `abort`, `longjmp`, ...) ends the process with status 70 and no result.
- `sh`: a tiny program that exits 0 with no output, whatever its arguments.
- `bin/NAME` for each utility under `modules/xcu` (chapter modules and the
  `sh-*` feature modules are not utilities): the same program, exit 0, no
  output.
- `cc`, a compiler wrapper, and `.launch`, a launcher (see below).

`ci/nullsys.sh build --poison DIR` builds the second return mode, for
tests that only compare against -1 or against "not zero". Integer stubs
then return a value no conforming system returns (0x5EED for 16-bit,
0x5EED5EED for 32-bit and 0x5EED5EED5EED5EED for 64-bit results, picked by
the type in the prototype) and set errno to 0x7ffffff0, outside any system's
range; floating stubs return a signalling-looking NaN with the sign bit set.
Pointer stubs still return a null pointer: a test that uses it crashes, and a
crash during the test is FAIL already. Use a separate DIR per mode.

## How it runs

```
ci/nullsys.sh build build/nullsys
ci/nullsys.sh run build/nullsys                 # all modules
ci/nullsys.sh run build/nullsys xsh/open xcu/ls # or a few
ci/nullsys.sh run --flip build/nullsys          # stubs exit 1, print "error"
ci/nullsys.sh run --real-sh build/nullsys       # real shell, stub utilities
ci/nullsys.sh build --poison build/nullsys-poison
ci/nullsys.sh run build/nullsys-poison
ci/nullsys.sh report build/nullsys build/nullsys/results   # list again
```

Options after DIR go to `posixcheck.sh` (`--cc` and `--sut-sh` are supplied;
the harness self-tests are skipped). Results are kept in `DIR/results`
(`results-flip`, `results-realsh`), with `nullsys.calls` (the call log),
`nullsys-suspicious.txt` (the list) and the driver's output. Generated
sources stay in DIR, which belongs under `build/`: they come from the
standard's model and are never committed (RULES.md, rule 3).

Run it in each mode. The default mode silently succeeds and returns -1: it
catches tests that accept success. `--flip` fails loudly with output: it
catches tests that look only at "nothing on stdout" or only at status 0.
`--poison` catches tests that compare only against -1 or against zero.

### The shell

Shell tests run under `DIR/sh`, so they cannot report a result at all and
come out UNINITIATED: that is the right answer for a shell that does
nothing, and it also means the default and `--flip` runs say little about
the XCU shell tests. `--real-sh` is the useful control for the utilities:
the shell is real, the stub utilities come first on its PATH, so a
`.sh` test sees a system whose utilities (other than the shell's built-ins)
do nothing. Built-ins stay real, and so do `command -p` and anything that
names a utility by absolute path.

### The C tests: how the stubs are switched

`pcexec` passes the test only `PC_*` variables, so the library cannot be put
in `LD_PRELOAD` from outside. The compiler wrapper `DIR/cc` (used through
`--cc`) does it: when it links a test (a link with `pc.o`) it renames the
result to `NAME.real` and installs `.launch` as `NAME`; `.launch` sets
`LD_PRELOAD` and runs `NAME.real`. It also builds tests with
`-fno-builtin -U_FORTIFY_SOURCE` where the compiler accepts them, so that a
call to `abs` or `snprintf` is a real call that a stub can see.

The stubs are on only while a test is between `pc_test()` and its verdict.
`core/pc.c` tells the library by calling `getenv("PC_NULLSYS_ON")` when the
test phase begins and `getenv("PC_NULLSYS_OFF")` whenever its own helper code
runs (the library defines `getenv()` and recognises those two names; without
the library the call is a harmless lookup). So:

- everything before `pc_test()` (setup) runs on the real system, as does
  everything `pc_*` does: result writing, setup helpers, the formatting of a
  failure note, `exit`. The harness is never stubbed.
- the test's own calls after `pc_test()` all hit stubs, including `open`,
  `write`, `exit` and `malloc`. A stub passes the call on to the C library
  only when the switch is off (setup), using the same arguments.
- the library removes `LD_PRELOAD` from the environment the first time the
  test asks for its environment, so programs the setup starts are not
  stubbed.

### The call log

Each active stub appends `ID function` to `$PC_NULLSYS_LOG`, once per
function per process (`ID` is `$PC_ASSERTION`); the stub utilities and shell
log `ID utility` and `ID sh` on each start. Nothing is logged for
harness-internal calls. A log that cannot be written is ignored. Duplicates
are removed when the list is made.

## How to read the suspicious set

The list has three parts.

**(a) PASS, subject never called.** The test finished with PASS without
calling the function (or running the utility) its module is about, so it did
not exercise its subject. Either the test is vacuous (it checks a condition
that holds whatever the subject does, or checks setup only), or it reached
the subject by a name the log does not know (a macro, an alias such as
`open64`, a call the compiler inlined, a utility run by absolute path).
Every entry here needs a look; a real vacuous test is a bug in the test.

**(b) PASS, subject called.** The stub returned a failure (or a garbage
value in poison mode) and the test still passed. Judge it against what the
assertion says:

- If the requirement is that something fails ("returns -1 with EBADF", "the
  utility exits non-zero", "no output") then a stub that fails, or a utility
  that prints nothing, can satisfy the observable part. That is only right if
  the test also checks the specific error: -1 alone with `ENOSYS` must not
  pass an assertion about `EBADF`. Run the poison mode and `--flip`: a test
  that still passes there and whose assertion names an errno or an output is
  too weak.
- If the requirement is "nothing happens" (a call that must leave a file
  unchanged, an option that must have no effect, a thing that "may" be done),
  PASS can be right: the null system does nothing. Check that the assertion
  could fail on some other system (is there a planted fault in
  `mutants/` that it catches?) before accepting it.
- If the assertion says the call succeeds or returns a value, PASS here is
  a defect in the test.
- A module with no single subject (the chapter modules) is placed by whether
  any stub was called; the entry shows which.

**xbd.** Header and compile-only modules: they only compile, so no stub is
involved. Their PASS is expected; they are listed apart so they do not
drown the rest.

Codes other than PASS that are not in the quiet set are printed with their
code in brackets. `INSPECT` (the FAIL of a disputed assertion) and
`UNTESTED` are counted as quiet.

## What cannot be stubbed cleanly

- `dlsym`: the runtime finds the real functions with it. Tests of it run on
  the real function. `dlopen`, `dlclose` and `dlerror` are stubbed.
- `setjmp`, `sigsetjmp`: they return twice and cannot be forwarded through
  a wrapper; `vfork` and the `ucontext` functions likewise. Not defined, so
  their tests run on the real functions.
- Calls that never reach a library function by its standard name: ones the
  compiler inlines, and ones a header maps to another name (checked or
  64-bit variants). The wrapper switches the first two off where the
  compiler accepts `-fno-builtin -U_FORTIFY_SOURCE`; a header that renames
  `stat` or `open` for large files on a 32-bit system defeats a stub. The
  log makes such cases visible: they land in (a).
- Macros that stand in for a function (`getc`, `isalpha`, ...) are not
  calls; a test that uses the macro form reaches the stub only if the test
  uses the function form.
- Variadic functions forward with plain argument patterns: `open`, `openat`,
  `fcntl`, `semctl`, `mq_open` and `sem_open` pass their extra arguments as
  integers or pointers, the `printf` and `scanf` families through their
  `v` forms, `exec*l*` through the vector forms, `strfmon` with one double.
  Outside the test phase only; in the test phase every variadic stub simply
  fails. Setup code that passes anything else through them is mis-forwarded.
- Threads: the switch is a plain flag. Threads created in setup that run
  during the test phase are stubbed too.
- Failing stubs are not always the failure the standard gives: `mmap`
  returns a null pointer, not `MAP_FAILED`, and `sem_open` not `SEM_FAILED`.
  A test that checks for the macro and then uses the pointer crashes, which
  is FAIL.
- `exit`, `_exit`, `abort` and other functions that cannot return end the
  process with status 70 instead of returning (which the caller does not
  expect).
- The failure note of a test is built in the test's own arguments before
  the helper runs; calls there (`strerror`, `snprintf`) are stubbed and the
  note may be empty. The verdict is unaffected.
- The null system is a model of "everything fails"; it is not a system that
  could exist. A test that calls a utility through `system()` or `popen()` in
  its test phase gets the stub of those calls (-1), not the stub utility.

Calling convention: the generated stubs take their arguments and results by
class (integers as `long`, pointers as `void *`) so that they need no system
header. Passing an `int` where `long` is read, and the reverse, is only
sound where `long` is the register width and arguments go in registers or
8-byte stack slots: the System V x86-64, AArch64 and similar LP64 ABIs.
On a 32-bit ABI with 64-bit file offsets, forwarding an `off_t` argument is
wrong (setup could then misbehave; the stubs themselves still fail as
intended). Build and run it on an LP64 system.
