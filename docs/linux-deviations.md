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

Compiled from documentation, release notes, man pages and published test
results (os-test); no other suite's code was read. POSIX requirements are
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

O.1 **`open()` with O_CREAT and a trailing slash fails with EISDIR.**
- POSIX (Issue 8 `open`, ERRORS): with O_CREAT, a path that ends in one or
  more slashes after a non-slash character fails with ENOENT or ENOTDIR,
  and ENOENT is not allowed when the name without the slashes exists.
  EISDIR is listed only for an existing directory.
- Linux: the kernel's last-component lookup rejects any O_CREAT path with
  a trailing slash with EISDIR, whether the name is missing or a regular
  file. Nothing is created, so only the errno is wrong.
- Comp: kernel. Kind: long-standing. Observe: XSH.open.044, XSH.open.045.
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
- Comp: kernel. Kind: long-standing. Observe: XSH.lseek.011.
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

### Caveats about the os-test data (sortix.org/os-test)

- os-test's **"linux"** column is "Linux 7.1.12+deb14-amd64", which is Debian 14 (forky/sid).
  - Its libc is therefore glibc, but a version **newer than 2.39** (about 2.41 or 2.42).
  - Anything listed as missing there is also missing in 2.39.
  - A few glibc 2.39 bugs fixed in 2.40 to 2.42 will **not** show up in os-test, but will show up on your Ubuntu 24.04 machine. They are listed separately below.
- os-test's **"musl"** column is "Linux 6.18.52-0-lts", which is Alpine.
  - It reports `posix_getdents` as working, so it is musl **1.2.6 or later**.
  - musl 1.2.4 is missing more than that column shows. See the musl WHATSNEW items below.
- In the header ("include") suite, os-test grades results as follows:
  - `extension`: the declaration exists, but only with system-specific feature macros (such as `_GNU_SOURCE` or `_DEFAULT_SOURCE`), not with `_POSIX_C_SOURCE=202405L` / `_XOPEN_SOURCE=800` alone.
  - `undeclared` / `unknown_type`: the declaration is absent.
  - `incompatible`: the prototype or type differs from POSIX.
  - `missing_optional`: an optional limit macro is absent. POSIX permits this, so it is **not** a deviation.
- A few os-test "undeclared" results for C keywords may be artefacts of compiling as C23, where these are keywords and not macros. Treat them as "verify your compiler mode":
  - `thread_local` in `<threads.h>`
  - `static_assert` in `<assert.h>`

Main source pages:

- https://sortix.org/os-test/ (overview)
- https://sortix.org/blog/os-testing-posix-headers/ (analysis)
- https://sortix.org/blog/posix-2024/ (list of Issue 8 additions)

---

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
- os-test grades these "missing_optional". They differ between glibc and musl (for example, musl defines `PAGESIZE`, `TZNAME_MAX`, `SYMLOOP_MAX` and `FILESIZEBITS`, while glibc does not). So do not hard-require them.
- Src: https://sortix.org/os-test/limits/

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
- Comp: kernel (root cause), glibc, musl. Kind: missing-2024. Fixed: no (still absent on kernel 7.1 per os-test).
- Observe: these do not compile:
  - `#ifdef O_CLOFORK`
  - os-test io tests `open-clofork-fork` and `dup3-clofork-fork`, which report "bad: no O_CLOFORK".
- Src:
  - https://sortix.org/os-test/io/
  - https://sortix.org/os-test/include/fcntl/
  - https://sortix.org/os-test/include/sys_socket/
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
- Src:
  - https://raw.githubusercontent.com/mkerrisk/man-pages/master/man2/close.2
  - https://lwn.net/Articles/576478/
  - musl `close.c`: https://git.musl-libc.org/cgit/musl/tree/src/unistd/close.c

1.3 **`posix_close()` / `POSIX_CLOSE_RESTART` missing in glibc.**
- POSIX: new in Issue 8, in `<unistd.h>`.
- glibc: undeclared. os-test basic/unistd `posix_close` gives compile_error on Linux.
- musl: has it since 0.9.15 (Dec 2013) as a plain wrapper for `close`. Upstream log: "Linux does not support EINTR semantics".
- Comp: glibc. Kind: missing-2024.
- Src:
  - https://sortix.org/os-test/basic/unistd/
  - https://sortix.org/os-test/include/unistd/
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
- Src:
  - https://raw.githubusercontent.com/mkerrisk/man-pages/master/man2/pread.2 (BUGS)
  - https://raw.githubusercontent.com/kraj/musl/master/WHATSNEW (1.2.6)

