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

The shared copy of the standard-issues report is the Claude Doc at
https://claude.ai/artifact/L3t5goEQYUJKhbTWAj2BXF; it is kept in step with
`standard-issues-report.md`.
