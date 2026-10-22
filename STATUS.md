# Status

Read this first when resuming work in a new session.

Copyright (c) 2026 Nicholas Martin. All rights reserved.

## Where things are

- Design doc: https://claude.ai/artifact/BCH6E4hts9mcGb2t44GRW5
- Standard: each contributor's own copy of the POSIX.1-2024 HTML edition
  (`susv5.tar`, personal-use download from The Open Group). It is never
  committed or bundled. Tools that read it take `--standard DIR`.
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
- Modules (34 assertions):
  - `xbd/fcntl.h` (5): FD_CLOEXEC, FD_CLOFORK, O_CLOEXEC, O_CLOFORK
    defined; fd flags bitwise-distinct.
  - `xsh/2.6-file-descriptor-allocation` (1): lowest free descriptor,
    tested with a gap.
  - `xsh/open` (18), `xsh/close` (4).
  - `xcu/true` (3), `xcu/false` (3): each checks the shell's handling and
    a direct exec (XCU 1.6).
- `ci/lint.py`: traceability and anchor checks; `ci/mutate.sh`: mutation
  checks (48 mutants); `ci/catalog.py`: shared parser.
- `analysis/analyze.py`: load, runs, verdict, failures, matrix, diff,
  coverage, html.

## Verified (on Linux x86_64, glibc, in the development container)

- Driver and tests under dash, bash --posix, busybox sh, ksh93, mksh, yash
  and posh, each with gcc and clang. (ksh93 exposed a driver bug: a bare
  `|` in `${x%%|*}` is pattern alternation there. Fixed by quoting.)
- As root and as an unprivileged user: same results.
- Results: all PASS except FD_CLOFORK/O_CLOFORK, which this glibc lacks
  (2 FAIL, 3 blocked), matching os-test's published findings.
- Mutation: 46 of 48 mutants caught, 0 missed. The 2 O_CLOFORK mutants
  are unchecked here (baseline not PASS); they need a system with
  O_CLOFORK.
- Planted breakage is caught: a broken result rule fails the self-tests;
  a broken `read` fails the trust checks; six kinds of catalog damage fail
  the linter; a corrupted log is refused by the analyzer.
- Independent review of every assertion and test against the standard's
  text: no test demands more than the text. Its findings were fixed:
  exec-path checks for true/false, a real gap in the 2.6 test, header
  defects owned by `xbd/fcntl.h`, wording, anchors, ENOSPC guard.

## Not yet verified

- Any BSD, macOS or illumos run. The mutation shims use LD_PRELOAD and
  `-ldl` (set `PC_MUTANT_LDFLAGS=` on the BSDs, where dlsym is in libc).

## Next

1. Run on FreeBSD, NetBSD, OpenBSD (VMs), then macOS and illumos; triage
   with `analyze.py matrix`.
2. `gen/`: the spec-model generator (parse the HTML into declarations,
   options and anchors with paragraph hashes), then Tier 0 header tests
   for every header, with header mutants.
3. Completeness checker: count requirement sentences per page vs.
   assertions, so coverage has a denominator.
4. More open()/close() assertions the review listed: O_CREAT on an
   existing file, EISDIR for O_CREAT on a directory, trailing slashes,
   ELOOP on a symlink loop, O_SEARCH/O_EXEC, FIFO ENXIO, timestamps,
   openat(), posix_close(); 2.6 for dup, F_DUPFD, pipe.
5. `xcu/getconf`: cross-check getconf against sysconf/confstr.
6. Controller script: copy the tree to a VM over ssh, set PC_TREE to the
   tree's hash, run, copy results back.

## Open decisions

- Project name (`posixcheck` is a placeholder).
- Since which edition XSH.open.017 (ENAMETOOLONG for a long component) is
  unconditional: when `_POSIX_NO_TRUNC` became mandatory.
- Permission for proprietary use of the standard's text in the generated
  spec model: ask The Open Group (the download is for personal use).

## Bundles

Each checkpoint is sent as `posixcheck-NNNN.bundle`, a complete copy of
`main`. To resume in a new session, attach the latest bundle and clone it:
`git clone posixcheck-NNNN.bundle posixcheck`. To update an existing clone:
`git pull /path/to/posixcheck-NNNN.bundle main`.