1.5 **`posix_getdents()` / `struct posix_dent` / `reclen_t` missing.**
- POSIX: new in Issue 8, in `<dirent.h>`.
- glibc: absent, even on the newer glibc that os-test used. os-test basic/dirent `posix_getdents` gives compile_error.
- musl: added in **1.2.6**, so it is absent in 1.2.4.
- Comp: glibc, musl<1.2.6. Kind: missing-2024.
- Src:
  - https://sortix.org/os-test/include/dirent/
  - https://sortix.org/os-test/basic/dirent/
  - musl WHATSNEW 1.2.6

1.6 **`<dirent.h>` DT_* constants are extension-only in glibc, and DT_MQ / DT_SEM / DT_SHM / DT_TMO are absent everywhere.**
- glibc exposes `DT_REG` and the others only under `_DEFAULT_SOURCE`. musl exposes them under POSIX 2024.
- Comp: glibc (macro gating); glibc + musl for the four absent ones. Kind: missing-2024.
- Src: https://sortix.org/os-test/include/dirent/

1.7 **O_SEARCH, O_EXEC, O_TTY_INIT undefined in glibc.**
- POSIX: `<fcntl.h>` shall define O_SEARCH and O_EXEC. O_TTY_INIT is also listed.
- glibc: undeclared. glibc only offers Linux's `O_PATH`, which has different semantics.
- musl: provides O_SEARCH and O_EXEC (both mapped to O_PATH) since 0.9.5/0.9.6, and O_TTY_INIT since 1.1.23.
- Comp: glibc. Kind: deliberate (glibc).
- Observe: these do not compile under glibc:
  - `#ifdef O_SEARCH`
  - `fexecve` via an `O_EXEC` fd
- Src:
  - https://sortix.org/os-test/include/fcntl/
  - musl WHATSNEW

1.8 **F_OFD_* locks and `F_SETOWN_EX` / `struct f_owner_ex` are extension-only in glibc.**
- Issue 8 standardizes them.
- glibc exposes them only with `_GNU_SOURCE`.
- `f_owner_ex` / `F_OWNER_PID` / `F_OWNER_PGRP` are extension-only in musl too.
- Comp: glibc, musl. Kind: missing-2024 (namespace).
- Src: https://sortix.org/os-test/include/fcntl/

1.9 **`struct flock` member types differ in glibc (os-test "incompatible").**
- The os-test header-analysis post lists glibc `struct flock` `l_type`/`l_whence` as not matching the required `short`.
- Unconfirmed on x86-64, where glibc uses `short`. It may be architecture-specific.
- Comp: glibc. Src: https://sortix.org/blog/os-testing-posix-headers/

1.10 **`<unistd.h>` and `<stdlib.h>` do not define the O_* flags that Issue 8 requires there.**
- POSIX 2024 requires these headers to define some O_* flags:
  - `<unistd.h>`: O_CLOEXEC and O_CLOFORK, for `pipe2`/`dup3`.
  - `<stdlib.h>`: O_APPEND, O_CLOEXEC, O_SYNC, O_DSYNC, O_RSYNC, O_NOCTTY and O_RDWR, for `mkostemp`/`posix_openpt`.
- Both libcs leave them undeclared unless `<fcntl.h>` is included.
- Comp: glibc, musl. Kind: missing-2024.
- Observe: compile a TU that includes only `<stdlib.h>` and uses `O_CLOEXEC`.
- Src:
  - https://sortix.org/os-test/include/unistd/
  - https://sortix.org/os-test/include/stdlib/

1.11 **`<semaphore.h>` (glibc) and `<sys/mman.h>` (both) do not define O_CREAT, O_EXCL and the other O_* flags.**
- Issue 8 requires `sem_open`/`shm_open` constants in those headers.
- The `<semaphore.h>` gap is glibc only; musl is fine there. The `<sys/mman.h>` gap is in both.
- Comp: glibc, musl. Src:
  - https://sortix.org/os-test/include/semaphore/
  - https://sortix.org/os-test/include/sys_mman/

1.12 **SEEK_DATA / SEEK_HOLE are extension-only in glibc.**
- Standardized in Issue 8.
- glibc needs `_GNU_SOURCE`. musl exposes them since 1.2.3.
- Comp: glibc. Src:
  - https://sortix.org/os-test/include/unistd/
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
  - On a pty master after the slave closes, poll returns `POLLIN|POLLOUT|POLLERR|POLLHUP` (os-test `pty-hup-poll`).
  - On a UDP socket connected to itself after send and `shutdown(SHUT_RDWR)`, poll returns `POLLIN|POLLOUT|POLLHUP` (os-test `bind-connect-self-send-shutdown-rw-poll`).
- Comp: kernel. Kind: long-standing.
- Src:
  - https://sortix.org/os-test/pty/
  - https://sortix.org/os-test/udp/
  - local `functions/poll.html`

