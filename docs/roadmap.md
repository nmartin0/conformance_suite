# Roadmap

Copyright (c) 2026 Nicholas Martin. All rights reserved.

Where posixcheck is going, in order. `STATUS.md` records what is done and
verified; this file is the plan. Update both when a step finishes.

## Done

- Harness: `posixcheck.sh` driver, `pcexec` sandbox, result codes and
  phases, self-tests, mutation checks (`ci/mutate.sh`), lint
  (`ci/lint.py`), coverage accounting against the user's copy of the
  standard (`ci/coverage.py`), offline analysis (`analysis/analyze.py`),
  calibration against known Linux deviations (`known/*.known`).
- File descriptors: `open`, `close`, `dup`, `pipe`, `fcntl`, `lseek`,
  `read`, `write`, `<fcntl.h>`, `<unistd.h>` (part).
- Processes: `fork`, `wait`, `_Exit`, `exit`, `exec`.
- Utilities: `true`, `false`.
- Every module reviewed independently; findings applied.

## Next

1. **Signals**: `sigaction`, `kill`, `raise`, `sigprocmask` and
   `pthread_sigmask`, `sigpending`, `sigsuspend`, `sigwait` and
   `sigtimedwait`, `sigqueue`, `alarm`, `pause`, `<signal.h>`, the XSH 2.4
   rules (generation and delivery, default actions, signal-safe
   functions, interrupted functions and SA_RESTART).
2. **Your OS**: run the suite on the OS under development as soon as it can
   run a POSIX shell and a C compiler (or cross-compiled tests with a
   small runner). Results are judged by `analyze.py` on another machine.
3. **BSD reference runs** under QEMU (FreeBSD, NetBSD, OpenBSD), then
   macOS and illumos, triaged with `analyze.py matrix`. Waiting on the
   images, which the user will supply.
4. **Spec-model generator** (`gen/`): parse the user's local copy of the
   standard into declarations, options and anchors (kept under `build/`,
   never committed), then Tier 0 header tests for every header, with
   header mutants.
5. **Files and directories**: `stat` family, `link`/`unlink`/`rename`,
   `mkdir`/`rmdir`, `opendir`/`readdir`, `chmod`/`chown`, `utimensat`,
   `access`, `truncate`, `umask`.
6. **Remaining todos** in the done modules (`ci/coverage.py --show MODULE`
   lists them): unprivileged EACCES cases, pseudo-terminal cases,
   environment cases (ENOSPC, EROFS, ENFILE), SA_NOCLDWAIT.
7. **stdio**: buffering, the XSH 2.5.1 handle rules, `fflush` and `fclose`
   offset rules (glibc 2.39 and musl both miss the input case).
8. **Threads**: `pthread_create`, joins, cancellation, mutexes, condition
   variables, thread-specific data.
9. **Shell and utilities** (XCU): the shell language first, then the
   utilities the OS needs.
10. **Controller**: copy the tree to a VM or test machine over ssh, run,
    copy the results back.

## Standard issues

Found while testing; kept in `standard-issues-report.md` (the full write-up
for The Austin Group) and indexed in `standard-issues.md`. File them on
https://austingroupbugs.net after checking for existing reports.
