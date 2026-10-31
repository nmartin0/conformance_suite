# FreeBSD triage, group unres-user

Counts: TEST-BUG 44, SYSTEM 50, STD-AMBIG 1, ENV 201, UNKNOWN 0.

How to read the class of an UNRESOLVED row: "honest" means UNRESOLVED is
the right outcome. ENV then means the run or this system legitimately
cannot provide the setup (an implementation-defined or optional choice,
configuration, privilege, profile, timing); SYSTEM means the cause is a
nonconformance of the system that another assertion reports (a FAIL or a
build failure it is blocked by, or a missing Issue 8 name that the Tier 0
header run reports). TEST-BUG rows hid a test or harness defect and are
fixed in the commits named. For the root/user rows (the last 132), the
class says whether the root/user difference breaks the "same verdict
unless needs root" rule (TEST-BUG) or is the system's (ENV when only
permission or configuration differs and the UNRESOLVED is honest, SYSTEM
when a real nonconformance shows in one of the two runs).

Commits (worktree agent-a658a9cdfe79273c6):

- 7660db10 pcexec: run each test without a controlling terminal (harness;
  33 'sh -i' timeouts and XCU.patch.010)
- 44c3bae4 ls, pax tests: give the file the effective group before setting
  S_ISGID (XCU.ls.041, .pax.026, .pax.032, .pax.034)
- bd49c78b XCU.sh.060: compare what the user is told, not the byte count
- 83c85317 XCU.at.005, XSH.catgets.005: a refusal of valid input is a FAIL
- 2f3bf0bc fputc, write tests: block every signal where SIGXFSZ is not
  defined (XSH.fputc.011, XSH.write.006, .007)