1.16 **`ppoll()` edge cases.**
- os-test signal suite, bad on both Linux and musl:
  - `ppoll-block-raise-write` returns "0 | POLLIN".
  - `ppoll-block-close-raise` returns "0 | POLLNVAL".
- In other words, ppoll reports readiness or POLLNVAL caused by actions taken inside the signal handler that interrupted it, rather than returning -1/EINTR.
- The exact POSIX expectation is os-test's interpretation. Treat it as "Linux returns events, not EINTR, when the handler changes fd state".
- Comp: kernel.
- Src: https://sortix.org/os-test/signal/

1.17 **`unlink()` on a directory fails with EISDIR, not EPERM.**
- POSIX: [EPERM] when the file is a directory and the implementation does not allow `unlink` of directories.
- Linux: EISDIR, "the non-POSIX value returned by Linux since 2.1.132".
- Comp: kernel. Kind: deliberate.
- Observe: `mkdir d; unlink("d")` gives errno EISDIR.
- Src: https://raw.githubusercontent.com/mkerrisk/man-pages/master/man2/unlink.2

1.18 **`futimens(fd, NULL)` permission check.**
- POSIX: a process with *write access to the file* may set both timestamps to now.
- Linux: `futimens()` checks whether the *file descriptor's access mode* allows writing. A non-owner who has write permission but opened O_RDONLY gets EACCES.
- Comp: kernel. Kind: long-standing.
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
- Src:
  - https://sortix.org/os-test/limits/
  - local `basedefs/limits.h.html`

1.23 **New Issue 8 `pathconf`/`sysconf`/`confstr` names are missing.**
- These are undeclared in both libcs:
  - `_PC_FALLOC`, `_PC_TEXTDOMAIN_MAX`, `_PC_TIMESTAMP_RESOLUTION`
  - all `_CS_POSIX_V8_*` and `_CS_V8_ENV`
  - `_POSIX_FALLOC`
- Comp: glibc, musl. Kind: missing-2024.
- Src: https://sortix.org/os-test/include/unistd/

1.24 **`NSIG_MAX`, `GETENTROPY_MAX` and `_SC_NSIG` are absent.**
- Issue 8 `<limits.h>` adds `NSIG_MAX` and `GETENTROPY_MAX`. Both are undeclared in glibc and musl.
- `_POSIX_SS_REPL_MAX` is undeclared in glibc.
- Comp: glibc, musl. Src:
  - https://sortix.org/os-test/include/limits/
  - https://sortix.org/os-test/limits/

1.25 **`<aio.h>` `lio_listio` prototype is incompatible in glibc.**
- The glibc prototype uses `struct aiocb *const restrict *restrict`, which does not match POSIX.
- Comp: glibc. Observe: a pointer-to-function-type test fails to compile.
- Src:
  - https://sortix.org/os-test/include/aio/
  - https://sortix.org/blog/os-testing-posix-headers/

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
- Linux: after the only child in the group calls `setpgid()`/`setsid()` to leave it, `waitpid(-pgid, ...)` keeps blocking. os-test killed it with SIGALRM.
- Comp: kernel. Kind: long-standing.
- Observe: os-test tests `waitpid-pgid-empty-on-setpgid`, `-on-setpgid-rejoin` and `-on-setsid` fail on both Linux and musl.
- Src: https://sortix.org/os-test/process/

2.2 **`kill(-1, sig)` does not signal the calling process.**
- POSIX: send to all processes the caller may signal, excluding only an unspecified set of system processes.
- Linux: the caller is excluded.
- Comp: kernel. Kind: deliberate.
- Observe: dangerous to test. Run it in a PID namespace or container.
- Src: https://raw.githubusercontent.com/mkerrisk/man-pages/master/man2/kill.2 (NOTES)

2.3 **The nice value is per-thread, not per-process.**
- POSIX: the nice value is a process attribute shared by all threads, for `nice`, `setpriority` and `getpriority`.
- Linux/NPTL: per-thread. `setpriority(PRIO_PROCESS, 0, n)` affects only the calling thread. pthreads(7) calls this "at least one nonconformance" that remains in NPTL.
- Comp: kernel (glibc and musl do not compensate). Kind: deliberate/long-standing.
- Observe: thread A calls `nice(5)`; thread B calls `getpriority(PRIO_PROCESS, 0)` and still gets 0.
- Src:
  - https://raw.githubusercontent.com/mkerrisk/man-pages/master/man2/getpriority.2 (BUGS)
  - https://raw.githubusercontent.com/mkerrisk/man-pages/master/man7/pthreads.7

