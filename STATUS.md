# Status

Read this first when resuming work in a new session.

Copyright (c) 2026 Nicholas Martin. All rights reserved.

## Where things are

- Design doc: https://claude.ai/artifact/BCH6E4hts9mcGb2t44GRW5
- Standard: the user's own copy of the POSIX.1-2024 HTML edition
  (`susv5.tar`, personal-use download from The Open Group). It is never
  committed or bundled (RULES.md). Tools that read it take `--standard DIR`.
- This repository travels as git bundles; see "Bundles" below.

## Done (phase 1, "Skeleton")

- `core/pc.h`, `core/pc.c`: C test helper (phases, results, checks, errno
  names from XSH 2.3, setup helpers, ENOSPC guard).
- `core/pc.sh`: shell test helper, built-ins only, immune to shadowed
  `printf` and `[`.
- `core/pcexec.c`: runs one test: process group, private temp dir, clean
  environment, umask 022, stdin from /dev/null, limits, user switch,
  timeout that kills the whole group, status file.
- `core/probe.c` + `core/options.list` (via `gen/gen_probe.py`): option
  claims per XBD `<unistd.h>` rules, system info, `_CS_PATH`, time, sleep.
- `core/trust.sh`: 17 known-answer checks of the shell features the
  driver relies on; run in the driver's shell and in the tests' shell.
- `posixcheck.sh`: the driver (trust checks, helper build, self-tests,
  claims, profile, module- and assertion-level prerequisites, `--only`,
  `--option`, `--resume`, `--mutant`), writing `run.log`.
- `core/selftest/`: 28 harness self-tests with known outcomes; any
  mismatch aborts the run.
