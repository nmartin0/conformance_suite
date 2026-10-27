# Known Linux deviations from POSIX.1-2024

Copyright (c) 2026 Nicholas Martin. All rights reserved.

What Linux (kernel, glibc, musl, dash, GNU coreutils) does differently from
POSIX.1-2024, so that a FAIL on a Linux reference run can be told apart
from a wrong test. A correct test **must** fail on these.

How it is used:

- `known/*.known` lists, per assertion, the result a reference system is
  expected to give and why. `analysis/analyze.py calibrate DB RUN
  known/linux-glibc.known` reports every other FAIL as SUSPECT (look at
  the test first) and every listed entry that no longer fails as STALE.
- When writing a test in an area below, expect the listed behaviour on
  Linux; if the test passes there anyway, it is probably too weak.
- Waivers (`waivers/`) are a different thing: they are for the system
  being judged, not for reference systems.

Compiled from documentation, release notes and man pages, and from runs in
the development container. No other test suite's code or results are used. POSIX requirements are
paraphrased. Re-check entries marked "unconfirmed" before relying on them.

## Observed by posixcheck

Development container: Linux 6.18 (x86_64), glibc 2.39 (Ubuntu
2.39-0ubuntu8), musl via musl-gcc, dash 0.5.12. Each entry was reproduced
by the named assertion.

| Assertion | Result | Cause | Entry |
| --- | --- | --- | --- |
| XBD.fcntl.h.002, .005 (XSH.open.011, .012 blocked) | FAIL | no `FD_CLOFORK` / `O_CLOFORK` in the kernel or either libc | 1.1 |
| XBD.fcntl.h.006 (XSH.close.007 blocked) | FAIL (glibc) | `F_OFD_*` not visible with `_POSIX_C_SOURCE=202405L` | 1.8, 11.1 |
| XBD.fcntl.h.007 (XSH.open.050 blocked) | FAIL (glibc) | glibc does not define `O_TTY_INIT`; musl defines it as 0, which is allowed (XSH.open.050 NOTINUSE) | 1.7 |
| XBD.unistd.h.001 (XSH.close.009, .010 blocked) | FAIL (glibc) | no `posix_close()` / `POSIX_CLOSE_RESTART` | 1.3 |
| XSH.open.044 | FAIL | `open("new/", O_CREAT...)` gives EISDIR; ENOENT or ENOTDIR required | O.1 |
| XSH.open.045 | FAIL | `open("file/", O_CREAT...)` gives EISDIR; ENOTDIR required | O.1 |
| XSH.fopen.027, .028 | FAIL | `fopen("file/", "w")` and `fopen("new/", "w")` give EISDIR (the O_CREAT open beneath) | O.1 |
| XSH.fopen.032, XSH.fdopen.009 | FAIL (glibc) | sysconf(_SC_STREAM_MAX) is 16, but any number of streams can be opened | O.31 |
| XSH.fopen.032, XSH.fdopen.009 | NOTINUSE (musl) | sysconf(_SC_STREAM_MAX) is -1 (indeterminate, which is allowed) | - |
| XSH.fdopen.005 | FAIL (glibc) | `fdopen()` ignores 'e': FD_CLOEXEC is not set | O.32 |
| XSH.fseek.004 | INSPECT (glibc) | after fflush() of an input stream, fseek() within the read buffer leaves the descriptor at the end of the read-ahead (disputed, Q-40) | O.33 |
| XSH.fmemopen.004, XSH.fmemopen.006, XSH.fmemopen.021 | FAIL | fmemopen() null-byte handling | O.34 |
| XSH.fmemopen.023 | FAIL | fmemopen() stream is byte-oriented at once | O.35 |
| XSH.open_memstream.015 | FAIL | open_memstream() with setvbuf() crashes | O.36 |
| XSH.open_memstream.005, XSH.open_memstream.007 | FAIL | open_memstream() size wrong after seek/close | O.37 |
| XSH.fgets.006, XSH.getdelim.012 | FAIL | fgets()/getdelim() with a non-blocking pipe | O.38 |
| XSH.fgetc.005, XSH.fputc.007 | FAIL | fgetc()/fputc() errno 0 | O.39 |
| XSH.freopen.011, XSH.freopen.022, XSH.freopen.008, XSH.freopen.013, XSH.freopen.014 | FAIL | freopen() order of close and open | O.40 |
| XSH.fscanf.014, XSH.fscanf.015, XSH.fscanf.043 | FAIL | fscanf() prefix and suppressed conversion | O.41 |
| XSH.localtime.006 | FAIL | all-year DST rule at the year end | O.42 |
| XSH.mlock.004 | FAIL | failed mlock() locks the first page | O.43 |
| XSH.pthread_cancel.005, XSH.pthread_mutex_lock.014 | FAIL | cleanup cancelability; unusable mutex locks | O.44 |
| XSH.pthread_exit.006 | FAIL | fork() child hangs after pthread_exit() | O.45 |
| XSH.readlink.003 | FAIL | readlink() into 0 bytes returns -1 | O.46 |
| XSH.seteuid.004, XSH.setegid.004, XSH.setregid.006, XSH.seteuid.005, XSH.setegid.005 | FAIL | effective and real ID rules | O.47 |
| XSH.strftime.002, XSH.strftime.007, XSH.strftime.012, XSH.strftime.014, XSH.strftime.008, XSH.strftime.009, XSH.strftime.013 | FAIL | strftime() conformance items | O.48 |
| XSH.strsignal.002 | FAIL | psignal()/strsignal() texts differ | O.49 |
| XSH.symlink.003 | FAIL | symlink("", path) gives ENOENT | O.50 |
| XSH.sysconf.001, XSH.sysconf.002, XSH.sysconf.004, XSH.sysconf.009, XSH.sysconf.005, XSH.sysconf.007 | FAIL | sysconf() values | O.51 |
| XSH.shm_open.006, XSH.fprintf.037 | FAIL | shm_open() O_NONBLOCK; %lc null | O.52 |
| XSH.fseek.016 | INSPECT | `fseeko(f, OFF_MAX, SEEK_END)` with output pending flushes, then gives EINVAL, not EOVERFLOW (disputed, Q-35) | O.3 |
| XSH.fdopen.013 | INSPECT (glibc) | `fdopen(fd, "a")` moves the descriptor's offset to the end of file (disputed, Q-36) | - |
| XSH.fseek.015, XSH.ftell.007 | NOTINUSE | long is as wide as off_t (LP64) | - |
| XSH.fopen.034 | NOTINUSE | any byte but slash and NUL can be in a name, so EILSEQ never arises | - |
| XSH.dup.012 to .015 | UNINITIATED (glibc) | `dup3()` exists but is not declared under the strict build | 11.1 |
| XSH.pipe.007 to .010 | UNINITIATED (glibc) | `pipe2()` exists but is not declared under the strict build | 11.1 |
| XBD.fcntl.h.008 (XSH.fcntl.016 blocked) | FAIL | glibc: `F_OWNER_PID`/`F_OWNER_PGRP` are enum constants only, never macros; musl 1.2.4: they and `struct f_owner_ex` need `_GNU_SOURCE` | O.2 |
| XBD.unistd.h.002 (XSH.lseek.012 to .014 blocked) | FAIL (glibc) | `SEEK_HOLE`/`SEEK_DATA` need `_GNU_SOURCE`; with it, the tests pass | 1.12 |
| XSH.lseek.011 | FAIL | `lseek()` gives EINVAL, not EOVERFLOW, when the result overflows `off_t` | O.3 |
| XSH.read.021 | FAIL | `read()`/`recv()` on an unconnected `AF_UNIX` stream socket gives EINVAL; ENOTCONN required (TCP is right) | O.4 |
| XSH.write.022 | FAIL | `pwrite()` on an `O_APPEND` descriptor appends instead of writing at the offset | 1.4 |
| XSH.fork.018 | UNINITIATED (glibc) | `_Fork()` exists but is not declared under the strict build | 2.10 |
| XSH.wait.019 | FAIL | with SIGCHLD blocked, `wait()` leaves the pending SIGCHLD pending after reporting the only child | O.5 |
| XSH._Exit.002, XSH.exit.002 | FAIL | `waitid()` reports only the low 8 bits of the exit status in `si_status` | O.6 |
| XSH.exit.012, XSH._Exit.014 | FAIL | the same 8 bits in the siginfo_t passed to a SIGCHLD handler | O.6 |
| XSH.exec.028 | FAIL | exec does not reset the initial thread's CPU-time clock | O.7 |
| XSH.exec.044, XSH.exec.045 (046, 047 blocked) | FAIL (musl) | execvp()/execlp() give ENOEXEC instead of running sh | O.8 |
| XSH.exec.043 | FAIL (musl) | sysconf(_SC_ARG_MAX) is 131072, below the kernel's limit, so the exec succeeds | O.9 |
| XSH.sigaction.023 | FAIL | SA_RESETHAND leaves SA_SIGINFO set | O.10 |
| XSH.kill.012 | FAIL | kill(-1, sig) does not signal the sender | 2.2 |
| XSH.sigaction.021 | INSPECT | sigqueue() does not queue SIGUSR1 even with SA_SIGINFO (disputed, SI-24) | O.11 (withdrawn) |
| XSH.sigqueue.005 | NOTINUSE (musl) | sysconf(_SC_SIGQUEUE_MAX) is -1 (no definite limit, which is allowed); glibc reports RLIMIT_SIGPENDING | - |
| XBD.signal.h.006 | FAIL (glibc) | the realtime range is 31 signals but {RTSIG_MAX} is 32 | O.12 |
| XSH.sigaltstack.005 | FAIL (glibc) | sigaltstack() accepts SS_ONSTACK in ss_flags | O.13 |
| XSH.psiginfo.004 | FAIL (glibc) | psignal() orients stderr | O.14 |
| XSH.sigaltstack.010 | FAIL (AVX-512 CPUs) | a handler does not fit in MINSIGSTKSZ bytes | O.16 |
| XSH.abort.006 | INSPECT (musl) | abort() with SIGABRT caught but blocked ends the process without the handler (disputed, Q-21) | - |
| XSH.psiginfo.002 | FAIL (musl) | psignal(sig, "") writes ": " | O.15 |
| XSH.psiginfo.003 | INSPECT (glibc) | psiginfo() adds the sender to the description (disputed, Q-19) | - |
| XSH.fstatat.013, XSH.link.021, XSH.unlink.021, XSH.rename.027, XSH.mkdir.018, XSH.chmod.019, XSH.chown.021, XSH.access.018 | UNRESOLVED (glibc), FAIL (musl) | glibc does not define `O_SEARCH`; musl's O_SEARCH (O_PATH) descriptors still get the search check | 1.7, O.18 |
| XSH.link.019 | INSPECT | `fs.protected_hardlinks` refuses a link to another user's readable file with EPERM (disputed, Q-24) | O.19 |
| XSH.rename.012 | FAIL | a final dot or dot-dot gives EBUSY; EINVAL required | O.20 |
| XSH.rename.014 | FAIL | `rename(dir, "new/")` succeeds | O.21 |
| XSH.futimens.024 | FAIL (musl) | `futimens(AT_FDCWD, t)` gives EFAULT, not EBADF | O.22 |
| XSH.psiginfo.007 | FAIL (glibc) | `psiginfo()` leaves the error indicator of stderr clear when the write fails | O.23 |
| XSH._Exit.019 | INSPECT (glibc) | an `aio_write()` outstanding at `_exit()` never reaches the file (disputed, Q-32) | O.24 |
| XSH.close.014 | FAIL (glibc) | an `aio_write()` outstanding at `close()` fails with EBADF | O.25 |
| XSH.close.014 | FAIL (musl) | `close()` with two writes queued on the descriptor never returns | O.30 |
| XSH.futimens.025 | FAIL (musl) | O_SEARCH (O_PATH) descriptors still get the search check | O.18 |
| XSH.fflush.005 | FAIL (glibc) | `fflush()` of an input stream with a pushed-back byte sets the offset to the read-ahead end minus the pushback (4095 for a 1 MiB file, not 1) | O.26 |
| XSH.fclose.003, .010, .011 | FAIL (glibc) | `fclose()` of a seekable input or update stream leaves the offset where read-ahead left it | O.27 |
| XSH.fflush.013, XSH.fclose.012 | FAIL (musl) | `fflush()`/`fclose()` of an `fmemopen()` stream holding more than fits drop the rest and return 0 | O.28 |
| XSH.fflush.013, XSH.fclose.012 | FAIL (glibc) | the same calls return EOF but leave errno unset (ENOSPC required) | O.29 |
| XSH.fflush.011 | INSPECT | neither library syncs input streams for `fflush(NULL)` (disputed, Q-28) | - |
| XSH.chown.008 | INSPECT | `chown(f, -1, -1)` by a non-owner succeeds (disputed, Q-25) | - |
| XSH.unlink.005, .010 | FAIL | `unlink()`/`unlinkat(fd, dir, 0)` of a directory give EISDIR; EPERM required | 1.17 |
| XSH.sig2str.001 to .006 | UNINITIATED | no sig2str() or str2sig() | 3.1 |
| XBD.signal.h.007 | FAIL | no `_SC_NSIG` (nor `NSIG_MAX`) | 1.24 |
| XBD.signal.h.008, .028 | FAIL, UNINITIATED | no `SIG2STR_MAX`, `sig2str()` or `str2sig()` | 3.1 |

O.1 **`open()` with O_CREAT and a trailing slash fails with EISDIR (and so does `fopen()` with "w" or "a").**
- POSIX (Issue 8 `open`, ERRORS): with O_CREAT, a path that ends in one or
  more slashes after a non-slash character fails with ENOENT or ENOTDIR,
  and ENOENT is not allowed when the name without the slashes exists.
  EISDIR is listed only for an existing directory.
