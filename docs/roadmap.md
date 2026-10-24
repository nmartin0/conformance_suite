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
- Signals: `sigaction`, `kill`, `raise`, `pthread_sigmask`/`sigprocmask`,
  `sigpending`, `sigsuspend`, `sigwait`, `sigtimedwait`/`sigwaitinfo`, `sigqueue`,
  `alarm`, `pause`, `<signal.h>`, the XSH 2.4 rules, the sigset functions,
  `signal`, `sigaltstack`, `psiginfo`/`psignal`, `sig2str`/`str2sig`, `killpg`,
  `abort`, the async-signal-safe list (2.4.3.016).
  The coverage todos of the signal, process, close, fork and fcntl modules
  were worked through (32 assertions); the few left are listed under Next.
- Spec model and Tier 0 (`gen/specmodel.py`, `gen/tier0.py`): every
  header's macros, types, structures, functions and variables, generated
  from the user's copy under `build/`; about 3,350 checks.
- Files and directories: `stat`/`lstat`/`fstatat`, `fstat`,
  `link`/`linkat`, `unlink`/`unlinkat`, `rename`/`renameat`, `mkdir`/`mkdirat`, `rmdir`,
  `chmod`/`fchmodat`, `fchmod`, `chown`/`fchownat`, `fchown`, `lchown`, `access`/`faccessat`,
  `truncate`, `ftruncate`, `umask`, `futimens`/`utimensat`/`utimes`, and the directory
  streams (`opendir`/`fdopendir`, `readdir`/`readdir_r`, `closedir`, `rewinddir`, `dirfd`,
  `seekdir`, `telldir`, `scandir`/`alphasort`).
- stdio (in progress): `fflush`, `fclose`, `ungetc`, `setvbuf`.
- Utilities: `true`, `false`.
- Every module reviewed independently and the findings applied, except
  `ungetc` and `setvbuf` (review running).

## Next

1. **Signals** (remaining): the few signal todos left (`ci/coverage.py
   --show` lists them): machine-dependent SIGILL/SIGFPE/SIGTRAP codes,
   `<stdatomic.h>` in handlers, signal() from several threads, the
   disputed SIGCHLD/SIGURG/SIGWINCH queue case (SI-24), the sigaction
   member checks that belong to Tier 0.
2. **Your OS**: run the suite on the OS under development as soon as it can
   run a POSIX shell and a C compiler (or cross-compiled tests with a
   small runner). Results are judged by `analyze.py` on another machine.
3. **BSD reference runs** under QEMU (FreeBSD, NetBSD, OpenBSD), then
   macOS and illumos, triaged with `analyze.py matrix`. Waiting on the
   images, which the user will supply.
3a. **Linux-ism audit** (done): independent reviewers given only the
   standard and the tests (not the Linux results) re-read every module
   reviewed before ungetc for expectations that Linux meets but the text
   does not require (glibc and musl agreeing does not rule these out, since
   both run on one kernel). 76 findings in eight areas, 14 new questions
   (Q-42 to Q-55) and 5 entries (SI-68 to SI-72); see the review log.
4. **Header mutants for Tier 0**, and Tier 0 for the prose-only headers
   (`<stdint.h>`, `<float.h>` and the like) that `gen/specmodel.py` does
   not yet read.
5. **Files and directories** (remaining): `posix_getdents` (no reference libc has it yet).
6. **Remaining todos** in the done modules (`ci/coverage.py --show MODULE`
   lists them): unprivileged EACCES cases, pseudo-terminal cases,
   environment cases (ENOSPC, EROFS, ENFILE), SA_NOCLDWAIT.
7. **stdio** (done: `fflush`, `fclose`, `ungetc`, `setvbuf`, `fopen`, `fdopen`,
   `fseek`, `ftell`): next the XSH 2.5.1 handle rules, the XSH 2.5.2
   wide-stream rule for psignal() and psiginfo(), and an `fmemopen` module
   (the fseek review found that neither glibc nor musl writes the NUL at
   buf[0] for mode "w"); EINTR on a flushing fseek(), and EFBIG at the
   offset maximum.
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
