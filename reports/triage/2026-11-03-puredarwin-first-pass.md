# PureDarwin 26.5: first-pass triage (system interfaces and headers)

Copyright (c) 2026 Nicholas Martin. All rights reserved.

Source: `reports/2026-11-03-puredarwin-amd64/root.run.log`. Root only. This pass
sorts the 704 FAIL results outside the utilities chapter (XSH 380, XBD 144 plus
the build failures) into causes. Nothing here has been judged against the
standard's text one assertion at a time; the groups say where to look first.
Utilities (XCU, 682 FAILs) are not covered: the guest userland is toybox and
most of those results are utilities that are not installed.

## Read this first: what the run measures

- The compiler is a patched TinyCC and the headers are Apple's, copied from the
  zig distribution (the guest installs no headers). A header finding is a
  finding about those headers, not about a PureDarwin SDK.
- The test directory is on the guest's ext4 driver (the only file system the
  scratch disk can use). File-system results (rename, rewinddir, mkdir mode
  bits, appends, O_SEARCH `*at` calls) may be that driver's, not the system's
  native one. The run header would have recorded the pathconf and statvfs
  values, but it was a resumed run and has no `# fs` lines. Re-run a sample on
  another file system before reporting these.
- QEMU TCG emulates the CPU. Floating-point exception flags (below) may be an
  emulation artefact.

## Groups

| Count | Group | Verdict so far |
|---|---|---|
| 117 | Header or declaration missing, or does not compile | Mostly real: `<endian.h>`, `<libintl.h>`, `<uchar.h>` absent; `locale_t` and `ssize_t` not declared where POSIX.1-2024 requires them (`<ctype.h>`, `<dirent.h>`, `<wchar.h>` etc.); `_l` function families, `strlcat`/`wcslcat`, `ppoll`, `pthread_barrier_*`, `pthread_cond_clockwait`, `sem_timedwait`, `sem_clockwait`, `clock_nanosleep`, `mkostemp`, `ptsname_r`, `struct posix_dent`, `reclen_t`, `clockid_t`, `ESOCKTNOSUPPORT`, `O_CLOFORK`, `F_OFD_GETLK`, `O_TTY_INIT`, `POSIX_CLOSE_RESTART`, `SEEK_HOLE`/`SEEK_DATA` macros, `_POSIX_VERSION` is not 202405L. Check each is absent from the standard's required set for strict mode before filing; Apple hides some behind `_DARWIN_C_SOURCE`. |
| 83 | `__STDC_NO_THREADS__` defined, no `<threads.h>` | The compiler defines the macro; Apple ships no `<threads.h>` either. The system part is real, the macro part is the compiler. Ask: is the macro check a fair test of the system? Probably not alone. |
| 26 | Killed by SIGSEGV (24 `isw*`, `raise`, `clock_getres`) | 24 were a compiler fault and now pass (see Probes). `raise.002` and `clock_getres.003` remain unexplained. |
| 18 | Math exception flags not raised (`log(0)`, `exp(big)`, `cosh` ...) | Suspect: QEMU TCG or the libm. Reproduce with `fetestexcept` after one call and compare with a host run. |
| 13 | `*at()` calls through an O_SEARCH descriptor fail | Real: permission is checked on every call (see Probes). |
| 9 | FD_CLOEXEC / FD_CLOFORK not honoured | Real for CLOFORK (not implemented); the `"re"` mode CLOEXEC results (fopen, freopen, fdopen) look real too. |
| 7 | `tc*()` on `/dev/null` give ENODEV, standard asks for ENOTTY | Real, plausible. |
| 5 | ENAMETOOLONG not returned | Possibly the ext4 driver. |
| 241 | Other (single assertions across `getopt`, `fscanf`, `wcsto*`, `strptime`, `regcomp`, `wordexp`, `waitid`, `rewinddir`, `fmemopen`, `ungetc`, ...) | Needs a one-by-one reading. Several look like real long-standing Darwin behaviours (`getopt` colon handling, `sscanf("0xg", "%x")`, `wcstol("0x")` end pointer, `regcomp(".*?c")`). |

## Build-failure UNINITIATED (1,427)

Unresolved references 399 (functions the system library lacks: for example
`timer_getoverrun`, `clock_nanosleep`, `pthread_mutex_clocklock`,
`posix_spawn_file_actions_addchdir`), undeclared or implicit declarations
about 590, missing `st_atim`/`st_mtim`/`st_ctim` 83, missing include files 50,
`_Complex` about 215 (compiler: not a system finding; 13 were labelled, the
rest are fixed by 67690f65 for the next run). These are absences the standard
does not allow, but they hide whatever behaviour the tests would have checked,
which is why UNINITIATED is large. A second compiler with a fuller C17 would
shrink the `_Complex` part only.

## Probes on the guest (same day)

Small programs in `puredarwin-probes/`, built with the same compiler and flags,
run on the root file system and on the scratch disk (both are ext4, so the
file-system results below are the guest's own, not a side effect of the scratch
disk). Results were identical on both.

- **Floating-point flags: real.** Hardware operations raise them (`1.0/0.0`
  sets FE_DIVBYZERO, `max*10` sets FE_OVERFLOW and FE_INEXACT, `sqrt(-1)` sets
  FE_INVALID), so the emulator is not the cause. The library functions
  `log(0)`, `exp(1000)`, `cosh(1000)` and `logl(0)` raise nothing. That is a
  system finding for the 18 results.
- **`rewinddir`: real.** After one full pass, `rewinddir()` then reading gives
  0 entries (first pass: 5).
- **ENAMETOOLONG: real.** `stat()` of a 256-byte name gives ENOENT.
- **`fopen(..., "re")`: real.** FD_CLOEXEC is not set.
- **O_SEARCH `*at` calls: real.** `open(dir, O_SEARCH)` succeeds, then after
  `chmod(dir, 0666)` an unprivileged process calling `mkdirat`, `fstatat` or
  `unlinkat` through that descriptor gets EACCES. The search permission is
  checked at each call, not once at open. That covers the 13 results.
- **Appends: not reproduced.** Two descriptors with O_APPEND gave the right size
  (2000 of 2000). The 9300 of 10000 in XSH.write.005 needs the actual test run
  again, in case it is load or timing.
- **`isw*` crashes: a compiler fault, now fixed.** The compiled test's
  table-of-functions code loads each `isw*` address from a GOT slot that the
  TinyCC Mach-O writer never bound, so the call went to address 0. Found by
  reading the binary's bind table on the host (12 slots with no bind entry),
  reproduced with a 5-line program, fixed in the compiler (see
  `pd-cc/NOTES.md`), and confirmed on the guest: the 12 `isw*` modules and
  `raise` were re-run (61 results) and 24 results changed from FAIL to PASS.
  The saved log carries those re-run lines. `XSH.raise.002` still crashes
  (a varargs function; cause not found, toolchain suspect).

## Next

1. Find why `raise.002` (and `clock_getres.003`) crash; they may be
   compiler faults too.
2. Read the 241 "other" results one by one against the standard.
3. Re-run with the current driver so the header carries `# cc-features` and
   `# fs` lines.
4. An ordinary-user run.
