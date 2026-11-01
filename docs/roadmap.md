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

## Status

Every function page and every utility page has a module (1,026 modules,
8,925 assertions), all reviewed blind. Twenty-one header pages and the sccs
page have none (see Next, 1). The suite has been run, as root and as an
ordinary user, on Linux, FreeBSD and an illumos distribution (NetBSD and
OpenBSD with an earlier version of it). Each run was triaged against the
standard's text alone and the test bugs it exposed were fixed (reports in
`reports/` and `reports/triage/`). Planted faults (`docs/hardening.md`) have
been written for 3,277 assertions; the rule that every assertion needs one
is being retired (Next, 2).

The coverage ledger (`ci/coverage.py`, 36,076 units on pages with modules):
58% covered by assertions, 5% partly covered, 31% set aside as not
testable or not a requirement on the implementation, 6% `todo`, 0.4% not
yet classified. A random sample of the set-aside units suggests about one
in ten is testable after all.

## Next

### 1. Missing tests (first: nothing adds more power)

- `ci/coverage.py`: fail on a standard page that has no module; today a
  page with no module is never counted as unaccounted.
- Modules for the 21 header pages without one (ctype.h, devctl.h,
  endian.h, fmtmsg.h, grp.h, iconv.h, libgen.h, libintl.h, monetary.h,
  ndbm.h, netinet_tcp.h, pwd.h, search.h, stdatomic.h, string.h, strings.h,
  sys_time.h, sys_times.h, sys_uio.h, syslog.h, threads.h) and for sccs.
- Classify the 130 unclassified units (mostly the ctype and strcasecmp
  families).
- Write tests for the 2,175 `todo` units, largest first (vi, ex, file, sh,
  bc). Cover before hardening.
- Re-audit the set-aside units (none, untestable, impl, optional),
  starting with "may fail" errors (if the call fails for that cause, the
  error must be the one listed), implementation-defined values (checkable
  for range and consistency) and "untestable: needs privilege" (the suite
  runs as root).

### 2. Verifying the suite itself

Two questions, kept apart. Whether a test checks exactly what its
assertion says can be decided mechanically; whether the assertion is what
the standard requires can only be cross-checked (independent readings,
real systems, the standards committee).

- Checks that ran: every test logs each check it reaches; a test that
  ends without reaching its checks is reported as a test failure, not a
  pass. (Change to `core/pc.c` and `core/pc.sh`.)
- Validated or not: each assertion gets a status. VALIDATED when the blind
  second reading agrees and its verdict on at least two independent
  systems either matches or every disagreement is a triaged, documented
  deviation of that system; otherwise UNVALIDATED. Uses the run data
  already collected. (The `derived` field and verdict of the earlier
  design.)
- Allowed and forbidden outcomes as data: each check declares the results
  it accepts; a self-test feeds it every accepted result (must PASS) and
  one result from each forbidden class (must FAIL). Exact per check, and no
  faulty implementations to build. Pilot on three modules (one function,
  one utility, one shell section) and time it before converting the rest.
- Sentence to check traceability: every requirement sentence an assertion
  covers maps to at least one check and every check to a sentence;
  enforced by the lint.
- Planted faults become a spot-check: retire the lint rule that every
  assertion declares a MUTANT; keep `ci/mutate.sh` for sampled audits, and
  read the reason column (a fault "caught" by a crash proves nothing).
  Faults that cannot be judged on the development machine are collected in
  `ci/pending.list` (`docs/pending-checks.md`).
- A different second reader (another model, or a person) for disputed and
  ambiguous assertions; truly ambiguous text goes to the standards
  committee.
- Cheap robustness runs: repeated runs to find flaky tests; the C tests
  built with the strictest warnings and with memory-error detectors; every
  shell test under several shells; a run in a stripped environment (env -i,
  odd umask, time zone and locale).
- Before the next hardening or verification batch: the final full run of
  the current tree and the blind reading of the assertions added in
  hardening batches 3 and 4, both skipped for bundle 0052.

### 3. Reference systems

Real systems are independent readings of the standard: where they agree
against a test, the test is re-read; the text decides. Majority never does.

- Re-run NetBSD, OpenBSD and the illumos distribution with the current
  suite.
- New systems, in order of value: PureDarwin (ready-made x86_64 images,
  pending a check of where they are hosted), QNX (free non-commercial
  licence, QEMU x86_64 images, needs an account), Oracle Solaris 11.4 CBE
  (free for non-production use), macOS on real Apple hardware (licence
  forbids other hardware).
- The user's own BSD fork, the eventual target for full conformance.
- Triage each failure against the standard before touching a test.

### 4. Untested residue

Terminal-dependent utilities beyond the pty tests already written for
stty, tabs, tput and vi (talk, write, vi screen contents, -r/-t/-w), job
control beyond one assertion, the ex/vi command sets, history/mail/vi-mode
parts of `sh`.

### 5. Cleanup

Simplify the older option and root handling in the first 231 modules;
write the remaining entries of `standard-issues-report.md` (Q-75 onward,
SI-89 to SI-96).

### 6. Python port (deferred), and the controller that copies the tree to a
VM or test machine over ssh, runs it and copies results back.

### Not now

- A full formal model of POSIX checked against recorded traces. Worth it
  only for the file-system calls, where the standard allows many outcomes,
  and only after the items above.
- A fully conforming reference system built to check the suite against.
  Several real systems plus the text do that job.

## Standard issues

Found while testing; kept in `standard-issues-report.md` (the full write-up
for The Austin Group) and indexed in `standard-issues.md`. File them on
https://austingroupbugs.net after checking for existing reports.