- Linux: the kernel's last-component lookup rejects any O_CREAT path with
  a trailing slash with EISDIR, whether the name is missing or a regular
  file. Nothing is created, so only the errno is wrong.
- Comp: kernel. Kind: long-standing. Observe: XSH.open.044, XSH.open.045,
  XSH.fopen.027, XSH.fopen.028.
- Src: posixcheck run, Linux 6.18, glibc and musl alike.

O.2 **`F_OWNER_PID` / `F_OWNER_PGRP` are not macros in glibc; hidden in musl.**
- POSIX (Issue 8 `<fcntl.h>`): defines F_GETOWN_EX, F_SETOWN_EX, the
  symbolic constants F_OWNER_PID and F_OWNER_PGRP, and struct f_owner_ex.
  A symbolic constant is a macro (XBD 3.363); it may also be an
  enumeration constant, but not only that.
- glibc 2.39: F_OWNER_* are members of `enum __pid_type` with no macro,
  even with `_GNU_SOURCE`; F_GETOWN_EX and struct f_owner_ex need
  `_GNU_SOURCE`.
- musl 1.2.4: F_GETOWN_EX is always defined, but F_OWNER_* and struct
  f_owner_ex only with `_GNU_SOURCE`.
- Behaviour is fine: with `_GNU_SOURCE`, XSH.fcntl.016 passes on musl.
- Comp: glibc, musl. Kind: missing-2024. Observe: XBD.fcntl.h.008.
- Src: posixcheck run; /usr/include/x86_64-linux-gnu/bits/fcntl-linux.h,
  /usr/include/x86_64-linux-musl/fcntl.h.

O.3 **`lseek()` reports EINVAL where EOVERFLOW is required.**
- POSIX (`lseek`, ERRORS): a resulting offset that cannot be represented
  in off_t fails with EOVERFLOW; EINVAL is for a bad whence or a
  negative result. The offset maximum of an open file description is the
  largest off_t (XSH `open`).
