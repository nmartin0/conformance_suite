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
- stdio: `fopen`, `fdopen`, `fflush`, `fclose`, `ungetc`, `setvbuf`, `fseek`/`fseeko`,
  `ftell`/`ftello`, and the XSH 2.5.1 stream and descriptor handle rules.
- Utilities: `true`, `false`.
- Every module reviewed independently and the findings applied; the
  Linux-ism audit (3a) and the conforming-variant retrofit with its own
  independent review are done (see the review log).

- Round of stdio, process and file modules (about 15 modules; 1880 assertions, 2807 mutants, 1451 variants in all); deviations O.34 to O.53, SI-73 to SI-88 and Q-62 to Q-74 recorded.

## Status (lean mode, nothing executed yet)

Every function page, every header (Tier 0 plus semantic modules under
`modules/xbd`) and every utility page (`modules/xcu`, including the shell
command language chapter 2) now has a module, all reviewed blind. About
970 modules in all. Nothing has been run against any C library or shell:
the modules are written from the standard alone and have no mutants or
variants yet.

## Next

1. **First runs on real systems**, starting with the user's own OS: expect
   test defects (hand-written expected strings, timing) as well as findings;
   triage each failure against the standard before touching a test.
2. **Hardening**: mutants and variants for the lean-mode modules, once the
   suite is stable; header mutants for Tier 0.
3. **Untested residue**: terminal-dependent utilities (stty, tabs, tput, vi,
   talk, write), job control beyond one assertion, the ex/vi command sets
   (many `todo` units in the `.cov` files), history/mail/vi-mode parts of
   `sh`.
4. **Cleanup**: simplify the older option and root handling in the first
   231 modules; write the remaining entries of `standard-issues-report.md`
   (Q-75 onward, SI-89 to SI-96).
5. **Python port** (deferred), and the controller that copies the tree to a
   VM or test machine over ssh, runs it and copies results back.

## Standard issues

Found while testing; kept in `standard-issues-report.md` (the full write-up
for The Austin Group) and indexed in `standard-issues.md`. File them on
https://austingroupbugs.net after checking for existing reports.
