# illumos triage, group user-diff

Counts: TEST-BUG 2, SYSTEM 2, STD-AMBIG 0, ENV 182, UNKNOWN 8.

All "needs root" skips were checked: each has `needs root` in its assertion, so the skip is by design (ENV). Spot checks of the privileged-setup ones (sched, getresuid, getgroups, setid) found real privilege needs, except XCU.id.007 (fixed).

| ID | result | class | reason |
|---|---|---|---|
| XBD.10.1-directory-structure-and-files.007 | UNRESOLVED | ENV | Test is UNRESOLVED by design when /dev/console cannot be opened; device permissions for an ordinary user are not a conformance matter. |
| XBD.4.5-directory-protection.001 | UNRESOLVED | ENV | Declares needs root (checked in the .asr); skipped for an ordinary user by design. |
| XBD.4.5-directory-protection.002 | UNRESOLVED | ENV | Declares needs root (checked in the .asr); skipped for an ordinary user by design. |
| XBD.4.7-file-access-permissions.002 | UNRESOLVED | ENV | Declares needs root (checked in the .asr); skipped for an ordinary user by design. |
| XBD.4.7-file-access-permissions.003 | UNRESOLVED | ENV | Declares needs root (checked in the .asr); skipped for an ordinary user by design. |
| XBD.4.7-file-access-permissions.005 | UNRESOLVED | ENV | Declares needs root (checked in the .asr); skipped for an ordinary user by design. |
| XBD.4.7-file-access-permissions.006 | UNRESOLVED | ENV | Declares needs root (checked in the .asr); skipped for an ordinary user by design. |
| XCU.chgrp.001 | UNRESOLVED | ENV | Declares needs root (checked in the .asr); skipped for an ordinary user by design. |
| XCU.chgrp.002 | UNRESOLVED | ENV | Declares needs root (checked in the .asr); skipped for an ordinary user by design. |
| XCU.chgrp.003 | UNRESOLVED | ENV | Declares needs root (checked in the .asr); skipped for an ordinary user by design. |
| XCU.chgrp.004 | UNRESOLVED | ENV | Declares needs root (checked in the .asr); skipped for an ordinary user by design. |
| XCU.chgrp.005 | UNRESOLVED | ENV | Declares needs root (checked in the .asr); skipped for an ordinary user by design. |
| XCU.chown.001 | UNRESOLVED | ENV | Declares needs root (checked in the .asr); skipped for an ordinary user by design. |
| XCU.chown.002 | UNRESOLVED | ENV | Declares needs root (checked in the .asr); skipped for an ordinary user by design. |
| XCU.chown.003 | UNRESOLVED | ENV | Declares needs root (checked in the .asr); skipped for an ordinary user by design. |
| XCU.chown.004 | UNRESOLVED | ENV | Declares needs root (checked in the .asr); skipped for an ordinary user by design. |
| XCU.chown.005 | UNRESOLVED | ENV | Declares needs root (checked in the .asr); skipped for an ordinary user by design. |
| XCU.chown.006 | UNRESOLVED | ENV | Declares needs root (checked in the .asr); skipped for an ordinary user by design. |
| XCU.cp.027 | UNRESOLVED | ENV | Declares needs root (checked in the .asr); skipped for an ordinary user by design. |
| XCU.find.013 | UNRESOLVED | ENV | Declares needs root (checked in the .asr); skipped for an ordinary user by design. |
| XCU.id.007 | UNRESOLVED | TEST-BUG | Declared needs root but asks nothing privileged (id NAME for the real user); declaration removed (dc9c46d1). |
| XCU.pax.039 | UNRESOLVED | ENV | Declares needs root (checked in the .asr); skipped for an ordinary user by design. |
| XCU.rm.007 | FAIL | UNKNOWN | Text requires -d and the test checks exactly that; root passes with the same PATH, so the cause is hidden: need `command -v rm`, PATH and stderr of `rm -d e plain` in the user session. |
| XCU.rm.009 | FAIL | UNKNOWN | Text requires the -i prompt on stderr to name the file; test correct; empty stderr only for the ordinary user suggests a different rm or setup in that session; need PATH and a trace. |
| XCU.rm.010 | FAIL | UNKNOWN | Test correct (-i, affirmative reply removes the file); cause hidden, same open question as rm.009. |
| XCU.rm.012 | FAIL | UNKNOWN | Test correct (-i after -f prompts naming the file); empty stderr, same open question as rm.009. |
| XCU.rm.014 | FAIL | UNKNOWN | Text requires a diagnostic for operands ending in dot or dot-dot; test correct; user run printed nothing; same open question as rm.009. |
| XCU.rm.016 | UNRESOLVED | UNKNOWN | localedef exited above 3 (no output) for the ordinary user only; refusal is allowed and the test is UNRESOLVED by design; need pcloc.err from the user run. |
| XCU.rm.017 | UNRESOLVED | UNKNOWN | Same as rm.016. |
| XCU.rm.018 | UNRESOLVED | UNKNOWN | Same as rm.016. |
| XSH.2.10-sockets.014 | UNRESOLVED | ENV | Declares needs root (checked in the .asr); skipped for an ordinary user by design. |
| XSH.sigqueue.007 | UNRESOLVED | ENV | Declares needs root (checked in the .asr); skipped for an ordinary user by design. |
| XSH.2.7-xsi-ipc.002 | UNRESOLVED | ENV | Declares needs root (checked in the .asr); skipped for an ordinary user by design. |
| XSH.2.7-xsi-ipc.003 | UNRESOLVED | ENV | Declares needs root (checked in the .asr); skipped for an ordinary user by design. |
| XSH.2.7-xsi-ipc.004 | UNRESOLVED | ENV | Declares needs root (checked in the .asr); skipped for an ordinary user by design. |
| XSH.2.7-xsi-ipc.005 | UNRESOLVED | ENV | Declares needs root (checked in the .asr); skipped for an ordinary user by design. |
| XSH.msync.004 | UNRESOLVED | ENV | mlock() may fail with EPERM without appropriate privileges; the test treats that as UNRESOLVED, as designed. |
| XSH.2.8-realtime.005 | UNRESOLVED | ENV | Blocked by XSH.msync.004 (mlock needs privilege for the ordinary user). |
| XSH.access.003 | UNRESOLVED | ENV | Declares needs root (checked in the .asr); skipped for an ordinary user by design. |
| XSH.access.004 | UNRESOLVED | ENV | Declares needs root (checked in the .asr); skipped for an ordinary user by design. |
| XSH.access.005 | UNRESOLVED | ENV | Declares needs root (checked in the .asr); skipped for an ordinary user by design. |
| XSH.access.019 | UNRESOLVED | ENV | Declares needs root (checked in the .asr); skipped for an ordinary user by design. |
| XSH.access.020 | UNRESOLVED | ENV | Declares needs root (checked in the .asr); skipped for an ordinary user by design. |
| XSH.chmod.006 | UNRESOLVED | ENV | Declares needs root (checked in the .asr); skipped for an ordinary user by design. |
| XSH.chmod.007 | UNRESOLVED | ENV | Declares needs root (checked in the .asr); skipped for an ordinary user by design. |
| XSH.chmod.021 | UNRESOLVED | ENV | Declares needs root (checked in the .asr); skipped for an ordinary user by design. |
| XSH.chown.001 | UNRESOLVED | ENV | Declares needs root (checked in the .asr); skipped for an ordinary user by design. |
| XSH.chown.002 | UNRESOLVED | ENV | Declares needs root (checked in the .asr); skipped for an ordinary user by design. |
| XSH.chown.003 | UNRESOLVED | ENV | Declares needs root (checked in the .asr); skipped for an ordinary user by design. |
| XSH.chown.005 | UNRESOLVED | ENV | Declares needs root (checked in the .asr); skipped for an ordinary user by design. |
| XSH.chown.006 | UNRESOLVED | TEST-BUG | Unprivileged path hard-coded group 1 as the foreign group, which an ordinary user may belong to; now searches for a group outside the caller groups (fixed, dc9c46d1). |
| XSH.chown.007 | UNRESOLVED | ENV | Declares needs root (checked in the .asr); skipped for an ordinary user by design. |
| XSH.chown.008 | UNRESOLVED | ENV | Declares needs root (checked in the .asr); skipped for an ordinary user by design. |
| XSH.chown.009 | UNRESOLVED | ENV | Declares needs root (checked in the .asr); skipped for an ordinary user by design. |
| XSH.chown.010 | UNRESOLVED | ENV | Declares needs root (checked in the .asr); skipped for an ordinary user by design. |
| XSH.chown.011 | UNRESOLVED | ENV | Declares needs root (checked in the .asr); skipped for an ordinary user by design. |
| XSH.erf.004 | NOTINUSE | ENV | erf(+-Inf) is MX-shaded, so NOTINUSE without the IEC macros is right; the gate is compile-time and cannot depend on uid, and the root output lacks erf.004.txt and erf.005 files: rerun with --only in both modes and compare the compile lines. |
| XSH.exec.052 | UNRESOLVED | ENV | Declares needs root (checked in the .asr); skipped for an ordinary user by design. |
| XSH.exec.053 | UNRESOLVED | ENV | Declares needs root (checked in the .asr); skipped for an ordinary user by design. |
| XSH.fchmod.005 | UNRESOLVED | ENV | Declares needs root (checked in the .asr); skipped for an ordinary user by design. |
| XSH.fchown.001 | UNRESOLVED | ENV | Declares needs root (checked in the .asr); skipped for an ordinary user by design. |
| XSH.fchown.002 | UNRESOLVED | ENV | Declares needs root (checked in the .asr); skipped for an ordinary user by design. |
| XSH.fchown.003 | UNRESOLVED | ENV | Declares needs root (checked in the .asr); skipped for an ordinary user by design. |
| XSH.futimens.010 | UNRESOLVED | ENV | Declares needs root (checked in the .asr); skipped for an ordinary user by design. |
| XSH.futimens.011 | UNRESOLVED | ENV | Declares needs root (checked in the .asr); skipped for an ordinary user by design. |
| XSH.futimens.012 | UNRESOLVED | ENV | Declares needs root (checked in the .asr); skipped for an ordinary user by design. |
| XSH.futimens.019 | UNRESOLVED | ENV | Declares needs root (checked in the .asr); skipped for an ordinary user by design. |
| XSH.futimens.021 | UNRESOLVED | ENV | Declares needs root (checked in the .asr); skipped for an ordinary user by design. |
| XSH.futimens.026 | UNRESOLVED | ENV | Declares needs root (checked in the .asr); skipped for an ordinary user by design. |
| XSH.getegid.002 | UNRESOLVED | ENV | Declares needs root (checked in the .asr); skipped for an ordinary user by design. |
| XSH.getenv.005 | UNRESOLVED | ENV | Declares needs root (checked in the .asr); skipped for an ordinary user by design. |
| XSH.geteuid.001 | UNRESOLVED | ENV | Declares needs root (checked in the .asr); skipped for an ordinary user by design. |
| XSH.getgid.002 | UNRESOLVED | ENV | Declares needs root (checked in the .asr); skipped for an ordinary user by design. |
| XSH.getgroups.001 | UNRESOLVED | ENV | Declares needs root (checked in the .asr); skipped for an ordinary user by design. |
| XSH.getgroups.002 | UNRESOLVED | ENV | Declares needs root (checked in the .asr); skipped for an ordinary user by design. |
| XSH.getgroups.003 | UNRESOLVED | ENV | Declares needs root (checked in the .asr); skipped for an ordinary user by design. |
| XSH.getresgid.002 | UNRESOLVED | ENV | Declares needs root (checked in the .asr); skipped for an ordinary user by design. |
| XSH.getresuid.002 | UNRESOLVED | ENV | Declares needs root (checked in the .asr); skipped for an ordinary user by design. |
| XSH.getuid.001 | UNRESOLVED | ENV | Declares needs root (checked in the .asr); skipped for an ordinary user by design. |
| XSH.kill.009 | UNRESOLVED | ENV | Declares needs root (checked in the .asr); skipped for an ordinary user by design. |
| XSH.kill.010 | UNRESOLVED | ENV | Declares needs root (checked in the .asr); skipped for an ordinary user by design. |
| XSH.kill.011 | UNRESOLVED | ENV | Declares needs root (checked in the .asr); skipped for an ordinary user by design. |
| XSH.kill.013 | UNRESOLVED | ENV | Declares needs root (checked in the .asr); skipped for an ordinary user by design. |
| XSH.kill.014 | UNRESOLVED | ENV | Declares needs root (checked in the .asr); skipped for an ordinary user by design. |
| XSH.kill.016 | UNRESOLVED | ENV | Declares needs root (checked in the .asr); skipped for an ordinary user by design. |
| XSH.killpg.004 | UNRESOLVED | ENV | Declares needs root (checked in the .asr); skipped for an ordinary user by design. |
| XSH.lchown.001 | UNRESOLVED | ENV | Declares needs root (checked in the .asr); skipped for an ordinary user by design. |
| XSH.lchown.002 | UNRESOLVED | ENV | Declares needs root (checked in the .asr); skipped for an ordinary user by design. |
| XSH.lchown.003 | UNRESOLVED | ENV | Declares needs root (checked in the .asr); skipped for an ordinary user by design. |
| XSH.unlink.018 | UNRESOLVED | ENV | Declares needs root (checked in the .asr); skipped for an ordinary user by design. |
| XSH.unlink.020 | UNRESOLVED | ENV | Declares needs root (checked in the .asr); skipped for an ordinary user by design. |
| XSH.link.019 | UNRESOLVED | ENV | Declares needs root (checked in the .asr); skipped for an ordinary user by design. |
| XSH.mlock.001 | UNRESOLVED | ENV | Blocked by XSH.msync.004 (mlock needs privilege for the ordinary user). |
| XSH.mlock.002 | UNRESOLVED | ENV | Blocked by XSH.msync.004 (mlock needs privilege for the ordinary user). |
| XSH.mlock.003 | UNRESOLVED | ENV | EPERM is allowed for mlock() without privilege; the test accepts it as UNRESOLVED. |
| XSH.mlock.004 | UNRESOLVED | ENV | Blocked by XSH.msync.004 (mlock needs privilege for the ordinary user). |
| XSH.mlock.005 | UNRESOLVED | ENV | Blocked by XSH.msync.004 (mlock needs privilege for the ordinary user). |
| XSH.mlockall.001 | UNRESOLVED | ENV | Blocked by XSH.msync.004 (mlock needs privilege for the ordinary user). |
| XSH.mlockall.002 | UNRESOLVED | ENV | Blocked by XSH.msync.004 (mlock needs privilege for the ordinary user). |
| XSH.mlockall.003 | UNRESOLVED | ENV | Blocked by XSH.msync.004 (mlock needs privilege for the ordinary user). |
| XSH.mlockall.004 | UNRESOLVED | ENV | EINVAL and EPERM both apply to mlockall(0); which is reported is unspecified, so UNRESOLVED is right. |
| XSH.mlockall.005 | FAIL | SYSTEM | munlockall() shall always return zero when supported and only mlockall() is said to need privilege; the ordinary user got a nonzero return. The message showed errno 0 because of argument evaluation order; message fixed (dc9c46d1). |
| XSH.mq_open.015 | UNRESOLVED | ENV | Declares needs root (checked in the .asr); skipped for an ordinary user by design. |
| XSH.msgctl.003 | UNRESOLVED | ENV | Declares needs root (checked in the .asr); skipped for an ordinary user by design. |
| XSH.msgctl.006 | UNRESOLVED | ENV | Declares needs root (checked in the .asr); skipped for an ordinary user by design. |
| XSH.nice.003 | FAIL | SYSTEM | nice() with a positive increment past the limit sets the maximum and succeeds; EPERM is only for a negative increment; the ordinary user got EPERM. |
| XSH.posix_spawn.045 | UNRESOLVED | ENV | Declares needs root (checked in the .asr); skipped for an ordinary user by design. |
| XSH.posix_spawn.046 | UNRESOLVED | ENV | Declares needs root (checked in the .asr); skipped for an ordinary user by design. |
| XSH.posix_spawn.047 | UNRESOLVED | ENV | Declares needs root (checked in the .asr); skipped for an ordinary user by design. |
| XSH.posix_spawn.048 | UNRESOLVED | ENV | Declares needs root (checked in the .asr); skipped for an ordinary user by design. |
| XSH.posix_spawn.049 | UNRESOLVED | ENV | Declares needs root (checked in the .asr); skipped for an ordinary user by design. |
| XSH.posix_spawn.050 | UNRESOLVED | ENV | Declares needs root (checked in the .asr); skipped for an ordinary user by design. |
| XSH.posix_spawn.051 | UNRESOLVED | ENV | Declares needs root (checked in the .asr); skipped for an ordinary user by design. |
| XSH.posix_spawn.052 | UNRESOLVED | ENV | Declares needs root (checked in the .asr); skipped for an ordinary user by design. |
| XSH.posix_spawnattr_getschedparam.003 | UNRESOLVED | ENV | Declares needs root (checked in the .asr); skipped for an ordinary user by design. |
| XSH.posix_spawnattr_getschedpolicy.003 | UNRESOLVED | ENV | Declares needs root (checked in the .asr); skipped for an ordinary user by design. |
| XSH.pthread_getschedparam.003 | UNRESOLVED | ENV | Declares needs root (checked in the .asr); skipped for an ordinary user by design. |
| XSH.pthread_getschedparam.004 | UNRESOLVED | ENV | Declares needs root (checked in the .asr); skipped for an ordinary user by design. |
| XSH.pthread_getschedparam.005 | UNRESOLVED | ENV | Declares needs root (checked in the .asr); skipped for an ordinary user by design. |
| XSH.pthread_getschedparam.006 | UNRESOLVED | ENV | Declares needs root (checked in the .asr); skipped for an ordinary user by design. |
| XSH.pthread_getschedparam.007 | UNRESOLVED | ENV | Declares needs root (checked in the .asr); skipped for an ordinary user by design. |
| XSH.pthread_setschedprio.001 | UNRESOLVED | ENV | Declares needs root (checked in the .asr); skipped for an ordinary user by design. |
| XSH.pthread_setschedprio.002 | UNRESOLVED | ENV | Declares needs root (checked in the .asr); skipped for an ordinary user by design. |
| XSH.pthread_setschedprio.003 | UNRESOLVED | ENV | Declares needs root (checked in the .asr); skipped for an ordinary user by design. |
| XSH.rename.028 | UNRESOLVED | ENV | Declares needs root (checked in the .asr); skipped for an ordinary user by design. |
| XSH.rename.029 | UNRESOLVED | ENV | Declares needs root (checked in the .asr); skipped for an ordinary user by design. |
| XSH.rename.030 | UNRESOLVED | ENV | Declares needs root (checked in the .asr); skipped for an ordinary user by design. |
| XSH.rmdir.012 | UNRESOLVED | ENV | Declares needs root (checked in the .asr); skipped for an ordinary user by design. |
| XSH.rmdir.013 | UNRESOLVED | ENV | Declares needs root (checked in the .asr); skipped for an ordinary user by design. |
| XSH.sched_getparam.004 | UNRESOLVED | ENV | Declares needs root (checked in the .asr); skipped for an ordinary user by design. |
| XSH.sched_getscheduler.003 | UNRESOLVED | ENV | Declares needs root (checked in the .asr); skipped for an ordinary user by design. |
| XSH.sched_rr_get_interval.001 | UNRESOLVED | ENV | Declares needs root (checked in the .asr); skipped for an ordinary user by design. |
| XSH.sched_setparam.001 | UNRESOLVED | ENV | Declares needs root (checked in the .asr); skipped for an ordinary user by design. |
| XSH.sched_setparam.002 | UNRESOLVED | ENV | Declares needs root (checked in the .asr); skipped for an ordinary user by design. |
| XSH.sched_setparam.004 | UNRESOLVED | ENV | Declares needs root (checked in the .asr); skipped for an ordinary user by design. |
| XSH.sched_setscheduler.001 | UNRESOLVED | ENV | Declares needs root (checked in the .asr); skipped for an ordinary user by design. |
| XSH.sched_setscheduler.002 | UNRESOLVED | ENV | Declares needs root (checked in the .asr); skipped for an ordinary user by design. |
| XSH.sched_setscheduler.003 | UNRESOLVED | ENV | Declares needs root (checked in the .asr); skipped for an ordinary user by design. |
| XSH.sched_setscheduler.004 | UNRESOLVED | ENV | Declares needs root (checked in the .asr); skipped for an ordinary user by design. |
| XSH.sched_setscheduler.007 | UNRESOLVED | ENV | Declares needs root (checked in the .asr); skipped for an ordinary user by design. |
| XSH.semctl.007 | UNRESOLVED | ENV | Declares needs root (checked in the .asr); skipped for an ordinary user by design. |
| XSH.semctl.012 | UNRESOLVED | ENV | Declares needs root (checked in the .asr); skipped for an ordinary user by design. |
| XSH.setegid.001 | UNRESOLVED | ENV | Declares needs root (checked in the .asr); skipped for an ordinary user by design. |
| XSH.setegid.002 | UNRESOLVED | ENV | Declares needs root (checked in the .asr); skipped for an ordinary user by design. |
| XSH.setegid.003 | UNRESOLVED | ENV | Declares needs root (checked in the .asr); skipped for an ordinary user by design. |
| XSH.setegid.004 | UNRESOLVED | ENV | Declares needs root (checked in the .asr); skipped for an ordinary user by design. |
| XSH.setegid.005 | UNRESOLVED | ENV | Declares needs root (checked in the .asr); skipped for an ordinary user by design. |
| XSH.setegid.006 | UNRESOLVED | ENV | Declares needs root (checked in the .asr); skipped for an ordinary user by design. |
| XSH.setegid.007 | UNRESOLVED | ENV | Declares needs root (checked in the .asr); skipped for an ordinary user by design. |
| XSH.seteuid.001 | UNRESOLVED | ENV | Declares needs root (checked in the .asr); skipped for an ordinary user by design. |
| XSH.seteuid.002 | UNRESOLVED | ENV | Declares needs root (checked in the .asr); skipped for an ordinary user by design. |
| XSH.seteuid.003 | UNRESOLVED | ENV | Declares needs root (checked in the .asr); skipped for an ordinary user by design. |
| XSH.seteuid.004 | UNRESOLVED | ENV | Declares needs root (checked in the .asr); skipped for an ordinary user by design. |
| XSH.seteuid.005 | UNRESOLVED | ENV | Declares needs root (checked in the .asr); skipped for an ordinary user by design. |
| XSH.seteuid.006 | UNRESOLVED | ENV | Declares needs root (checked in the .asr); skipped for an ordinary user by design. |
| XSH.seteuid.007 | UNRESOLVED | ENV | Declares needs root (checked in the .asr); skipped for an ordinary user by design. |
| XSH.setgid.001 | UNRESOLVED | ENV | Declares needs root (checked in the .asr); skipped for an ordinary user by design. |
| XSH.setgid.002 | UNRESOLVED | ENV | Declares needs root (checked in the .asr); skipped for an ordinary user by design. |
| XSH.setgid.003 | UNRESOLVED | ENV | Declares needs root (checked in the .asr); skipped for an ordinary user by design. |
| XSH.setgid.004 | UNRESOLVED | ENV | Declares needs root (checked in the .asr); skipped for an ordinary user by design. |
| XSH.setgid.005 | UNRESOLVED | ENV | Declares needs root (checked in the .asr); skipped for an ordinary user by design. |
| XSH.setgid.006 | UNRESOLVED | ENV | Declares needs root (checked in the .asr); skipped for an ordinary user by design. |
| XSH.setgid.007 | UNRESOLVED | ENV | Declares needs root (checked in the .asr); skipped for an ordinary user by design. |
| XSH.setregid.001 | UNRESOLVED | ENV | Declares needs root (checked in the .asr); skipped for an ordinary user by design. |
| XSH.setregid.002 | UNRESOLVED | ENV | Declares needs root (checked in the .asr); skipped for an ordinary user by design. |
| XSH.setregid.003 | UNRESOLVED | ENV | Declares needs root (checked in the .asr); skipped for an ordinary user by design. |
| XSH.setregid.004 | UNRESOLVED | ENV | Declares needs root (checked in the .asr); skipped for an ordinary user by design. |
| XSH.setregid.005 | UNRESOLVED | ENV | Declares needs root (checked in the .asr); skipped for an ordinary user by design. |
| XSH.setregid.006 | UNRESOLVED | ENV | Declares needs root (checked in the .asr); skipped for an ordinary user by design. |
| XSH.setregid.007 | UNRESOLVED | ENV | Declares needs root (checked in the .asr); skipped for an ordinary user by design. |
| XSH.setregid.008 | UNRESOLVED | ENV | Declares needs root (checked in the .asr); skipped for an ordinary user by design. |
| XSH.setregid.009 | UNRESOLVED | ENV | Declares needs root (checked in the .asr); skipped for an ordinary user by design. |
| XSH.setresgid.003 | UNRESOLVED | ENV | Declares needs root (checked in the .asr); skipped for an ordinary user by design. |
| XSH.setresgid.004 | UNRESOLVED | ENV | Declares needs root (checked in the .asr); skipped for an ordinary user by design. |
| XSH.setresuid.003 | UNRESOLVED | ENV | Declares needs root (checked in the .asr); skipped for an ordinary user by design. |
| XSH.setresuid.004 | UNRESOLVED | ENV | Declares needs root (checked in the .asr); skipped for an ordinary user by design. |
| XSH.setreuid.001 | UNRESOLVED | ENV | Declares needs root (checked in the .asr); skipped for an ordinary user by design. |
| XSH.setreuid.002 | UNRESOLVED | ENV | Declares needs root (checked in the .asr); skipped for an ordinary user by design. |
| XSH.setreuid.003 | UNRESOLVED | ENV | Declares needs root (checked in the .asr); skipped for an ordinary user by design. |
| XSH.setreuid.004 | UNRESOLVED | ENV | Declares needs root (checked in the .asr); skipped for an ordinary user by design. |
| XSH.setreuid.005 | UNRESOLVED | ENV | Declares needs root (checked in the .asr); skipped for an ordinary user by design. |
| XSH.setreuid.006 | UNRESOLVED | ENV | Declares needs root (checked in the .asr); skipped for an ordinary user by design. |
| XSH.setreuid.007 | UNRESOLVED | ENV | Declares needs root (checked in the .asr); skipped for an ordinary user by design. |
| XSH.setuid.001 | UNRESOLVED | ENV | Declares needs root (checked in the .asr); skipped for an ordinary user by design. |
| XSH.setuid.002 | UNRESOLVED | ENV | Declares needs root (checked in the .asr); skipped for an ordinary user by design. |
| XSH.setuid.003 | UNRESOLVED | ENV | Declares needs root (checked in the .asr); skipped for an ordinary user by design. |
| XSH.setuid.004 | UNRESOLVED | ENV | Declares needs root (checked in the .asr); skipped for an ordinary user by design. |
| XSH.setuid.005 | UNRESOLVED | ENV | Declares needs root (checked in the .asr); skipped for an ordinary user by design. |
| XSH.setuid.006 | UNRESOLVED | ENV | Declares needs root (checked in the .asr); skipped for an ordinary user by design. |
| XSH.setuid.007 | UNRESOLVED | ENV | Declares needs root (checked in the .asr); skipped for an ordinary user by design. |
| XSH.shm_open.013 | UNRESOLVED | ENV | Declares needs root (checked in the .asr); skipped for an ordinary user by design. |
| XSH.shm_unlink.005 | UNRESOLVED | ENV | Declares needs root (checked in the .asr); skipped for an ordinary user by design. |
| XSH.shmctl.003 | UNRESOLVED | ENV | Declares needs root (checked in the .asr); skipped for an ordinary user by design. |
| XSH.shmctl.008 | UNRESOLVED | ENV | Declares needs root (checked in the .asr); skipped for an ordinary user by design. |
