# posixcheck

A proprietary, unofficial, clean-room conformance checker for POSIX.1-2024
(Issue 8), written in POSIX `sh` and C.

Not affiliated with the IEEE or The Open Group. POSIX is a trademark of the
IEEE. This project does not certify conformance.

Copyright (c) 2026 Nicholas Martin. All rights reserved. See `LICENSE`.

For personal use, to verify an operating system under development. Read
`RULES.md` before adding anything: it keeps the suite within the
standard's terms and other projects' licenses.

Design: https://claude.ai/artifact/BCH6E4hts9mcGb2t44GRW5
Status and next steps: `STATUS.md`.

## Running it on a system

Copy this directory to the system under test and run:

    ./posixcheck.sh                     # everything the system claims
    ./posixcheck.sh xsh/open xcu/true   # chosen modules
    ./posixcheck.sh --only XSH.open.003 # one assertion
    ./posixcheck.sh --help

It needs only the system's own `sh`, a C compiler (`c17`, else `cc`) and
`mkdir`. It writes `results/run.log` and never deletes anything, so each
run needs a new `--results` directory (or `--resume` to continue one).

What a run does, in order:

1. **Trust checks.** Known-answer checks of every shell feature the driver
   uses, in the shell running the driver and in the shell the tests use. If
   one fails, the run stops with `ABORT`: a wrong log is worse than none.
2. **Helpers.** Builds `pcexec` (runs each test in its own process group,
   private temporary directory and clean environment, with a timeout) and
   `probe` (reports claimed options, limits and system facts).
3. **Harness self-tests.** Synthetic tests with known outcomes
   (`core/selftest/`) prove the driver classifies results correctly on this
   system before any real result is logged.
4. **Claims.** Which options the system claims, from `<unistd.h>` macros and
   `sysconf()`, recorded in the log header.
5. **Tests.** Every assertion yields exactly one line in `run.log`.

Requirements on the environment:

- The results directory must not carry default ACLs (some systems apply
  them instead of the umask, which breaks the `O_CREAT` mode tests).
- Running as root is not required. Tests that need it say so
  (`needs root`) and are UNRESOLVED without it.

A profile (`--profile FILE`) supplies what the environment provides; see
`profiles/example.prof`.

## Results

`run.log` is plain text: `#` header lines, then one tab-separated line per
assertion: code, assertion, milliseconds, note.

| Code | Meaning |
| --- | --- |
| PASS | the assertion held |
| FAIL | the assertion is demonstrably false |
| UNRESOLVED | the test could not decide (setup failed, timeout, blocked by a failed prerequisite, environment missing) |
| NOTINUSE | does not apply in this configuration |
| UNSUPPORTED | conditional assertion; the option is not claimed |
| UNTESTED | no test yet, or untestable (with a reason) |
| UNINITIATED | the test never produced a result (did not build, exited without reporting) |
| INSPECT | output saved for a person to check; also a FAIL of a `disputed` assertion |

An assertion of class `disputed` tests one reading of text that the standard
itself leaves open (its `note` names the standard issue); its FAIL is
logged as INSPECT, not counted against the system.

`results/out/<ID>.txt` holds each test's full output.

## Analyzing results (on another computer)

Copy `results/` off each system, then:

    analysis/analyze.py load db.sqlite results-freebsd/ --name freebsd-15
    analysis/analyze.py load db.sqlite results-netbsd/ --name netbsd-11
    analysis/analyze.py verdict db.sqlite      # per option tier
    analysis/analyze.py failures db.sqlite     # FAILs with the rule text
    analysis/analyze.py matrix db.sqlite       # systems side by side, by pattern
    analysis/analyze.py diff db.sqlite A B     # regressions and fixes
    analysis/analyze.py coverage db.sqlite
    analysis/analyze.py html db.sqlite report.html
    analysis/analyze.py calibrate db.sqlite linux-glibc known/linux-glibc.known

`calibrate` is for reference systems (Linux, the BSDs), whose results judge
the tests rather than the system: every non-PASS result must match a
documented deviation in `known/*.known` (see `docs/linux-deviations.md`).
An unlisted FAIL is SUSPECT (check the test first); a listed entry that now
passes is STALE.