2.4 **Credentials are per-thread in the kernel.**
- POSIX: all threads share uid/gid/groups.
- Linux kernel: credentials are per-thread. glibc NPTL emulates process-wide credentials by broadcasting the change to every thread with a real-time signal (`SIGSETXID`). musl does the same; a sequencing bug in its multithreaded set*id was fixed in 1.2.5.
- Comp: kernel (root), handled by glibc and musl. Kind: deliberate.
- Observe: conformant through libc wrappers. A test that uses `syscall(SYS_setuid, ...)` sees per-thread behaviour.
- Fixed: musl race fixed in 1.2.5 (musl 1.2.4 "multithreaded set*id could malfunction").
- Src:
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
- Src:
  - https://raw.githubusercontent.com/mkerrisk/man-pages/master/man2/sched_setscheduler.2 (BUGS)
  - https://git.musl-libc.org/cgit/musl/tree/src/sched/sched_setscheduler.c

2.7 **`tcsetpgrp()` error semantics.**
- os-test pty suite on both Linux and musl:
  - `tcsetpgrp-wrong-member`: the call succeeds ("exit: 0") where an error was expected. POSIX requires [EPERM] when the pgid is not in the caller's session.
  - `tcsetpgrp-wrong-orphan`: gives ENOTTY.
  - `tcsetpgrp-wrong-pid`: ESRCH, graded "unknown".
- The precise scenarios are os-test's. Treat Linux `tcsetpgrp` error codes as unreliable for negative tests.
- Comp: kernel.
- Src: https://sortix.org/os-test/pty/

2.8 **`posix_spawn_file_actions_addchdir()` / `addfchdir()` are missing.**
- POSIX: new in Issue 8 (SPN option).
- glibc: only `posix_spawn_file_actions_addchdir_np` and `_addfchdir_np`, added in 2.29 as "GNU extensions". The un-suffixed names are undeclared even in the newer glibc os-test used.
- musl: also undeclared, although its `posix_spawn` implements the chdir/fchdir actions internally.
- Comp: glibc, musl. Kind: missing-2024.
- Src:
  - https://sortix.org/os-test/include/spawn/
  - glibc NEWS 2.29: https://raw.githubusercontent.com/bminor/glibc/master/NEWS
  - https://git.musl-libc.org/cgit/musl/tree/src/process/posix_spawn.c

2.9 **`POSIX_SPAWN_SETSID` is extension-only in glibc.** The glibc flag exists but needs `_GNU_SOURCE`. musl has had it since 1.1.17. Src: https://sortix.org/os-test/include/spawn/

2.10 **`_Fork()`.** glibc has had it since 2.34, but it is extension-gated (os-test `extension`). musl has had it since 1.2.2. Src: os-test include/unistd; glibc NEWS 2.34; musl WHATSNEW 1.2.2.

2.11 **`WCOREDUMP` is extension-only in glibc.** It is standardized in Issue 8. Src: https://sortix.org/os-test/include/sys_wait/

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
- os-test basic/signal: `sig2str` undeclared and `str2sig` compile_error on both.
- Comp: glibc, musl. Kind: missing-2024. Fixed: not in glibc through 2.43 per NEWS.
- Src:
  - https://sortix.org/os-test/basic/signal/
  - https://sortix.org/os-test/include/signal/

3.2 **musl `sigismember()` does not fail on a negative signal number.**
- POSIX: [EINVAL] for an invalid signal number.
- musl: no error. glibc: passes.
- Comp: musl. Observe: `sigismember(&s, -1)` returns 0/1 instead of -1/EINVAL.
- Src: https://sortix.org/os-test/basic/signal/

3.3 **`abort()` is not async-signal-safe in glibc < 2.41.**
- POSIX lists `abort` as async-signal-safe.
- glibc bug 26275 ("abort(3) is not signal-safe") was fixed in **2.41**, so 2.39 is affected. This will not appear in os-test's newer-glibc column.
- Comp: glibc. Observe: hard to observe deterministically. A deadlock is possible when `abort` is called from a handler while another `abort`/stdio lock is held.
- Src: glibc NEWS 2.41 (bug list).

3.4 **`sigaltstack()` accepts `SS_ONSTACK` in `ss_flags` as a no-op.** POSIX uses `SS_ONSTACK` only as an output flag. This is an extension; do not test for EINVAL. Src: sigaltstack.2 BUGS.

3.5 **`sigaction-exec-flags`.** os-test grades `sigaction` flags across exec as "unknown" ("0") on both Linux and musl. This is an unclear area; avoid asserting it. Src: https://sortix.org/os-test/signal/

3.6 **The hardware-exception signal is architecture-dependent.**
- For example, x86 `int` with a bad vector gives SIGSEGV, not SIGILL.
- Tests that trigger faults should accept SIGSEGV or SIGBUS.
- Src: https://raw.githubusercontent.com/mkerrisk/man-pages/master/man7/signal.7 (BUGS)

