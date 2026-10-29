# Conventions suite: status

Copyright (c) 2026 Nicholas Martin. All rights reserved.

Branch `conv/unix`, worktree `/home/claude/posixcheck-conv`. Running log;
read this first when resuming.

## Source key

The BSD hier(7) and section 5 manuals are cited as BSD-A, BSD-B and BSD-C in
the assertions, in the order of the file names under `conv-docs`:
BSD-A = FreeBSD, BSD-B = NetBSD, BSD-C = OpenBSD. The FHS pages were fetched
from https://refspecs.linuxfoundation.org/FHS_3.0/fhs/ (ch03, ch04, ch05,
ch06 and the s-pages). The section 5 manual pages came from the manual-page
servers of the three projects; the Linux manual host (man7.org) is blocked
from the build environment, as are the Linux-distribution documentation
hosts tried (die.net, kernel.org man-pages).

## Done (batch 1)

- Driver: `posixcheck.sh` knows the volume `conv` (two lines: ID prefix
  `CONV` to directory `conv`); nothing else changed. `ci/catalog.py` knows
  the volume; `ci/lint.py` skips the `.cov` rule and the `since` rule for it
  and refuses operating-system names in the conv tree.
- 49 assertions in 6 modules (see README). Lint clean:
  `python3 ci/lint.py --modules conv/modules` gives 0 problems (11 mutants).
- Negative checks: `sh conv/neg/run.sh`, 56 cases, 0 missed (as root and as
  `nobody`).
- `ci/mutate.sh --root conv/modules 'CONV.*'`: 11 mutants, 8 caught, 0
  missed, 3 unchecked (their baselines fail on the system: CONV.utf8.004,
  CONV.utf8.008 twice).
- POSIX suite unchanged: `./posixcheck.sh --no-selftest --only
  XBD.limits.h.017` gives the same result before and after.

## First run (root; ordinary user identical)

49 assertions: 46 PASS, 2 FAIL (CONV.utf8.004, .008), 1 INSPECT
(CONV.etcdb.013), 0 NOTINUSE with `XDG_RUNTIME_DIR` supplied (2 without).
See known-deviations.md.

## Deferred (later batches)

- terminfo; ECMA-48 and xterm control sequences through the pty
  (`core/pcpty.c` exists for this)
- loopback TCP and UDP and DNS behaviour from RFC 9293, 768, 1122, 1035
- syslog message format (RFC 5424)
- ELF loader conventions
- a common library extensions profile
- negative checks for CONV.layout.008, CONV.devnodes.005 and CONV.etcdb.005,
  .010, .012 (they compare the system's own tools or library with the real
  files; the only way to fail them is to break the system)
- passwd/group databases that are not files (name-service back ends) are
  NOTINUSE, not judged
