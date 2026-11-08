# OpenBSD, current tree: triage

Run: reports/2026-11-08-openbsd-amd64-current. Root 9011 assertions: PASS 5777, FAIL 764,
INSPECT 16, UNRESOLVED 143, NOTINUSE 575, UNSUPPORTED 1262, UNINITIATED 434.
User phase: PASS 5695, FAIL 752, UNRESOLVED 241 (the extra UNRESOLVED are privilege-dependent).
The previous OpenBSD run (2026-10-30) used an older tree of 8812 root assertions.

## Changes against the old run (root, assertions in both)
- UNINITIATED to NOTINUSE 203, to PASS 104, to FAIL 19: toolchain and option labelling fixes.
- NOTINUSE to FAIL 75: assertions that now run (the option is claimed) and fail.
- UNRESOLVED to PASS 58, FAIL to PASS 24.
- PASS to not-pass 22 (below).

## PASS to not-pass, checked against the other systems' current runs
Linux passes all of these. By pattern:
1. Fail on NetBSD and OpenBSD, and on PureDarwin where it ran: m4.004, m4.012, od.002,
   patch.006, tr.008, cp.013, chmod.006, csplit.007, find.016, xargs.008, ps.006. These
   look like real BSD-userland differences that the older tree did not test as strictly;
   not test bugs as far as the failure messages show. Worth reading each against the
   standard before filing.
2. Fail only on OpenBSD (and sometimes PureDarwin): find.009 (-links), m4.006, patch.007
   (-b), pr.009 (-i).
3. Timeouts at 60 s, UNRESOLVED, passing elsewhere: cmp.001, csplit.001, diff.009,
   tr.007. The VM is slow; treat as environment until a repeat run says otherwise.
4. nanosleep.003 and .004 fail only here: the remaining time reported is 4679 ms
   although at most about 315 ms of the 5 s had passed. This could be a real OpenBSD
   behaviour (rmtp not updated after a signal) or the VM clock; needs a repeat run
   and a read of the test before it is counted.
5. cmp.008 INSPECT (disputed note), also INSPECT on NetBSD: needs a decision.
6. fpathconf.007 UNRESOLVED: no controlling terminal; same on all systems, environment.

## Largest FAIL clusters (root)
ex 62, vi 57, mailx 21, make 16, pax 14, mtx_lock 12, unistd.h 11, fscanf 11, signal.h 10,
aio.h 9, pathchk 8, cnd_wait 8, file 7. The ex/vi/mailx/make clusters are the same
families that fail on the other systems; the threads (C11 mtx/cnd/thrd) families come from
Issue 8 additions the system lacks.

## Not done
Per-assertion reading of the clusters, and a repeat run of the timeout and timing items.
