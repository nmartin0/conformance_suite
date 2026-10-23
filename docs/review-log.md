# Review log

Copyright (c) 2026 Nicholas Martin. All rights reserved.

Every module gets an independent review after it is written, by a
reviewer that has not seen it being built. The reviewer's reports quote
the standard to make their points, so under RULES.md rule 2 they are kept
outside the repository; this log records what each review covered and
what came of it. Standard issues a review found are in
`standard-issues-report.md`; Linux behaviour it confirmed is in
`linux-deviations.md`.

| Review | Scope | What it led to | Applied in |
| --- | --- | --- | --- |
| close (batch 1) | `xsh/close` | close() fixes; header assertions split out; strict compiles | ce8b2c7 |
| open (batch 1) | `xsh/open` 019-050 | tests start with a clean descriptor table (pcexec) | bcf5e50 |
| coverage | `ci/coverage.py` classifications for open, close, 2.6, headers | 34 findings: hidden testable units, overstated coverage, units the extractor missed | (with the coverage work) |
| dup and pipe | `xsh/dup`, `xsh/pipe` | mutants that cannot be planted are reported UNCHECKED | 52b7507 |
| fcntl | `xsh/fcntl`, `XBD.fcntl.h.008` | fcntl() fixes | 30c917f |
| lseek | `xsh/lseek`, `XBD.unistd.h.002` | no signed overflow in the largest-offset loop | e26a4f4 |
| read, write | `xsh/read`, `xsh/write` | interrupt tests without setitimer(); clean signal state at start | 7c21e3a |
| fork | `xsh/fork` | fork() fixes; Linux waitid() keeps 8 bits (O.6) found | 95451cf |
| wait, _Exit | `xsh/wait`, `xsh/_Exit` | fixes in both | b0aa2c8 |
| exit (two rounds) | `xsh/exit` | fixes; `--only` runs same-module prerequisites | e7ab535 |
| exec | `xsh/exec` | error tests run in a child; coverage tags options per shaded span | 278242c |
| sigaction | `xsh/sigaction` | the `disputed` class (FAIL logged as INSPECT); O.11 withdrawn (SI-24) | 85753c8 |
| kill, masks, raise | `xsh/kill`, `pthread_sigmask`, `raise` | kill(-1) only as an unused user ID with a proof of no privilege; race-free ESRCH | 0ffc3f1 |
| signal waits | `sigpending`, `sigsuspend`, `sigwait`, `sigtimedwait`, `sigqueue`, `alarm`, `pause` | 19 test findings, 8 standard issues (SI-33 to SI-38, Q-14, Q-15): no hangs or leftovers, stronger checks, new faults | 1a6e149 |
| `<signal.h>` | `xbd/signal.h` | 16 test findings, 5 standard issues (SI-39 to SI-41, Q-16, SI-10 extended): member types by _Generic, constant expressions, SIGURG split | 913b32b |
| XSH 2.4 | the four 2.4 modules | 19 test findings, 6 standard issues (SI-42 to SI-44, Q-17, Q-18, SI-30 extended): deadlines instead of hangs, atomics, 16 faults planted | d55daf8 |
| remaining signal functions | sigset functions, `signal`, `sigaltstack`, `psiginfo`, `sig2str`, `killpg`, `abort` | 21 test findings, 6 standard issues (SI-47 to SI-49, Q-20 to Q-23); O.13 and O.14 corrected; O.16 found | a9561a4 |
| spec model and Tier 0 | `gen/specmodel.py`, `gen/tier0.py` and their output | about 380 names the model missed, IP6 and TSA+TSS labels, the Encryption Option Group, structure and array checks that could not fail, value checks; the model now audits itself; SI-51 | 5b311b7 |
| stat, fstat | `xsh/fstatat`, `xsh/fstat` | 13 findings: trailing slashes through lstat(), absolute paths with bad descriptors, permission cases run unprivileged, /dev/null dropped; O.18 found | a6eebc7 |
| link, unlink | `xsh/link`, `xsh/unlink` | 9 findings: directory cases only unprivileged, isolated EACCES, sticky rule per XBD 4.5, link.019 disputed (Q-24), new cases | c1b0b65 |
| rename | `xsh/rename` | 7 findings: the trailing-slash rule is required (XBD 4.16 footnote), so SI-52 narrowed and O.21 found; clean dot cases; dot-dot after a move; mixed absolute and relative paths | (rename fixes) |
| mkdir, rmdir | `xsh/mkdir`, `xsh/rmdir` | 10 findings: S_ISGID inherited in setgid directories, EBUSY for open directories, {LINK_MAX}-safe time loops, dot-files, trailing slashes, EILSEQ; chapter 4 anchors fixed suite-wide | (mkdir fixes) |
| chmod, fchmod | `xsh/chmod`, `xsh/fchmod` | 9 findings: S_ISGID refused with EPERM (BSD) accepted, non-member group chosen at run time, set-ID bits set and cleared, inherited descriptors, O_SEARCH descriptors; "." made searchable in setup suite-wide | (chmod fixes) |
| chown, fchown, lchown | `xsh/chown`, `xsh/fchown`, `xsh/lchown` | 11 findings: BSD group inheritance (files prepared by the privileged parent), groups chosen at run time, real changes for S_ISUID/S_ISGID clearing, no partial change on EPERM, lchown restriction rules; SI-53 corrected; the ENOENT overlap (Q-8) made disputed suite-wide | (chown fixes) |
| access, faccessat | `xsh/access` | 9 findings: real/effective IDs set apart with two non-zero IDs (privilege no longer decides), directory searches and faccessat(..., 0) with the IDs apart, real/effective group IDs (020), the group class (019), privileged execute and search rules with a privilege precondition, run-time groups, F_OK at mode 0 and permissions through a link, errno read after the call; `faccessat.search_checked` stays unverified until a system passes XSH.access.018 | (access fixes) |
| truncate, ftruncate, umask | `xsh/truncate`, `xsh/ftruncate`, `xsh/umask` | 13 findings: privilege probe in `unprivileged()` (capabilities give UNRESOLVED, suite-wide), no file checks after a failed truncate() (SI-57), file-type EINVAL split out (2024; ftruncate's disputed, SI-56), limits tested in a child with SIGXFSZ blocked and pending, the limit itself allowed, `since` years per the change history, 2^32 + 5 lengths, umask(0), a fuller round trip, S_ISVTX not applied; SI-58, Q-26 | (truncate fixes) |
| futimens, utimensat, utimes | `xsh/futimens` | 11 findings: tighter time windows (100 ns where the file system keeps them, else 1 s), exact old times and clock checks in setup, no resets in the test phase, NFS root-squash guard, futimens() permission rules (021), mixed UTIME_NOW cases, EINVAL boundaries, trailing slashes and "f/x" (022, disputed), futimens(AT_FDCWD) (024, O.22 musl), utimes() tv_usec (023); SI-59 to SI-61, Q-27; deviation 1.18 not seen on Linux 6.18 | (futimens fixes) |
| directory streams | `xsh/opendir`, `readdir`, `closedir`, `rewinddir`, `dirfd`, `seekdir`, `telldir`, `alphasort` | 10 findings: noatime probe and a no_atime fault, FD_CLOEXEC check of a closed descriptor, errno at the first end, `since` years split by edition (non-portable names, dirfd()'s own descriptor 2024, ELOOP 2001), seekdir/telldir under XSI, telldir tested on its own, honest `.cov` lines, faithful rewinddir.stale and scandir.unsorted faults; new: 3000-entry directory, far seekdir(), fork, readdir_r() guard bytes, sel sees every entry, fdopendir() of an O_SEARCH descriptor; SI-62, SI-63 | (directory-stream fixes) |
| signal todos | 16 assertions across sigaction, sigaltstack, sigqueue, sigtimedwait, kill, killpg, alarm, psiginfo, sigsuspend, 2.4.2, 2.4.3 | 13 findings: shared queue limit gives UNRESOLVED (and runs on musl), kill.016 no longer hangs or leaks processes, killpg ESRCH without PID reuse, si_code requirements disputed (SI-64) and the SIGBUS code dropped (SI-65), CLOCK_REALTIME for the timer, raise() for per-thread delivery, SA_NOCLDWAIT blocking (sigaction.025), faithful faults; SI-66 | (signal todo fixes) |
| fflush, fclose | `xsh/fflush`, `xsh/fclose` | 11 findings: 1 MiB files so "not at EOF" holds either way (Q-29), a 16 KiB buffer for fmemopen() (O.29 found), refused set-up steps UNRESOLVED and a check that the data is still buffered, alarm-based EINTR tests with a rescue, pipes filled byte by byte, O.26/O.27 corrected, since years, the discarded byte only (Q-30), 2 s for timestamps; new: fflush(NULL) with a failing stream, at end of file, update streams, pushback at fclose(), fclose() EFBIG and ENOSPC; SI-67 | (stdio fixes) |

The shared copy of the standard-issues report is the Claude Doc at
https://claude.ai/artifact/L3t5goEQYUJKhbTWAj2BXF; it is kept in step with
`standard-issues-report.md`.