Reference runs use a profile that names what the machine provides; for the
Linux container that is `profiles/linux-reference.prof`
(`./posixcheck.sh --profile profiles/linux-reference.prof`). Tests that need
something a profile lacks (for example `needs kill_uid`, an otherwise unused
user ID for `kill(-1)`) are UNRESOLVED without it. On your own machine, only
name a user ID that no process uses: XSH.kill.005 and 012 signal every
process of that user.

Waivers (`waivers/*.waiver`) are applied here, never on the system.

## Layout

    posixcheck.sh        the driver
    core/                C helper (pc.h, pc.c), shell helper (pc.sh),
                         pcexec.c, probe.c, trust.sh, options.list,
                         selftest/ (harness self-tests)
    modules/<vol>/<page>/  one module per page of the standard:
        MODULE           manifest: module, page, volume, option, origin, uses
        <page>.asr       assertions, in our own words
        t/NNN-slug.c|sh  one test per assertion
        mutants/         planted faults (C preload library, or shell
                         functions) used by ci/mutate.sh
    ci/                  lint.py, mutate.sh (off-system)
    analysis/            analyze.py (off-system)
    gen/                 generators run on a development machine
    docs/                project documents (docs/README.md): issues found in
                         the standard, for The Austin Group; known
                         deviations of reference systems; the roadmap
    known/               expected non-PASS results on reference systems
    profiles/  waivers/

Volumes: `xbd` (Base Definitions, including headers such as `xbd/fcntl.h`),
`xsh` (System Interfaces), `xcu` (Shell and Utilities). An assertion ID is
`VOLUME.page.NNN`, for example `XSH.open.003`; its module is derived from it.

## Writing a test

1. Read the passage in your own copy of the standard. Write the assertion in
   `<page>.asr` in your own words, with an `anchor` (`page#SECTION/item`).
   Never copy test code from another suite.
2. Arrange for exactly one error condition to apply: XSH 2.3 lets an
   implementation report any one of several that apply.
   Lowering a limit with `setrlimit()` also limits the test itself: keep
   RLIMIT_NOFILE above every open descriptor, and run anything under a
   lowered RLIMIT_FSIZE in a child that reports over a pipe, since the
   result file is subject to it too.
3. Write `t/NNN-slug.c` (or `.sh`): `pc_begin`, setup, `pc_test`, checks,
   `pc_pass`. Put `ASSERTION <id>` and one or more `MUTANT <name>` lines
   in the header comment.
4. Implement each mutant: for C, a fault in
   `modules/xsh/<function>/mutants/<function>.c`; for a utility, a file
   `modules/xcu/<utility>/mutants/<name>.sh` defining functions that shadow
   it (and, for the exec path, a broken copy first on `PATH`).
5. Account for every unit on the page in `<page>.cov` (`ci/coverage.py
   --stub`), then run `ci/lint.py --standard <your copy>`,
   `ci/coverage.py --standard <your copy>` and `ci/mutate.sh` (with gcc and
   musl-gcc): every mutant must be CAUGHT. Then have someone review the test
   and the classification against the passage.

## CI tools (off-system)

    ci/lint.py --standard ~/susv5-html       # traceability and anchors
    ci/mutate.sh --sh dash [--cc musl-gcc]   # every declared fault is caught
    ci/coverage.py --standard ~/susv5-html   # every requirement accounted for

`ci/coverage.py` splits each page of your copy of the standard into
requirement units (every sentence of the normative sections, list items,
table rows and struct members, each tagged shall/unspec/item/stmt/row/
member for triage) and checks the module's `<page>.cov`, which says for each
unit (by section and hash, never its text) which assertions cover it, or why
none can (`app`, `impl`, `optional`, `none`, `untestable`), or that it is a
known gap (`todo`). `--show MODULE` prints the units with their text on your
terminal; `--stub MODULE` lists unaccounted ones; `--total` sizes the whole
standard.
