# Issues in the standard: index

Copyright (c) 2026 Nicholas Martin. All rights reserved.

Inconsistencies, apparent mistakes and unclear points found in POSIX.1-2024
while writing posixcheck. The full write-up, for raising with The Austin
Group, is `standard-issues-report.md` (a shared copy:
https://claude.ai/artifact/L3t5goEQYUJKhbTWAj2BXF)

An assertion whose verdict depends on one of these names its ID in its
`note`. Add a new entry to the report first, then list it here.

| ID | Where | Kind | Assertions |
| --- | --- | --- | --- |
| SI-1 | write() ERRORS: ECONNRESET worded as ENOTCONN | defect | XSH.write.027 |
| SI-2 | write() vs send() EPIPE: SIGPIPE for SOCK_SEQPACKET | conflict | XSH.write.026 |
| SI-3 | pread()/pwrite() EINVAL: "offset unchanged" sentence | editorial | XSH.read.017, XSH.write.024 |
| SI-4 | fcntl() F_DUPFD at {OPEN_MAX}: EINVAL and EMFILE overlap | unclear | XSH.fcntl.005 |
| SI-5 | true APPLICATION USAGE on "--" vs XCU 1.4 and false | conflict | (todo in true.cov, false.cov) |
| SI-6 | fork() still resets XSI interval timers, removed in Issue 8 | defect | (todo in fork.cov) |
| SI-7 | fork() process-shared lock item lacks TSH shading | editorial | XSH.fork.014 |
| SI-8 | wait(): DESCRIPTION requires discarding SIGCHLD, RATIONALE treats it as optional | conflict | XSH.wait.019 |
| SI-9 | XSH 2.12 unshaded vs XSI shading elsewhere: SIG_IGN/SA_NOCLDWAIT discard status | conflict | XSH._Exit.009 |
| SI-10 | `<signal.h>` CHANGE HISTORY: SA_NOCLDWAIT moved to Base, still XSI-shaded | editorial | XBD.signal.h.017 (SA_NOCLDWAIT behaviour todo in _Exit.cov) |
| SI-11 | wait() RETURN VALUE: WNOHANG sentence lacks "shall", overlapping conditions | editorial | XSH.wait.010 |
| SI-12 | exit(): "active handle to its underlying file descriptor" (description is meant) | editorial | XSH.exit.010 |
| SI-13 | XSH 2.5.1: abort() and signals count as closing a stream | conflict | (not tested) |
| SI-14 | fexecve() EBADF "open for executing" vs APPLICATION USAGE (O_RDONLY is fine) | unclear | XSH.exec.048, XSH.exec.049 |
| SI-15 | exec: "saved resource limits" defined nowhere | defect | (exec.cov) |
| SI-16 | exec: PTHREAD_CANCEL_ENABLED (no such symbol) | editorial | XSH.exec.030 |
| SI-17 | fexecve(): no-execute EACCES excluded in ERRORS, promised in APPLICATION USAGE | conflict | (not tested) |
| SI-18 | exec: "regular, executable file" vs EACCES for unsupported file types | conflict | XSH.exec.036 |
| SI-19 | exec EINVAL: a file "has appropriate privileges" | editorial | (not tested) |
| SI-20 | exec inherited list: file size limit twice, a "process signal mask" | editorial | XSH.exec.018, XSH.exec.025 |
| SI-21 | sigaction() SA_RESETHAND: informative SIGILL/SIGTRAP note contradicts the reset rule | conflict | (sigaction.cov) |
| SI-22 | sigaction(): signal blocked during an SA_RESETHAND handler? three statements disagree | conflict | (sigaction.cov) |
| SI-23 | sigaction(): non-XSI SA_SIGINFO "may fail" names no errno | defect | (sigaction.cov) |
| SI-24 | queuing with SA_SIGINFO outside SIGRTMIN-SIGRTMAX: sigaction/2.4.2/sigqueue vs <signal.h> | conflict | XSH.sigaction.021 (disputed) |
| SI-25 | sigaction() SA_NOCLDSTOP: "if the implementation supports SIGCHLD" | editorial | XSH.sigaction.007 |
| SI-26 | sigaction(): SA_ONSTACK and handler mask worded per process | editorial | XSH.sigaction.009, XSH.sigaction.014 |
| SI-27 | raise() ERRORS vs its pthread_kill() equivalence (sig 0, unsupported) | conflict | XSH.raise.003 |
| SI-28 | kill() CHANGE HISTORY: "calling" process where the receiver is meant | editorial | XSH.kill.014 |
| SI-29 | kill(): "a signal" vs "that signal" for the pid cases | editorial | (not tested) |
| SI-30 | killpg() (and in two 2.4.3 sentences pthread_kill()) missing from the generating-function lists | editorial | XSH.pthread_sigmask.011 |
| SI-31 | pthread_sigmask(): no-EINTR sentence covers sigprocmask() | editorial | (not tested) |
| SI-32 | sigwaitinfo()/sigtimedwait(): wait ends on a caught signal, but [EINTR] is may-fail | editorial | (not tested) |
| SI-33 | sigwait(): leftover "if the implementation supports queued signals" | editorial | XSH.sigwait.003 |
| SI-34 | sigwait(): implementation-defined remainder sentence overlaps the queued-signal rule | editorial | XSH.sigwait.003 |
| SI-35 | sigqueue(): "the pending, unblocked signal" vs kill()'s "at least one" | editorial | XSH.sigqueue.003 |
| SI-36 | sigtimedwait() RATIONALE: a "value" argument that does not exist | editorial | (not tested) |
| SI-37 | sigtimedwait() CHANGE HISTORY: RETURN VALUE named for an ERRORS sentence | editorial | (not tested) |
| SI-38 | alarm() SEE ALSO: empty link to itself | editorial | (not tested) |
| SI-39 | <signal.h> si_code table: option spans opened but not closed | editorial | XBD.signal.h.024 |
| SI-40 | <limits.h>: {NSIG_MAX} has no minimum | editorial | XBD.signal.h.007 |
| SI-41 | <signal.h>: SIGRTMIN/SIGRTMAX "declared"; non-overlap without "shall" | editorial | XBD.signal.h.006 |
| SI-42 | XSH 2.4.3: si_addr still the faulting instruction's address | editorial | XSH.2.4.3-signal-actions.015 |
| SI-43 | XSH 2.4.3 SIG_DFL: queued-values sentence's scope and wording | editorial | XSH.2.4.3-signal-actions.010 |
| SI-44 | XSH 2.4.1: a sigwait() thread has the signal "delivered" | editorial | XSH.sigwait.006 |
| SI-45 | sigaddset() etc.: "shall" vs "should"; effects without "shall" | editorial | XSH.sigaddset.001, XSH.sigdelset.001 |
| SI-46 | sigaltstack(): "in some implementations ... shall" | editorial | (impl) |
| SI-47 | sigaltstack(): ENOMEM has no exception for SS_DISABLE | editorial | XSH.sigaltstack.011 (disputed) |
| SI-48 | signal(): the errno rule is not CX-shaded | editorial | XSH.signal.006 |
| SI-49 | str2sig(): "a string containing" | editorial | (not tested) |
| SI-50 | <stdio.h>: dprintf() prototype lacks its semicolon | editorial | (gen/specmodel.py) |
| SI-51 | <float.h>: unclosed paragraphs in the HTML edition | editorial (HTML) | (gen/specmodel.py) |
| SI-52 | rename(): no error number for a directory renamed to "new/" | omission | XSH.rename.014 |
| SI-53 | lchown()/fchown(): ERRORS out of step with chown() | inconsistency | XSH.lchown.003 |
| SI-54 | truncate(): [EFBIG] names a "SIGFSZ" signal | editorial | XSH.truncate.008 |
| SI-55 | opendir(): ENOENT overlaps ENOTDIR | inconsistency | XSH.opendir.008 |
| SI-56 | ftruncate(): unspecified file types vs required EINVAL | inconsistency | XSH.ftruncate.010 |
| SI-57 | truncate(): out of step with ftruncate() | inconsistency | XSH.truncate.006 |
| SI-58 | umask(): only permission bits used vs other bits implementation-defined | inconsistency | XSH.umask.003 |
| SI-59 | utimes(): errors in tv_nsec terms; XSI shading | editorial | XSH.futimens.023 |
| SI-60 | utimes(): nearest-second rounding vs XBD 4.12 | inconsistency | XSH.futimens.018 |
| SI-61 | *at(): fd search EACCES not limited to relative paths | omission | XSH.futimens.007 |
| SI-62 | readdir(): _SC_NAME_MAX for fpathconf() | editorial | (none) |
| SI-63 | alphasort()/scandir(): RETURN VALUE wording | editorial | (none) |
| Q-1 | SIGPIPE after shutdown(SHUT_WR)? | question | XSH.write.017 |
| Q-2 | lseek() beyond the file system's largest file | question | XSH.lseek.011 |
| Q-3 | unnamed semaphore (pshared 0) in a fork child | question | (not tested) |
| Q-4 | is a zombie still a group member for "orphaned process group"? | question | XSH._Exit.010 (avoids it) |
| Q-5 | wait(): "if the implementation queues SIGCHLD" vs XSH 2.4.1 | question | XSH.wait.019 (both readings agree) |
| Q-6 | wait(): when is "another child's status available"? | question | (not tested) |
| Q-7 | exit(): streams with no file descriptor (memory streams) | question | (not tested) |
| Q-8 | XSH 2.3: ENAMETOOLONG/ENOTDIR always overlap ENOENT for lookups | question | XSH.exec.038, XSH.exec.041 |
| Q-9 | exec E2BIG: which {ARG_MAX} when sysconf() under-reports | question | XSH.exec.043 |
| Q-10 | execvp() sh fallback when argv[0] is a null pointer | question | (not tested) |
| Q-11 | raise()/pthread_kill(): delivery before return (raise() settled by its own rule) | question | XSH.raise.001 |
| Q-12 | kill() with the most negative pid_t | question | (not tested) |
| Q-13 | alarm(): rounding of the seconds returned | question | XSH.alarm.002, XSH.alarm.003 |
| Q-14 | alarm(): may SIGALRM come early? | question | XSH.alarm.001 |
| Q-15 | pause()/sigsuspend(): delivery to the calling thread? | question | XSH.sigsuspend.006 |
| Q-16 | {RTSIG_MAX}: the range's size or a lower bound? | question | XBD.signal.h.006 |
| Q-17 | 2.4.2: kill() occurrences vs sigqueue() FIFO order | question | XSH.2.4.2-realtime-signals.004 |
| Q-18 | 2.4.1/2.4.3: SIGCONT and stopped processes | question | XSH.2.4.3-signal-actions.006 |
| Q-19 | psiginfo(): may the description carry siginfo_t details? | question | XSH.psiginfo.003 (disputed) |
| Q-20 | sigaltstack(): must oss report SS_ONSTACK/SS_DISABLE? | question | XSH.sigaltstack.009 (disputed) |
| Q-21 | abort(): caught but blocked SIGABRT: must the handler run? | question | XSH.abort.006 (disputed) |
| Q-22 | <signal.h>: MINSIGSTKSZ constant vs CPU-dependent frames | question | XSH.sigaltstack.010 |
| Q-23 | signal(): return value after failed calls or sigaction() | question | XSH.signal.001 |
| Q-24 | link(): what access to the existing file may be required, and with which error | question | XSH.link.019 |
| Q-25 | chown(-1, -1) of another user's file | question | XSH.chown.008 |
| Q-26 | truncate(): file size limit when shrinking | question | XSH.truncate.007 |
| Q-27 | futimens(): write access of file or descriptor | question | XSH.futimens.021 |
| Q-28 | fflush(NULL) and input streams | question | XSH.fflush.011 |