---

## 4. Threads and synchronization

4.1 **`pthread_cond_clockwait`, `pthread_mutex_clocklock`, `pthread_rwlock_clockrdlock`, `pthread_rwlock_clockwrlock` and `sem_clockwait` are missing in musl.**
- POSIX: new in Issue 8.
- glibc: all present since **2.30**, but declared only under `_GNU_SOURCE` (os-test `extension`). basic tests pass at runtime.
- musl: missing in all versions tested, including 1.2.6. They are compile_error in os-test basic/pthread and basic/semaphore.
- Comp: musl (missing); glibc (gating). Kind: missing-2024.
- Src:
  - https://sortix.org/os-test/basic/pthread/
  - https://sortix.org/os-test/basic/semaphore/
  - https://sortix.org/os-test/include/pthread/
  - glibc NEWS 2.30

4.2 **`PTHREAD_NULL` is undefined in glibc.** It is new in Issue 8. musl has had it since 1.2.1. Src: https://sortix.org/os-test/include/pthread/; musl WHATSNEW.

4.3 **musl declares `pthread_mutexattr_getprioceiling`/`setprioceiling` but they do not link.** Src: https://sortix.org/blog/os-testing-posix-headers/

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

4.7 **`thread_local` (`<threads.h>`) and `static_assert` (`<assert.h>`) are reported undeclared for glibc by os-test.**
- This is probably a C23 artefact, because they are keywords in C23 and glibc does not define the macros then. Verify with your compiler mode before treating it as a deviation.
- Src:
  - https://sortix.org/os-test/include/threads/
  - https://sortix.org/os-test/include/assert/

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
- Src: https://sortix.org/os-test/include/time/

5.2 **musl < 1.2.6 `strptime` does not accept the conversion specifiers added in Issue 8.**
- The new specifiers include `%s`, `%z`, `%Z`, `%F` and `%V` (list per the sortix POSIX-2024 post; unconfirmed which ones musl lacked).
- Fixed: musl **1.2.6**.
- Src: musl WHATSNEW 1.2.6; https://sortix.org/blog/posix-2024/

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
- Comp: glibc. Fixed: **2.42**, so 2.39 is affected and the os-test column will not show it.
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
- On failure, Issue 8 requires `*strp` to be handled in a specified way. os-test saw differences, all graded good:
  - glibc sets it to NULL.
  - musl leaves the old value.
  - glibc reports ENOMEM where musl reports EOVERFLOW.
- Src:
  - https://sortix.org/os-test/include/stdio/
  - https://sortix.org/os-test/stdio/

6.4 **`swprintf` error code when output does not fit.**
- os-test basic/wchar wants `swprintf` to fail with EOVERFLOW.
- glibc sets **E2BIG**. musl leaves **errno == 0**.
- Comp: glibc, musl. Observe: `swprintf(buf, 2, L"abc")` returns -1, then check errno.
- Src: https://sortix.org/os-test/basic/wchar/

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
- Src:
  - https://raw.githubusercontent.com/mkerrisk/man-pages/master/man3/fmemopen.3
  - open_memstream.3

6.10 **glibc `initstate()` does not return NULL on error.** POSIX requires NULL. glibc sets errno but returns non-NULL. Comp: glibc. Src: https://raw.githubusercontent.com/mkerrisk/man-pages/master/man3/random.3 (BUGS)

6.11 **`encrypt()` / `setkey()` are not available in glibc.**
- POSIX: still in Issue 8 (XSI, obsolescent).
- glibc **2.28** made them unavailable to newly linked binaries ("no longer available"). os-test basic/unistd `encrypt` and basic/stdlib `setkey` give compile_error on Linux.
- musl provides them.
- Comp: glibc. Kind: deliberate.
- Src:
  - https://sortix.org/os-test/basic/unistd/
  - https://sortix.org/os-test/basic/stdlib/
  - glibc NEWS 2.28

6.12 **`strfmon()` in the POSIX locale prints a decimal point.**
- os-test expected `"%foo90142bar-13710quxf42010 "`.
- glibc gave `"%foo901.42bar-137.10qux f42.010 "` and musl gave `"%foo901.42bar-137.10qux 42.010"`.
- My reading is that in the POSIX locale `mon_decimal_point` is "", so no '.' should appear. Field/padding handling also differs.
- Both `strfmon` and `strfmon_l` fail on both libcs.
- Comp: glibc, musl.
- Src: https://sortix.org/os-test/basic/monetary/

