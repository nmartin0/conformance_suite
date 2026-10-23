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
- Modules (161 assertions):
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
  - `xbd/unistd.h` (2): POSIX_CLOSE_RESTART, SEEK_*; `xbd/fcntl.h` now 8.
  - `xcu/true` (3), `xcu/false` (3): each checks the shell's handling and
    a direct exec (XCU 1.6).
- `ci/lint.py`: traceability and anchor checks; `ci/mutate.sh`: mutation
  checks (234 mutants); `ci/catalog.py`: shared parser.
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
  the file. Current six modules: 264 units, 30 asserted, 13 partial, 104
  classified (app/impl/optional/none/untestable), 117 todo, 0
  unaccounted. Whole standard: about 42,900 units on reference pages
  (XSH 25,368; XCU 14,527; XBD 3,029) plus the chapters.
- An independent review of every classification (34 findings: hidden
  testable requirements, overstated coverage, units the extractor missed)
  was applied.

## Verified (on Linux x86_64, glibc, in the development container)

- The driver refuses a compiler that accepts calls to undeclared
  functions, adding -Werror=implicit-function-declaration where that
  helps (gcc only warns by default, which let a test link against glibc's
  undeclared dup3()). Self-test ST.c.014 guards this.

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

1. Run on FreeBSD, NetBSD, OpenBSD under QEMU (no KVM here, so software
   emulation), then macOS and illumos; triage with `analyze.py matrix`.
   Waiting on images from the user: the BSD download sites are blocked
   from the development container.
2. `gen/`: the spec-model generator (parse the HTML into declarations,
   options and anchors with paragraph hashes), then Tier 0 header tests
   for every header, with header mutants.
3. Turn the `todo` units of open, close and fcntl.h into assertions
   (`ci/coverage.py --show MODULE` lists them).
4. open(): O_SEARCH/O_EXEC (after the fcntl.h header checks), EACCES
   cases (unprivileged user), O_SYNC, pseudo-terminal cases, environment
   cases (ENOSPC, EROFS, ENFILE). close(): posix_close(), locks, FIFO and
   socket cases. Then read/write.
5. `xcu/getconf`: cross-check getconf against sysconf/confstr.
6. Controller script: copy the tree to a VM over ssh, set PC_TREE to the
   tree's hash, run, copy results back.

## Scope and rules

posixcheck is for personal use, to verify an OS under development. It is
not distributed. `RULES.md` lists the rules that keep it within the
standard's terms and other projects' licenses; read it before adding
anything.

## Open decisions

- Since which edition XSH.open.017 (ENAMETOOLONG for a long component) is
  unconditional: when `_POSIX_NO_TRUNC` became mandatory.

## Bundles

Each checkpoint is sent as `posixcheck-NNNN.bundle`, a complete copy of
`main`. To resume in a new session, attach the latest bundle and clone it:
`git clone posixcheck-NNNN.bundle posixcheck`. To update an existing clone:
`git pull /path/to/posixcheck-NNNN.bundle main`.
