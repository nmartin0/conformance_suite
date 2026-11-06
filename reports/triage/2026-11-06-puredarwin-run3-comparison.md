# PureDarwin run3: stability and comparison with the other five systems

Run: reports/2026-11-05-puredarwin-amd64-run3 (9,011 assertions, root only).

## Stability against the first PureDarwin run

10 of 9,011 results differ from the first run (reports/2026-11-03-puredarwin-amd64).

| Change | IDs | Reading |
|---|---|---|
| UNRESOLVED -> PASS | nanosleep.003, .004, .005, .008 | timing on a slow emulated guest; not a system fact |
| FAIL/INSPECT -> UNINITIATED | abort.005, abort.006, raise.002 | now labelled as a compiler limit (`_Thread_local` compiles but does not run) |
| PASS -> UNRESOLVED | XBD.12.1-utility-argument-syntax.002 | takes 54-57 s on the guest against a 60 s limit; five repeats all PASS |
| PASS -> FAIL | posix_spawn.058, posix_spawn.060 | intermittent, see below |

## Repeat of the three unsettled tests (five runs each, same guest)

(repeat-differing.txt in the run3 report directory)

- posix_spawn.058: FAIL, PASS, PASS, FAIL, FAIL.
- posix_spawn.060: FAIL, PASS, FAIL, FAIL, FAIL.
- XBD.12.1-utility-argument-syntax.002: PASS five times, 54-57 s each.

For the two posix_spawn tests the check is that after a failed spawn no child
process exists. A failed spawn that leaves a child behind is a defect on every
run where it happens, so an intermittent FAIL is still a real finding: the
system sometimes leaves a child alive (or not yet reaped) after reporting the
error. The tests themselves look sound; the cause is probably that the library
creates the child first and reports the exec error afterwards. Not verified by
reading the library, since no source is available here.

The argument-syntax test is an environment effect: the test is correct, the
guest is slow. Its limit could be raised for emulated hosts.

## Comparison with the other five systems

The other five runs are older runs of an earlier tree (about 8,850 assertions
each), so only identifiers present in both are compared and numbers are
indicative.

Of PureDarwin's 1,182 FAILs:

| other systems also failing the assertion | count |
|---|---|
| 0 | 535 |
| 1 | 249 |
| 2 | 184 |
| 3 | 111 |
| 4 | 64 |
| 5 | 39 |

- 232 fail only on PureDarwin (every other system with a result passes). Most
  are in the utilities (ed 15, m4 12, diff 9, patch 9, test 9, csplit 7,
  getopts 7, pr 7, join 6, timeout 6, bc 5, logger 5, mkfifo 5, and so on),
  which fits the guest's old and incomplete userland more than the kernel.
- 39 fail on all six systems; these are the best candidates for a test or
  standard-reading problem rather than a system problem, and should be read
  first (list kept in the scratchpad of the session that produced this note;
  regenerate it with the same join).
- 33 failures have too few comparable results elsewhere.

## Open items

- Read the 39 all-six failures against the standard.
- Re-run the other five systems with the current tree so the comparison is on
  one suite version.
- Repeat posix_spawn.058/.060 on one of the other systems that uses a
  library-level spawn, to see whether the intermittent child is general.