6.13 **`mkostemp`, `qsort_r`, `reallocarray`, `secure_getenv`, `realpath` (os-test), `ptsname_r` are extension-only.**
- os-test grades them `extension` in `<stdlib.h>`. The symbols exist, but glibc needs `_DEFAULT_SOURCE`/`_GNU_SOURCE`, and musl gates `qsort_r`, `reallocarray` and `secure_getenv` similarly.
- Versions:
  - `qsort_r`: glibc 2.8, with the same argument order as POSIX 2024; musl **1.2.3**.
  - `reallocarray`: glibc 2.26; musl 1.2.2.
  - `mkostemp`: glibc 2.7; musl 0.9.10.
- Src: https://sortix.org/os-test/include/stdlib/; musl WHATSNEW; glibc NEWS.

6.14 **`memmem`, `strlcpy`, `strlcat` are extension-only.**
- `memmem` is extension-only in glibc. musl exposes it by default since **1.2.4**.
- `strlcpy`/`strlcat`: glibc added them in **2.38**. Both libcs gate them.
- Src: https://sortix.org/os-test/include/string/; glibc NEWS 2.38; musl WHATSNEW 1.2.4.

6.15 **`wcslcpy` / `wcslcat` missing in musl.** glibc has them, extension-gated. musl does not have them, even in 1.2.6 (compile_error). Src: https://sortix.org/os-test/basic/wchar/; https://sortix.org/os-test/include/wchar/

6.16 **`getopt()` permutes argv in glibc.**
- POSIX: `getopt` returns -1 at the first non-option argument.
- glibc: by default permutes argv so options after operands are parsed. It is POSIX only when `POSIXLY_CORRECT` is set or optstring starts with '+'.
- musl: stops at the first non-option, as POSIX requires.
- Comp: glibc. Kind: deliberate.
- Observe: argv = {"p","file","-a"} with optstring "a". glibc returns 'a'; POSIX returns -1 with optind=1.
- Src:
  - https://raw.githubusercontent.com/mkerrisk/man-pages/master/man3/getopt.3
  - musl wiki

6.17 **`basename()` / `strerror_r()` GNU variants.**
- With `_GNU_SOURCE`, glibc's `<string.h>` `basename` and `strerror_r` have GNU semantics (`strerror_r` returns `char *`). Not an issue in strict POSIX mode, but beware if the test suite defines `_GNU_SOURCE` globally.
- musl < 1.2.5 provided a non-prototype `basename` in `string.h`.
- Src: musl wiki; musl WHATSNEW 1.2.5; strerror.3.

6.18 **The `<inttypes.h>` `wcstoimax`/`wcstoumax` declarations need `wchar_t`, which glibc does not provide in strict mode.** os-test reports `unknown_type` for glibc. Src: https://sortix.org/os-test/include/inttypes/

6.19 **`<endian.h>` in glibc.**
- POSIX: new in Issue 8. It shall define the byte-order macros, `be16toh` and the related functions, and the `uint16_t`/`uint32_t`/`uint64_t` types.
- glibc: all macros and functions are `extension`, and the types are not declared (`unknown_type`).
- musl: fully good since it aimed at "future POSIX" in 1.2.0.
- Comp: glibc. Src: https://sortix.org/os-test/include/endian/

---

## 7. Locale, wide characters, iconv, message catalogs

7.1 **glibc's C/POSIX locale is 7-bit.**
- POSIX.1-2024 §6.2: the POSIX locale has 256 single-byte characters, so every byte value is a character.
- glibc: in the C locale, `mbrtowc`/`mbtowc` reject bytes 0x80-0xFF with EILSEQ. Yet `regex`, `fnmatch` and `glob` accept them, per the musl wiki.
- musl: since 1.1.11 the C locale maps 0x80-0xFF to individual abstract characters.
- Comp: glibc. Kind: long-standing.
- Fixed: not found in glibc NEWS through 2.43, so unconfirmed whether still true. Test `mbrtowc` on "\x80" in the C locale.
- Src:
  - https://wiki.musl-libc.org/functional-differences-from-glibc.html
  - local `basedefs/V1_chap06.html`

7.2 **glibc `iconv()` fails with EILSEQ for valid-but-unrepresentable characters.**
- POSIX.1-2024: with no indicator suffix, `iconv` shall perform an implementation-defined conversion and count it in the return value as an irreversible conversion. It must not fail.
- glibc: returns EILSEQ. musl substitutes '*' and counts it, which conforms.
- Comp: glibc. Kind: deliberate.
- Observe: UTF-8 "é" converted to ASCII without suffix. glibc gives -1/EILSEQ.
- Src:
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

7.5 **`getlocalename_l()` is missing in both libcs.** It is new in Issue 8 (compile_error/undeclared). Src: https://sortix.org/os-test/basic/locale/; https://sortix.org/os-test/include/locale/