| ID | result | class | reason |
|---|---|---|---|
| XBD.11.1-interface-characteristics.003 | UNRESOLVED | ENV | honest: XBD 11.1.3 leaves it implementation-defined whether a session leader's open() of a terminal makes it the controlling terminal, and POSIX has no other way to get one; here it does not |
| XBD.11.1-interface-characteristics.004 | UNRESOLVED | ENV | honest: XBD 11.1.3 leaves it implementation-defined whether a session leader's open() of a terminal makes it the controlling terminal, and POSIX has no other way to get one; here it does not |
| XBD.11.1-interface-characteristics.005 | UNRESOLVED | ENV | honest: XBD 11.1.3 leaves it implementation-defined whether a session leader's open() of a terminal makes it the controlling terminal, and POSIX has no other way to get one; here it does not |
| XBD.11.1-interface-characteristics.006 | UNRESOLVED | ENV | honest: XBD 11.1.3 leaves it implementation-defined whether a session leader's open() of a terminal makes it the controlling terminal, and POSIX has no other way to get one; here it does not |
| XBD.11.1-interface-characteristics.007 | UNRESOLVED | ENV | honest: XBD 11.1.3 leaves it implementation-defined whether a session leader's open() of a terminal makes it the controlling terminal, and POSIX has no other way to get one; here it does not |
| XBD.11.1-interface-characteristics.008 | UNRESOLVED | ENV | honest: XBD 11.1.3 leaves it implementation-defined whether a session leader's open() of a terminal makes it the controlling terminal, and POSIX has no other way to get one; here it does not |
| XBD.11.1-interface-characteristics.009 | UNRESOLVED | ENV | honest: XBD 11.1.3 leaves it implementation-defined whether a session leader's open() of a terminal makes it the controlling terminal, and POSIX has no other way to get one; here it does not |
| XBD.11.1-interface-characteristics.010 | UNRESOLVED | ENV | honest: XBD 11.1.3 leaves it implementation-defined whether a session leader's open() of a terminal makes it the controlling terminal, and POSIX has no other way to get one; here it does not |
| XBD.11.1-interface-characteristics.011 | UNRESOLVED | ENV | honest: XBD 11.1.3 leaves it implementation-defined whether a session leader's open() of a terminal makes it the controlling terminal, and POSIX has no other way to get one; here it does not |
| XBD.11.1-interface-characteristics.012 | UNRESOLVED | ENV | honest: XBD 11.1.3 leaves it implementation-defined whether a session leader's open() of a terminal makes it the controlling terminal, and POSIX has no other way to get one; here it does not |
| XBD.11.1-interface-characteristics.013 | UNRESOLVED | ENV | honest: XBD 11.1.3 leaves it implementation-defined whether a session leader's open() of a terminal makes it the controlling terminal, and POSIX has no other way to get one; here it does not |
| XBD.11.1-interface-characteristics.014 | UNRESOLVED | ENV | honest: XBD 11.1.3 leaves it implementation-defined whether a session leader's open() of a terminal makes it the controlling terminal, and POSIX has no other way to get one; here it does not |
| XBD.11.1-interface-characteristics.015 | UNRESOLVED | ENV | honest: XBD 11.1.3 leaves it implementation-defined whether a session leader's open() of a terminal makes it the controlling terminal, and POSIX has no other way to get one; here it does not |
| XBD.11.1-interface-characteristics.016 | UNRESOLVED | ENV | honest: XBD 11.1.3 leaves it implementation-defined whether a session leader's open() of a terminal makes it the controlling terminal, and POSIX has no other way to get one; here it does not |
| XBD.11.1-interface-characteristics.035 | UNRESOLVED | ENV | honest: XBD 11.1.3 leaves it implementation-defined whether a session leader's open() of a terminal makes it the controlling terminal, and POSIX has no other way to get one; here it does not |
| XBD.11.1-interface-characteristics.036 | UNRESOLVED | ENV | honest: XBD 11.1.3 leaves it implementation-defined whether a session leader's open() of a terminal makes it the controlling terminal, and POSIX has no other way to get one; here it does not |
| XBD.11.1-interface-characteristics.037 | UNRESOLVED | ENV | honest: XBD 11.1.3 leaves it implementation-defined whether a session leader's open() of a terminal makes it the controlling terminal, and POSIX has no other way to get one; here it does not |
| XBD.11.2-terminal-parameters.021 | UNRESOLVED | ENV | honest: XBD 11.1.3 leaves it implementation-defined whether a session leader's open() of a terminal makes it the controlling terminal, and POSIX has no other way to get one; here it does not |
| XBD.11.2-terminal-parameters.022 | UNRESOLVED | ENV | honest: XBD 11.1.3 leaves it implementation-defined whether a session leader's open() of a terminal makes it the controlling terminal, and POSIX has no other way to get one; here it does not |
| XBD.dirent.h.001 | UNRESOLVED | SYSTEM | honest: DT_BLK..DT_UNKNOWN are base names since Issue 8 (dirent.h DESCRIPTION) and are not visible in the strict build; the Tier 0 header run (not among these inputs) is where that is the FAIL |
| XBD.dirent.h.002 | UNRESOLVED | SYSTEM | honest: DT_BLK..DT_UNKNOWN are base names since Issue 8 (dirent.h DESCRIPTION) and are not visible in the strict build; the Tier 0 header run (not among these inputs) is where that is the FAIL |
| XBD.fnmatch.h.002 | UNRESOLVED | SYSTEM | honest: FNM_CASEFOLD/FNM_IGNORECASE are unshaded in fnmatch.h (Issue 8) and missing; reported by the Tier 0 header run |
| XBD.langinfo.h.004 | UNRESOLVED | SYSTEM | honest: Issue 8 langinfo items (ABALTMON_*, ...) are missing; XBD.langinfo.h.003 FAILs for it |
| XBD.pthread.h.004 | UNRESOLVED | SYSTEM | honest: PTHREAD_MUTEX_NORMAL/ERRORCHECK/RECURSIVE moved from XSI to base in Issue 8 (pthread.h CHANGE HISTORY) and are hidden in the base build |
| XBD.pthread.h.012 | UNRESOLVED | SYSTEM | honest: PTHREAD_NULL (pthread.h, Issue 8, unshaded) is missing |
| XBD.spawn.h.001 | UNRESOLVED | SYSTEM | honest: a POSIX_SPAWN_* flag (POSIX_SPAWN_SETSID is new and unshaded in Issue 8 spawn.h) is missing |
| XBD.sys_mman.h.002 | UNRESOLVED | SYSTEM | honest: MAP_ANONYMOUS (sys/mman.h, Issue 8, unshaded) is not declared (XSH.mmap.010's build log shows it) |
| XBD.sys_mman.h.003 | UNRESOLVED | SYSTEM | honest: MAP_ANONYMOUS (sys/mman.h, Issue 8, unshaded) is not declared (XSH.mmap.010's build log shows it) |
| XBD.sys_socket.h.013 | UNRESOLVED | SYSTEM | honest: blocked by XBD.sys_socket.h.012, which does not build (CMSG_SPACE undeclared) |
| XBD.sys_socket.h.016 | UNRESOLVED | SYSTEM | honest: blocked by XBD.sys_socket.h.012, which does not build (CMSG_SPACE undeclared) |
| XBD.sys_socket.h.014 | UNRESOLVED | SYSTEM | honest: blocked by XBD.sys_socket.h.012, which does not build (CMSG_SPACE undeclared) |
| XBD.sys_stat.h.007 | UNRESOLVED | SYSTEM | honest: S_TYPEISMQ/S_TYPEISSEM/S_TYPEISSHM are unshaded sys/stat.h macros and are missing |
| XBD.sys_wait.h.006 | UNRESOLVED | SYSTEM | honest: WCOREDUMP (sys/wait.h, Issue 8, unshaded) is missing |
| XCU.2.5-parameters.020 | UNRESOLVED | TEST-BUG | harness: tests ran in a background process group of the driver's terminal, where XCU 2.11 makes an interactive shell stop itself (SIGTTIN), so every 'sh -i' test hung to the timeout; fixed in 7660db10 (reproduced and cured on a reference system) |
| XCU.2.5-parameters.021 | UNRESOLVED | TEST-BUG | harness: tests ran in a background process group of the driver's terminal, where XCU 2.11 makes an interactive shell stop itself (SIGTTIN), so every 'sh -i' test hung to the timeout; fixed in 7660db10 (reproduced and cured on a reference system) |
| XCU.2.5-parameters.022 | UNRESOLVED | TEST-BUG | harness: tests ran in a background process group of the driver's terminal, where XCU 2.11 makes an interactive shell stop itself (SIGTTIN), so every 'sh -i' test hung to the timeout; fixed in 7660db10 (reproduced and cured on a reference system) |
| XCU.2.5-parameters.023 | UNRESOLVED | TEST-BUG | harness: tests ran in a background process group of the driver's terminal, where XCU 2.11 makes an interactive shell stop itself (SIGTTIN), so every 'sh -i' test hung to the timeout; fixed in 7660db10 (reproduced and cured on a reference system) |
| XCU.2.5-parameters.024 | UNRESOLVED | TEST-BUG | harness: tests ran in a background process group of the driver's terminal, where XCU 2.11 makes an interactive shell stop itself (SIGTTIN), so every 'sh -i' test hung to the timeout; fixed in 7660db10 (reproduced and cured on a reference system) |
| XCU.at.002 | UNRESOLVED | SYSTEM | honest: setup needs the 'job ID at DATE' line that at STDERR requires; at writes 'Job N will be executed using /bin/sh', which XCU.at.001 FAILs |
| XCU.at.003 | UNRESOLVED | SYSTEM | honest: setup needs the 'job ID at DATE' line that at STDERR requires; at writes 'Job N will be executed using /bin/sh', which XCU.at.001 FAILs |
| XCU.at.005 | UNRESOLVED | TEST-BUG | a refusal of every timespec was UNRESOLVED, hiding 'garbled time' for 'noon Dec 9, 2035', which the at OPERANDS grammar allows; now a -t probe decides whether at is usable and a refusal FAILs (83c85317) |
| XCU.at.007 | UNRESOLVED | SYSTEM | honest: setup needs the 'job ID at DATE' line that at STDERR requires; at writes 'Job N will be executed using /bin/sh', which XCU.at.001 FAILs |
| XCU.bg.002 | UNRESOLVED | TEST-BUG | harness: tests ran in a background process group of the driver's terminal, where XCU 2.11 makes an interactive shell stop itself (SIGTTIN), so every 'sh -i' test hung to the timeout; fixed in 7660db10 (reproduced and cured on a reference system) |
| XCU.bg.003 | UNRESOLVED | TEST-BUG | harness: tests ran in a background process group of the driver's terminal, where XCU 2.11 makes an interactive shell stop itself (SIGTTIN), so every 'sh -i' test hung to the timeout; fixed in 7660db10 (reproduced and cured on a reference system) |
| XCU.dot.006 | UNRESOLVED | TEST-BUG | harness: tests ran in a background process group of the driver's terminal, where XCU 2.11 makes an interactive shell stop itself (SIGTTIN), so every 'sh -i' test hung to the timeout; fixed in 7660db10 (reproduced and cured on a reference system) |
| XCU.exec.011 | UNRESOLVED | TEST-BUG | harness: tests ran in a background process group of the driver's terminal, where XCU 2.11 makes an interactive shell stop itself (SIGTTIN), so every 'sh -i' test hung to the timeout; fixed in 7660db10 (reproduced and cured on a reference system) |
| XCU.fc.001 | UNRESOLVED | TEST-BUG | harness: tests ran in a background process group of the driver's terminal, where XCU 2.11 makes an interactive shell stop itself (SIGTTIN), so every 'sh -i' test hung to the timeout; fixed in 7660db10 (reproduced and cured on a reference system) |
| XCU.fc.002 | UNRESOLVED | TEST-BUG | harness: tests ran in a background process group of the driver's terminal, where XCU 2.11 makes an interactive shell stop itself (SIGTTIN), so every 'sh -i' test hung to the timeout; fixed in 7660db10 (reproduced and cured on a reference system) |
| XCU.fc.003 | UNRESOLVED | TEST-BUG | harness: tests ran in a background process group of the driver's terminal, where XCU 2.11 makes an interactive shell stop itself (SIGTTIN), so every 'sh -i' test hung to the timeout; fixed in 7660db10 (reproduced and cured on a reference system) |
| XCU.fc.004 | UNRESOLVED | TEST-BUG | harness: tests ran in a background process group of the driver's terminal, where XCU 2.11 makes an interactive shell stop itself (SIGTTIN), so every 'sh -i' test hung to the timeout; fixed in 7660db10 (reproduced and cured on a reference system) |
| XCU.fc.005 | UNRESOLVED | TEST-BUG | harness: tests ran in a background process group of the driver's terminal, where XCU 2.11 makes an interactive shell stop itself (SIGTTIN), so every 'sh -i' test hung to the timeout; fixed in 7660db10 (reproduced and cured on a reference system) |
| XCU.fc.006 | UNRESOLVED | TEST-BUG | harness: tests ran in a background process group of the driver's terminal, where XCU 2.11 makes an interactive shell stop itself (SIGTTIN), so every 'sh -i' test hung to the timeout; fixed in 7660db10 (reproduced and cured on a reference system) |
| XCU.fc.007 | UNRESOLVED | TEST-BUG | harness: tests ran in a background process group of the driver's terminal, where XCU 2.11 makes an interactive shell stop itself (SIGTTIN), so every 'sh -i' test hung to the timeout; fixed in 7660db10 (reproduced and cured on a reference system) |
| XCU.fc.008 | UNRESOLVED | TEST-BUG | harness: tests ran in a background process group of the driver's terminal, where XCU 2.11 makes an interactive shell stop itself (SIGTTIN), so every 'sh -i' test hung to the timeout; fixed in 7660db10 (reproduced and cured on a reference system) |
| XCU.fc.009 | UNRESOLVED | TEST-BUG | harness: tests ran in a background process group of the driver's terminal, where XCU 2.11 makes an interactive shell stop itself (SIGTTIN), so every 'sh -i' test hung to the timeout; fixed in 7660db10 (reproduced and cured on a reference system) |
| XCU.fc.010 | UNRESOLVED | TEST-BUG | harness: tests ran in a background process group of the driver's terminal, where XCU 2.11 makes an interactive shell stop itself (SIGTTIN), so every 'sh -i' test hung to the timeout; fixed in 7660db10 (reproduced and cured on a reference system) |
| XCU.fc.011 | UNRESOLVED | TEST-BUG | harness: tests ran in a background process group of the driver's terminal, where XCU 2.11 makes an interactive shell stop itself (SIGTTIN), so every 'sh -i' test hung to the timeout; fixed in 7660db10 (reproduced and cured on a reference system) |
| XCU.fc.014 | UNRESOLVED | TEST-BUG | harness: tests ran in a background process group of the driver's terminal, where XCU 2.11 makes an interactive shell stop itself (SIGTTIN), so every 'sh -i' test hung to the timeout; fixed in 7660db10 (reproduced and cured on a reference system) |
| XCU.fc.015 | UNRESOLVED | TEST-BUG | harness: tests ran in a background process group of the driver's terminal, where XCU 2.11 makes an interactive shell stop itself (SIGTTIN), so every 'sh -i' test hung to the timeout; fixed in 7660db10 (reproduced and cured on a reference system) |
| XCU.fc.016 | UNRESOLVED | TEST-BUG | harness: tests ran in a background process group of the driver's terminal, where XCU 2.11 makes an interactive shell stop itself (SIGTTIN), so every 'sh -i' test hung to the timeout; fixed in 7660db10 (reproduced and cured on a reference system) |
| XCU.fc.017 | UNRESOLVED | TEST-BUG | harness: tests ran in a background process group of the driver's terminal, where XCU 2.11 makes an interactive shell stop itself (SIGTTIN), so every 'sh -i' test hung to the timeout; fixed in 7660db10 (reproduced and cured on a reference system) |
| XCU.fc.018 | UNRESOLVED | TEST-BUG | harness: tests ran in a background process group of the driver's terminal, where XCU 2.11 makes an interactive shell stop itself (SIGTTIN), so every 'sh -i' test hung to the timeout; fixed in 7660db10 (reproduced and cured on a reference system) |
| XCU.fc.019 | UNRESOLVED | TEST-BUG | harness: tests ran in a background process group of the driver's terminal, where XCU 2.11 makes an interactive shell stop itself (SIGTTIN), so every 'sh -i' test hung to the timeout; fixed in 7660db10 (reproduced and cured on a reference system) |
| XCU.fc.020 | UNRESOLVED | TEST-BUG | harness: tests ran in a background process group of the driver's terminal, where XCU 2.11 makes an interactive shell stop itself (SIGTTIN), so every 'sh -i' test hung to the timeout; fixed in 7660db10 (reproduced and cured on a reference system) |
| XCU.fc.021 | UNRESOLVED | TEST-BUG | harness: tests ran in a background process group of the driver's terminal, where XCU 2.11 makes an interactive shell stop itself (SIGTTIN), so every 'sh -i' test hung to the timeout; fixed in 7660db10 (reproduced and cured on a reference system) |
| XCU.fc.022 | UNRESOLVED | TEST-BUG | harness: tests ran in a background process group of the driver's terminal, where XCU 2.11 makes an interactive shell stop itself (SIGTTIN), so every 'sh -i' test hung to the timeout; fixed in 7660db10 (reproduced and cured on a reference system) |
| XCU.fg.002 | UNRESOLVED | TEST-BUG | harness: tests ran in a background process group of the driver's terminal, where XCU 2.11 makes an interactive shell stop itself (SIGTTIN), so every 'sh -i' test hung to the timeout; fixed in 7660db10 (reproduced and cured on a reference system) |
| XCU.gencat.001 | UNRESOLVED | ENV | honest: the catalog reader is built with c17, a CD-shaded utility this system does not provide |
| XCU.gencat.002 | UNRESOLVED | ENV | honest: the catalog reader is built with c17, a CD-shaded utility this system does not provide |
| XCU.gencat.003 | UNRESOLVED | ENV | honest: the catalog reader is built with c17, a CD-shaded utility this system does not provide |
| XCU.gencat.004 | UNRESOLVED | ENV | honest: the catalog reader is built with c17, a CD-shaded utility this system does not provide |
| XCU.gencat.005 | UNRESOLVED | ENV | honest: the catalog reader is built with c17, a CD-shaded utility this system does not provide |
| XCU.man.002 | UNRESOLVED | ENV | honest: man -k sort finds no summary line (the summary database of this VM is empty); the purpose wording is unspecified, so the test cannot go on |
| XCU.man.003 | UNRESOLVED | ENV | honest: man -k sort finds no summary line (the summary database of this VM is empty); the purpose wording is unspecified, so the test cannot go on |
| XCU.patch.010 | UNRESOLVED | TEST-BUG | harness, same cause: the second patch -i asks whether to assume -R and reads the answer from the terminal, which stops a background group; with 7660db10 there is no terminal and the default is taken (confirm on a rerun) |
| XCU.pax.027 | UNRESOLVED | SYSTEM | honest: setup needs pax -w -x pax, which this pax refuses (XCU.pax.008 FAILs: the pax format is required) |
| XCU.pax.037 | UNRESOLVED | SYSTEM | honest: setup needs pax -w -x pax, which this pax refuses (XCU.pax.008 FAILs: the pax format is required) |
| XCU.pax.039 | UNRESOLVED | SYSTEM | honest: setup needs pax -w -x pax, which this pax refuses (XCU.pax.008 FAILs: the pax format is required) |
| XCU.pax.042 | UNRESOLVED | SYSTEM | honest: setup needs pax -w -x pax, which this pax refuses (XCU.pax.008 FAILs: the pax format is required) |
| XCU.sh-errors.010 | UNRESOLVED | TEST-BUG | harness: tests ran in a background process group of the driver's terminal, where XCU 2.11 makes an interactive shell stop itself (SIGTTIN), so every 'sh -i' test hung to the timeout; fixed in 7660db10 (reproduced and cured on a reference system) |
| XCU.sh-errors.011 | UNRESOLVED | TEST-BUG | harness: tests ran in a background process group of the driver's terminal, where XCU 2.11 makes an interactive shell stop itself (SIGTTIN), so every 'sh -i' test hung to the timeout; fixed in 7660db10 (reproduced and cured on a reference system) |
| XCU.sh.016 | UNRESOLVED | TEST-BUG | harness: tests ran in a background process group of the driver's terminal, where XCU 2.11 makes an interactive shell stop itself (SIGTTIN), so every 'sh -i' test hung to the timeout; fixed in 7660db10 (reproduced and cured on a reference system) |
| XSH.sigtimedwait.001 | UNRESOLVED | SYSTEM | honest: blocked by XSH.sigqueue.001, which does not build because sa_sigaction is not visible in the strict build (XBD.signal.h.014 FAILs) |
| XSH.2.4.2-realtime-signals.004 | UNRESOLVED | SYSTEM | honest: blocked by XSH.sigqueue.001, which does not build because sa_sigaction is not visible in the strict build (XBD.signal.h.014 FAILs) |
| XSH.2.4.2-realtime-signals.005 | UNRESOLVED | SYSTEM | honest: blocked by XSH.sigqueue.001, which does not build because sa_sigaction is not visible in the strict build (XBD.signal.h.014 FAILs) |
| XSH.2.4.3-signal-actions.010 | UNRESOLVED | SYSTEM | honest: blocked by XSH.sigqueue.001, which does not build because sa_sigaction is not visible in the strict build (XBD.signal.h.014 FAILs) |
| XSH.2.5.2-stream-orientation.007 | UNRESOLVED | SYSTEM | honest: blocked by XSH.2.5.2-stream-orientation.006 FAIL |
| XSH.2.9-threads.020 | UNRESOLVED | ENV | honest: the first mq_open() kills the test with SIGSYS (signal 12): the message queue facility is not loaded in this kernel although the build claims MSG |
| XSH.2.9-threads.033 | UNRESOLVED | ENV | honest: the observing thread got fewer than 50 looks in a round (scheduling of the VM); no time bound is stated, so no verdict |
| XSH._Exit.018 | UNRESOLVED | ENV | honest: the first mq_open() kills the test with SIGSYS (signal 12): the message queue facility is not loaded in this kernel although the build claims MSG |
| XSH.aio_cancel.002 | UNRESOLVED | ENV | honest: aio_read/aio_write on a pipe is refused with ENOTSUP (an additional error XSH 2.3 allows; such I/O is off by configuration here); no other file type gives a request that stays in progress |
| XSH.aio_cancel.004 | UNRESOLVED | ENV | honest: aio_read/aio_write on a pipe is refused with ENOTSUP (an additional error XSH 2.3 allows; such I/O is off by configuration here); no other file type gives a request that stays in progress |
| XSH.aio_error.001 | UNRESOLVED | ENV | honest: aio_read/aio_write on a pipe is refused with ENOTSUP (an additional error XSH 2.3 allows; such I/O is off by configuration here); no other file type gives a request that stays in progress |
| XSH.aio_error.003 | UNRESOLVED | ENV | honest: aio_read/aio_write on a pipe is refused with ENOTSUP (an additional error XSH 2.3 allows; such I/O is off by configuration here); no other file type gives a request that stays in progress |
| XSH.aio_return.002 | UNRESOLVED | ENV | honest: aio_read/aio_write on a pipe is refused with ENOTSUP (an additional error XSH 2.3 allows; such I/O is off by configuration here); no other file type gives a request that stays in progress |
| XSH.aio_suspend.001 | UNRESOLVED | ENV | honest: aio_read/aio_write on a pipe is refused with ENOTSUP (an additional error XSH 2.3 allows; such I/O is off by configuration here); no other file type gives a request that stays in progress |
| XSH.aio_suspend.002 | UNRESOLVED | ENV | honest: aio_read/aio_write on a pipe is refused with ENOTSUP (an additional error XSH 2.3 allows; such I/O is off by configuration here); no other file type gives a request that stays in progress |
| XSH.aio_suspend.005 | UNRESOLVED | ENV | honest: aio_read/aio_write on a pipe is refused with ENOTSUP (an additional error XSH 2.3 allows; such I/O is off by configuration here); no other file type gives a request that stays in progress |
| XSH.aio_suspend.006 | UNRESOLVED | ENV | honest: aio_read/aio_write on a pipe is refused with ENOTSUP (an additional error XSH 2.3 allows; such I/O is off by configuration here); no other file type gives a request that stays in progress |
| XSH.catgets.005 | UNRESOLVED | TEST-BUG | gencat refusing a source without $set was UNRESOLVED, but gencat EXTENDED DESCRIPTION requires such messages to go to NL_SETD; now a catalog with sets proves gencat works and the refusal FAILs (83c85317) |
| XSH.close.007 | UNRESOLVED | SYSTEM | honest: blocked by XBD.fcntl.h.006 FAIL (no F_OFD_GETLK etc., unshaded in Issue 8 fcntl.h) |
| XSH.close.009 | UNRESOLVED | SYSTEM | honest: blocked by XBD.unistd.h.001 FAIL (no POSIX_CLOSE_RESTART) |
| XSH.close.010 | UNRESOLVED | SYSTEM | honest: blocked by XBD.unistd.h.001 FAIL (no POSIX_CLOSE_RESTART) |
| XSH.confstr.006 | UNRESOLVED | SYSTEM | honest: _CS_V8_ENV (unistd.h, Issue 8) is not defined |
| XSH.ctermid.004 | UNRESOLVED | ENV | honest: XBD 11.1.3 leaves it implementation-defined whether a session leader's open() of a terminal makes it the controlling terminal, and POSIX has no other way to get one; here it does not |
| XSH.exec.056 | UNRESOLVED | SYSTEM | honest: blocked by XBD.fcntl.h.006 FAIL (no F_OFD_GETLK etc., unshaded in Issue 8 fcntl.h) |
| XSH.fcntl.016 | UNRESOLVED | SYSTEM | honest: blocked by XBD.fcntl.h.008 FAIL (no F_GETOWN_EX/F_SETOWN_EX) |
| XSH.fcntl.037 | UNRESOLVED | SYSTEM | honest: blocked by XBD.fcntl.h.006 FAIL (no F_OFD_GETLK etc., unshaded in Issue 8 fcntl.h) |
| XSH.fcntl.038 | UNRESOLVED | SYSTEM | honest: blocked by XBD.fcntl.h.006 FAIL (no F_OFD_GETLK etc., unshaded in Issue 8 fcntl.h) |
| XSH.fcntl.039 | UNRESOLVED | SYSTEM | honest: blocked by XBD.fcntl.h.006 FAIL (no F_OFD_GETLK etc., unshaded in Issue 8 fcntl.h) |
| XSH.fcntl.040 | UNRESOLVED | SYSTEM | honest: blocked by XBD.fcntl.h.006 FAIL (no F_OFD_GETLK etc., unshaded in Issue 8 fcntl.h) |
| XSH.fcntl.041 | UNRESOLVED | SYSTEM | honest: blocked by XBD.fcntl.h.006 FAIL (no F_OFD_GETLK etc., unshaded in Issue 8 fcntl.h) |
| XSH.fcntl.042 | UNRESOLVED | SYSTEM | honest: blocked by XBD.fcntl.h.006 FAIL (no F_OFD_GETLK etc., unshaded in Issue 8 fcntl.h) |
| XSH.fcntl.043 | UNRESOLVED | SYSTEM | honest: blocked by XBD.fcntl.h.006 FAIL (no F_OFD_GETLK etc., unshaded in Issue 8 fcntl.h) |
| XSH.fork.020 | UNRESOLVED | ENV | honest: the first mq_open() kills the test with SIGSYS (signal 12): the message queue facility is not loaded in this kernel although the build claims MSG |
| XSH.fork.023 | UNRESOLVED | ENV | honest: aio_read/aio_write on a pipe is refused with ENOTSUP (an additional error XSH 2.3 allows; such I/O is off by configuration here); no other file type gives a request that stays in progress |
| XSH.fpathconf.006 | UNRESOLVED | SYSTEM | honest: _PC_TIMESTAMP_RESOLUTION (unistd.h, Issue 8) is not defined |
| XSH.fputc.011 | UNRESOLVED | TEST-BUG | SIGXFSZ is XSI-shaded, so the base build need not see it, but the system still sends it; the child ignored it only under #ifdef and died of it (0x19), so UNRESOLVED; now every signal is blocked when the name is absent (2f3bf0bc) |
| XSH.kill.005 | UNRESOLVED | ENV | honest: needs the profile entry kill_uid (a user ID nothing uses), which this run did not give |
| XSH.kill.012 | UNRESOLVED | ENV | honest: needs the profile entry kill_uid (a user ID nothing uses), which this run did not give |
| XSH.lseek.012 | UNRESOLVED | SYSTEM | honest: blocked by XBD.unistd.h.002 FAIL (no SEEK_HOLE/SEEK_DATA) |
| XSH.lseek.013 | UNRESOLVED | SYSTEM | honest: blocked by XBD.unistd.h.002 FAIL (no SEEK_HOLE/SEEK_DATA) |
| XSH.lseek.014 | UNRESOLVED | SYSTEM | honest: blocked by XBD.unistd.h.002 FAIL (no SEEK_HOLE/SEEK_DATA) |
| XSH.posix_spawn.043 | UNRESOLVED | SYSTEM | honest: a POSIX_SPAWN_* flag (POSIX_SPAWN_SETSID is new and unshaded in Issue 8 spawn.h) is missing |
| XSH.pthread_mutexattr_getprioceiling.001 | UNRESOLVED | STD-AMBIG | setprioceiling(min SCHED_FIFO) gave EINVAL; the page says ceilings lie within the SCHED_FIFO range and EINVAL is for an invalid one, which can be read as every value in range being valid or not; the test already makes this UNRESOLVED (not disputed; nothing to change) |
| XSH.shm_open.005 | UNRESOLVED | SYSTEM | honest: blocked by XSH.mmap.010, which does not build (MAP_ANONYMOUS undeclared) |
| XSH.sigwait.003 | UNRESOLVED | SYSTEM | honest: blocked by XSH.sigqueue.001, which does not build because sa_sigaction is not visible in the strict build (XBD.signal.h.014 FAILs) |
| XSH.socket.013 | UNRESOLVED | ENV | honest: AF_UNIX accepts protocol 99999; which protocols a family supports is the implementation's, so the test cannot call 99999 unsupported |
| XSH.socketpair.010 | UNRESOLVED | ENV | honest: AF_UNIX accepts protocol 99999; which protocols a family supports is the implementation's, so the test cannot call 99999 unsupported |
| XSH.sysconf.014 | UNRESOLVED | SYSTEM | honest: _SC_NPROCESSORS_CONF/_ONLN (unistd.h, Issue 8) are not defined |
| XSH.tcdrain.006 | UNRESOLVED | ENV | honest: XBD 11.1.3 leaves it implementation-defined whether a session leader's open() of a terminal makes it the controlling terminal, and POSIX has no other way to get one; here it does not |
| XSH.tcdrain.007 | UNRESOLVED | ENV | honest: XBD 11.1.3 leaves it implementation-defined whether a session leader's open() of a terminal makes it the controlling terminal, and POSIX has no other way to get one; here it does not |
| XSH.tcdrain.008 | UNRESOLVED | ENV | honest: XBD 11.1.3 leaves it implementation-defined whether a session leader's open() of a terminal makes it the controlling terminal, and POSIX has no other way to get one; here it does not |
| XSH.tcflow.008 | UNRESOLVED | ENV | honest: XBD 11.1.3 leaves it implementation-defined whether a session leader's open() of a terminal makes it the controlling terminal, and POSIX has no other way to get one; here it does not |
| XSH.tcflow.009 | UNRESOLVED | ENV | honest: XBD 11.1.3 leaves it implementation-defined whether a session leader's open() of a terminal makes it the controlling terminal, and POSIX has no other way to get one; here it does not |
| XSH.tcflow.010 | UNRESOLVED | ENV | honest: XBD 11.1.3 leaves it implementation-defined whether a session leader's open() of a terminal makes it the controlling terminal, and POSIX has no other way to get one; here it does not |
| XSH.tcflush.009 | UNRESOLVED | ENV | honest: XBD 11.1.3 leaves it implementation-defined whether a session leader's open() of a terminal makes it the controlling terminal, and POSIX has no other way to get one; here it does not |
| XSH.tcflush.010 | UNRESOLVED | ENV | honest: XBD 11.1.3 leaves it implementation-defined whether a session leader's open() of a terminal makes it the controlling terminal, and POSIX has no other way to get one; here it does not |
| XSH.tcflush.011 | UNRESOLVED | ENV | honest: XBD 11.1.3 leaves it implementation-defined whether a session leader's open() of a terminal makes it the controlling terminal, and POSIX has no other way to get one; here it does not |
| XSH.tcgetattr.004 | UNRESOLVED | ENV | honest: XBD 11.1.3 leaves it implementation-defined whether a session leader's open() of a terminal makes it the controlling terminal, and POSIX has no other way to get one; here it does not |
| XSH.tcgetpgrp.003 | UNRESOLVED | ENV | honest: XBD 11.1.3 leaves it implementation-defined whether a session leader's open() of a terminal makes it the controlling terminal, and POSIX has no other way to get one; here it does not |
| XSH.tcgetpgrp.004 | UNRESOLVED | ENV | honest: XBD 11.1.3 leaves it implementation-defined whether a session leader's open() of a terminal makes it the controlling terminal, and POSIX has no other way to get one; here it does not |
| XSH.tcgetpgrp.005 | UNRESOLVED | ENV | honest: XBD 11.1.3 leaves it implementation-defined whether a session leader's open() of a terminal makes it the controlling terminal, and POSIX has no other way to get one; here it does not |
| XSH.tcgetpgrp.007 | UNRESOLVED | ENV | honest: XBD 11.1.3 leaves it implementation-defined whether a session leader's open() of a terminal makes it the controlling terminal, and POSIX has no other way to get one; here it does not |
| XSH.tcgetsid.001 | UNRESOLVED | ENV | honest: XBD 11.1.3 leaves it implementation-defined whether a session leader's open() of a terminal makes it the controlling terminal, and POSIX has no other way to get one; here it does not |
| XSH.tcgetsid.002 | UNRESOLVED | ENV | honest: XBD 11.1.3 leaves it implementation-defined whether a session leader's open() of a terminal makes it the controlling terminal, and POSIX has no other way to get one; here it does not |
| XSH.tcgetsid.003 | UNRESOLVED | ENV | honest: XBD 11.1.3 leaves it implementation-defined whether a session leader's open() of a terminal makes it the controlling terminal, and POSIX has no other way to get one; here it does not |
| XSH.tcgetsid.004 | UNRESOLVED | ENV | honest: XBD 11.1.3 leaves it implementation-defined whether a session leader's open() of a terminal makes it the controlling terminal, and POSIX has no other way to get one; here it does not |
| XSH.tcgetsid.006 | UNRESOLVED | ENV | honest: XBD 11.1.3 leaves it implementation-defined whether a session leader's open() of a terminal makes it the controlling terminal, and POSIX has no other way to get one; here it does not |
| XSH.tcsendbreak.005 | UNRESOLVED | ENV | honest: XBD 11.1.3 leaves it implementation-defined whether a session leader's open() of a terminal makes it the controlling terminal, and POSIX has no other way to get one; here it does not |
| XSH.tcsendbreak.006 | UNRESOLVED | ENV | honest: XBD 11.1.3 leaves it implementation-defined whether a session leader's open() of a terminal makes it the controlling terminal, and POSIX has no other way to get one; here it does not |
| XSH.tcsendbreak.007 | UNRESOLVED | ENV | honest: XBD 11.1.3 leaves it implementation-defined whether a session leader's open() of a terminal makes it the controlling terminal, and POSIX has no other way to get one; here it does not |
| XSH.tcsetattr.011 | UNRESOLVED | ENV | honest: XBD 11.1.3 leaves it implementation-defined whether a session leader's open() of a terminal makes it the controlling terminal, and POSIX has no other way to get one; here it does not |
| XSH.tcsetattr.012 | UNRESOLVED | ENV | honest: XBD 11.1.3 leaves it implementation-defined whether a session leader's open() of a terminal makes it the controlling terminal, and POSIX has no other way to get one; here it does not |
| XSH.tcsetattr.013 | UNRESOLVED | ENV | honest: XBD 11.1.3 leaves it implementation-defined whether a session leader's open() of a terminal makes it the controlling terminal, and POSIX has no other way to get one; here it does not |
| XSH.tcsetpgrp.003 | UNRESOLVED | ENV | honest: XBD 11.1.3 leaves it implementation-defined whether a session leader's open() of a terminal makes it the controlling terminal, and POSIX has no other way to get one; here it does not |
| XSH.tcsetpgrp.004 | UNRESOLVED | ENV | honest: XBD 11.1.3 leaves it implementation-defined whether a session leader's open() of a terminal makes it the controlling terminal, and POSIX has no other way to get one; here it does not |
| XSH.tcsetpgrp.005 | UNRESOLVED | ENV | honest: XBD 11.1.3 leaves it implementation-defined whether a session leader's open() of a terminal makes it the controlling terminal, and POSIX has no other way to get one; here it does not |
| XSH.tcsetpgrp.006 | UNRESOLVED | ENV | honest: XBD 11.1.3 leaves it implementation-defined whether a session leader's open() of a terminal makes it the controlling terminal, and POSIX has no other way to get one; here it does not |
| XSH.tcsetpgrp.007 | UNRESOLVED | ENV | honest: XBD 11.1.3 leaves it implementation-defined whether a session leader's open() of a terminal makes it the controlling terminal, and POSIX has no other way to get one; here it does not |
| XSH.tcsetpgrp.009 | UNRESOLVED | ENV | honest: XBD 11.1.3 leaves it implementation-defined whether a session leader's open() of a terminal makes it the controlling terminal, and POSIX has no other way to get one; here it does not |
| XSH.tcsetpgrp.010 | UNRESOLVED | ENV | honest: XBD 11.1.3 leaves it implementation-defined whether a session leader's open() of a terminal makes it the controlling terminal, and POSIX has no other way to get one; here it does not |
| XSH.write.006 | UNRESOLVED | TEST-BUG | SIGXFSZ is XSI-shaded, so the base build need not see it, but the system still sends it; the child ignored it only under #ifdef and died of it (0x19), so UNRESOLVED; now every signal is blocked when the name is absent (2f3bf0bc) |
| XSH.write.007 | UNRESOLVED | TEST-BUG | SIGXFSZ is XSI-shaded, so the base build need not see it, but the system still sends it; the child ignored it only under #ifdef and died of it (0x19), so UNRESOLVED; now every signal is blocked when the name is absent (2f3bf0bc) |
| XBD.10.1-directory-structure-and-files.007 | root PASS / user UNRESOLVED | ENV | the system's: /dev/console is readable by root only (EACCES); the test allows that and says UNRESOLVED; not a rule break |
| XBD.4.7-file-access-permissions.002 | root PASS / user UNRESOLVED | ENV | declares needs root (it changes IDs, owners or scheduling, or needs privilege to set up); skipped as the user, as the rule allows |
| XBD.4.7-file-access-permissions.003 | root PASS / user UNRESOLVED | ENV | declares needs root (it changes IDs, owners or scheduling, or needs privilege to set up); skipped as the user, as the rule allows |
| XBD.4.7-file-access-permissions.005 | root PASS / user UNRESOLVED | ENV | declares needs root (it changes IDs, owners or scheduling, or needs privilege to set up); skipped as the user, as the rule allows |
| XBD.4.7-file-access-permissions.006 | root PASS / user UNRESOLVED | ENV | declares needs root (it changes IDs, owners or scheduling, or needs privilege to set up); skipped as the user, as the rule allows |
| XCU.at.001 | root FAIL / user UNRESOLVED | ENV | the system's: at.allow/at.deny (at INPUT FILES) keep the user out ('you do not have permission'), so the test cannot submit; the root FAIL (no job line) stands |
| XCU.at.004 | root FAIL / user UNRESOLVED | ENV | the system's: at.allow/at.deny (at INPUT FILES) keep the user out ('you do not have permission'), so the test cannot submit; the root FAIL (no job line) stands |
| XCU.at.008 | root FAIL / user PASS | SYSTEM | the system's: same test, but at -r of an unknown ID exits 0 as root and >0 as user; the root FAIL is the system's (at EXIT STATUS) |
| XCU.batch.001 | root FAIL / user UNRESOLVED | ENV | the system's: at.allow/at.deny (at INPUT FILES) keep the user out ('you do not have permission'), so the test cannot submit; the root FAIL (no job line) stands |
| XCU.chgrp.001 | root PASS / user UNRESOLVED | ENV | declares needs root (it changes IDs, owners or scheduling, or needs privilege to set up); skipped as the user, as the rule allows |
| XCU.chgrp.002 | root PASS / user UNRESOLVED | ENV | declares needs root (it changes IDs, owners or scheduling, or needs privilege to set up); skipped as the user, as the rule allows |
| XCU.chgrp.003 | root PASS / user UNRESOLVED | ENV | declares needs root (it changes IDs, owners or scheduling, or needs privilege to set up); skipped as the user, as the rule allows |
| XCU.chgrp.004 | root PASS / user UNRESOLVED | ENV | declares needs root (it changes IDs, owners or scheduling, or needs privilege to set up); skipped as the user, as the rule allows |
| XCU.chgrp.005 | root PASS / user UNRESOLVED | ENV | declares needs root (it changes IDs, owners or scheduling, or needs privilege to set up); skipped as the user, as the rule allows |
| XCU.chown.001 | root PASS / user UNRESOLVED | ENV | declares needs root (it changes IDs, owners or scheduling, or needs privilege to set up); skipped as the user, as the rule allows |
| XCU.chown.002 | root PASS / user UNRESOLVED | ENV | declares needs root (it changes IDs, owners or scheduling, or needs privilege to set up); skipped as the user, as the rule allows |
| XCU.chown.003 | root PASS / user UNRESOLVED | ENV | declares needs root (it changes IDs, owners or scheduling, or needs privilege to set up); skipped as the user, as the rule allows |
| XCU.chown.004 | root PASS / user UNRESOLVED | ENV | declares needs root (it changes IDs, owners or scheduling, or needs privilege to set up); skipped as the user, as the rule allows |
| XCU.chown.005 | root PASS / user UNRESOLVED | ENV | declares needs root (it changes IDs, owners or scheduling, or needs privilege to set up); skipped as the user, as the rule allows |
| XCU.chown.006 | root PASS / user UNRESOLVED | ENV | declares needs root (it changes IDs, owners or scheduling, or needs privilege to set up); skipped as the user, as the rule allows |
| XCU.cp.027 | root PASS / user UNRESOLVED | ENV | declares needs root (it changes IDs, owners or scheduling, or needs privilege to set up); skipped as the user, as the rule allows |
| XCU.df.003 | root PASS / user FAIL | SYSTEM | the system's: df STDOUT requires the percentage rounded up; 581624/(581624+3259176) is 15.14%, df printed 15%; root PASS came from other numbers on its file system, not from privilege |
| XCU.find.013 | root PASS / user UNRESOLVED | ENV | declares needs root (it changes IDs, owners or scheduling, or needs privilege to set up); skipped as the user, as the rule allows |
| XCU.id.007 | root PASS / user UNRESOLVED | ENV | declares needs root (it changes IDs, owners or scheduling, or needs privilege to set up); skipped as the user, as the rule allows |
| XCU.ls.041 | root PASS / user UNRESOLVED | TEST-BUG | rule broken by the test: a new file may take its directory's group (open() allows either), so the unprivileged owner could not set S_ISGID; the file is now chgrp'd to id -g first (44c3bae4) |
| XCU.pax.026 | root FAIL / user UNRESOLVED | TEST-BUG | rule broken by the test: a new file may take its directory's group (open() allows either), so the unprivileged owner could not set S_ISGID; the file is now chgrp'd to id -g first (44c3bae4) |
| XCU.pax.032 | root PASS / user UNRESOLVED | TEST-BUG | rule broken by the test: a new file may take its directory's group (open() allows either), so the unprivileged owner could not set S_ISGID; the file is now chgrp'd to id -g first (44c3bae4) |
| XCU.pax.034 | root PASS / user UNRESOLVED | TEST-BUG | rule broken by the test: a new file may take its directory's group (open() allows either), so the unprivileged owner could not set S_ISGID; the file is now chgrp'd to id -g first (44c3bae4) |
| XCU.sh.060 | root PASS / user FAIL | TEST-BUG | rule broken by timing: the byte counts of two pty sessions differed by one doubly echoed typed line (11 bytes); now the sessions are compared without typed-command lines (bd49c78b) |
| XCU.ulimit.006 | root FAIL / user PASS | SYSTEM | the system's: same test; as root a soft limit above the hard one is accepted with exit 0 (ulimit DESCRIPTION forbids it), as user it is refused |
| XSH.2.10-sockets.014 | root UNINITIATED / user UNRESOLVED | ENV | declares needs root (it changes IDs, owners or scheduling, or needs privilege to set up); skipped as the user, as the rule allows |
| XSH.sigqueue.007 | root UNINITIATED / user UNRESOLVED | ENV | declares needs root (it changes IDs, owners or scheduling, or needs privilege to set up); skipped as the user, as the rule allows |
| XSH.access.003 | root PASS / user UNRESOLVED | ENV | declares needs root (it changes IDs, owners or scheduling, or needs privilege to set up); skipped as the user, as the rule allows |
| XSH.access.004 | root PASS / user UNRESOLVED | ENV | declares needs root (it changes IDs, owners or scheduling, or needs privilege to set up); skipped as the user, as the rule allows |
| XSH.access.019 | root PASS / user UNRESOLVED | ENV | declares needs root (it changes IDs, owners or scheduling, or needs privilege to set up); skipped as the user, as the rule allows |
| XSH.chmod.006 | root PASS / user UNRESOLVED | ENV | declares needs root (it changes IDs, owners or scheduling, or needs privilege to set up); skipped as the user, as the rule allows |
| XSH.chmod.007 | root PASS / user UNRESOLVED | ENV | declares needs root (it changes IDs, owners or scheduling, or needs privilege to set up); skipped as the user, as the rule allows |
| XSH.chmod.021 | root PASS / user UNRESOLVED | ENV | declares needs root (it changes IDs, owners or scheduling, or needs privilege to set up); skipped as the user, as the rule allows |
| XSH.chown.001 | root PASS / user UNRESOLVED | ENV | declares needs root (it changes IDs, owners or scheduling, or needs privilege to set up); skipped as the user, as the rule allows |
| XSH.chown.002 | root PASS / user UNRESOLVED | ENV | declares needs root (it changes IDs, owners or scheduling, or needs privilege to set up); skipped as the user, as the rule allows |
| XSH.chown.003 | root PASS / user UNRESOLVED | ENV | declares needs root (it changes IDs, owners or scheduling, or needs privilege to set up); skipped as the user, as the rule allows |
| XSH.chown.005 | root PASS / user UNRESOLVED | ENV | declares needs root (it changes IDs, owners or scheduling, or needs privilege to set up); skipped as the user, as the rule allows |
| XSH.chown.007 | root PASS / user UNRESOLVED | ENV | declares needs root (it changes IDs, owners or scheduling, or needs privilege to set up); skipped as the user, as the rule allows |
| XSH.chown.008 | root INSPECT / user UNRESOLVED | ENV | declares needs root (it changes IDs, owners or scheduling, or needs privilege to set up); skipped as the user, as the rule allows |
| XSH.chown.009 | root PASS / user UNRESOLVED | ENV | declares needs root (it changes IDs, owners or scheduling, or needs privilege to set up); skipped as the user, as the rule allows |
| XSH.chown.010 | root PASS / user UNRESOLVED | ENV | declares needs root (it changes IDs, owners or scheduling, or needs privilege to set up); skipped as the user, as the rule allows |
| XSH.chown.011 | root PASS / user UNRESOLVED | ENV | declares needs root (it changes IDs, owners or scheduling, or needs privilege to set up); skipped as the user, as the rule allows |
| XSH.exec.052 | root PASS / user UNRESOLVED | ENV | declares needs root (it changes IDs, owners or scheduling, or needs privilege to set up); skipped as the user, as the rule allows |
| XSH.exec.053 | root PASS / user UNRESOLVED | ENV | declares needs root (it changes IDs, owners or scheduling, or needs privilege to set up); skipped as the user, as the rule allows |
| XSH.fchmod.005 | root PASS / user UNRESOLVED | ENV | declares needs root (it changes IDs, owners or scheduling, or needs privilege to set up); skipped as the user, as the rule allows |
| XSH.fchown.001 | root PASS / user UNRESOLVED | ENV | declares needs root (it changes IDs, owners or scheduling, or needs privilege to set up); skipped as the user, as the rule allows |
| XSH.fchown.002 | root PASS / user UNRESOLVED | ENV | declares needs root (it changes IDs, owners or scheduling, or needs privilege to set up); skipped as the user, as the rule allows |
| XSH.fchown.003 | root PASS / user UNRESOLVED | ENV | declares needs root (it changes IDs, owners or scheduling, or needs privilege to set up); skipped as the user, as the rule allows |
| XSH.futimens.010 | root PASS / user UNRESOLVED | ENV | declares needs root (it changes IDs, owners or scheduling, or needs privilege to set up); skipped as the user, as the rule allows |
| XSH.futimens.011 | root PASS / user UNRESOLVED | ENV | declares needs root (it changes IDs, owners or scheduling, or needs privilege to set up); skipped as the user, as the rule allows |
| XSH.futimens.012 | root PASS / user UNRESOLVED | ENV | declares needs root (it changes IDs, owners or scheduling, or needs privilege to set up); skipped as the user, as the rule allows |
| XSH.futimens.021 | root PASS / user UNRESOLVED | ENV | declares needs root (it changes IDs, owners or scheduling, or needs privilege to set up); skipped as the user, as the rule allows |
| XSH.futimens.026 | root PASS / user UNRESOLVED | ENV | declares needs root (it changes IDs, owners or scheduling, or needs privilege to set up); skipped as the user, as the rule allows |
| XSH.getegid.002 | root PASS / user UNRESOLVED | ENV | declares needs root (it changes IDs, owners or scheduling, or needs privilege to set up); skipped as the user, as the rule allows |
| XSH.getenv.005 | root UNINITIATED / user UNRESOLVED | ENV | declares needs root (it changes IDs, owners or scheduling, or needs privilege to set up); skipped as the user, as the rule allows |
| XSH.geteuid.001 | root PASS / user UNRESOLVED | ENV | declares needs root (it changes IDs, owners or scheduling, or needs privilege to set up); skipped as the user, as the rule allows |
| XSH.getgid.002 | root PASS / user UNRESOLVED | ENV | declares needs root (it changes IDs, owners or scheduling, or needs privilege to set up); skipped as the user, as the rule allows |
| XSH.getgroups.001 | root PASS / user UNRESOLVED | ENV | declares needs root (it changes IDs, owners or scheduling, or needs privilege to set up); skipped as the user, as the rule allows |
| XSH.getgroups.002 | root PASS / user UNRESOLVED | ENV | declares needs root (it changes IDs, owners or scheduling, or needs privilege to set up); skipped as the user, as the rule allows |
| XSH.getgroups.003 | root PASS / user UNRESOLVED | ENV | declares needs root (it changes IDs, owners or scheduling, or needs privilege to set up); skipped as the user, as the rule allows |
| XSH.getuid.001 | root PASS / user UNRESOLVED | ENV | declares needs root (it changes IDs, owners or scheduling, or needs privilege to set up); skipped as the user, as the rule allows |
| XSH.kill.009 | root PASS / user UNRESOLVED | ENV | declares needs root (it changes IDs, owners or scheduling, or needs privilege to set up); skipped as the user, as the rule allows |
| XSH.kill.010 | root PASS / user UNRESOLVED | ENV | declares needs root (it changes IDs, owners or scheduling, or needs privilege to set up); skipped as the user, as the rule allows |
| XSH.kill.011 | root PASS / user UNRESOLVED | ENV | declares needs root (it changes IDs, owners or scheduling, or needs privilege to set up); skipped as the user, as the rule allows |
| XSH.kill.013 | root PASS / user UNRESOLVED | ENV | declares needs root (it changes IDs, owners or scheduling, or needs privilege to set up); skipped as the user, as the rule allows |
| XSH.kill.014 | root PASS / user UNRESOLVED | ENV | declares needs root (it changes IDs, owners or scheduling, or needs privilege to set up); skipped as the user, as the rule allows |
| XSH.kill.016 | root PASS / user UNRESOLVED | ENV | declares needs root (it changes IDs, owners or scheduling, or needs privilege to set up); skipped as the user, as the rule allows |
| XSH.lchown.001 | root PASS / user UNRESOLVED | ENV | declares needs root (it changes IDs, owners or scheduling, or needs privilege to set up); skipped as the user, as the rule allows |
| XSH.lchown.002 | root PASS / user UNRESOLVED | ENV | declares needs root (it changes IDs, owners or scheduling, or needs privilege to set up); skipped as the user, as the rule allows |
| XSH.lchown.003 | root PASS / user UNRESOLVED | ENV | declares needs root (it changes IDs, owners or scheduling, or needs privilege to set up); skipped as the user, as the rule allows |
| XSH.link.019 | root PASS / user UNRESOLVED | ENV | declares needs root (it changes IDs, owners or scheduling, or needs privilege to set up); skipped as the user, as the rule allows |
| XSH.mq_open.015 | root UNINITIATED / user UNRESOLVED | ENV | declares needs root (it changes IDs, owners or scheduling, or needs privilege to set up); skipped as the user, as the rule allows |
| XSH.posix_spawn.045 | root PASS / user UNRESOLVED | ENV | declares needs root (it changes IDs, owners or scheduling, or needs privilege to set up); skipped as the user, as the rule allows |
| XSH.posix_spawn.046 | root PASS / user UNRESOLVED | ENV | declares needs root (it changes IDs, owners or scheduling, or needs privilege to set up); skipped as the user, as the rule allows |
| XSH.posix_spawn.047 | root PASS / user UNRESOLVED | ENV | declares needs root (it changes IDs, owners or scheduling, or needs privilege to set up); skipped as the user, as the rule allows |
| XSH.posix_spawn.048 | root PASS / user UNRESOLVED | ENV | declares needs root (it changes IDs, owners or scheduling, or needs privilege to set up); skipped as the user, as the rule allows |
| XSH.posix_spawn.049 | root PASS / user UNRESOLVED | ENV | declares needs root (it changes IDs, owners or scheduling, or needs privilege to set up); skipped as the user, as the rule allows |
| XSH.posix_spawn.050 | root PASS / user UNRESOLVED | ENV | declares needs root (it changes IDs, owners or scheduling, or needs privilege to set up); skipped as the user, as the rule allows |
| XSH.posix_spawn.051 | root PASS / user UNRESOLVED | ENV | declares needs root (it changes IDs, owners or scheduling, or needs privilege to set up); skipped as the user, as the rule allows |
| XSH.posix_spawn.052 | root PASS / user UNRESOLVED | ENV | declares needs root (it changes IDs, owners or scheduling, or needs privilege to set up); skipped as the user, as the rule allows |
| XSH.posix_spawnattr_getschedparam.003 | root PASS / user UNRESOLVED | ENV | declares needs root (it changes IDs, owners or scheduling, or needs privilege to set up); skipped as the user, as the rule allows |
| XSH.posix_spawnattr_getschedpolicy.003 | root PASS / user UNRESOLVED | ENV | declares needs root (it changes IDs, owners or scheduling, or needs privilege to set up); skipped as the user, as the rule allows |
| XSH.pthread_getschedparam.003 | root PASS / user UNRESOLVED | ENV | declares needs root (it changes IDs, owners or scheduling, or needs privilege to set up); skipped as the user, as the rule allows |
| XSH.pthread_getschedparam.004 | root PASS / user UNRESOLVED | ENV | declares needs root (it changes IDs, owners or scheduling, or needs privilege to set up); skipped as the user, as the rule allows |
| XSH.pthread_getschedparam.005 | root PASS / user UNRESOLVED | ENV | declares needs root (it changes IDs, owners or scheduling, or needs privilege to set up); skipped as the user, as the rule allows |
| XSH.pthread_getschedparam.006 | root FAIL / user UNRESOLVED | ENV | declares needs root (it changes IDs, owners or scheduling, or needs privilege to set up); skipped as the user, as the rule allows |
| XSH.pthread_getschedparam.007 | root PASS / user UNRESOLVED | ENV | declares needs root (it changes IDs, owners or scheduling, or needs privilege to set up); skipped as the user, as the rule allows |
| XSH.pthread_setschedprio.001 | root UNINITIATED / user UNRESOLVED | ENV | declares needs root (it changes IDs, owners or scheduling, or needs privilege to set up); skipped as the user, as the rule allows |
| XSH.pthread_setschedprio.002 | root UNINITIATED / user UNRESOLVED | ENV | declares needs root (it changes IDs, owners or scheduling, or needs privilege to set up); skipped as the user, as the rule allows |
| XSH.pthread_setschedprio.003 | root UNINITIATED / user UNRESOLVED | ENV | declares needs root (it changes IDs, owners or scheduling, or needs privilege to set up); skipped as the user, as the rule allows |
| XSH.sched_getparam.004 | root PASS / user UNRESOLVED | ENV | declares needs root (it changes IDs, owners or scheduling, or needs privilege to set up); skipped as the user, as the rule allows |
| XSH.sched_getscheduler.003 | root PASS / user UNRESOLVED | ENV | declares needs root (it changes IDs, owners or scheduling, or needs privilege to set up); skipped as the user, as the rule allows |
| XSH.sched_rr_get_interval.001 | root PASS / user UNRESOLVED | ENV | declares needs root (it changes IDs, owners or scheduling, or needs privilege to set up); skipped as the user, as the rule allows |
| XSH.sched_setparam.001 | root PASS / user UNRESOLVED | ENV | declares needs root (it changes IDs, owners or scheduling, or needs privilege to set up); skipped as the user, as the rule allows |
| XSH.sched_setparam.002 | root PASS / user UNRESOLVED | ENV | declares needs root (it changes IDs, owners or scheduling, or needs privilege to set up); skipped as the user, as the rule allows |
| XSH.sched_setparam.004 | root PASS / user UNRESOLVED | ENV | declares needs root (it changes IDs, owners or scheduling, or needs privilege to set up); skipped as the user, as the rule allows |
| XSH.sched_setscheduler.001 | root FAIL / user UNRESOLVED | ENV | declares needs root (it changes IDs, owners or scheduling, or needs privilege to set up); skipped as the user, as the rule allows |
| XSH.sched_setscheduler.002 | root PASS / user UNRESOLVED | ENV | declares needs root (it changes IDs, owners or scheduling, or needs privilege to set up); skipped as the user, as the rule allows |
| XSH.sched_setscheduler.003 | root PASS / user UNRESOLVED | ENV | declares needs root (it changes IDs, owners or scheduling, or needs privilege to set up); skipped as the user, as the rule allows |
| XSH.sched_setscheduler.004 | root PASS / user UNRESOLVED | ENV | declares needs root (it changes IDs, owners or scheduling, or needs privilege to set up); skipped as the user, as the rule allows |
| XSH.sched_setscheduler.007 | root PASS / user UNRESOLVED | ENV | declares needs root (it changes IDs, owners or scheduling, or needs privilege to set up); skipped as the user, as the rule allows |
| XSH.setegid.001 | root PASS / user UNRESOLVED | ENV | declares needs root (it changes IDs, owners or scheduling, or needs privilege to set up); skipped as the user, as the rule allows |
| XSH.setegid.003 | root PASS / user UNRESOLVED | ENV | declares needs root (it changes IDs, owners or scheduling, or needs privilege to set up); skipped as the user, as the rule allows |
| XSH.setegid.004 | root PASS / user UNRESOLVED | ENV | declares needs root (it changes IDs, owners or scheduling, or needs privilege to set up); skipped as the user, as the rule allows |
| XSH.setegid.005 | root FAIL / user UNRESOLVED | ENV | declares needs root (it changes IDs, owners or scheduling, or needs privilege to set up); skipped as the user, as the rule allows |
| XSH.setegid.006 | root PASS / user UNRESOLVED | ENV | declares needs root (it changes IDs, owners or scheduling, or needs privilege to set up); skipped as the user, as the rule allows |
| XSH.setegid.007 | root PASS / user UNRESOLVED | ENV | declares needs root (it changes IDs, owners or scheduling, or needs privilege to set up); skipped as the user, as the rule allows |
| XSH.seteuid.001 | root PASS / user UNRESOLVED | ENV | declares needs root (it changes IDs, owners or scheduling, or needs privilege to set up); skipped as the user, as the rule allows |
| XSH.seteuid.003 | root PASS / user UNRESOLVED | ENV | declares needs root (it changes IDs, owners or scheduling, or needs privilege to set up); skipped as the user, as the rule allows |
| XSH.seteuid.004 | root PASS / user UNRESOLVED | ENV | declares needs root (it changes IDs, owners or scheduling, or needs privilege to set up); skipped as the user, as the rule allows |
| XSH.seteuid.005 | root FAIL / user UNRESOLVED | ENV | declares needs root (it changes IDs, owners or scheduling, or needs privilege to set up); skipped as the user, as the rule allows |
| XSH.seteuid.006 | root PASS / user UNRESOLVED | ENV | declares needs root (it changes IDs, owners or scheduling, or needs privilege to set up); skipped as the user, as the rule allows |
| XSH.seteuid.007 | root PASS / user UNRESOLVED | ENV | declares needs root (it changes IDs, owners or scheduling, or needs privilege to set up); skipped as the user, as the rule allows |
| XSH.setgid.001 | root PASS / user UNRESOLVED | ENV | declares needs root (it changes IDs, owners or scheduling, or needs privilege to set up); skipped as the user, as the rule allows |
| XSH.setgid.002 | root PASS / user UNRESOLVED | ENV | declares needs root (it changes IDs, owners or scheduling, or needs privilege to set up); skipped as the user, as the rule allows |
| XSH.setgid.004 | root FAIL / user UNRESOLVED | ENV | declares needs root (it changes IDs, owners or scheduling, or needs privilege to set up); skipped as the user, as the rule allows |
| XSH.setgid.005 | root FAIL / user UNRESOLVED | ENV | declares needs root (it changes IDs, owners or scheduling, or needs privilege to set up); skipped as the user, as the rule allows |
| XSH.setgid.006 | root PASS / user UNRESOLVED | ENV | declares needs root (it changes IDs, owners or scheduling, or needs privilege to set up); skipped as the user, as the rule allows |
| XSH.setgid.007 | root PASS / user UNRESOLVED | ENV | declares needs root (it changes IDs, owners or scheduling, or needs privilege to set up); skipped as the user, as the rule allows |
| XSH.setuid.001 | root PASS / user UNRESOLVED | ENV | declares needs root (it changes IDs, owners or scheduling, or needs privilege to set up); skipped as the user, as the rule allows |
| XSH.setuid.002 | root PASS / user UNRESOLVED | ENV | declares needs root (it changes IDs, owners or scheduling, or needs privilege to set up); skipped as the user, as the rule allows |
| XSH.setuid.004 | root FAIL / user UNRESOLVED | ENV | declares needs root (it changes IDs, owners or scheduling, or needs privilege to set up); skipped as the user, as the rule allows |
| XSH.setuid.005 | root FAIL / user UNRESOLVED | ENV | declares needs root (it changes IDs, owners or scheduling, or needs privilege to set up); skipped as the user, as the rule allows |
| XSH.setuid.006 | root PASS / user UNRESOLVED | ENV | declares needs root (it changes IDs, owners or scheduling, or needs privilege to set up); skipped as the user, as the rule allows |
| XSH.setuid.007 | root PASS / user UNRESOLVED | ENV | declares needs root (it changes IDs, owners or scheduling, or needs privilege to set up); skipped as the user, as the rule allows |
| XSH.shm_open.013 | root PASS / user UNRESOLVED | ENV | declares needs root (it changes IDs, owners or scheduling, or needs privilege to set up); skipped as the user, as the rule allows |
| XSH.shm_unlink.005 | root PASS / user UNRESOLVED | ENV | declares needs root (it changes IDs, owners or scheduling, or needs privilege to set up); skipped as the user, as the rule allows |