- Modules (389 assertions):
  - `xbd/fcntl.h` (5): FD_CLOEXEC, FD_CLOFORK, O_CLOEXEC, O_CLOFORK
    defined; fd flags bitwise-distinct.
  - `xsh/2.6-file-descriptor-allocation` (1): lowest free descriptor,
    tested with a gap.
  - `xsh/open` (50): access modes, O_CREAT/O_EXCL/O_TRUNC/O_APPEND,
    descriptor flags, FIFO open (blocking, non-blocking, EINTR, ENXIO),
    openat(), EMFILE, ELOOP, trailing slashes, timestamps, ownership.
  - `xsh/close` (12): deallocation, EBADF, lock release, FIFO data,
    OFD-lock and pipe description lifetime, sockets, posix_close().
  - `xsh/dup` (15): dup(), dup2(), dup3().
  - `xsh/pipe` (10): pipe(), pipe2().
  - `xsh/fcntl` (43): F_DUPFD/F_DUPFD_CLOEXEC, descriptor and status
    flags, exec and FD_CLOEXEC, socket owner (F_SETOWN, F_SETOWN_EX),
    process-owned record locks (conflicts, ranges, whence, waiting,
    EINTR, inheritance, exit), OFD-owned locks.
  - `xsh/lseek` (14): whence, beyond EOF, gaps, shared offset, errors,
    SEEK_HOLE/SEEK_DATA.
  - `xsh/read` (23): regular files, EOF, zero length, unwritten bytes,
    pipes and FIFOs (EOF, EAGAIN, blocking, partial, EINTR), sockets
    (EAGAIN, ENOTCONN, ECONNRESET), atime, pread().
  - `xsh/write` (27): offsets, growth, zero length, O_APPEND (also two
    processes at once), RLIMIT_FSIZE (partial, EFBIG, SIGXFSZ), EINTR
    before and after data, {PIPE_BUF} atomicity with four writers,
    non-blocking pipes, EPIPE and SIGPIPE, sockets, mtime, pwrite().
  - `xsh/fork` (19): return values and IDs, descriptors and
    FD_CLOFORK, directory streams, times, alarm, pending signals,
    private and shared mappings, timers, threads, process-shared
    mutexes, CPU-time clock, semaphores, inherited attributes, _Fork().
  - `xsh/wait` (21): blocking and immediate wait, status macros, pid
    selection (any, one, own group, other group), WNOHANG, WUNTRACED,
    WCONTINUED, EINTR, ECHILD, EINVAL, consumption, SIGCHLD clearing,
    reparenting, last-thread exit.
  - `xsh/_Exit` (14): status bits (wait, waitid, SIGCHLD siginfo),
    _exit/_Exit equivalence, no atexit or at_quick_exit handlers, no
    flushing, descriptors closed, SIGCHLD, zombies, XSI SIGCHLD ignored
    (no zombie), orphaned process groups, no thread cleanup, no signal
    handlers.
  - `xsh/exit` (13): status bits, atexit order, handlers registered
    during exit, repeated registration, no at_quick_exit handlers, a
    handler that does not return, flushing (after the handlers), stream
    offset synced, SIGCHLD, EXIT_SUCCESS/EXIT_FAILURE.
  - `xsh/exec` (58): each test runs itself again as the new image, which
    reports what it got: argv and environment (execv, execl, execve,
    execle, environ), descriptors (kept, FD_CLOEXEC, FD_CLOFORK, locks,
    directory streams), signal actions, mask and pending signals,
    inherited attributes (alarm, cwd, umask, IDs, limits, times, CPU-time
    clocks), timers, cancellation state, exec from a thread, atime,
    errors, execvp/execlp search and the sh fallback, fexecve, start-up
    locale and floating-point state, XSI set-ID bits, shared memory,
    nice.
  - `xsh/sigaction` (23): querying and replacing actions, SIGKILL and
    SIGSTOP never masked, one- and three-argument handlers, siginfo_t for
    kill() and sigqueue(), SA_NOCLDSTOP, SA_ONSTACK, SA_RESETHAND,
    SA_RESTART, SA_NODEFER, the handler's mask, signal() round trip,
    EINVAL, queuing with SA_SIGINFO, SIG_IGN.
  - `xsh/kill` (15): to a process, to the own and another group, to all
    (pid -1, as an unused user ID from the profile), the null signal,
    delivery before return, permissions (same user, EPERM, the receiver's
    real and saved IDs, SIGCONT within and outside the session), zombies,
    EINVAL, ESRCH. `xsh/pthread_sigmask` (11): SIG_BLOCK, SIG_UNBLOCK,
    SIG_SETMASK, oset and queries, delivery on unblocking, SIGKILL and
    SIGSTOP, EINVAL returned, queries with any how (2024), sigprocmask(),
    per-thread masks, SIGSEGV and others sent while blocked.
    `xsh/raise` (3): waits for the handler, thread-directed, EINVAL.
  - `xsh/sigpending` (3), `xsh/sigsuspend` (5), `xsh/sigwait` (6),
    `xsh/sigtimedwait` (7, with sigwaitinfo()), `xsh/sigqueue` (7),
    `xsh/alarm` (4), `xsh/pause` (3): the pending set per thread and
    after delivery; sigsuspend()'s mask during and after the wait,
    termination, SIGKILL; selecting and clearing, queued remainders,
    lowest realtime signal first, thread-directed and single-waiter
    returns; siginfo_t and values, timeouts on CLOCK_MONOTONIC, zero
    timeouts; values and SI_QUEUE to another process, the null signal,
    delivery to self before return, EAGAIN at {SIGQUEUE_MAX}, EINVAL,
    EPERM; alarm firing, cancelling, rescheduling, not stacking; pause()
    through handlers, termination and ignored signals.
  - `xbd/unistd.h` (2): POSIX_CLOSE_RESTART, SEEK_*; `xbd/fcntl.h` now 8.
  - `xcu/true` (3), `xcu/false` (3): each checks the shell's handling and
    a direct exec (XCU 1.6).
- `ci/lint.py`: traceability, anchor and `since` checks; `ci/mutate.sh`: mutation
  checks (434 mutants); `ci/catalog.py`: shared parser.
- `analysis/analyze.py`: load, runs, verdict, failures, matrix, diff,
  coverage, html, calibrate (checks a reference run against
  `known/*.known`, its expected failures).
- `docs/linux-deviations.md`: what Linux, glibc, musl and dash do
  differently from POSIX.1-2024 (about 145 entries, with sources), and
  what posixcheck has observed. `known/linux-glibc.known`,
  `known/linux-musl.known`: the expected non-PASS results.