- Linux: SEEK_CUR of the largest off_t from offset 1 gives EINVAL with
  the offset unchanged (ext4, Linux 6.18); the sum most likely wraps
  negative inside the kernel, which then reports a negative offset.
  SEEK_SET to the largest off_t also gives EINVAL (above the file
  system's maximum size); that case is arguably a permitted extra error
  and is not asserted.
- fseeko() inherits it: with output pending, fseeko(f, OFF_MAX, SEEK_END)
  flushes the data and then fails with EINVAL (glibc and musl).
- Comp: kernel. Kind: long-standing. Observe: XSH.lseek.011, XSH.fseek.016
  (disputed under Q-35, so INSPECT).
- Src: posixcheck run, glibc and musl alike.

O.4 **Unconnected `AF_UNIX` stream socket: EINVAL instead of ENOTCONN.**
- POSIX (`read`, `recv`, ERRORS): a read on a connection-mode socket that
  is not connected fails with ENOTCONN. Neither page lists EINVAL for it.
- Linux: the Unix-domain stream receive path returns EINVAL when the
  socket is not in the connected state; TCP sockets give ENOTCONN.
- Comp: kernel. Kind: long-standing. Observe: XSH.read.021 (AF_UNIX).
- Src: posixcheck run; checked with read() and recv() on AF_UNIX and
  AF_INET sockets.

O.5 **`wait()` does not clear a pending, blocked SIGCHLD.**
- POSIX (`wait`, DESCRIPTION): on an implementation that does not queue
  SIGCHLD, when SIGCHLD is blocked and wait() or waitpid() returns a
  child's status, a pending SIGCHLD is cleared unless another child's
  status is available.
- Linux: SIGCHLD is not queued (one pending instance), and it stays
  pending after wait() reaps the only child; a later sigwait() or
  unblock still delivers it.
- Evidence that both of wait()'s SIGCHLD paragraphs fail it: Linux does not
  queue SIGCHLD (two children exiting while it is blocked leave one
  acceptable SIGCHLD), and the leftover signal's si_pid is the reaped
  child's. The RATIONALE's wording (standard issue SI-8) does not change
  the requirement.
- Comp: kernel. Kind: long-standing. Observe: XSH.wait.019.
- Src: posixcheck run, glibc and musl alike.

O.6 **`waitid()` gives only 8 bits of the exit status.**
- POSIX (`_Exit`, `exit`, XBD `<signal.h>`): only the low 8 bits of the
  status are seen through wait() and waitpid(), but the full value is
  available from waitid() and in the siginfo_t for SIGCHLD (Austin Group
  defect 594, in POSIX.1-2008 TC2, 2016).
- Linux: the kernel keeps only the low 8 bits of the exit code, so
  `_exit(0x1234)` or `exit(0x1234)` gives si_status 0x34 from waitid()
  and in a SIGCHLD handler's siginfo_t.
- Comp: kernel. Kind: long-standing. Observe: XSH._Exit.002,
  XSH.exit.002, XSH.exit.012, XSH._Exit.014.
- Src: posixcheck run, glibc and musl alike.

O.7 **exec does not reset the initial thread's CPU-time clock.**
- POSIX (`exec`, TCT): the CPU-time clock of the new image's initial
  thread starts at zero.
- Linux: the thread that calls exec keeps its accumulated run time, so
  CLOCK_THREAD_CPUTIME_ID in the new image goes on from where the calling
  thread left it (500 ms used before exec reads as about 500 ms after).
- Comp: kernel. Kind: long-standing. Observe: XSH.exec.028.
- Src: posixcheck run, glibc and musl alike.

O.8 **musl: execvp() and execlp() do not run sh for a file in an unknown format.**
- POSIX (`exec`): where execve() would fail with ENOEXEC, execlp() and
  execvp() run the file with the command interpreter (sh), passing the
  arguments. Austin Group defect 1645 relaxed only what sh gets as its
  argv[0], not the fallback itself.
- musl 1.2.4: execvp() returns -1 with ENOEXEC. glibc runs /bin/sh.
- Comp: musl. Kind: long-standing (a no-fallback thread on the musl list
  dates from 2018). Observe: XSH.exec.044, XSH.exec.045 (and 046, 047,
  which need 044, are blocked).
- Src: posixcheck run; https://www.openwall.com/lists/musl/2018/03/09/2

O.9 **musl: sysconf(_SC_ARG_MAX) reports less than the real limit.**
- POSIX (`<limits.h>`, `sysconf`, `exec` E2BIG): the actual value of
  {ARG_MAX} is given by sysconf(), and exec fails with E2BIG when the
  arguments and environment exceed it.
- musl 1.2.4: sysconf(_SC_ARG_MAX) is the constant 131072 (and
  `<limits.h>` defines ARG_MAX), while Linux accepts about a quarter of
  the stack limit (2 MiB with an 8 MiB stack). So an exec with more than
  {ARG_MAX} bytes succeeds. glibc reports the kernel's limit.
- The fault is sysconf()'s value, not exec: exec enforces the real limit.
  The exec page itself does not say which value it compares against
  (standard question Q-9).
- Comp: musl. Kind: long-standing. Observe: XSH.exec.043.
- Src: posixcheck run.

O.10 **SA_RESETHAND does not clear SA_SIGINFO.**
- POSIX (`sigaction`, SA_RESETHAND): on entry to the handler the action is
  reset to SIG_DFL and SA_SIGINFO is cleared.
- Linux: the handler is reset to SIG_DFL but sa_flags are left alone, so
  sigaction() inside the handler reports SA_SIGINFO still set.
- Comp: kernel. Kind: long-standing. Observe: XSH.sigaction.023.
- Src: posixcheck run, glibc and musl alike.

O.11 **Withdrawn: sigqueue() not queuing signals below SIGRTMIN is not a deviation.**
- `sigaction()`, XSH 2.4.2 and `sigqueue()` read as if any signal with
  SA_SIGINFO set were queued, but `<signal.h>` supports the realtime
  behaviour only in SIGRTMIN-SIGRTMAX. The text conflicts (standard issue
  SI-24), so Linux's behaviour (realtime signals only) is a reasonable
  reading. XSH.sigaction.021 is `disputed`: its FAIL is logged as INSPECT.
- Src: posixcheck review.

O.12 **glibc: SIGRTMIN..SIGRTMAX holds one signal fewer than {RTSIG_MAX}.**
- POSIX (`<signal.h>`): the range SIGRTMIN through SIGRTMAX includes at
  least {RTSIG_MAX} signal numbers.
- glibc: `RTSIG_MAX` (a fixed 32) and sysconf(_SC_RTSIG_MAX) are 32, but
  of the kernel's 33 realtime signals (32..64) glibc keeps 32 and 33 for
  its own use, so SIGRTMIN is 34 and the range 34..64 has 31 numbers. musl
  reports 30 from sysconf() for its range of 30 (and leaves `RTSIG_MAX`
  undefined), which is consistent.
- Comp: glibc. Kind: long-standing. Observe: XBD.signal.h.006.
- Src: posixcheck run.

O.13 **sigaltstack() accepts SS_ONSTACK in ss_flags.**
- POSIX (`sigaltstack`, ERRORS): ss_flags with SS_ONSTACK fails with
  EINVAL. Issue 7 required EINVAL for any flag other than SS_DISABLE;
  Issue 8 (Defect 1187) narrowed that to SS_ONSTACK or invalid flags, so
  SS_ONSTACK is rejected under both.
- Linux: the kernel treats SS_ONSTACK like 0 (kept for old programs) and
  sets the stack; glibc passes the call through. musl checks the flags
  itself and fails with EINVAL, as required.
- Comp: kernel (glibc builds). Kind: long-standing (nonconforming since
  Issue 7 at least). Observe: XSH.sigaltstack.005.
- Src: posixcheck run.

O.14 **glibc: psignal() gives stderr a byte orientation.**
- POSIX (`psiginfo`): these functions shall not change the orientation of
  the standard error stream.
- glibc: after psignal() on an unoriented stderr, fwide(stderr, 0) is
  negative; psiginfo() leaves it unoriented, as does musl.
- Comp: glibc. Kind: long-standing. Observe: XSH.psiginfo.004.
- Src: posixcheck run.

O.15 **musl: psignal() with an empty message writes a leading ": ".**
- POSIX (`psiginfo`): the message and ": " are written only when the
  message is neither a null pointer nor the empty string.
- musl: psignal(sig, "") writes ": " before the description; NULL is
  right. glibc is right in both cases.
- Comp: musl. Kind: long-standing. Observe: XSH.psiginfo.002.
- Src: posixcheck run.

O.16 **MINSIGSTKSZ is too small for the signal frame on x86-64 with AVX-512.**
- POSIX (`sigaltstack`): MINSIGSTKSZ is the minimum stack size for a signal
  handler.
- Linux: glibc and musl define MINSIGSTKSZ as 2048, but on CPUs with
  AVX-512 the kernel's signal frame is larger, so a trivial handler on an
  alternate stack of exactly MINSIGSTKSZ bytes gets SIGSEGV. glibc 2.34
  and later give the run-time value through sysconf(_SC_MINSIGSTKSZ), and
  make MINSIGSTKSZ use it under _DYNAMIC_STACK_SIZE_SOURCE or _GNU_SOURCE,
  not in the strict build.
- Comp: kernel with glibc and musl. Kind: depends on the CPU. Observe:
  XSH.sigaltstack.010 (standard question Q-22).
- Src: posixcheck run (this machine has AVX-512).

O.17 **Tier 0: what the headers lack under the strict build.**
- POSIX (XBD 13, each header's DESCRIPTION): every listed symbolic
  constant is an object-like macro (XBD 3, "Symbolic Constant"; it may also
  be an enumeration constant, but must be a macro), every type, member and
  function is visible with the stated type.
- Linux: the generated Tier 0 checks (about 3,350, built from the user's
  copy; see README "Tier 0") fail 290 on glibc 2.39 and 229 on musl 1.2.4.
  Nearly all are Issue 8 additions not yet made, or made only under
  `_GNU_SOURCE`/`_DEFAULT_SOURCE`:
  - both: `<endian.h>` functions, `DT_*` and `struct posix_dent`,
    `posix_getdents`, `reclen_t`, `ALTMON_*`/`ABALTMON_*`, `*_CLOFORK`,
    `F_OWNER_*`/`struct f_owner_ex`, `FNM_IGNORECASE`, `FTW_XDEV`,
    `REG_MINIMAL`, the `*_l` gettext functions, `GETENTROPY_MAX`, `NSIG_MAX`,
    `SIG2STR_MAX`/`sig2str`/`str2sig`, the `*clock*` timed waits,
    `posix_tnode`, `ppoll`, `qsort_r`, `strlcpy`/`strlcat`/`wcslcpy`/`wcslcat`,
    `reallocarray`, `secure_getenv`, `ptsname_r`, `getentropy`,
    `getres[ug]id`/`setres[ug]id`, `getlocalename_l`, `dladdr`/`Dl_info_t`,
    `tm_gmtoff`/`tm_zone`, `posix_spawn_file_actions_add[f]chdir`, the
    `M_*l` and new `M_*` constants of `<math.h>`, the `_CS_POSIX_V8_*`,
    `_SC_V8_*`, `_SC_NSIG`, `_PC_FALLOC` names, the `O_*` names newly
    required in `<stdlib.h>`, `<sys/mman.h>` and `<unistd.h>`, the
    option constants that Issue 8 fixes at 202405L, `_POSIX_VERSION`,
    `_POSIX2_VERSION` and `_POSIX2_C_BIND` (both still say 200809L) and
    `_XOPEN_VERSION` (700, not 800), `SHM_FAILED` and `intptr_t` in
    `<sys/shm.h>`, `struct timespec` in `<poll.h>`.
  - glibc only: `PTHREAD_MUTEX_*`, `PTHREAD_PRIO_*` and `WRDE_*` are
    enumeration constants only, not macros; `PTHREAD_NULL`, `O_EXEC`,
    `O_SEARCH`, `O_TTY_INIT`, `F_OFD_*`, `SEEK_HOLE`/`SEEK_DATA`, `dup3`,
    `pipe2`, `accept4`, `_Fork`, `posix_close`, `mkostemp`, `memmem`,
    `ffsl`/`ffsll`, `tcgetwinsize`, `suseconds_t`, `TSVTX` (no longer
    XSI-only), `_XOPEN_NAME_MAX`/`_XOPEN_PATH_MAX`, `ino_t` from
    `<dirent.h>`, `wchar_t` from `<inttypes.h>` are hidden or missing;
    type mismatches: `lio_listio` (no `restrict`), `msghdr.msg_iovlen`
    and `msg_controllen` and `cmsghdr.cmsg_len` (`size_t`),
    `semid_ds.sem_nsems` (`unsigned long`), `utmpx.ut_tv` (a private
    structure on x86-64), `<ndbm.h>` `datum` (`char *dptr`, `int dsize`).
  - musl only: no `<ndbm.h>`; `<mqueue.h>` does not define the `O_*`
    flags; `<dirent.h>` does not define `size_t`; no `_XOPEN_SHM`.
- Comp: glibc, musl. Kind: Issue 8 lag (most), long-standing (the enum
  constants, the socket member types, `ndbm.h`). Observe: `T0.*`
  (generated; not in `known/`, since the IDs live only under `build/`).
- Src: posixcheck Tier 0 run, 2026-10-23.

O.18 **musl: an O_SEARCH descriptor still gets the search check.**
- POSIX (`fstatat`, `openat` and the other *at functions): when the
  descriptor was opened with O_SEARCH, the function does not check search
  permission on the directory; it does when the access mode is not
  O_SEARCH.
- Linux: musl defines O_SEARCH as O_PATH. The kernel checks search
  permission at each lookup through an O_PATH descriptor, so once the
  directory's mode denies search, fstatat(fd, "f", ...) fails with EACCES
  for an unprivileged process. glibc does not define O_SEARCH at all
  (1.7), so the test is UNRESOLVED there.
- Comp: kernel with musl. Kind: long-standing. Observe: XSH.fstatat.013,
  XSH.link.021, XSH.unlink.021, XSH.rename.027, XSH.mkdir.018, XSH.chmod.019, XSH.chown.021, XSH.access.018.
- Src: posixcheck run.

O.19 **Linux: link() of another user's file fails with EPERM (protected hard links).**
- POSIX (`link`): an implementation may require permission to access the
  existing file; when that permission is missing the error is EACCES.
  Nothing allows refusing a file the process may read.
- Linux: with `fs.protected_hardlinks=1` (the default on most
  distributions), an unprivileged process may link only files it owns or
  may both read and write; otherwise link() and linkat() fail with EPERM.
  Setting the sysctl to 0 restores the POSIX behaviour.
- Comp: kernel (configuration). Kind: deliberate (security hardening).
  Observe: XSH.link.019 (disputed: the standard does not say what access
  may be required, Q-24).
- Src: posixcheck run; kernel documentation of `protected_hardlinks`.

O.20 **Linux: rename() with a final dot or dot-dot gives EBUSY, not EINVAL.**
- POSIX (`rename`, ERRORS): [EINVAL] when either pathname's final
  component is dot or dot-dot. [EBUSY] is only for a directory in use by
  the system or another process.
- Linux: rename("d/.", "x"), rename("d/..", "x") and rename("f", "d/.")
  fail with EBUSY even when d is not in use (not the working directory of
  any process, not a mount point).
- Comp: kernel. Kind: long-standing. Observe: XSH.rename.012.
- Src: posixcheck run.

O.21 **Linux: rename() of a directory to "new/" succeeds.**
- POSIX (`rename`, DESCRIPTION, CX): if new does not resolve to an
  existing directory and ends with trailing slashes, rename() shall fail.
  XBD 4.16 allows interfaces to constrain trailing slashes further, and
  its footnote names rename() as one that does.
- Linux: rename("d", "nd/") with d a directory and nd not existing
  succeeds and creates nd (the kernel accepts trailing slashes whenever
  the source is a directory). Non-directories are refused with ENOTDIR,
  as required.
- Comp: kernel. Kind: long-standing. Observe: XSH.rename.014.
- Src: posixcheck run.

O.22 **musl: futimens(AT_FDCWD, t) gives EFAULT, not EBADF.**
- POSIX (`futimens`, ERRORS): [EBADF] when fd is not a valid file
  descriptor; AT_FDCWD is not one.
- musl 1.2.4: futimens() passes the descriptor to utimensat() with a null
  path, so the kernel takes AT_FDCWD and reports EFAULT for the path.
  glibc reports EBADF.
- Comp: musl. Kind: long-standing. Observe: XSH.futimens.024.
- Src: posixcheck run.

O.23 **glibc: psiginfo() does not set stderr's error indicator on failure.**
- POSIX (`psiginfo`, DESCRIPTION): on error psiginfo() and psignal() set
  the error indicator of the stream stderr points to, and set errno.
- glibc 2.39: with descriptor 2 closed, psiginfo() sets errno (EBADF) but
  ferror(stderr) stays 0; psignal() sets both. musl sets both.
- Comp: glibc. Kind: long-standing. Observe: XSH.psiginfo.007.
- Src: posixcheck run.

O.24 **glibc: asynchronous I/O outstanding at _exit() is dropped (disputed).**
- POSIX (`_exit`, Consequences of Process Termination): operations not
  canceled complete as if _exit() had not occurred; which operations may
  be canceled is implementation-defined.
- glibc 2.39: aio is done by helper threads inside the process; _exit()
  ends them with it, so a 64 KiB aio_write() started just before _exit()
  never reaches the file (0 bytes every time). Nothing outside the process
  can tell this from a cancellation, and glibc does not document it as
  one. musl usually completes 64 KiB but truncates larger writes the same
  way.
- Comp: glibc (and musl for large writes). Kind: design (user-space aio).
  Observe: XSH._Exit.019 (disputed, Q-32).
- Src: posixcheck run.

O.25 **glibc: asynchronous I/O outstanding at close() fails with EBADF.**
- POSIX (`close`, DESCRIPTION): operations that are not canceled complete
  as if close() had blocked until they did.
- glibc 2.39: with two aio_write() calls queued on a full pipe, close()
  returns and one of them (glibc serializes per descriptor) fails with
  EBADF, which is neither completion nor cancellation.
- Comp: glibc. Kind: design (user-space aio). Observe: XSH.close.014.
- Src: posixcheck run.

O.30 **musl: close() with two aio writes queued never returns.**
- POSIX (`close`, DESCRIPTION): close() may cancel outstanding operations
  or wait for them, and returns.
- musl 1.2.4: with two aio_write() calls queued on a full pipe (a thread
  draining it), the first completes, the second's thread waits on an
  internal lock, and close() (in aio_cancel()) waits for it for ever.
  With a single queued write musl cancels it (ECANCELED); a single
  200000-byte write instead reports success with a short count (65536).
- Comp: musl. Kind: long-standing. Observe: XSH.close.014.
- Src: posixcheck run.

O.31 **glibc: sysconf(_SC_STREAM_MAX) reports a limit that is not enforced.**
- POSIX (`fopen`, `fdopen`, ERRORS; XBD `<limits.h>`): with {STREAM_MAX}
  streams open, fopen() and fdopen() shall fail with EMFILE, and sysconf()
  gives the value a system actually supports (or -1 when it is
  indeterminate, as musl reports).
- glibc 2.39: sysconf(_SC_STREAM_MAX) returns FOPEN_MAX (16), yet streams
  are limited only by descriptors and memory, so the 17th fopen() succeeds.
- Comp: glibc. Kind: long-standing. Observe: XSH.fopen.032, XSH.fdopen.009.
- Src: posixcheck run.

O.32 **glibc: fdopen() ignores 'e'.**
- POSIX (`fdopen`, DESCRIPTION; Issue 8): with 'e' in mode, fdopen() sets
  FD_CLOEXEC on the descriptor.
- glibc 2.39: fdopen(fd, "re") returns a stream and leaves FD_CLOEXEC clear
  (its fopen() does handle 'e'). musl sets the flag.
- Comp: glibc. Kind: Issue 8 feature not implemented. Observe:
  XSH.fdopen.005.
- Src: posixcheck run.

O.33 **glibc: fseek() after fflush() does not set the descriptor's offset.**
- POSIX (`fseek`, DESCRIPTION, CX): when the most recent operation on a
  stream (other than ftell()) is fflush(), fseek() adjusts the offset of
  the underlying open file description to the new position.
- glibc 2.39: fflush() of an input stream sets the offset to the stream's
  position (1 after one read) but keeps the buffer; the following
  fseek(f, 7, SEEK_SET) lands inside the buffer and makes no lseek(), so
  the offset stays at the end of the read-ahead (10 for a 10-byte file,
  4096 for a large one). musl gives 7. Output streams are right.
- Comp: glibc. Kind: long-standing. Observe: XSH.fseek.004 (disputed, Q-40:
  the offset is read through the descriptor, a second handle).
- Src: posixcheck run.

O.34 **fmemopen(): null byte at open and at a full buffer.**
- POSIX: `fmemopen`, DESCRIPTION: a null byte is written at the start of a "w" buffer, none over the data of a full buffer, and a zero-size buffer is left alone.
- Both: after fmemopen(buf, n, "w") the buffer is unchanged until the first write (XSH.fmemopen.004); fmemopen(buf, 0, "w+") changes buf[0] to 0 (006); a write that fills a "w" buffer leaves a null over the last data byte (021).
- Comp: glibc, musl. Kind: long-standing. Observe: XSH.fmemopen.004, XSH.fmemopen.006, XSH.fmemopen.021.
- Src: posixcheck run.

O.35 **glibc: an fmemopen() stream already has an orientation.**
- POSIX: XSH 2.5: a stream has no orientation until its first operation, so fwide(f, 0) gives 0 on a new memory stream.
- glibc 2.39: fwide(f, 0) right after fmemopen("r") returns -1.
- Comp: glibc. Kind: long-standing. Observe: XSH.fmemopen.023.
- Src: posixcheck run.

O.36 **glibc: setvbuf() or unbuffered output on open_memstream() kills the process.**
- POSIX: `open_memstream`, XSH 2.5: a dynamic memory stream works with a buffer from setvbuf() and when unbuffered.
- glibc 2.39: XSH.open_memstream.015 is killed by signal 11.
- Comp: glibc. Kind: long-standing. Observe: XSH.open_memstream.015.
- Src: posixcheck run.

O.37 **musl: open_memstream() size after a seek and at close.**
- POSIX: `open_memstream`, DESCRIPTION: the size is the smaller of the current position and the buffer length after fflush() or fclose().
- musl: after fseek(2) and fflush() the size is 5, expected 2 (.005); after fclose() it is 8, expected 14 (.007).
- Comp: musl. Kind: long-standing. Observe: XSH.open_memstream.005, XSH.open_memstream.007.
- Src: posixcheck run.

O.38 **glibc: fgets() and getdelim() after a non-blocking read.**
- POSIX: `fgets`, `getdelim`: a read that cannot complete gives a null pointer (-1) with the error indicator set; a set error indicator does not stop getdelim() reading more input.
- glibc: fgets() returns s after reading "ab" with nothing more available (.006); with the error indicator set and "ab:" in the pipe, getdelim() returns -1 with errno 0 (.012).
- Comp: glibc. Kind: long-standing. Observe: XSH.fgets.006, XSH.getdelim.012.
- Src: posixcheck run.

O.39 **musl: fgetc() and fputc() leave errno 0 on a stream open the wrong way.**
- POSIX: `fgetc`, `fputc`, ERRORS: [EBADF] when the stream is not open for reading (writing).
- musl: both fail with errno 0.
- Comp: musl. Kind: long-standing. Observe: XSH.fgetc.005, XSH.fputc.007.
- Src: posixcheck run.

O.40 **freopen(): descriptor order, FD_CLOEXEC and a null pathname.**
- POSIX: `freopen`: the old descriptor is closed first, so the new one is the lowest available and the call can succeed when every descriptor is in use; 'e' controls FD_CLOEXEC; with a null pathname "w" truncates and the indicators are cleared.
- Both: the descriptor is 6, not the lowest (4) (.011); with all descriptors in use freopen() fails (.022). glibc: FD_CLOEXEC is set after a reopen with "r" (.008). musl: freopen(NULL, "w") leaves the file 3 bytes long (.013) and the end-of-file indicator set after freopen(NULL, "r") (.014).
- Comp: glibc, musl. Kind: long-standing. Observe: XSH.freopen.011, XSH.freopen.022, XSH.freopen.008, XSH.freopen.013, XSH.freopen.014.
- Src: posixcheck run.

O.41 **fscanf(): invalid prefixes and a suppressed first conversion.**
- POSIX: `fscanf`, DESCRIPTION and RETURN VALUE: a sequence that is only a prefix of valid input is a matching failure; EOF is returned only if input ends before the first conversion completes (a suppressed one counts).
- glibc: sscanf("100ergs", "%f%n") returns 1 (.014); sscanf("0xg", "%x%n") returns 1 (.015). Both: sscanf("1", "%*d%d") returns -1, expected 0 (.043).
- Comp: glibc, musl. Kind: long-standing. Observe: XSH.fscanf.014, XSH.fscanf.015, XSH.fscanf.043.
- Src: posixcheck run.

O.42 **localtime(): a TZ rule whose DST covers the whole year.**
- POSIX: XBD 8.3 (TZ): "0/0,J365/25" puts DST in effect for the whole year.
- Both: with TZ=PCS5PCD,0/0,J365/25 localtime(1609470000) gives 2020-12-31 22:00:00, a standard-time value, where DST (23:00:00) is expected.
- Comp: glibc, musl. Kind: long-standing. Observe: XSH.localtime.006.
- Src: posixcheck run.

O.43 **mlock(): a failing call locks the pages before the bad one.**
- POSIX: `mlock`, ERRORS: on [ENOMEM] the memory is not locked.
- Both: after the failed call the first page is locked.
- Comp: glibc, musl. Kind: long-standing. Observe: XSH.mlock.004.
- Src: posixcheck run.

O.44 **glibc: cancellation cleanup state and an unusable robust mutex.**
- POSIX: XSH 2.9.5.3: cleanup handlers run with cancelability disabled. `pthread_mutex_lock`: an unusable robust mutex gives ENOTRECOVERABLE to every later lock.
- glibc: in the handler the state is PTHREAD_CANCEL_ENABLE (pthread_cancel.005); the lock after a trylock of an unusable mutex has not returned after 10 s (pthread_mutex_lock.014).
- Comp: glibc. Kind: long-standing. Observe: XSH.pthread_cancel.005, XSH.pthread_mutex_lock.014.
- Src: posixcheck run.

O.45 **musl: pthread_exit() in a fork() child's last thread hangs.**
- POSIX: `pthread_exit`: the process exits with status 0 after the last thread terminates.
- musl: XSH.pthread_exit.006 (a fork() child, no exec) times out after 20 s.
- Comp: musl. Kind: long-standing. Observe: XSH.pthread_exit.006.
- Src: posixcheck run.

O.46 **glibc: readlink() with bufsize 0 fails.**
- POSIX: `readlink`, RATIONALE: a bufsize of zero represents historical practice; no error is listed for it.
- glibc: readlink() into 0 bytes returns -1 (the kernel's EINVAL). musl returns 0.
- Comp: glibc. Kind: long-standing. Observe: XSH.readlink.003.
- Src: posixcheck run.

O.47 **seteuid(), setegid(), setregid(): privilege checks.**
- POSIX: `seteuid`, `setegid`, ERRORS: [EPERM] unless the ID equals the real or saved ID, and [EINVAL] for an invalid ID. `setregid`: the saved set-group-ID may become the real ID.
- Both: setegid()/seteuid() of the current effective ID (neither real nor saved) return 0 (.004); setregid(saved, -1) fails with EPERM (.006). musl: setegid()/seteuid() of (id_t)-1 return 0 (.005).
- Comp: glibc, musl. Kind: long-standing. Observe: XSH.seteuid.004, XSH.setegid.004, XSH.setregid.006, XSH.seteuid.005, XSH.setegid.005.
- Src: posixcheck run.

O.48 **strftime(): padding, flags, tzset, errno, signs, %s.**
- POSIX: `strftime`: %C is two digits; the flags and width apply to %Y; the call acts as if tzset() were called; ERANGE when the result does not fit; a successful call leaves errno alone; %Y has no sign without '+'; %s is mktime() of local time.
- glibc: %C gives "2" (.002); "%+4Y" is copied out unchanged (.007). Both: tzname[0] is not updated (.012); a result needing exactly maxsize bytes leaves errno 0 (glibc) or EINVAL (musl) instead of ERANGE (.014). musl: %Y of 12345 gives "+12345" (.008); %s is 3600 s off (.009); a success changes errno (.013).
- Comp: glibc, musl. Kind: long-standing. Observe: XSH.strftime.002, XSH.strftime.007, XSH.strftime.012, XSH.strftime.014, XSH.strftime.008, XSH.strftime.009, XSH.strftime.013.
- Src: posixcheck run.

O.49 **glibc: psignal() and strsignal() differ for a real-time signal.**
- POSIX: `strsignal`, `psignal`: both give the same description for a signal.
- glibc: for SIGRTMIN psignal() writes "Unknown signal 34", strsignal() gives "Real-time signal 0".
- Comp: glibc. Kind: long-standing. Observe: XSH.strsignal.002.
- Src: posixcheck run.

O.50 **symlink() with an empty first argument.**
- POSIX: `symlink`: path1 is stored as given; no error names an empty one.
- Both: symlink("", "e") fails with ENOENT.
- Comp: glibc, musl. Kind: long-standing. Observe: XSH.symlink.003.
- Src: posixcheck run.

O.51 **sysconf(): values that disagree with the headers and minima.**
- POSIX: `sysconf`; XBD <unistd.h>, <limits.h>: an option macro of a supported option has a sysconf() value other than -1; {HOST_NAME_MAX} is at least 255; _SC_VERSION is 202405 and supported options give 202405 or -1; _SC_ARG_MAX is not changed by setrlimit() of other resources.
- glibc: _SC_THREAD_ROBUST_PRIO_INHERIT is EINVAL (.001) and -1 although the macro is 200809 (.004); _SC_HOST_NAME_MAX is 64 (.002); _SC_ARG_MAX goes from 2097152 to 4194304 after setrlimit() (.009). Both: _SC_VERSION is 200809 (.005); _SC_ADVISORY_INFO is 200809 (.007).
- Comp: glibc, musl. Kind: long-standing. Observe: XSH.sysconf.001, XSH.sysconf.002, XSH.sysconf.004, XSH.sysconf.009, XSH.sysconf.005, XSH.sysconf.007.
- Src: posixcheck run.

O.52 **musl: shm_open() descriptor flags and %lc of a null wide character.**
- POSIX: `shm_open`: no O_NONBLOCK unless asked. `fprintf` %lc: a null wide character is written as a null byte.
- musl: F_GETFL shows O_NONBLOCK after shm_open() (shm_open.006); "[%lc]" with L'\0' gives "[]" (fprintf.037).
- Comp: musl. Kind: long-standing. Observe: XSH.shm_open.006, XSH.fprintf.037.
- Src: posixcheck run.

O.53 **Names and declarations absent under the strict build.**
- POSIX: XSH and XBD name `realpath`, `asprintf`, `vasprintf`, `secure_getenv`, `pthread_cond_clockwait`, `PTHREAD_NULL`, and the `tm_gmtoff` and `tm_zone` members of `struct tm`.
- Both libraries, built with `_POSIX_C_SOURCE=202405L` and `-Werror`: the first five are not declared (implicit declaration), `struct tm` has no `tm_zone`/`tm_gmtoff`, and glibc has no `PTHREAD_NULL`. The tests that need them end UNINITIATED (build failure). See 6.3 and 11.1.
- Comp: glibc, musl. Kind: feature-test. Observe: XSH.realpath.001-.010, XSH.fprintf.055, .056, XSH.vfprintf.008, XSH.getenv.004, .005, XSH.pthread_cond_wait.014-.016, XSH.gmtime.003, XSH.localtime.012, XSH.pthread_equal.003 (glibc).
- Src: posixcheck run.

O.26 **glibc: fflush() of an input stream with pushback sets the wrong offset.**
- POSIX (`fflush`, DESCRIPTION, CX): for a seekable input stream not at
  end of file, the file offset is set to the stream's position, and
  pushed-back bytes are discarded without changing the offset further.
- glibc 2.39: with a 1 MiB file read twice with fgetc() and then
  ungetc('x') (stream position 1), fflush() sets the offset to the end of
  the read-ahead minus the pushback count (4095), ignoring the unread
  buffered bytes; the next fgetc() returns '2'. It happens only when the
  pushed-back byte differs from the one read (ungetc('1') gives 1).
  Without pushback the offset is right. musl gives 1.
- Comp: glibc. Kind: long-standing. Observe: XSH.fflush.005.
- Src: posixcheck run.

O.27 **glibc: fclose() of an input stream does not set the file offset.**
- POSIX (`fclose`, DESCRIPTION, CX): if the stream is the active handle and
  the file is seekable and not at EOF, the offset of the open file
  description is set to the stream's position.
- glibc 2.39: after one fgetc() on a 1 MiB file, a duplicate descriptor
  sees offset 4096 (where read-ahead left it) after fclose(), for "r" and
  for "r+" streams, with or without a pushed-back byte. musl sets 1.
  (glibc bugzilla 12724 is believed to be the upstream record.)
- Comp: glibc. Kind: long-standing. Observe: XSH.fclose.003, .010, .011.
- Src: posixcheck run.

O.28 **musl: fflush() of a full fmemopen() stream succeeds silently.**
- POSIX (`fflush`, ERRORS): [ENOSPC] when there is no free space in the
  buffer used by fmemopen().
- musl 1.2.4: with 10 bytes buffered for a 4-byte memory buffer, fflush()
  and fclose() return 0 and the excess is lost.
- Comp: musl. Kind: long-standing. Observe: XSH.fflush.013, XSH.fclose.012.
- Src: posixcheck run.

O.29 **glibc: fflush() of a full fmemopen() stream leaves errno unset.**
- POSIX (`fflush`, `fclose`, RETURN VALUE and ERRORS): on failure errno is
  set; [ENOSPC] when there is no free space in the buffer used by
  fmemopen().
- glibc 2.39: with 10 bytes buffered (16 KiB stream buffer) for a 4-byte
  memory buffer, fflush() and fclose() return EOF but errno stays 0. (With
  a stream buffer under 128 bytes glibc writes through at once, and the
  fputs() fails instead, also without errno.)
- Comp: glibc. Kind: long-standing. Observe: XSH.fflush.013, XSH.fclose.012.
- Src: posixcheck run.

## Things the strict build changes

Tests are compiled with `-D_POSIX_C_SOURCE=202405L` (or
`-D_XOPEN_SOURCE=800`) and nothing else. glibc does not know these values
(entry 11.1) and exposes only its POSIX.1-2008 set, so an Issue 8
interface that glibc does implement can still fail to compile in a test.
That is a real conformance failure (a conforming application cannot see
the interface), but it is a header problem, not a missing behaviour. The
log note says which; to see whether the behaviour itself works, rerun the
module with a compiler wrapper that adds `-D_GNU_SOURCE` (for example a
two-line script `exec cc -D_GNU_SOURCE "$@"` passed as `--cc`), on a
reference system only.

---

Reference targets: Linux kernel (Ubuntu 24.04 era, 6.8), glibc 2.39, musl 1.2.4, dash 0.5.12, GNU coreutils 9.4.
Compiled 2026-10-23 from documentation, release notes, man pages and published test results only.
No test-suite source code was read.

## How to read this

Each item gives:

- **POSIX:** what POSIX.1-2024 requires, in paraphrase. Where I checked the wording, I used the personal copy of the POSIX.1-2024 HTML edition.
- **Linux:** what the reference system does instead.
- **Comp:** the component at fault: `kernel`, `glibc`, `musl`, `dash` or `coreutils`.
- **Kind:** one of three:
  - *deliberate/long-standing*: a design choice or historical behaviour.
  - *missing-2024*: an Issue 8 addition that is absent or exposed only under extension macros.
  - *bug*: a defect.
- **Fixed:** the version that fixed it, where one exists.
- **Observe:** how a test would see it.
- **Src:** a URL I opened. "Unconfirmed" marks claims I could not check against a primary source, or that may be out of date.

## 0. Things that look like deviations but are NOT (avoid false positives)

0.1 **ENOTSUP == EOPNOTSUPP on Linux.**
- POSIX.1-2024 `<errno.h>` explicitly says each "may be the same value as" the other. Austin Group interpretation 1003.1-2001 #050 allowed this in Issue 7.
- The Linux errno(3) man page still says they "should be distinct". That statement is stale.
- A test must not require them to differ.
- Src: XBD `<errno.h>` (personal copy of the standard); https://raw.githubusercontent.com/mkerrisk/man-pages/master/man3/errno.3

0.2 **EAGAIN == EWOULDBLOCK.** POSIX explicitly permits this. Src: XBD `<errno.h>` (personal copy of the standard).

0.3 **`link()` does not follow a symlink in `oldpath`.**
- Since POSIX.1-2008, whether `link()` dereferences a symlink is implementation-defined. Linux never follows.
- For a deterministic test, use `linkat()` with or without `AT_SYMLINK_FOLLOW`.
- Src: https://raw.githubusercontent.com/mkerrisk/man-pages/master/man2/link.2 (NOTES)

0.4 **`read()` on a directory fails with EISDIR.** POSIX lists [EISDIR] for an implementation that does not allow directories to be read with `read()`. This is conforming.

0.5 **`mq` priorities: `sysconf(_SC_MQ_PRIO_MAX)` returns 32768.** That is at least the minimum of 32, so it is fine. Src: https://raw.githubusercontent.com/mkerrisk/man-pages/master/man7/mq_overview.7

0.6 **`fork()` child directory streams do not share position with the parent.** POSIX says they "may" share. Src: fork.2 man page (same mirror).

0.7 **`execve()` with SIGCHLD set to SIG_IGN keeps it ignored.** POSIX allows either behaviour. Src: execve.2.

0.8 **Many `<limits.h>` runtime-invariant values are absent:**
- `OPEN_MAX`, `CHILD_MAX`, `LINK_MAX`, `SYMLINK_MAX`, `TIMER_MAX`, `STREAM_MAX`, `ATEXIT_MAX`, `PTHREAD_THREADS_MAX` and others.
- The values vary per process or filesystem, so they are left undefined, which is permitted. `sysconf()`/`pathconf()` may return -1 without setting errno (indeterminate). Also permitted.
-

0.9 **`select()` with fd > 1023.** `FD_SETSIZE` is 1024 in glibc. POSIX allows an implementation limit. Src: select.2 BUGS.

0.10 **`getsid()` never fails with EPERM on Linux.** POSIX says "may fail". Src: getsid.2.

0.11 **`access(X_OK)` as root succeeds only if at least one execute bit is set.** POSIX permits this either way. Src: access.2.

0.12 **The pathname "" fails with ENOENT.** This is conforming. Src: path_resolution.7.

0.13 **`/bin/sh` is dash on Ubuntu.**
- POSIX does not fix the path of `sh`; use `getconf PATH`.
- dash has real Issue 8 gaps, though. See section 11.

---

## 1. File I/O and file descriptors

1.1 **O_CLOFORK / FD_CLOFORK / F_DUPFD_CLOFORK / SOCK_CLOFORK / MSG_CMSG_CLOFORK absent.**
- POSIX: Issue 8 adds close-on-fork for `open`, `fcntl`, `dup3`, `socket`, `accept4`, `pipe2` and recvmsg cmsg.
- Linux: the kernel has no close-on-fork. A patch series ("Implement close-on-fork", Nate Karstens, v2 May 2020) was not merged. Neither libc defines the macros.
- Observe: these do not compile:
  - `#ifdef O_CLOFORK`

  - https://lwn.net/Articles/820652

1.2 **`close()` interrupted by a signal returns EINTR with the descriptor already closed.**
- POSIX (Issue 8 `close`): if `close()` is interrupted, it must do one of three things:
  - return -1/EINTR **with fildes left open**, or
  - return -1/EINPROGRESS with fildes closed, or
  - return 0.
- EINTR is forbidden if `POSIX_CLOSE_RESTART` is 0.
- Linux: the kernel always frees the fd early. If a flush is interrupted, the kernel reports EINTR anyway, and glibc passes it through. The result is EINTR with the fd *closed*, which Issue 8 forbids. Retrying `close()` is wrong on Linux.
- musl: maps -EINTR to 0, which conforms.
- Comp: kernel + glibc. Kind: deliberate/long-standing. Fixed: no.
- Observe: hard to trigger; it needs a filesystem or device whose flush blocks interruptibly, such as NFS or a tape. If triggered, `fcntl(fd, F_GETFD)` after EINTR returns EBADF.

  - https://raw.githubusercontent.com/mkerrisk/man-pages/master/man2/close.2
  - https://lwn.net/Articles/576478/
  - musl `close.c`: https://git.musl-libc.org/cgit/musl/tree/src/unistd/close.c

1.3 **`posix_close()` / `POSIX_CLOSE_RESTART` missing in glibc.**
- POSIX: new in Issue 8, in `<unistd.h>`.
- musl: has it since 0.9.15 (Dec 2013) as a plain wrapper for `close`. Upstream log: "Linux does not support EINTR semantics".
- Comp: glibc. Kind: missing-2024.

  - https://git.musl-libc.org/cgit/musl/log/src/unistd/posix_close.c?showmsg=1

1.4 **`pwrite()` on an O_APPEND descriptor appends and ignores `offset`.**
- POSIX: `pwrite` writes at `offset` "regardless of whether O_APPEND is set".
- Linux: if the file was opened with O_APPEND, `pwrite()` appends to the end of the file whatever `offset` is.
- Comp: kernel (also glibc, which passes through). musl ≤1.2.5 is the same.
- Kind: deliberate/long-standing.
- Fixed:
  - musl **1.2.6** ("pwrite now handles O_APPEND correctly, reports error if it can't").
  - Kernel: `pwritev2(RWF_NOAPPEND)` exists in newer kernels (6.9?). Unconfirmed.
- Observe: open with `O_WRONLY|O_APPEND` on a file of 10 bytes, call `pwrite(fd,"X",1,0)`, then check that byte 0 changed. On Linux the file grows to 11 bytes instead.

  - https://raw.githubusercontent.com/mkerrisk/man-pages/master/man2/pread.2 (BUGS)
  - https://raw.githubusercontent.com/kraj/musl/master/WHATSNEW (1.2.6)

1.5 **`posix_getdents()` / `struct posix_dent` / `reclen_t` missing.**
- POSIX: new in Issue 8, in `<dirent.h>`.
- musl: added in **1.2.6**, so it is absent in 1.2.4.
- Comp: glibc, musl<1.2.6. Kind: missing-2024.

  - musl WHATSNEW 1.2.6

1.6 **`<dirent.h>` DT_* constants are extension-only in glibc, and DT_MQ / DT_SEM / DT_SHM / DT_TMO are absent everywhere.**
- glibc exposes `DT_REG` and the others only under `_DEFAULT_SOURCE`. musl exposes them under POSIX 2024.
- Comp: glibc (macro gating); glibc + musl for the four absent ones. Kind: missing-2024.
-

1.7 **O_SEARCH, O_EXEC, O_TTY_INIT undefined in glibc.**
- POSIX: `<fcntl.h>` shall define O_SEARCH and O_EXEC. O_TTY_INIT is also listed.
- glibc: undeclared. glibc only offers Linux's `O_PATH`, which has different semantics.
- musl: provides O_SEARCH and O_EXEC (both mapped to O_PATH) since 0.9.5/0.9.6, and O_TTY_INIT since 1.1.23.
- Comp: glibc. Kind: deliberate (glibc).
- Observe: these do not compile under glibc:
  - `#ifdef O_SEARCH`
  - `fexecve` via an `O_EXEC` fd

  - musl WHATSNEW

1.8 **F_OFD_* locks and `F_SETOWN_EX` / `struct f_owner_ex` are extension-only in glibc.**
- Issue 8 standardizes them.
- glibc exposes them only with `_GNU_SOURCE`.
- `f_owner_ex` / `F_OWNER_PID` / `F_OWNER_PGRP` are extension-only in musl too.
- Comp: glibc, musl. Kind: missing-2024 (namespace).
-

- Unconfirmed on x86-64, where glibc uses `short`. It may be architecture-specific.
- Comp: glibc.

1.10 **`<unistd.h>` and `<stdlib.h>` do not define the O_* flags that Issue 8 requires there.**
- POSIX 2024 requires these headers to define some O_* flags:
  - `<unistd.h>`: O_CLOEXEC and O_CLOFORK, for `pipe2`/`dup3`.
  - `<stdlib.h>`: O_APPEND, O_CLOEXEC, O_SYNC, O_DSYNC, O_RSYNC, O_NOCTTY and O_RDWR, for `mkostemp`/`posix_openpt`.
- Both libcs leave them undeclared unless `<fcntl.h>` is included.
- Comp: glibc, musl. Kind: missing-2024.
- Observe: compile a TU that includes only `<stdlib.h>` and uses `O_CLOEXEC`.

1.11 **`<semaphore.h>` (glibc) and `<sys/mman.h>` (both) do not define O_CREAT, O_EXCL and the other O_* flags.**
- Issue 8 requires `sem_open`/`shm_open` constants in those headers.
- The `<semaphore.h>` gap is glibc only; musl is fine there. The `<sys/mman.h>` gap is in both.
- Comp: glibc, musl. Src:

1.12 **SEEK_DATA / SEEK_HOLE are extension-only in glibc.**
- Standardized in Issue 8.
- glibc needs `_GNU_SOURCE`. musl exposes them since 1.2.3.
- Comp: glibc. Src:
  - musl WHATSNEW 1.2.3

1.13 **`select()` ignores fds in the sets above the highest open fd, instead of failing with EBADF.**
- POSIX: [EBADF] if any set contains an fd that is not open.
- Linux: fds greater than the highest currently open fd are silently ignored. A closed fd below the maximum still gives EBADF.
- Comp: kernel. Kind: deliberate.
- Observe: with fds 0-2 open, `FD_SET(100, &r)` and `select(101, &r, ...)` with a zero timeout returns 0, not -1/EBADF.
- Src: https://raw.githubusercontent.com/mkerrisk/man-pages/master/man2/select.2 (BUGS)

1.14 **`select()` may modify `timeout` when interrupted by a signal (EINTR).**
- Linux man page: "This is not permitted by POSIX.1."
- The raw `pselect` syscall does the same, but the glibc wrapper hides it.
- Comp: kernel. Observe: compare the timeval before and after EINTR.
- Src: select.2 BUGS (as 1.13).

1.15 **`poll()` reports POLLHUP together with POLLOUT.**
- POSIX `poll()`: POLLHUP "and POLLOUT are mutually-exclusive".
- Linux:
- Comp: kernel. Kind: long-standing.

  - local `functions/poll.html`

1.16 **`ppoll()` edge cases.**
- In other words, ppoll reports readiness or POLLNVAL caused by actions taken inside the signal handler that interrupted it, rather than returning -1/EINTR.
- Comp: kernel.
-

1.17 **`unlink()` on a directory fails with EISDIR, not EPERM.**
- POSIX: [EPERM] when the file is a directory and the implementation does not allow `unlink` of directories.
- Linux: EISDIR, "the non-POSIX value returned by Linux since 2.1.132".
- Comp: kernel. Kind: deliberate.
- Observe: `mkdir d; unlink("d")` gives errno EISDIR (XSH.unlink.005; unlinkat() without AT_REMOVEDIR, XSH.unlink.010).
- Src: https://raw.githubusercontent.com/mkerrisk/man-pages/master/man2/unlink.2

1.18 **`futimens(fd, NULL)` permission check.**
- POSIX: a process with *write access to the file* may set both timestamps to now.
- Linux: `futimens()` checks whether the *file descriptor's access mode* allows writing. A non-owner who has write permission but opened O_RDONLY gets EACCES.
- Comp: kernel. Kind: long-standing.
- Observed here: on Linux 6.18 a non-owner with write permission and an O_RDONLY descriptor succeeds (XSH.futimens.021 passes), so the man page's BUGS note does not hold for this kernel.
- Src: https://raw.githubusercontent.com/mkerrisk/man-pages/master/man2/utimensat.2 (BUGS)

1.19 **mmap partial page beyond EOF keeps written data.**
- POSIX: the partial page beyond EOF is zero-filled, and modifications beyond EOF are never written to the object.
- Linux: data written there stays in the page cache, so later mappings may see it. This affects tmpfs and `shm_open` objects in particular.
- Comp: kernel. Observe: map a 100-byte file, write at offset 200 in the mapping, unmap, re-map, read offset 200: nonzero.
- Src: https://raw.githubusercontent.com/mkerrisk/man-pages/master/man2/mmap.2 (BUGS)

1.20 **`fcntl(F_SETFL)` cannot change O_DSYNC/O_SYNC; the change is silently ignored.**
- Comp: kernel. Observe: set O_SYNC via F_SETFL, then F_GETFL does not show it.
- Whether this is a deviation depends on reading: POSIX lets F_SETFL set "file status flags", and O_SYNC is one of them.
- Src: https://raw.githubusercontent.com/mkerrisk/man-pages/master/man2/fcntl.2 (BUGS)

1.21 **`fcntl` F_SETLKW deadlock detection gives false negatives and false positives.**
- Dependency search is limited to 10 steps, and there are spurious EDEADLK errors with CLONE_FILES.
- POSIX expects EDEADLK when a deadlock is detected (the requirement is "may").
- Comp: kernel. Observe: an 11-process lock cycle is not detected.
- Src: fcntl.2 BUGS.

1.22 **`HOST_NAME_MAX` is 64 in glibc, below `_POSIX_HOST_NAME_MAX` (255).**
- POSIX: if defined, `HOST_NAME_MAX` must be ≥ `_POSIX_HOST_NAME_MAX`, which is 255.
- glibc: 64, matching the kernel's `__NEW_UTS_LEN`. musl: 255, which passes.
- Comp: glibc (and kernel limit). Kind: deliberate.
- Observe: `#if HOST_NAME_MAX < _POSIX_HOST_NAME_MAX`, or `sethostname` with 65 bytes fails with EINVAL.

  - local `basedefs/limits.h.html`

1.23 **New Issue 8 `pathconf`/`sysconf`/`confstr` names are missing.**
- These are undeclared in both libcs:
  - `_PC_FALLOC`, `_PC_TEXTDOMAIN_MAX`, `_PC_TIMESTAMP_RESOLUTION`
  - all `_CS_POSIX_V8_*` and `_CS_V8_ENV`
  - `_POSIX_FALLOC`
- Comp: glibc, musl. Kind: missing-2024.
-

1.24 **`NSIG_MAX`, `GETENTROPY_MAX` and `_SC_NSIG` are absent.**
- Issue 8 `<limits.h>` adds `NSIG_MAX` and `GETENTROPY_MAX`. Both are undeclared in glibc and musl.
- `_POSIX_SS_REPL_MAX` is undeclared in glibc.
- Comp: glibc, musl. Src:

1.25 **`<aio.h>` `lio_listio` prototype is incompatible in glibc.**
- The glibc prototype uses `struct aiocb *const restrict *restrict`, which does not match POSIX.
- Comp: glibc. Observe: a pointer-to-function-type test fails to compile.

1.26 **`aio_suspend()` is not async-signal-safe in glibc**, though POSIX requires it to be.
- Comp: glibc. Unconfirmed for 2.39; the man-page statement is old.
- Src: https://raw.githubusercontent.com/mkerrisk/man-pages/master/man3/aio_suspend.3

1.27 **Historical: `read()`/`write()` file-offset updates were not atomic between threads sharing an open file description.**
- Fixed in Linux 3.14, so it is not relevant to 6.x. Listed so you do not chase it.
- Src: read.2 / write.2 BUGS.

---

## 2. Processes, sessions, job control, credentials

2.1 **`waitpid(-pgid)` blocks when the target group has emptied because the child moved out of it.**
- POSIX: [ECHILD] if "the process group specified by pid does not exist or does not have any member process that is a child of the calling process".
- Comp: kernel. Kind: long-standing.
-

2.2 **`kill(-1, sig)` does not signal the calling process.**
- POSIX: send to all processes the caller may signal, excluding only an unspecified set of system processes.
- Linux: the caller is excluded.
- Comp: kernel. Kind: deliberate.
- Observe: XSH.kill.012, run as a user ID nothing else uses (profile `have kill_uid U`), after proving no privilege is left, with SIGURG (default action: ignore). Without that profile entry the test is UNRESOLVED.
- Src: https://raw.githubusercontent.com/mkerrisk/man-pages/master/man2/kill.2 (NOTES); posixcheck run, glibc and musl alike.

2.3 **The nice value is per-thread, not per-process.**
- POSIX: the nice value is a process attribute shared by all threads, for `nice`, `setpriority` and `getpriority`.
- Linux/NPTL: per-thread. `setpriority(PRIO_PROCESS, 0, n)` affects only the calling thread. pthreads(7) calls this "at least one nonconformance" that remains in NPTL.
- Comp: kernel (glibc and musl do not compensate). Kind: deliberate/long-standing.
- Observe: thread A calls `nice(5)`; thread B calls `getpriority(PRIO_PROCESS, 0)` and still gets 0.

  - https://raw.githubusercontent.com/mkerrisk/man-pages/master/man2/getpriority.2 (BUGS)
  - https://raw.githubusercontent.com/mkerrisk/man-pages/master/man7/pthreads.7

2.4 **Credentials are per-thread in the kernel.**
- POSIX: all threads share uid/gid/groups.
- Linux kernel: credentials are per-thread. glibc NPTL emulates process-wide credentials by broadcasting the change to every thread with a real-time signal (`SIGSETXID`). musl does the same; a sequencing bug in its multithreaded set*id was fixed in 1.2.5.
- Comp: kernel (root), handled by glibc and musl. Kind: deliberate.
- Observe: conformant through libc wrappers. A test that uses `syscall(SYS_setuid, ...)` sees per-thread behaviour.
- Fixed: musl race fixed in 1.2.5 (musl 1.2.4 "multithreaded set*id could malfunction").

  - https://raw.githubusercontent.com/mkerrisk/man-pages/master/man7/nptl.7
  - setuid.2
  - musl WHATSNEW 1.2.5

2.5 **`setrlimit(RLIMIT_CPU)`: the kernel raises the soft limit by 1 s each time SIGXCPU is caught.**
- POSIX: SIGXCPU when the soft limit is exceeded.
- Linux: after the handler runs, the soft limit is increased by one second, repeatedly. The man page says this is "probably not standards conformant".
- A limit of 0 is treated as 1 s.
- Comp: kernel.
- Observe: `getrlimit` after the first SIGXCPU shows rlim_cur+1.
- Src: https://raw.githubusercontent.com/mkerrisk/man-pages/master/man2/getrlimit.2 (BUGS)

2.6 **`sched_setscheduler()` returns 0 instead of the previous policy (glibc), and is a stub in musl.**
- POSIX: return the former scheduling policy on success.
- Linux/glibc: always 0 on success.
- musl: `sched_setscheduler` always fails with **ENOSYS**. Linux's syscall applies to a thread, not a process, so musl does not implement it.
- Unconfirmed: musl's `sched_setparam`, `sched_getparam` and `sched_getscheduler` are probably similar stubs. Verify them.
- Comp: kernel/glibc (return value); musl (ENOSYS). Kind: deliberate.
- Observe: as root, set SCHED_FIFO; check the return value equals the old policy (SCHED_OTHER = 0 makes this ambiguous, so set SCHED_RR first).

  - https://raw.githubusercontent.com/mkerrisk/man-pages/master/man2/sched_setscheduler.2 (BUGS)
  - https://git.musl-libc.org/cgit/musl/tree/src/sched/sched_setscheduler.c

2.7 **`tcsetpgrp()` error semantics.**
- Comp: kernel.
-

2.8 **`posix_spawn_file_actions_addchdir()` / `addfchdir()` are missing.**
- POSIX: new in Issue 8 (SPN option).
- musl: also undeclared, although its `posix_spawn` implements the chdir/fchdir actions internally.
- Comp: glibc, musl. Kind: missing-2024.

  - glibc NEWS 2.29: https://raw.githubusercontent.com/bminor/glibc/master/NEWS
  - https://git.musl-libc.org/cgit/musl/tree/src/process/posix_spawn.c

2.9 **`POSIX_SPAWN_SETSID` is extension-only in glibc.** The glibc flag exists but needs `_GNU_SOURCE`. musl has had it since 1.1.17.

2.11 **`WCOREDUMP` is extension-only in glibc.** It is standardized in Issue 8.

2.12 **`waitid(..., infop=NULL, ...)` succeeds on Linux.**
- POSIX requires `infop` to be non-NULL; NULL is undefined behaviour.
- This is an extension, not a failure, but do not write a test that expects EFAULT.
- Src: https://raw.githubusercontent.com/mkerrisk/man-pages/master/man2/wait.2 (BUGS)

2.13 **`waitid(WNOHANG)` with nothing waitable must zero `si_pid` and `si_signo` (TC1 2013).**
- The man page says "not all implementations follow" this.
- Unconfirmed whether current Linux zeroes them; I believe kernels ≥4.13 do. Initialise the struct to garbage and check.
- Src: wait.2.

2.14 **`SA_NOCLDWAIT` with a SIGCHLD handler: Linux still generates SIGCHLD.** POSIX leaves this unspecified, so it is not a deviation. Do not assert either way. Src: https://raw.githubusercontent.com/mkerrisk/man-pages/master/man2/sigaction.2

2.15 **`getlogin()` in glibc historically used stdin instead of the controlling terminal.**
- glibc man BUGS: "glibc does not follow the POSIX specification and uses stdin instead of /dev/tty".
- Modern glibc consults `/proc/self/loginuid` first, so this is unconfirmed for 2.39.
- Comp: glibc. Observe: `getlogin()` with stdin redirected from a file.
- Src: https://raw.githubusercontent.com/mkerrisk/man-pages/master/man3/getlogin.3

2.16 **`exit()` called concurrently from several threads.**
- glibc bug 14697 ("Behavior of exit is nonconformant with respect to threads and stdio") was fixed in glibc 2.38, so 2.39 is fine.
- musl does not support recursive or concurrent `exit` before 1.2.6. 1.2.6 notes: "exit is now explicitly thread-safe".
- Src: glibc NEWS 2.38; musl WHATSNEW 1.2.6; https://wiki.musl-libc.org/functional-differences-from-glibc.html

---

## 3. Signals

3.1 **`sig2str()` / `str2sig()` / `SIG2STR_MAX` are missing in both glibc and musl.**
- POSIX: new in Issue 8, in `<signal.h>`.
- Comp: glibc, musl. Kind: missing-2024. Fixed: not in glibc through 2.43 per NEWS.

3.2 **musl `sigismember()` does not fail on a negative signal number.**
- POSIX: [EINVAL] for an invalid signal number.
- musl: no error. glibc: passes.
- Comp: musl. Observe: `sigismember(&s, -1)` returns 0/1 instead of -1/EINVAL.
-

3.3 **`abort()` is not async-signal-safe in glibc < 2.41.**
- POSIX lists `abort` as async-signal-safe.
- Comp: glibc. Observe: hard to observe deterministically. A deadlock is possible when `abort` is called from a handler while another `abort`/stdio lock is held.
- Src: glibc NEWS 2.41 (bug list).

3.4 **`sigaltstack()` accepts `SS_ONSTACK` in `ss_flags` as a no-op.** POSIX uses `SS_ONSTACK` only as an output flag. This is an extension; do not test for EINVAL. Src: sigaltstack.2 BUGS.

3.6 **The hardware-exception signal is architecture-dependent.**
- For example, x86 `int` with a bad vector gives SIGSEGV, not SIGILL.
- Tests that trigger faults should accept SIGSEGV or SIGBUS.
- Src: https://raw.githubusercontent.com/mkerrisk/man-pages/master/man7/signal.7 (BUGS)

---

## 4. Threads and synchronization

4.1 **`pthread_cond_clockwait`, `pthread_mutex_clocklock`, `pthread_rwlock_clockrdlock`, `pthread_rwlock_clockwrlock` and `sem_clockwait` are missing in musl.**
- POSIX: new in Issue 8.
- Comp: musl (missing); glibc (gating). Kind: missing-2024.

  - glibc NEWS 2.30

4.2 **`PTHREAD_NULL` is undefined in glibc.** It is new in Issue 8. musl has had it since 1.2.1. musl WHATSNEW.

4.3 **musl declares `pthread_mutexattr_getprioceiling`/`setprioceiling` but they do not link.**

4.4 **glibc `pthread_attr_setstacksize` may round a size that is not a multiple of 16 *down*.**
- POSIX requires at least the requested size.
- The man page states this as of glibc 2.8, so it is unconfirmed for 2.39.
- Src: https://raw.githubusercontent.com/mkerrisk/man-pages/master/man3/pthread_attr_setstacksize.3

4.5 **musl's default thread stack is 128 KiB; glibc's comes from RLIMIT_STACK (typically 8 MiB).**
- This is not a conformance issue, but deep-recursion tests in threads may crash on musl.
- Src: https://wiki.musl-libc.org/functional-differences-from-glibc.html

4.6 **Cancellation semantics differ.**
- musl guarantees that a cancelled cancellation point either acts as if it failed with EINTR or completes without acting on cancellation. glibc makes no such guarantee: side effects may have happened, which POSIX permits.
- glibc 2.34 fixed spurious EINTR on cancellation, and 2.41/2.42 fixed cancellation races (bugs 12683 and 32782).
- Src: musl wiki; glibc NEWS 2.34, 2.41, 2.42.

- This is probably a C23 artefact, because they are keywords in C23 and glibc does not define the macros then. Verify with your compiler mode before treating it as a deviation.

4.8 **`<sched.h>` / `pthread_setschedparam` with RT policies needs CAP_SYS_NICE or RLIMIT_RTPRIO.**
- POSIX's "appropriate privileges" covers this. Tests should skip on EPERM.
- glibc bug: `pthread_attr_setinheritsched(PTHREAD_EXPLICIT_SCHED)` without an explicit policy wrongly inherited the creator's scheduling (as of 2.8; unconfirmed now).
- Src: https://raw.githubusercontent.com/mkerrisk/man-pages/master/man3/pthread_attr_setinheritsched.3

---

## 5. Time and clocks

5.1 **`struct tm` `tm_gmtoff` / `tm_zone` are extension-only in both libcs.**
- POSIX: Issue 8 requires them in `struct tm`.
- glibc and musl name them `__tm_gmtoff` / `__tm_zone` unless `_DEFAULT_SOURCE` or `_GNU_SOURCE` is defined.
- Comp: glibc, musl. Kind: missing-2024 (namespace).
-

5.2 **musl < 1.2.6 `strptime` does not accept the conversion specifiers added in Issue 8.**
- Fixed: musl **1.2.6**.

5.3 **`clock_nanosleep(CLOCK_THREAD_CPUTIME_ID, ...)` fails with EINVAL.**
- POSIX: sleeping on the calling thread's CPU clock shall fail.
- Linux: EINVAL. POSIX lists [EINVAL] for this case (Unconfirmed: check whether ENOTSUP is required).
- Src: https://raw.githubusercontent.com/mkerrisk/man-pages/master/man2/clock_nanosleep.2

5.4 **glibc `tzset` could fault on a very short TZ string** (bug 31931). Fixed in **2.40**, so 2.39 is affected. Src: glibc NEWS 2.40.

5.5 **`timespec_get`.** glibc has had it since 2.16; musl since 1.1.5 (C11 coverage). Not a gap. Src: glibc NEWS; musl WHATSNEW.

5.6 **`strftime` sets no errno on overflow.** POSIX does not require errno, so this is not a deviation. Src: strftime.3.

5.7 **TZ handling.**
- POSIX.1-2024 revised TZ, adding support for the `<...>` quoted std/dst names.
- musl 1.2.3: "POSIX TZ parsing is stricter to allow more names to fallback to files".
- I found no primary source listing glibc TZ nonconformance against Issue 8. Unconfirmed; test empirically.
- Src: musl WHATSNEW 1.2.3.

---

## 6. stdio / printf / stdlib

6.1 **glibc < 2.42: `fclose()`/`fflush()` on a seekable *input* stream does not reposition the underlying fd.**
- POSIX: `fclose`/`fflush` on a seekable input stream sets the file offset of the underlying open file description to the stream's position.
- glibc 2.39 leaves the fd offset at the read-ahead position. Bugs 12724 ("fclose violates POSIX 2008 on seekable input streams") and 5994 ("fflush after ungetc on seekable input stream").
- Observe: `fopen` a 10 kB file, `fgetc` once, `fflush`/`fclose`, then `lseek(fd, 0, SEEK_CUR)` on a dup'd fd returns 4096 or more instead of 1.
- Src: glibc NEWS 2.42 bug list, https://raw.githubusercontent.com/bminor/glibc/master/NEWS
- posixcheck: `exit()` is a different path. With glibc 2.39, XSH.exit.010
  (2 bytes read of 8192 through a 4096-byte buffer, then exit()) finds the
  shared offset at 2, as required; musl too. The fclose()/fflush() case is
  for the stdio module.

6.2 **EOF stickiness (glibc < 2.28).**
- Before 2.28, glibc returned new data after EOF had been set. "All stdio functions now treat end-of-file as a sticky condition" since **2.28**.
- Fine in 2.39. Listed for completeness.
- Src: glibc NEWS 2.28; musl wiki.

6.3 **`asprintf`/`vasprintf` are extension-only in both libcs.**
- POSIX: new in Issue 8, in `<stdio.h>`.
- Both need `_GNU_SOURCE` (musl also accepts `_BSD_SOURCE`).

6.4 **`swprintf` error code when output does not fit.**
- glibc sets **E2BIG**. musl leaves **errno == 0**.
- Comp: glibc, musl. Observe: `swprintf(buf, 2, L"abc")` returns -1, then check errno.
-

6.5 **musl < 1.2.6: `printf` did not honor hex-float (`%a`) precision in some cases.** Fixed in **1.2.6**. Observe: `printf("%.1a", 1.96875)` rounding. Src: musl WHATSNEW 1.2.6.

6.6 **musl < 1.2.5 bugs.**
- `printf("%lc", 0)` produced no output.
- `snprintf`/`swprintf` failed when `n > INT_MAX`.
- Fixed: **1.2.5** ("standards updates").
- Src: musl WHATSNEW 1.2.5.

6.7 **Floating-point printf rounding mode.**
- musl wiki: musl honors the current rounding mode in float conversions and glibc does not. POSIX/C leave the effect of the rounding mode on decimal conversion partly implementation-defined.
- Unconfirmed for glibc 2.39; the wiki may be dated.
- Src: https://wiki.musl-libc.org/functional-differences-from-glibc.html

6.8 **glibc accepts nonstandard printf lengths.**
- For example, `%Ld` is accepted as `%lld`, and `ll`/`L` are synonyms.
- musl rejects them with an error.
- This is UB in POSIX, so do not test it.
- Src: musl wiki; printf.3.

6.9 **`fmemopen` / `open_memstream` in old glibc.**
- glibc < 2.22 had several POSIX-2008 `fmemopen` bugs: size 0 gave EINVAL, append mode was broken, and SEEK_END was subtracted instead of added.
- glibc < 2.7 failed `open_memstream` seeks past the end.
- All fixed long ago, so they do not apply to 2.39.

  - https://raw.githubusercontent.com/mkerrisk/man-pages/master/man3/fmemopen.3
  - open_memstream.3

6.10 **glibc `initstate()` does not return NULL on error.** POSIX requires NULL. glibc sets errno but returns non-NULL. Comp: glibc. Src: https://raw.githubusercontent.com/mkerrisk/man-pages/master/man3/random.3 (BUGS)

6.11 **`encrypt()` / `setkey()` are not available in glibc.**
- POSIX: still in Issue 8 (XSI, obsolescent).
- musl provides them.
- Comp: glibc. Kind: deliberate.

  - glibc NEWS 2.28

6.12 **`strfmon()` in the POSIX locale prints a decimal point.**
- glibc gave `"%foo901.42bar-137.10qux f42.010 "` and musl gave `"%foo901.42bar-137.10qux 42.010"`.
- My reading is that in the POSIX locale `mon_decimal_point` is "", so no '.' should appear. Field/padding handling also differs.
- Both `strfmon` and `strfmon_l` fail on both libcs.
- Comp: glibc, musl.
-

- Versions:
  - `qsort_r`: glibc 2.8, with the same argument order as POSIX 2024; musl **1.2.3**.
  - `reallocarray`: glibc 2.26; musl 1.2.2.
  - `mkostemp`: glibc 2.7; musl 0.9.10.
- musl WHATSNEW; glibc NEWS.

6.14 **`memmem`, `strlcpy`, `strlcat` are extension-only.**
- `memmem` is extension-only in glibc. musl exposes it by default since **1.2.4**.
- `strlcpy`/`strlcat`: glibc added them in **2.38**. Both libcs gate them.
- glibc NEWS 2.38; musl WHATSNEW 1.2.4.

6.16 **`getopt()` permutes argv in glibc.**
- POSIX: `getopt` returns -1 at the first non-option argument.
- glibc: by default permutes argv so options after operands are parsed. It is POSIX only when `POSIXLY_CORRECT` is set or optstring starts with '+'.
- musl: stops at the first non-option, as POSIX requires.
- Comp: glibc. Kind: deliberate.
- Observe: argv = {"p","file","-a"} with optstring "a". glibc returns 'a'; POSIX returns -1 with optind=1.

  - https://raw.githubusercontent.com/mkerrisk/man-pages/master/man3/getopt.3
  - musl wiki

6.17 **`basename()` / `strerror_r()` GNU variants.**
- With `_GNU_SOURCE`, glibc's `<string.h>` `basename` and `strerror_r` have GNU semantics (`strerror_r` returns `char *`). Not an issue in strict POSIX mode, but beware if the test suite defines `_GNU_SOURCE` globally.
- musl < 1.2.5 provided a non-prototype `basename` in `string.h`.
- Src: musl wiki; musl WHATSNEW 1.2.5; strerror.3.

6.19 **`<endian.h>` in glibc.**
- POSIX: new in Issue 8. It shall define the byte-order macros, `be16toh` and the related functions, and the `uint16_t`/`uint32_t`/`uint64_t` types.
- glibc: all macros and functions are `extension`, and the types are not declared (`unknown_type`).
- musl: fully good since it aimed at "future POSIX" in 1.2.0.
- Comp: glibc.

---

## 7. Locale, wide characters, iconv, message catalogs

7.1 **glibc's C/POSIX locale is 7-bit.**
- POSIX.1-2024 §6.2: the POSIX locale has 256 single-byte characters, so every byte value is a character.
- glibc: in the C locale, `mbrtowc`/`mbtowc` reject bytes 0x80-0xFF with EILSEQ. Yet `regex`, `fnmatch` and `glob` accept them, per the musl wiki.
- musl: since 1.1.11 the C locale maps 0x80-0xFF to individual abstract characters.
- Comp: glibc. Kind: long-standing.
- Fixed: not found in glibc NEWS through 2.43, so unconfirmed whether still true. Test `mbrtowc` on "\x80" in the C locale.

  - https://wiki.musl-libc.org/functional-differences-from-glibc.html
  - local `basedefs/V1_chap06.html`

7.2 **glibc `iconv()` fails with EILSEQ for valid-but-unrepresentable characters.**
- POSIX.1-2024: with no indicator suffix, `iconv` shall perform an implementation-defined conversion and count it in the return value as an irreversible conversion. It must not fail.
- glibc: returns EILSEQ. musl substitutes '*' and counts it, which conforms.
- Comp: glibc. Kind: deliberate.
- Observe: UTF-8 "é" converted to ASCII without suffix. glibc gives -1/EILSEQ.

  - musl wiki
  - local `functions/iconv.html`

7.3 **`iconv_open` indicator suffixes.**
- Issue 8 adds `//IGNORE`, `//NON_IDENTICAL_DISCARD` and `//TRANSLIT`.
- musl: no `//TRANSLIT` (musl wiki).
- glibc: supports `//TRANSLIT` and `//IGNORE`. `//NON_IDENTICAL_DISCARD` support is unconfirmed (probably absent in both libcs).
- Src: musl wiki; local `functions/iconv_open.html`

7.4 **musl iconv is source-only for many legacy charsets.**
- musl converts from many legacy multibyte charsets but not to them. JIS destinations were added in 1.1.19.
- musl's UTF-16/32 handling honors a BOM but never emits one.
- Src: musl wiki.

7.6 **`ALTMON_x` / `ABALTMON_x` (`<langinfo.h>`).**
- `ABALTMON_1..12` are undeclared in both libcs. glibc has only the nonstandard `_NL_ABALTMON_*`.
- `ALTMON_1..12` are extension-only in glibc (since 2.27) and undeclared in musl.
- Comp: glibc, musl. Kind: missing-2024.
- glibc NEWS 2.27

7.7 **`<libintl.h>` `*_l` variants are missing.**
- Issue 8 adds `gettext_l`, `dgettext_l`, `dcgettext_l`, `ngettext_l`, `dngettext_l` and `dcngettext_l`.
- They are missing from both libcs.
- In musl, the base gettext functions are graded "outside_libc".
- musl 1.2.6 fixed a `bind_textdomain_codeset` return-value bug.
- musl WHATSNEW 1.2.6

7.9 **musl < 1.2.6 `mbsnrtowcs` partial-character handling.** It did not meet the Issue 8 requirement for a partial character at the end of input. Fixed in **1.2.6**. Src: musl WHATSNEW 1.2.6.

7.10 **UTF-8 validity differs.**
- musl rejects surrogates, overlong forms and 5- and 6-byte forms.
- glibc accepts 5- and 6-byte sequences.
- Affects tests of `mbrtowc` returning EILSEQ.
- Src: musl wiki.

7.11 **Default locale differs.** With LANG/LC_* unset, glibc uses "C" and musl uses "C.UTF-8". Before `setlocale` is called, both start in "C" as ISO C requires. Src: musl wiki.

7.12 **`wcwidth` differences between glibc and musl.** Each follows its own Unicode version (glibc 2.39 uses Unicode 15.x; 2.41 moved to 16.0). Do not assert exact widths for non-ASCII. Unconfirmed. Src: glibc NEWS 2.41.

---

## 8. Regex, fnmatch, glob

8.1 **glibc regex: subexpressions inside a repeated alternation keep stale matches.**
- ERE `((a)|(b))*` on "ab": POSIX requires `\2` to report "not matched" (-1), because `(a)` did not participate in the last iteration.
- glibc reports "a" [0,1].
- Comp: glibc.

8.2 **glibc regex `REG_ICASE` with ranges.** `[A-f]` with `REG_ICASE` does not match "G", in both BRE and ERE. Comp: glibc.

8.3 **glibc regex range with collating-element endpoints.** BRE `[[.[.]-[.\.]]` did not match "]". Comp: glibc.

8.4 **musl regex does not support collating symbols or equivalence classes.**
- `[[.a.]]`, `[[=a=]]` and `[[.b.]-[.d.]]` are rejected at `regcomp` time ("Unknown collating element").
- Comp: musl. Kind: long-standing (TRE-based).

8.5 **musl regex: nested interval submatch.** `\(a\(\(a\{1,2\}\)\{1,2\}\)\)` on "aaa" reports submatch "a" [2-3] instead of "aa" [1-3], in both BRE and ERE. Comp: musl.

8.6 **`REG_MINIMAL` is undeclared in both libcs.** It is new in Issue 8.

8.7 **glibc `regoff_t` is `int` (32-bit) even on LP64.**
- POSIX requires `regoff_t` to hold the largest value in `ssize_t` or `ptrdiff_t`, so matches beyond 2 GiB break.
- musl uses the correct type.
- Comp: glibc. Kind: deliberate (ABI).
- Src: musl wiki.

8.9 **glibc fnmatch: backslash inside bracket expression class, collating and equivalence names.** `[[:al\num:]]`, `[[.\a.]]` and `[[=\a=]]` do not match "a". Comp: glibc; musl also fails the first.

8.10 **glibc fnmatch `FNM_CASEFOLD` with bracket expressions.**
- `[[.a.]]`, `[[=a=]]` and `[A-f]` with FNM_CASEFOLD fail.
- In UTF-8, `[[:lower:]]` does not match "Λ", and `[[.ø.]]`/`[[=ø=]]` do not match "Ø".
- Comp: glibc.

8.11 **glibc fnmatch with `FNM_PATHNAME` / `FNM_PERIOD` and escaped slash or period.**
- `\*\/` with FNM_PATHNAME does not match "./".
- `\/?` with FNM_PATHNAME|FNM_PERIOD does not reject "/.".
- Comp: glibc.

8.12 **glibc fnmatch matches invalid UTF-8 bytes with `?` / `[!x]`.**
- In a UTF-8 locale, `?` or `[!x]` matches invalid sequences: continuation-only bytes, missing bytes, values over U+10FFFF, overlong forms and surrogates.
- Comp: glibc.

8.13 **musl fnmatch bracket expressions.**
- No collating symbols or equivalence classes (`[[.a.]]` and `[[=a=]]` fail), including UTF-8 ones.
- Backslash handling inside brackets is wrong: `[x\y]` matches "\" and `[a\-b]` does not match "-".
- `[foo\]` does not match "[foo]".
- Reversed ranges (`[d-b]`) are not rejected.
- `[[...][./.]]` periodslash cases fail, and `\.` with FNM_PERIOD fails.
- Comp: musl.

8.14 **`FNM_IGNORECASE` is undeclared in both; `FNM_CASEFOLD` is extension-only in glibc.** Both are new in Issue 8.

8.15 **musl < 1.2.5 `glob` could wrongly return GLOB_NOMATCH if aborted before any matches.** Fixed in 1.2.5. Src: musl WHATSNEW 1.2.5.

8.16 **glob(7) on Linux.** POSIX requires an unmatched or syntactically invalid pattern to be left unchanged in the shell; the man page notes this. This is a shell-level requirement (`GLOB_NOCHECK` in `glob()`), not a deviation by itself. Src: https://raw.githubusercontent.com/mkerrisk/man-pages/master/man7/glob.7

---

## 9. Terminals and pseudo-terminals

9.1 **`tcgetwinsize()` / `tcsetwinsize()` / `struct winsize` (in `<termios.h>`) are missing in glibc.**
- POSIX: new in Issue 8.
- glibc: not declared (compile_error). glibc only has `TIOCGWINSZ` ioctl and `struct winsize` in `<sys/ioctl.h>`.
- musl: has them since **1.2.2**.
- Comp: glibc. Kind: missing-2024.

  - musl WHATSNEW 1.2.2

9.2 **`ptsname_r` is extension-only in both libcs.**
- It is new in Issue 8 (XSI). glibc needs `_GNU_SOURCE`.
- glibc ≤2.25 returned EINVAL for buf NULL. musl < 1.0.0 returned negated error codes.
- ptsname.3; musl WHATSNEW

9.3 **`pty-hup-poll`: POLLHUP is set together with POLLOUT on the master after the slave closes.** See 1.15.

9.4 **`tcsetpgrp` error semantics.** See 2.7.

9.5 **`IMAXBEL` is not implemented; Linux acts as if it is always set.** This is not POSIX, so it is informational. Src: termios.3.

9.6 **glibc 2.42 changed `speed_t` to the plain baud rate number, for arbitrary rates.**
- Code that assumes `B9600 != 9600` breaks.
- This does not affect 2.39, but note it if the suite later runs on newer glibc.
- Src: glibc NEWS 2.42.

9.7 **musl < 1.2.6 termios input speed handling was wrong** (`cfgetispeed`/`cfsetispeed`). Fixed in 1.2.6. Src: musl WHATSNEW 1.2.6.

9.8 **musl < 1.2.6 `isatty` collapsed all errors to ENOTTY.** POSIX allows EBADF. Fixed in 1.2.6. Observe: `isatty(-1)` gives errno ENOTTY instead of EBADF. Src: musl WHATSNEW 1.2.6.

9.9 **`ctermid()` returns "/dev/tty" and does not uniquely identify the terminal.** POSIX permits this. Src: ctermid.3.

---

## 10. IPC, sockets, shared memory

10.1 **The `struct msghdr` `msg_iovlen`/`msg_controllen` and `struct cmsghdr` `cmsg_len` types are wrong in glibc.**
- POSIX: `msg_iovlen` is `int`, and `msg_controllen`/`cmsg_len` are `socklen_t`.
- glibc: `size_t`, following the kernel ABI. musl uses POSIX types with padding.
- Comp: glibc. Kind: deliberate (ABI).
- Observe: `_Static_assert(sizeof(((struct msghdr*)0)->msg_iovlen) == sizeof(int))`.

  - https://raw.githubusercontent.com/mkerrisk/man-pages/master/man2/recv.2 (NOTES)

10.2 **`accept4`, `SO_DOMAIN`, `SO_PROTOCOL` are extension-only in glibc.** They are standardized in Issue 8. glibc needs `_GNU_SOURCE`; musl exposes them.

10.3 **`NI_NUMERICSCOPE` is undeclared in glibc.**

10.4 **UDP edge cases.**
- Many `SO_REUSEADDR` and broadcast bind-conflict cases are graded "unknown". Avoid strict assertions there.
- Comp: kernel.

10.5 **`send()` on a non-connected stream socket may give EPIPE instead of ENOTCONN.** Src: https://raw.githubusercontent.com/mkerrisk/man-pages/master/man2/send.2 (BUGS)

10.6 **Message queue descriptors are file descriptors, with close-on-exec set automatically.**
- POSIX does not require this, so it is permitted. Tests must not assume `mqd_t` survives `exec`.
- Before Linux 2.6.14, the umask was not applied in `mq_open`.
- Src: mq_overview.7; mq_open.3.

10.7 **POSIX semaphores and shm live in `/dev/shm`.**
- `sem_open` names are limited to NAME_MAX-4 (251) characters.
- If `/dev/shm` is not mounted, as in some containers, `sem_open`/`shm_open` fail.
- Src: https://raw.githubusercontent.com/mkerrisk/man-pages/master/man7/sem_overview.7

10.8 **musl < 1.2.6 semaphores.**
- 1.2.4 fixed missed wakes.
- 1.2.6 fixed "lost or delayed wakes in sem_post under race condition", so musl 1.2.4 can still lose wakes.
- Src: musl WHATSNEW 1.2.4 and 1.2.6.

10.10 **`<devctl.h>` / `posix_devctl` is missing** (optional DC).

10.11 **AF_UNIX `sun_path`.** Linux appends a NUL if one is not supplied, so a 108-byte path round-trips without a terminator. Src: unix.7 BUGS.

---

## 11. Header namespace and the Issue 8 feature-test macro

11.1 **glibc does not honour `_POSIX_C_SOURCE=202405L` / `_XOPEN_SOURCE=800` as "POSIX 2024".**
- glibc NEWS through 2.43 contains no entry adding POSIX.1-2024 feature-macro support.
- Kind: missing-2024.
- Practical advice:
  - Compile with `-D_GNU_SOURCE` on glibc, or treat `extension` as a soft failure.
  - Define nothing, or `_DEFAULT_SOURCE`, on musl.

11.2 **musl < 1.2.5: `ppoll` is not exposed in the default feature profile.** In 1.2.4 it needs `_GNU_SOURCE`/`_BSD_SOURCE`. Src: musl WHATSNEW 1.2.5.

11.3 **Namespace pollution.**
- musl additionally pollutes `<aio.h>`, `<libintl.h>`, `<wchar.h>` and `<spawn.h>`.
- A namespace-cleanliness test will fail on both.
-

11.4 **`dladdr` / `Dl_info_t`.**
- POSIX Issue 8 names the type `Dl_info_t` (`Dl_info` was added by a later fix).
- Both libcs have only `Dl_info`, extension-gated. `Dl_info_t` and the `dladdr` test give `unknown_type`.
-

11.6 **`LONG_BIT` and the `NL_ARGMAX`/`NL_MSGMAX`/`NL_SETMAX`/`NL_TEXTMAX` limits are extension-only in glibc.**

### Availability table for Issue 8 additions

"ext" means present but needs `_GNU_SOURCE`/`_DEFAULT_SOURCE`. Versions come from glibc NEWS and musl WHATSNEW unless noted otherwise.

| Interface | glibc (first version) | musl (first version) |
|---|---|---|
| `accept4`, `dup3`, `pipe2` | 2.10 / 2.9 (ext) [from memory] | 0.9.x (pipe2 0.9.3) |
| `asprintf` | old (ext) | old (ext) |
| `dladdr` | old (ext; no `Dl_info_t`) | 0.9.5 (ext; no `Dl_info_t`) |
| `endian.h` | old (ext; no uintN_t) | 0.8.10; POSIX-aligned 1.2.0 |
| `getentropy` | 2.25 | 1.1.20 |
| `getlocalename_l` | missing | missing |
| `memmem` | old (ext) | 0.9.7; default 1.2.4 |
| `mkostemp` | 2.7 (ext) | 0.9.10 |
| `posix_close` | **missing** | 0.9.15 |
| `posix_getdents` | **missing** | **1.2.6** |
| `posix_spawn_file_actions_addchdir`/`addfchdir` | only `_np` (2.29) | missing (non-`_np`) |
| `ppoll` | 2.4 (ext) | default since 1.2.5 |
| `pthread_*clock*`, `sem_clockwait` | 2.30 (ext) | **missing** |
| `ptsname_r` | old (ext) | 0.9.2 |
| `qsort_r` | 2.8 (ext) | 1.2.3 |
| `quick_exit`/`at_quick_exit` | 2.10; C11 decl 2.16 | ~1.1.x [unconfirmed] |
| `reallocarray` | 2.26 (ext since 2.29 `_DEFAULT_SOURCE`) | 1.2.2 |
| `secure_getenv` | 2.17 (ext) [from memory] | 1.1.24 |
| `sig2str`/`str2sig` | **missing** | **missing** |
| `strlcpy`/`strlcat` | 2.38 (ext) | old (ext) |
| `tcgetwinsize`/`tcsetwinsize` | **missing** | 1.2.2 |
| `timespec_get` | 2.16 | 1.1.5 |
| `wcslcpy`/`wcslcat` | 2.38? (ext) [unconfirmed version] | **missing** |
| `_Fork` | 2.34 (ext) | 1.2.2 |
| `O_CLOFORK` family | **missing (kernel)** | **missing (kernel)** |
| `REG_MINIMAL`, `FNM_IGNORECASE`, `ABALTMON_*`, `NSIG_MAX`, `GETENTROPY_MAX` | missing | missing |

---

## 12. Shell and utilities (`/bin/sh` = dash on Ubuntu, GNU coreutils)

12.1 **dash 0.5.12 (Ubuntu 24.04) lacks `$'...'` dollar-single-quotes.**
- POSIX: new in Issue 8 (XCU 2.2.4).
- dash: added by commit "parser: Add dollar single quote" (2024), first released in **0.5.13** (2025). 0.5.13.x continued to fix it, including octal escapes and CTLESC handling.
- Comp: dash. Kind: missing-2024.
- Observe: `sh -c "printf %s \$'a\\tb'"` prints a literal `$a\tb`.
- Src: https://kernel.googlesource.com/pub/scm/utils/dash/dash/+log

12.2 **dash 0.5.12 lacks `set -o pipefail`.**
- New in Issue 8.
- Added by "jobs: Implement pipefail option" (2024-04-05), which is after the 0.5.12 tag of 2022-12-11, so it is in 0.5.13.
- Observe: `set -o pipefail` gives an "Illegal option" error.
- Src: https://kernel.org/pub/scm/utils/dash/dash.git/log/

12.3 **dash 0.5.12 has no multibyte or locale support.**
- dash did not call `setlocale` until 2024, and `${#var}`, pattern matching, field splitting and `read` worked on bytes. Multibyte support arrived in 0.5.13, in the series "parser: Add support for multi-byte characters", "shell: Call setlocale" and others.
- Observe: `x=é; echo ${#x}` prints 2 in a UTF-8 locale.
- Src: https://kernel.googlesource.com/pub/scm/utils/dash/dash/+log

12.4 **dash 0.5.12 `test -nt` / `-ot` do not follow the Issue 8 rules.** Issue 8 changed how a nonexistent file compares. Fixed by "builtin: Align test -nt and -ot with POSIX.1-2024" (0.5.13). Src: as 12.3.

12.5 **dash 0.5.12 `getopts` does not skip the first `--` and mishandles ':' in optstring.**
- Commits "builtin: Ignore first -- in getopts per POSIX" (2023-01-08) and "options: Fix getopts handling of colon in optstr" (2024-04) both postdate 0.5.12.
- Src: https://kernel.org/pub/scm/utils/dash/dash.git/log/

12.6 **dash `test`: `=` and `!=` must use byte comparison.** Fixed in 0.5.13 ("test = and != are strcmp, not strcoll"). This only matters once setlocale is called. Src: dash log.

12.7 **Other Issue 8 sh features in dash 0.5.12 (unconfirmed).**
- I found no source showing `read -d`, `cd -e`, `ulimit` with the new options, or `test <`/`>` in 0.5.12.
- Assume they are missing, and verify with `sh -c 'read -d x v <<EOF ...'`.

12.8 **GNU coreutils `echo` does not interpret backslash escapes by default.**
- POSIX/XSI: `echo` shall interpret `\c`, `\n` and the other escapes, and accept no options.
- GNU `/bin/echo`: escapes are off by default (`-E`) and `-n`/`-e` are options. With `POSIXLY_CORRECT` set, escapes are always enabled.
- The dash builtin `echo` does interpret escapes. Test both `/bin/echo` and the `echo` builtin.
- Comp: coreutils. Kind: deliberate.
- Src: https://www.gnu.org/software/coreutils/manual/html_node/echo-invocation.html

12.9 **GNU utilities need `POSIXLY_CORRECT` in general.**
- "In a few cases, the GNU utilities' default behavior is incompatible with the POSIX standard. To suppress these incompatibilities, define the POSIXLY_CORRECT environment variable."
- Commonly affected, from my knowledge (unconfirmed in detail):
  - `df`/`du`/`ls -s` use 1024-byte blocks unless `POSIXLY_CORRECT` (or `-P`/`-k`) is used.
  - Option permutation after operands in all getopt-using tools. See 6.16.
- `_POSIX2_VERSION` supports only 199209, 200112 and 200809. There is no 2024 value.
- Src: https://www.gnu.org/software/coreutils/manual/html_node/Standards-conformance.html

12.10 **`system()` / `popen()` use `/bin/sh`.** They therefore inherit every dash gap above. Also:
- glibc `system()` is thread-safe; musl's is not, per the musl wiki.
- musl < 1.2.3 `popen` leaked other `popen` pipe fds to the child, which POSIX forbids.
- Src: musl wiki; musl WHATSNEW 1.2.3.

---

## 13. Historical items fixed long ago (so you can dismiss them)

Each is fixed before the reference versions.

- `getcwd` returned "(unreachable)" relative paths: fixed in glibc 2.27. Src: getcwd.3.
- `ftw` `FTW_SLN` stat buffer: fixed in glibc 2.30. Src: ftw.3.
- `execl`/`execle` not async-signal-safe: fixed in glibc 2.24. Src: exec.3; signal-safety.7.
- `posix_spawn` failed with `SETSCHEDULER` without `SETSCHEDPARAM`: fixed in glibc 2.14. Src: posix_spawn.3.
- `setitimer` accepted `tv_usec` ≥ 1e6: fixed in kernel 2.6.22. Src: getitimer.2.
- `times`/`getrusage` per-thread: fixed in kernel 2.6.9. Src: pthreads.7.
- `faccessat` ignored ACLs when emulating flags: fixed in glibc 2.33 with kernel 5.8+ (`faccessat2`). Src: access.2.
- `utimensat` `tv_sec` had to be 0 for UTIME_NOW/OMIT: fixed in kernel 2.6.26. Src: utimensat.2.
- `read`/`write` offset atomicity: fixed in kernel 3.14. Src: read.2/write.2.

---

### Primary source index (all opened during this research)

- musl:
  - https://wiki.musl-libc.org/functional-differences-from-glibc.html
  - WHATSNEW (GitHub mirror): https://raw.githubusercontent.com/kraj/musl/master/WHATSNEW
  - https://git.musl-libc.org/cgit/musl/tree/src/unistd/close.c
  - https://git.musl-libc.org/cgit/musl/tree/src/sched/sched_setscheduler.c
  - https://git.musl-libc.org/cgit/musl/tree/src/process/posix_spawn.c
  - https://git.musl-libc.org/cgit/musl/log/src/unistd/posix_close.c?showmsg=1
- glibc NEWS (GitHub mirror of master, covering 2.43 and earlier): https://raw.githubusercontent.com/bminor/glibc/master/NEWS
- Linux man-pages:
  - GitHub mirror of Michael Kerrisk's tree (man-pages ≈5.13, 2021): https://raw.githubusercontent.com/mkerrisk/man-pages/master/
  - man7.org itself could not be fetched from this environment. Some BUGS text may since have been updated upstream.
- LWN:
  - https://lwn.net/Articles/576478/ (close/EINTR)
  - https://lwn.net/Articles/820652 (close-on-fork patches)
- dash:
  - https://kernel.googlesource.com/pub/scm/utils/dash/dash/+log
  - https://kernel.org/pub/scm/utils/dash/dash.git/log/
- GNU coreutils manual:
  - https://www.gnu.org/software/coreutils/manual/html_node/Standards-conformance.html
  - https://www.gnu.org/software/coreutils/manual/html_node/echo-invocation.html
- POSIX.1-2024 text: local copy at `the personal copy of the standard`. I checked `close`, `poll`, `pwrite`, `waitpid`, `select`, `sched_setscheduler`, `iconv`, `iconv_open`, `kill`, `<errno.h>`, `<limits.h>` and XBD 6.2 there.
