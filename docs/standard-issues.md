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
| SI-55 | opendir(): ENOENT overlaps ENOTDIR | inconsistency | XSH.opendir.008, .011, XSH.alphasort.006, .009 |
| SI-56 | ftruncate(): unspecified file types vs required EINVAL | inconsistency | XSH.ftruncate.010 |
| SI-57 | truncate(): out of step with ftruncate() | inconsistency | XSH.truncate.006 |
| SI-58 | umask(): only permission bits used vs other bits implementation-defined | inconsistency | XSH.umask.003 |
| SI-59 | utimes(): errors in tv_nsec terms; XSI shading | editorial | XSH.futimens.023 |
| SI-60 | utimes(): nearest-second rounding vs XBD 4.12 | inconsistency | XSH.futimens.018 |
| SI-61 | *at(): fd search EACCES not limited to relative paths | omission | XSH.futimens.007 |
| SI-62 | readdir(): _SC_NAME_MAX for fpathconf() | editorial | (none) |
| SI-63 | alphasort()/scandir(): RETURN VALUE wording | editorial | (none) |
| SI-64 | <signal.h>: si_code values stated, not required | ambiguity | XSH.2.4.3-signal-actions.020 |
| SI-65 | SIGBUS past a mapping's end has no si_code | omission | XSH.2.4.3-signal-actions.021 |
| SI-66 | SI_MESGQ shading | editorial | XSH.2.4.3-signal-actions.017 |
| SI-67 | fflush(): stale RATIONALE | inconsistency | XSH.fflush.004 |
| SI-68 | truncate(): application sentence against EACCES | inconsistency | XSH.truncate.011 |
| SI-69 | getrlimit(): stale RATIONALE on RLIMIT_FSIZE | editorial | none |
| SI-70 | sigaction(): stale APPLICATION USAGE on longjmp() | editorial | none |
| SI-71 | pthread_sigmask(): requirement on how | editorial | XSH.pthread_sigmask.010 |
| SI-72 | 2.5.1: "open file descriptor"; whose further actions count | editorial | XSH.2.5.1-stream-handles.012 |
| SI-73 | XSH fscanf(): "(except for %n)" and end-of-file | conflict | (fscanf.cov) |
| SI-74 | XSH fopen() and freopen(): the two ENAMETOOLONG conditions are swapped | inconsistency | XSH.fopen.025, XSH.freopen.020 |
| SI-75 | XSH strerror(): two statements about overwriting by strerror_l() | editorial | XSH.strerror.006, .007 |
| SI-76 | XSH gmtime(): tm_zone is set only in the gmtime_r() text | editorial | XSH.gmtime.003 |
| SI-77 | XSH mktime(): "shall attempt to determine" whether DST is in effect | editorial | (none) |
| SI-78 | XSH strftime(): %z with tm_isdst 0 and a DST tm_gmtoff | unclear | XSH.strftime.011 |
| SI-79 | XSH fmemopen(): the append position "one byte after the end of the buffer" | editorial | XSH.fmemopen.010 |
| SI-80 | XSH pthread_mutex_lock(): NORMAL relock "deadlock" against the may-fail [EDEADLK] | unclear | XSH.pthread_mutex_lock.011 |
| SI-81 | XSH sysconf(): the table names _POSIX_SS_REPL_MAX for a limit | editorial | XSH.sysconf.002 |
| SI-82 | XSH fchdir() and readlink(): wording slips | editorial | (none) |
| SI-83 | XSH fprintf(): wording gaps | editorial | XSH.fprintf.026 |
| SI-84 | XBD 3.253 against _Exit(): when a child's parent changes | conflict | XSH.getppid.002 |
| SI-85 | XSH getgroups(): CHANGE HISTORY says 0 is not permitted, DESCRIPTION allows it | conflict | XSH.getgroups.001 to .003 |
| SI-86 | XSH setreuid(), setregid(), setgid(): wording slips | editorial | (none) |
| SI-87 | XSH nanosleep() against clock_getres(): measured by CLOCK_REALTIME, or independent of it | unclear | XSH.nanosleep.001, .003, .006, .008 |
| SI-88 | XSH clock_getres() and clock_nanosleep(): wording slips and an overlap | editorial | XSH.clock_getres.008, .009, XSH.clock_nanosleep.005, .008 |
| SI-89 | XSH waitid(): options with none of WEXITED, WSTOPPED, WCONTINUED: EINVAL not stated (waitid.html#DESCRIPTION, #ERRORS/EINVAL) | unclear | XSH.waitid.018 |
| SI-90 | XSH popen(): the FD_CLOFORK sentence for mode "w" names STDOUT_FILENO (popen.html#DESCRIPTION) | editorial | (none) |
| SI-91 | XSH times(): DESCRIPTION says waitid() returns a process ID (times.html#DESCRIPTION) | editorial | (none) |
| SI-92 | XSH pclose(): the ECHILD list omits waitid() with P_ALL (pclose.html#DESCRIPTION) | unclear | (none) |
| Q-1 | SIGPIPE after shutdown(SHUT_WR)? | question | XSH.write.017 |
| Q-2 | lseek() beyond the file system's largest file | question | XSH.lseek.011 |
| Q-3 | unnamed semaphore (pshared 0) in a fork child | question | (not tested) |
| Q-4 | is a zombie still a group member for "orphaned process group"? | question | XSH._Exit.010 (avoids it) |
| Q-5 | wait(): "if the implementation queues SIGCHLD" vs XSH 2.4.1 | question | XSH.wait.019 (both readings agree) |
| Q-6 | wait(): when is "another child's status available"? | question | (not tested) |
| Q-7 | exit(): streams with no file descriptor (memory streams) | question | (not tested) |
| Q-8 | XSH 2.3: ENAMETOOLONG/ENOTDIR/ELOOP always overlap ENOENT for lookups | question | XSH.exec.038, XSH.exec.041, XSH.unlink.013, XSH.rmdir.009, XSH.link.026, XSH.rename.036 |
| Q-9 | exec E2BIG: which {ARG_MAX} when sysconf() under-reports | question | XSH.exec.043 |
| Q-10 | execvp() sh fallback when argv[0] is a null pointer | question | (not tested) |
| Q-11 | raise()/pthread_kill(): delivery before return (raise() settled by its own rule) | question | XSH.raise.001 |
| Q-12 | kill() with the most negative pid_t | question | (not tested) |
| Q-13 | alarm(): rounding of the seconds returned | question | XSH.alarm.002, XSH.alarm.003 |
| Q-14 | alarm(), sleep(): may the wake-up come early? | question | XSH.alarm.001, XSH.sleep.002 |
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
| Q-29 | "not already at EOF" | question | XSH.fflush.004 |
| Q-30 | fflush(): position after discarding pushback | question | XSH.fflush.005 |
| Q-31 | other waiters after the last child | question | XSH._Exit.020 |
| Q-32 | aio at exit | question | XSH._Exit.019 |
| Q-33 | buffering against implementation-defined support | question | XSH.setvbuf.003, .004, .006; XSH.fopen.019, .020; XSH.setbuf.002 |
| Q-34 | ungetc limit omits fflush() | question | none (permission) |
| Q-35 | fseek() ERRORS preamble | question | XSH.fseek.009 to .011, XSH.fseek.014 to .017 |
| Q-36 | fdopen() 'a' modes | question | XSH.fdopen.013 |
| Q-37 | {STREAM_MAX} | question | XSH.fopen.032, XSH.fdopen.009 |
| Q-38 | a failed create | question | none |
| Q-39 | trailing slash with "wx" | question | none |
| Q-40 | fseek() offset after fflush() against 2.5.1 | question | XSH.fseek.004 |
| Q-41 | fseek() EBADF wording | question | none |
| Q-42 | RLIMIT_NOFILE below {_POSIX_OPEN_MAX} | question | EMFILE tests |
| Q-43 | F_SETOWN: EINVAL or ESRCH | question | XSH.fcntl.013 |
| Q-44 | rmdir() ENOENT wording | question | XSH.rmdir.008, .015 |
| Q-45 | bind() and the socket file | question | XSH.fstatat.006 |
| Q-46 | utimes() tv_usec | question | XSH.futimens.023 |
| Q-47 | ungetc() against fflush() on pushback | question | XSH.ungetc.010 |
| Q-48 | chmod(): set-ID bits ignored | question | chmod, fchmod, umask |
| Q-49 | 2.5.1 and popen(), system(), posix_spawn() | question | XSH.2.5.1-stream-handles.009, .010 |
| Q-50 | mmap() against 2.8.3.3 | question | XSH.2.4.3-signal-actions.021 |
| Q-51 | signal(): "prevent ... from occurring" | question | XSH.signal.004 |
| Q-52 | siginfo without SA_SIGINFO | question | XSH.sigtimedwait.004, .008 |
| Q-53 | SIGCHLD ordering; E2BIG against ENOMEM | question | XSH._Exit.014, XSH.wait.002, XSH.exec.043 |
| Q-54 | <unistd.h> intptr_t | question | Tier 0 |
| Q-55 | <signal.h> constants distinct | question | XBD.signal.h.016, .017 |
| Q-56 | aio_read() on a pipe | question | XSH.fork.023 |
| Q-57 | signal() return after reset | question | (none) |
| Q-58 | scandir sel once | question | XSH.alphasort.010 |
| Q-59 | _Exit consequences timing | question | XSH._Exit.016, .017, .018, .021 |
| Q-60 | fcntl locks beyond EOF | question | XSH.fcntl.027 |
| Q-61 | socket peer gone: EPIPE+SIGPIPE or ECONNRESET | question | XSH.write.026, XSH.read.022 |
| Q-62 | XSH msync(): flags 0, MS_INVALIDATE alone, MS_ASYNC|MS_SYNC | question | XSH.msync.002, .007, .008 |
| Q-63 | XSH strndup() and strnlen(): may bytes after the null byte be read? | question | XSH.strdup.007, XSH.strnlen.005 |
| Q-64 | XSH open_memstream(): byte-oriented from the start, or unoriented until used? | question | XSH.open_memstream.012, .014 |
| Q-65 | XSH fprintf(): infinity and NaN with e, g and a | question | XSH.fprintf.058 |
| Q-66 | XSH getcwd(): is ENAMETOOLONG excluded for a long absolute name? | question | XSH.getcwd.008 |
| Q-67 | XSH chdir(): ENOENT for a regular file | question | XSH.chdir.004 |
| Q-68 | XSH mknod(): unspecified behaviour against the required EPERM | question | XSH.mknod.018, .019 |
| Q-69 | XSH setreuid() and setregid(): is the saved ID otherwise left alone? | question | XSH.setreuid.004, XSH.setregid.004 |
| Q-70 | XSH setregid(): which saved set-group-ID may be assigned to the real group ID? | question | XSH.setregid.006 |
| Q-71 | XSH shm_unlink(): EPERM or EACCES when the caller may not remove the name | question | XSH.shm_unlink.005 |
| Q-72 | XSH 2.9.5.1: asynchronous cancellation "may be acted upon at any time" | question | XSH.pthread_setcancelstate.006 |
| Q-73 | XBD <limits.h> and pthread_create()/pthread_key_create(): how {PTHREAD_THREADS_MAX} and {PTHREAD_KEYS_MAX} count | question | XSH.pthread_create.012, XSH.pthread_key_create.005 |
| Q-74 | XSH sleep(): rounding of the unslept time | question | XSH.sleep.003 |
| Q-75 | XSH fsync() and fdatasync(): which file types (pipes, directories) give EINVAL is unspecified (fsync.html#ERRORS/EINVAL, fdatasync.html#ERRORS/EINVAL) | question | XSH.fsync, XSH.fdatasync (tolerant) |
| Q-76 | XSH fsync(): a descriptor open for reading only (fsync.html#DESCRIPTION) | question | (none) |
| Q-77 | XSH pread(): whether the data of a pipe is consumed by the failed ESPIPE call is unstated (pread.html#ERRORS/ESPIPE) | question | XSH.pread.006, .007 (not checked) |
| Q-78 | XSH fdatasync(): the EINVAL condition is linked to _PC_SYNC_IO only implicitly (fdatasync.html#ERRORS/EINVAL) | question | XSH.fdatasync |
| Q-79 | XSH ftrylockfile(): whether a call by the thread that already owns the lock must succeed; the count rule is stated only for flockfile() (flockfile.html#DESCRIPTION) | question | XSH.flockfile.008 (disputed) |
| Q-80 | XSH ttyname() and ttyname_r(): the errors for a descriptor that is not a terminal are "may fail", while RETURN VALUE gives only success or failure; may a non-terminal get a name? (ttyname.html#ERRORS, #RETURN VALUE) | question | XSH.ttyname.006, .007 (tested as required) |
| Q-81 | XSH ttyname_r() and XBD limits.h: does {TTY_NAME_MAX} include the terminating null byte ({_POSIX_TTY_NAME_MAX} says it does; the ttyname page says "length") (ttyname.html#DESCRIPTION) | question | XSH.ttyname.005 (weaker reading tested) |
| Q-82 | XSH tcsetpgrp(): the EIO condition does not say the caller is in a background group, so read literally it also covers an orphaned foreground caller (tcsetpgrp.html#ERRORS/EIO, XBD 11.1.4) | question | XSH.tcsetpgrp.007 (background caller only) |
| Q-83 | XSH posix_fadvise(): EINVAL covers a negative len but not a negative offset, unlike posix_fallocate() (posix_fadvise.html#ERRORS/EINVAL) | question | (none) |
| Q-84 | XSH ctermid(): RATIONALE says the result for a null argument is undefined, RETURN VALUE defines it (static area, empty string on failure) (ctermid.html#RATIONALE, #RETURN VALUE) | conflict | XSH.ctermid.003 |
| Q-85 | XSH pselect() and poll()/ppoll(): ERRORS [EINVAL] "an invalid timeout interval" does not say which intervals are invalid (select.html#ERRORS/EINVAL, poll.html#ERRORS/EINVAL) | question | (none; not tested) |
| Q-86 | XSH nice() and getpriority(): nice() says a maximum of 2*{NZERO}-1 "shall be imposed" and larger requests are set to it, while getpriority() says implementations may enforce more restrictive limits (nice.html#DESCRIPTION, getpriority.html#DESCRIPTION) | conflict | XSH.nice.002, XSH.nice.003, XSH.getpriority.005, XSH.getpriority.006 (tolerant) |
| Q-87 | XSH pselect() and ppoll(): ERRORS lists [EINTR] as a "shall fail" error without condition, while DESCRIPTION leaves it unspecified whether the call fails or returns the ready descriptors when some are ready (select.html#ERRORS/EINTR, #DESCRIPTION; poll.html#ERRORS/EINTR) | unclear | XSH.select.016, XSH.poll.022 (nothing ready) |
| Q-88 | XSH getrusage(): the contents of ru_utime and ru_stime are not defined beyond "measures of resources used" (accuracy, granularity), and RUSAGE_CHILDREN is silent about a child that has terminated but not been waited for (getrusage.html#DESCRIPTION) | question | XSH.getrusage.001, .002, .003 (only positive and not decreasing) |
| Q-89 | XSH posix_spawn_file_actions_addclose(): "already closed at the time when the new process is spawned" does not say whether a descriptor closed by an earlier action of the same object counts (posix_spawn_file_actions_addclose.html#DESCRIPTION, posix_spawn.html#ERRORS) | question | XSH.posix_spawn_file_actions_addclose.006 (disputed) |
| Q-90 | XSH posix_spawn_file_actions_addclose() RATIONALE: the reason given for not requiring the {OPEN_MAX} check (the soft limit can drop below an open descriptor) applies equally to addopen() and adddup2(), whose ERRORS say they shall fail with EBADF at or above {OPEN_MAX} (posix_spawn_file_actions_addclose.html#RATIONALE, #ERRORS/EBADF; posix_spawn_file_actions_adddup2.html#ERRORS/EBADF) | question | XSH.posix_spawn_file_actions_addclose.013, XSH.posix_spawn_file_actions_adddup2.007 (tested as written) |
| Q-91 | XSH posix_spawnp(): "the environment variable PATH" does not say whether the search uses the caller's environment or the envp array given to the call (posix_spawn.html#DESCRIPTION) | question | XSH.posix_spawn.010, .011, .012, .013, .014, .059 (tested with equal PATH in both) |
| Q-92 | XSH posix_spawn(): the null file_actions case says the child inherits no "process-owned file locks", the non-null list says "any file locks"; open-file-description locks are shared with the inherited description (posix_spawn.html#DESCRIPTION) | question | XSH.posix_spawn.019, .020 (process-owned locks only) |
| Q-93 | XSH posix_spawn() POSIX_SPAWN_SETSID: the child "shall have no controlling terminal", but the page and XBD 11.1.3 give no portable observation (opening the ctermid() pathname with no controlling terminal is not specified) (posix_spawn.html#DESCRIPTION, ctermid.html#DESCRIPTION) | question | XSH.posix_spawn.043 (not checked) |
| Q-94 | XSH glob(): the DESCRIPTION calls the contents of gl_pathv for a zero count implementation-defined, RETURN VALUE and APPLICATION USAGE call it undefined/unspecified (glob.html#DESCRIPTION, #RETURN VALUE) | conflict | (not checked) |
| Q-95 | XSH glob(): the page does not say which errors are "related to file system contents" (decides whether a null errfunc ignores EACCES); only XRAT C.2.14.3 lists them (glob.html#DESCRIPTION, xrat C.2.14.3) | unclear | XSH.glob.030 |
| Q-96 | XSH nftw(): ERRORS lists as a "shall fail" EACCES the case that fn returns -1 and does not reset errno, while RETURN VALUE says any non-zero return of fn is returned as it is and that errno may be set by fn (nftw.html#ERRORS/EACCES, #RETURN VALUE) | conflict | (none; not tested) |
| Q-97 | XSH nftw(): ERRORS makes EACCES for a path whose read permission is denied a failure, while DESCRIPTION reports an unreadable directory with FTW_DNR; unclear which applies to the root of the walk (nftw.html#ERRORS/EACCES, #DESCRIPTION/FTW_DNR) | unclear | XSH.nftw.016, XSH.nftw.022 (root readable, search-denied path only) |
| Q-98 | XSH nftw() FTW_CHDIR: the page does not say which directory is current while a directory itself is reported, nor whether the original working directory is restored when nftw() returns (nftw.html#DESCRIPTION/FTW_CHDIR) | question | XSH.nftw.010 (non-directory objects only; the test changes back itself) |
| Q-99 | XSH nftw(): fd_limit values below 1 and the case of a tree deeper than fd_limit are not described (error, or slower walk) (nftw.html#DESCRIPTION) | question | XSH.nftw.007 (fd_limit 1 expected to complete) |
| Q-100 | XSH basename() and dirname(): thread safety is demanded by RATIONALE and CHANGE HISTORY but the DESCRIPTION says only that a result may be invalidated when the input is modified or freed; whether a result must survive later calls for other strings is only implied (basename.html#RATIONALE, #DESCRIPTION; dirname.html#DESCRIPTION) | question | XSH.basename.005, XSH.dirname.006 |
| Q-101 | XSH atan() and atan2(): results are stated as the numbers pi, pi/2, pi/4 and 3pi/4, which no binary type holds; the page does not say the value is the nearest representable one (atan.html#RETURN VALUE, atan2.html#RETURN VALUE) | question | XSH.atan.004, XSH.atan2.004, .005, .008, .011, .012, .013, .014 (disputed) |
| Q-102 | XSH j0(), j1(), jn(), y0(), y1(), yn(): "finite and too large in magnitude" has no stated threshold, so the large-argument range error cannot be provoked portably (j0.html#RETURN VALUE, y0.html#ERRORS) | question | (none; not tested) |
| Q-103 | XSH logb() and ilogb(): the exponent is called the integral part of the radix-r logarithm of |x|, but the inequality given with it selects the floor of the logarithm; for |x| below one inside a binade (0.375) truncation toward zero and the floor differ (logb.html#DESCRIPTION, ilogb.html#DESCRIPTION) | unclear | XSH.logb.003, XSH.ilogb.002 (disputed, follow the inequality) |
| Q-104 | XSH remquo(): "the integral quotient of x/y" does not say whether it is the quotient rounded to nearest (as in the remainder) or the truncated quotient; they differ whenever the fractional part exceeds one half (remquo.html#DESCRIPTION) | unclear | XSH.remquo.002 (only quotients with fractional part below one half) |
| Q-105 | XSH remainder(): the page gives no result for a finite x and an infinite y (the ISO C result is x), nor says what remainder(+-0, y) returns (remainder.html#RETURN VALUE) | question | (not tested) |
| Q-106 | XSH nextafter(): for a subnormal or zero correct value the text offers "[MXX] the correct function value (if representable) or [MX] 0.0", so it is unclear whether a subnormal result may be replaced by zero (nextafter.html#RETURN VALUE) | unclear | XSH.nextafter.006 (value accepted between 0 and the smallest normal value) |
| Q-107 | XSH isunordered(): the quiet-comparison rule ("shall not raise the invalid floating-point exception") is stated in the DESCRIPTION of isgreater() and its relatives but not of isunordered(), whose APPLICATION USAGE only describes it as a quiet version of a relational operator (isunordered.html#DESCRIPTION, isgreater.html#DESCRIPTION) | question | (not tested for isunordered()) |
| Q-108 | XSH fma(): the rounding mode is "characterized by the value of FLT_ROUNDS", while fegetround() is what selects and reports the mode; FLT_ROUNDS may not follow fesetround() (fma.html#DESCRIPTION) | question | XSH.fma.002 (nearest value asserted only when fegetround() reports to-nearest) |
| Q-109 | XSH frexp(): the fraction is required to lie in [1/2, 1) whatever the radix, and exactness is required only when the radix is a power of two; for a radix of 16 or 10 the interval and the equality cannot both hold with an exact fraction (frexp.html#RETURN VALUE) | question | XSH.frexp.001, XSH.frexp.002 (values chosen for a binary radix) |
| SI-93 | XSH fmod(): the underflow sentence names "fmod(), modf(), and fmodl()" where fmodf() is meant (fmod.html#RETURN VALUE) | editorial | (not tested) |
| SI-94 | XSH lround(): "a domain shall occur" lacks the word "error" in two RETURN VALUE sentences (lround.html#RETURN VALUE) | editorial | XSH.lround.006, XSH.lround.007 |
| Q-110 | XSH feholdexcept(): a non-stop mode is installed only "if available", and the return value is zero only if it was installed; the page does not say whether an implementation may lack a non-stop mode, so a nonzero return cannot be judged a failure (feholdexcept.html#DESCRIPTION, #RETURN VALUE) | question | XSH.feholdexcept.001 (UNRESOLVED on a nonzero return), and setup of XSH.feholdexcept.002 to .005, XSH.feupdateenv.002, .007 |
| Q-111 | XSH carg(), cacos(), cacosh(), casin(), casinh(), catan(), catanh(), clog() (and the other complex pages through their tables): results are stated as the numbers pi, pi/2, pi/4 and 3pi/4, which no binary type holds; the pages do not say the value is the nearest representable one (as for Q-101) (carg.html#RETURN VALUE, cacos.html#RETURN VALUE, clog.html#RETURN VALUE) | question | XSH.carg.003, XSH.cacos.002/.004/.006/.009/.012/.014, XSH.cacosh.002/.004/.006/.009/.012/.014, XSH.casin.003/.007, XSH.casinh.003/.007, XSH.catan and XSH.catanh pi-valued rows, XSH.clog.002/.004/.007/.010/.012 (disputed) |
| Q-112 | XSH casin(), catan(), csin(), ctan(), ccos(): the special-value rows are stated for "iz" and for "-i (...)" without defining the notation; a formal complex product turns an infinite part into NaN (0 times Inf), so the tests read it as the exact exchange of the parts with one sign change (casin.html#RETURN VALUE, and the same wording in catan, csin, ctan, ccos) | question | XSH.casin, XSH.catan, XSH.csin, XSH.ctan, XSH.ccos special-value assertions (tested with the exchange reading) |
| Q-113 | XSH cacos(), cacosh(), casin(), casinh(), catan(), catanh(), ccos(), ccosh(), cexp(), clog(), csin(), csinh(), csqrt(), ctan(), ctanh(): the symmetry rules (conjugate, odd, even) hold "exactly" including for special values, while many rows leave a sign unspecified (+-Inf, +-0); for an argument with a NaN imaginary part the two rules together force the unspecified sign to follow the sign bit of the NaN, which the pages do not mention (cacos.html#RETURN VALUE, csinh.html#RETURN VALUE) | unclear | symmetry assertions of those modules (points with a NaN imaginary part, and for odd and even rules any NaN part, are left out) |
| Q-114 | XSH cacosh(), ctan(), ctanh(), csqrt(), casinh(), csinh(): rows use an unsigned "0" where neighbouring rows say "+0" or "+-0", say "positive" in some pages and "positive-signed" in others for the same condition, and csqrt() says "the imaginary part of z is Inf" without a sign; whether -0, +0 and -Inf are included is not stated (cacosh.html#RETURN VALUE, ctanh.html#RETURN VALUE, csinh.html#RETURN VALUE, csqrt.html#RETURN VALUE) | question | XSH.cacosh.005/.006, XSH.ctanh.002/.004, XSH.csqrt.002 (only +0, positive values and +Inf are used); XSH.casin.001, XSH.casinh.001 |