7.6 **`ALTMON_x` / `ABALTMON_x` (`<langinfo.h>`).**
- `ABALTMON_1..12` are undeclared in both libcs. glibc has only the nonstandard `_NL_ABALTMON_*`.
- `ALTMON_1..12` are extension-only in glibc (since 2.27) and undeclared in musl.
- Comp: glibc, musl. Kind: missing-2024.
- Src: https://sortix.org/os-test/include/langinfo/; glibc NEWS 2.27

7.7 **`<libintl.h>` `*_l` variants are missing.**
- Issue 8 adds `gettext_l`, `dgettext_l`, `dcgettext_l`, `ngettext_l`, `dngettext_l` and `dcngettext_l`.
- They are missing from both libcs.
- In musl, the base gettext functions are graded "outside_libc".
- musl 1.2.6 fixed a `bind_textdomain_codeset` return-value bug.
- Src: https://sortix.org/os-test/include/libintl/; musl WHATSNEW 1.2.6

7.8 **musl `nl_types` (`catopen`/`catgets`) fails at runtime.** os-test basic `nl_types` scores 0/3 on musl and 3/3 on glibc. Src: https://sortix.org/os-test/basic/

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

Source for most items: os-test regex and fnmatch suites (https://sortix.org/os-test/regex/, https://sortix.org/os-test/fnmatch/). The grading is os-test's interpretation of POSIX.

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

8.6 **`REG_MINIMAL` is undeclared in both libcs.** It is new in Issue 8. Src: https://sortix.org/os-test/include/regex/

8.7 **glibc `regoff_t` is `int` (32-bit) even on LP64.**
- POSIX requires `regoff_t` to hold the largest value in `ssize_t` or `ptrdiff_t`, so matches beyond 2 GiB break.
- musl uses the correct type.
- Comp: glibc. Kind: deliberate (ABI).
- Src: musl wiki.

8.8 **musl BRE GNU extensions.** These are not POSIX. musl lacked `\|`, `\+` and `\?` in BRE until 1.1.13, and rejects `a{,3}`. Do not test them as POSIX. Src: musl wiki; os-test regex (nonstandard section).

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
- os-test expects rejection. musl rejects.
- Comp: glibc.

8.13 **musl fnmatch bracket expressions.**
- No collating symbols or equivalence classes (`[[.a.]]` and `[[=a=]]` fail), including UTF-8 ones.
- Backslash handling inside brackets is wrong: `[x\y]` matches "\" and `[a\-b]` does not match "-".
- `[foo\]` does not match "[foo]".
- Reversed ranges (`[d-b]`) are not rejected.
- `[[...][./.]]` periodslash cases fail, and `\.` with FNM_PERIOD fails.
- Comp: musl.

8.14 **`FNM_IGNORECASE` is undeclared in both; `FNM_CASEFOLD` is extension-only in glibc.** Both are new in Issue 8. Src: https://sortix.org/os-test/include/fnmatch/

8.15 **musl < 1.2.5 `glob` could wrongly return GLOB_NOMATCH if aborted before any matches.** Fixed in 1.2.5. Src: musl WHATSNEW 1.2.5.

8.16 **glob(7) on Linux.** POSIX requires an unmatched or syntactically invalid pattern to be left unchanged in the shell; the man page notes this. This is a shell-level requirement (`GLOB_NOCHECK` in `glob()`), not a deviation by itself. Src: https://raw.githubusercontent.com/mkerrisk/man-pages/master/man7/glob.7

---

## 9. Terminals and pseudo-terminals

9.1 **`tcgetwinsize()` / `tcsetwinsize()` / `struct winsize` (in `<termios.h>`) are missing in glibc.**
- POSIX: new in Issue 8.
- glibc: not declared (compile_error). glibc only has `TIOCGWINSZ` ioctl and `struct winsize` in `<sys/ioctl.h>`.
- musl: has them since **1.2.2**.
- Comp: glibc. Kind: missing-2024.
- Src:
  - https://sortix.org/os-test/basic/termios/
  - https://sortix.org/os-test/include/termios/
  - musl WHATSNEW 1.2.2

9.2 **`ptsname_r` is extension-only in both libcs.**
- It is new in Issue 8 (XSI). glibc needs `_GNU_SOURCE`.
- glibc ≤2.25 returned EINVAL for buf NULL. musl < 1.0.0 returned negated error codes.
- Src: https://sortix.org/os-test/include/stdlib/; ptsname.3; musl WHATSNEW

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
- Src:
  - https://sortix.org/os-test/include/sys_socket/
  - https://raw.githubusercontent.com/mkerrisk/man-pages/master/man2/recv.2 (NOTES)

10.2 **`accept4`, `SO_DOMAIN`, `SO_PROTOCOL` are extension-only in glibc.** They are standardized in Issue 8. glibc needs `_GNU_SOURCE`; musl exposes them. Src: https://sortix.org/os-test/include/sys_socket/

10.3 **`NI_NUMERICSCOPE` is undeclared in glibc.** Src: https://sortix.org/os-test/include/netdb/

10.4 **UDP edge cases.**
- After `shutdown(SHUT_RD)`, a later `send` to self can still be received; os-test grades `bind-connect-self-shutdown-r-send-recv` "bad: x" on both.
- Many `SO_REUSEADDR` and broadcast bind-conflict cases are graded "unknown". Avoid strict assertions there.
- Comp: kernel. Src: https://sortix.org/os-test/udp/

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

10.9 **Typed memory objects are not supported.** `posix_typed_mem_open` and related interfaces (TYM option) are absent, which is fine because the option is optional. `S_TYPEISTMO` is undeclared in glibc. Src: https://sortix.org/os-test/include/sys_mman/; https://sortix.org/os-test/include/sys_stat/

10.10 **`<devctl.h>` / `posix_devctl` is missing** (optional DC). Src: https://sortix.org/os-test/namespace/

10.11 **AF_UNIX `sun_path`.** Linux appends a NUL if one is not supplied, so a 108-byte path round-trips without a terminator. Src: unix.7 BUGS.

---

## 11. Header namespace and the Issue 8 feature-test macro

11.1 **glibc does not honour `_POSIX_C_SOURCE=202405L` / `_XOPEN_SOURCE=800` as "POSIX 2024".**
- Nearly every Issue 8 interface that glibc *does* have is reported by os-test as `extension`, meaning it is visible only with `_DEFAULT_SOURCE`/`_GNU_SOURCE`:
  - `endian.h`, `accept4`, `asprintf`, `memmem`, `strlcpy`, `mkostemp`, `qsort_r`, `reallocarray`
  - `pthread_*clock*`, `sem_clockwait`, `ppoll`
  - `SEEK_DATA`, `F_OFD_*`, `POSIX_SPAWN_SETSID`, `WCOREDUMP`, `tm_gmtoff`/`tm_zone`, `DT_*`, `LONG_BIT`, `NL_*MAX`, `ALTMON_*`
- glibc NEWS through 2.43 contains no entry adding POSIX.1-2024 feature-macro support.
- Kind: missing-2024.
- Practical advice:
  - Compile with `-D_GNU_SOURCE` on glibc, or treat `extension` as a soft failure.
  - Define nothing, or `_DEFAULT_SOURCE`, on musl.
  - musl needs 19 feature-macro adjustments, per the os-test blog.
- Src:
  - https://sortix.org/blog/os-testing-posix-headers/
  - https://sortix.org/os-test/include/

11.2 **musl < 1.2.5: `ppoll` is not exposed in the default feature profile.** In 1.2.4 it needs `_GNU_SOURCE`/`_BSD_SOURCE`. Src: musl WHATSNEW 1.2.5.

11.3 **Namespace pollution.**
- os-test namespace suite: both libcs leak non-reserved identifiers from these headers:
  - `<arpa/inet.h>`, `<netdb.h>`, `<netinet/in.h>`, `<pthread.h>`, `<signal.h>`, `<stdlib.h>`
  - `<sys/resource.h>`, `<sys/socket.h>`, `<sys/wait.h>`, `<threads.h>`, `<time.h>`
- musl additionally pollutes `<aio.h>`, `<libintl.h>`, `<wchar.h>` and `<spawn.h>`.
- A namespace-cleanliness test will fail on both.
- Src: https://sortix.org/os-test/namespace/

11.4 **`dladdr` / `Dl_info_t`.**
- POSIX Issue 8 names the type `Dl_info_t` (`Dl_info` was added by a later fix).
- Both libcs have only `Dl_info`, extension-gated. `Dl_info_t` and the `dladdr` test give `unknown_type`.
- Src: https://sortix.org/os-test/include/dlfcn/

11.5 **`struct stat` `st_dev` and `struct statvfs` `f_fsid` use glibc-internal types.** os-test flags 16 "incompatible" declarations in glibc in total. Src: https://sortix.org/blog/os-testing-posix-headers/

11.6 **`LONG_BIT` and the `NL_ARGMAX`/`NL_MSGMAX`/`NL_SETMAX`/`NL_TEXTMAX` limits are extension-only in glibc.** Src: https://sortix.org/os-test/include/limits/

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

- os-test results:
  - https://sortix.org/os-test/
  - suites: io, process, signal, pty, regex, fnmatch, limits, paths, udp, stdio, basic, include, namespace
  - per-header pages under `include/` and `basic/` as cited above
- os-test blog:
  - https://sortix.org/blog/os-testing-posix-headers/
  - https://sortix.org/blog/posix-2024/
  - https://sortix.org/blog/
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