- `ci/coverage.py`: splits each page of the user's copy of the standard
  into requirement units (every normative sentence, list item, table row
  and struct member; XCU 1.4 default sections), identified by section and
  hash. Each module's `<page>.cov` accounts for every unit; lint requires
  the file. All 18 modules: 1,174 units, 405 asserted, 77 partial, 281
  classified (app/impl/optional/none/untestable), 411 todo, 0
  unaccounted. Whole standard: about 43,200 units on reference pages
  (XSH 25,482; XCU 14,615; XBD 3,117) plus the chapters.
- An independent review of every classification (34 findings: hidden
  testable requirements, overstated coverage, units the extractor missed)
  was applied.

## Verified (on Linux x86_64, glibc, in the development container)

- The driver refuses a compiler that accepts calls to undeclared
  functions, adding -Werror=implicit-function-declaration where that
  helps (gcc only warns by default, which let a test link against glibc's
  undeclared dup3()). Self-test ST.c.014 guards this.
- Tests start with only descriptors 0-2 open, no signal blocked and
  every signal at its default action (pcexec; self-tests ST.c.015,
  ST.c.016), whatever the driver's shell inherited.
- Interrupt tests use alarm() re-armed in the handler (setitimer() was
  removed in Issue 8); after two alarms the handler unblocks a call that
  wrongly restarted, so it FAILs instead of timing out.

- Driver and tests under dash, bash --posix, busybox sh, ksh93, mksh, yash
  and posh, each with gcc and clang. (ksh93 exposed a driver bug: a bare
  `|` in `${x%%|*}` is pattern alternation there. Fixed by quoting.)
- As root and as an unprivileged user: same results.
- With musl (musl-gcc): same verdicts as glibc.
- Results: all PASS except known Linux deviations, and `calibrate`
  finds no unexplained result with glibc or musl:
  - FD_CLOFORK/O_CLOFORK missing (2 FAIL, 3 blocked);
  - O_CREAT with a trailing slash gives EISDIR (XSH.open.044, .045);
  - glibc lacks O_TTY_INIT (XSH.open.050 UNRESOLVED; musl passes).
- Mutation: 0 missed. Unchecked here (baseline not PASS): the O_CLOFORK,
  trailing-slash and O_TTY_INIT mutants; they need a system that passes
  those assertions.
- Planted breakage is caught: a broken result rule fails the self-tests;
  a broken `read` fails the trust checks; six kinds of catalog damage fail
  the linter; a corrupted log is refused by the analyzer.
- Independent review of every assertion and test against the standard's
  text: no test demands more than the text. Its findings were fixed:
  exec-path checks for true/false, a real gap in the 2.6 test, header
  defects owned by `xbd/fcntl.h`, wording, anchors, ENOSPC guard.

## Not yet verified

- Every result so far comes from one kernel (Linux 6.18 in the container).
- Any BSD, macOS or illumos run. The mutation shims use LD_PRELOAD and
  `-ldl` (set `PC_MUTANT_LDFLAGS=` on the BSDs, where dlsym is in libc).

## Next

See `docs/roadmap.md`. Next up: signals.

## Scope and rules

posixcheck is for personal use, to verify an OS under development. It is
not distributed. `RULES.md` lists the rules that keep it within the
standard's terms and other projects' licenses; read it before adding
anything.

## Issues in the standard

`docs/standard-issues-report.md` is the full write-up of conflicts,
mistakes and unclear points found in POSIX.1-2024 (SI-1 to SI-32, Q-1 to
Q-13 so far), for The Austin Group; `docs/standard-issues.md` indexes them
by assertion. A shared copy: https://claude.ai/artifact/L3t5goEQYUJKhbTWAj2BXF.
Reviews should report any new ones; add them to both files.

## Open decisions

- Since which edition XSH.open.017 (ENAMETOOLONG for a long component) is
  unconditional: when `_POSIX_NO_TRUNC` became mandatory.

## Bundles

Each checkpoint is sent as `posixcheck-NNNN.bundle`, a complete copy of
`main` (each one contains all earlier ones, so only the newest is needed).
Since bundle 0013 the history includes the GitHub repository's first
commit (github.com/nmartin0/conformance_suite), so a clone of that
repository updates by fast-forward:

    git pull --ff-only ~/Downloads/posixcheck-NNNN.bundle main

To start from a bundle alone: `git clone posixcheck-NNNN.bundle posixcheck`.
