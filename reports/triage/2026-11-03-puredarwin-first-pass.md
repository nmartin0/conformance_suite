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
| 26 | Killed by SIGSEGV (14 `isw*` functions, `raise`, `clock_getres` ...) | Suspect. The `isw*` crash is likely my link stubs or Apple's inline `__istype` path, not the system. Reproduce with a small program on the guest before counting it. |
| 18 | Math exception flags not raised (`log(0)`, `exp(big)`, `cosh` ...) | Suspect: QEMU TCG or the libm. Reproduce with `fetestexcept` after one call and compare with a host run. |
| 13 | `*at()` calls through an O_SEARCH descriptor fail | Possibly the ext4 driver or a real gap; unresolved. |
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

## Next

1. Reproduce on the guest (VM relaunch, a few small programs): `iswalpha`
   crash, floating-point flags after `log(0.0)`, `fopen(..., "re")` and the
   O_SEARCH `*at` calls on both the ext4 driver and a tmpfs-like location if
   any exists.
2. Read the 241 "other" results one by one against the standard.
3. Re-run with the current driver so the header carries `# cc-features` and
   `# fs` lines.
4. An ordinary-user run.
