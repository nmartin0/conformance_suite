# POSIX.1-2024: issues found while building posixcheck

Copyright (c) 2026 Nicholas Martin. All rights reserved.

A running list of places where the POSIX.1-2024 text (Issue 8) conflicts
with itself, contains an apparent error, or leaves a requirement unclear,
written for raising with The Austin Group (The Open Group / IEEE). Every
entry came up while writing posixcheck tests and was checked against the
HTML edition. The short index, with the assertions that depend on each
entry, is `standard-issues.md`; a shared copy of this report is kept at
https://claude.ai/artifact/L3t5goEQYUJKhbTWAj2BXF.

- **Wording.** Following RULES.md, nothing here is copied from the
  standard: requirements are paraphrased and located by page, section and
  error name. When filing, quote the exact text from your own copy.
- **Kinds.** *Conflict*: two places disagree. *Defect*: the text is wrong
  as written. *Unclear*: it can be read more than one way. *Question*: the
  text is consistent, but the intent, or its fit with practice, is unclear.
- **Each entry** gives the location, the problem, what Linux, glibc and
  musl do (where known), how posixcheck handles it meanwhile, and a
  suggested change.
- **Before filing**, search the Austin Group tracker
  (https://austingroupbugs.net) for an existing defect; none of these has
  been checked against it yet.

## Summary

| ID | Where | Kind | Severity | How posixcheck handles it |
| --- | --- | --- | --- | --- |
| SI-1 | XSH `write()`, ERRORS: [ECONNRESET] | Defect | Medium | Accepts ENOTCONN or ECONNRESET (XSH.write.027) |
| SI-2 | XSH `write()` and `send()`, ERRORS: [EPIPE] | Conflict | Low | Requires SIGPIPE only for SOCK_STREAM (XSH.write.026) |
| SI-3 | XSH `pread()`/`pwrite()`, ERRORS: [EINVAL] | Defect (editorial) | Low | Checks the offset is unchanged anyway (XSH.read.017, XSH.write.024) |
| SI-4 | XSH `fcntl()`, ERRORS: [EINVAL] and [EMFILE] for F_DUPFD | Unclear | Low | Accepts either at arg = {OPEN_MAX} (XSH.fcntl.005) |
| SI-5 | XCU `true`, APPLICATION USAGE, against XCU 1.4 and `false` | Conflict | Low | No assertion yet |
| SI-6 | XSH `fork()`/`_Fork()`: XSI interval timers | Defect | Low | Unit left as a todo |
| SI-7 | XSH `fork()`: process-shared locks, no TSH shading | Defect (editorial) | Low | Treats the test as TSH-conditional (XSH.fork.014) |
| SI-8 | XSH `wait()`: DESCRIPTION against RATIONALE on discarding SIGCHLD | Conflict | Medium | Follows DESCRIPTION (XSH.wait.019; Linux fails) |
| SI-9 | XSH 2.12 (unshaded) against XSI shading elsewhere: SIG_IGN and SA_NOCLDWAIT | Conflict | Medium | Treats it as XSI (XSH._Exit.009) |
| SI-10 | XBD `<signal.h>`: SA_NOCLDWAIT moved to Base, yet XSI-shaded | Defect (editorial) | Low | XBD.signal.h.017 |
| SI-11 | XSH `wait()`, RETURN VALUE: the WNOHANG sentence | Defect (editorial) | Low | Treats it as a requirement (XSH.wait.010) |
| SI-12 | XSH `exit()`: active handle "to a file descriptor" | Defect (editorial) | Low | Reads it as file description (XSH.exit.010) |
| SI-13 | XSH 2.5.1: abort() and signals count as closing a stream | Conflict | Low | Not tested |
| SI-14 | XSH `fexecve()`, [EBADF] wording against APPLICATION USAGE | Unclear | Low | Uses an O_RDONLY descriptor (XSH.exec.048, 049) |
| SI-15 | XSH exec: "saved resource limits" defined nowhere | Defect | Low | Nothing to observe |
| SI-16 | XSH exec: PTHREAD_CANCEL_ENABLED (no such symbol) | Defect (editorial) | Low | Reads it as PTHREAD_CANCEL_ENABLE (XSH.exec.030) |
| SI-17 | XSH `fexecve()`: execute-permission EACCES excluded, yet promised | Conflict | Medium | Not tested |
| SI-18 | XSH exec: "regular" image file against EACCES for other file types | Conflict | Low | Assumes directories are not run (XSH.exec.036) |
| SI-19 | XSH exec, [EINVAL]: a file with "privileges" | Defect (editorial) | Low | Not tested |
| SI-20 | XSH exec: inherited-attribute list | Defect (editorial) | Low | Tests the thread's mask (XSH.exec.018) |
| SI-21 | XSH `sigaction()`, SA_RESETHAND: the SIGILL/SIGTRAP note against the reset rule | Conflict | Low | Follows the normative rule; no assertion |
| SI-22 | XSH `sigaction()`: is the signal blocked during an SA_RESETHAND handler? | Conflict | Low | Not tested |
| SI-23 | XSH `sigaction()`: the non-XSI SA_SIGINFO failure names no errno | Defect | Low | Uses SIGRTMIN with SA_SIGINFO |
| SI-24 | XSH `sigaction()`, 2.4.2, `sigqueue()` against `<signal.h>`: queuing outside the realtime range | Conflict | Medium | 021 disputed (INSPECT) |
| SI-25 | XSH `sigaction()`, SA_NOCLDSTOP: "if the implementation supports SIGCHLD" | Defect (editorial) | Low | Treats SIGCHLD as mandatory |
| SI-26 | XSH `sigaction()`: SA_ONSTACK and the handler mask speak of the process | Defect (editorial) | Low | Reads them per thread |
| SI-27 | XSH `raise()` ERRORS against its pthread_kill() equivalence | Conflict | Low | Uses an invalid number (XSH.raise.003) |
| SI-28 | XSH `kill()` CHANGE HISTORY: "calling" process for the receiving one | Defect (editorial) | Low | Follows the DESCRIPTION (XSH.kill.014) |
| SI-29 | XSH `kill()`: "a signal" against "that signal" in the pid cases | Defect (editorial) | Low | Not tested |
| SI-30 | XSH `pthread_sigmask()`, 2.4.3: killpg() missing from the generating functions | Defect (editorial) | Low | Uses kill() |
| SI-31 | XSH `pthread_sigmask()`: no-EINTR sentence covers sigprocmask() | Defect (editorial) | Low | Not tested |
| SI-32 | XSH `sigwaitinfo()`/`sigtimedwait()`: wait ends on a caught signal, but [EINTR] is may-fail | Defect (editorial) | Low | Not tested |
| SI-33 | XSH `sigwait()`: leftover "if the implementation supports queued signals" | Defect (editorial) | Low | Tests the rule unconditionally (XSH.sigwait.003) |
| SI-34 | XSH `sigwait()`: implementation-defined remainder sentence overlaps the queued-signal rule | Defect (editorial) | Low | Follows the queued-signal rule (XSH.sigwait.003) |
| SI-35 | XSH `sigqueue()`: "the pending, unblocked signal" vs kill()'s "at least one" | Defect (editorial) | Low | XSH.sigqueue.003 |
| SI-36 | XSH `sigtimedwait()` RATIONALE: a "value" argument that does not exist | Defect (editorial) | Low | Not tested |
| SI-37 | XSH `sigtimedwait()` CHANGE HISTORY: RETURN VALUE named for an ERRORS sentence | Defect (editorial) | Low | Not tested |
| SI-38 | XSH `alarm()` SEE ALSO: empty link to itself | Defect (editorial) | Low | Not tested |
| SI-39 | XBD `<signal.h>` si_code table: option spans opened but not closed | Defect (editorial) | Low | Only the TRAP_ codes as XSI (XBD.signal.h.024) |
| SI-40 | XBD `<limits.h>`: {NSIG_MAX} has no minimum | Defect (editorial) | Low | XBD.signal.h.007 |
| SI-41 | XBD `<signal.h>`: SIGRTMIN/SIGRTMAX "declared"; non-overlap without "shall" | Defect (editorial) | Low | Treats non-overlap as required (XBD.signal.h.006) |
| SI-42 | XSH 2.4.3: si_addr still the faulting instruction's address | Defect (editorial) | Low | Follows `<signal.h>` (XSH.2.4.3-signal-actions.015) |
| SI-43 | XSH 2.4.3 SIG_DFL: queued-values sentence's scope and wording | Defect (editorial) | Low | SIG_IGN case only (XSH.2.4.3-signal-actions.010) |
| SI-44 | XSH 2.4.1: a sigwait() thread has the signal "delivered" | Defect (editorial) | Low | XSH.sigwait.006 |
| SI-45 | XSH `sigaddset()` etc.: "shall" vs "should" for initialising sets; effects without "shall" | Defect (editorial) | Low | Treats the effects as required |
| SI-46 | XSH `sigaltstack()`: "in some implementations ... shall" | Defect (editorial) | Low | Classified impl |
| SI-47 | XSH `sigaltstack()`: ENOMEM has no exception for SS_DISABLE | Defect (editorial) | Low | Disputed (XSH.sigaltstack.011) |
| SI-48 | XSH `signal()`: the errno rule is not CX-shaded | Defect (editorial) | Low | XSH.signal.006 |
| SI-49 | XSH `str2sig()`: "a string containing" | Defect (editorial) | Low | Plain names and numbers only |
| SI-50 | XBD `<stdio.h>`: dprintf() prototype lacks its semicolon | Defect (editorial) | Low | Generator tolerates it |
| SI-51 | XBD `<float.h>`: unclosed paragraphs in the HTML edition | Defect (editorial, HTML) | Low | Generator repairs it |
| SI-52 | XSH `rename()`: no error number for a directory renamed to "new/" | Defect (omission) | Low | Any error accepted (XSH.rename.014) |
| SI-53 | XSH `chown()`/`fchown()`/`lchown()`: ERRORS out of step with each other | Defect (inconsistency) | Low | chown() rules applied |
| SI-54 | XSH `truncate()`: [EFBIG] names a "SIGFSZ" signal | Defect (editorial) | Low | Read as SIGXFSZ (XSH.truncate.008) |
| SI-55 | XSH `opendir()`/`scandir()`: [ENOENT] overlaps [ENOTDIR] and [ELOOP] | Defect (inconsistency) | Low | ENOTDIR cases disputed (XSH.opendir.008, XSH.alphasort.006) |
| SI-56 | XSH `ftruncate()`: "unspecified" for other file types contradicts the required [EINVAL] | Defect (inconsistency) | Low | Disputed (XSH.ftruncate.010) |
| SI-57 | XSH `truncate()`: no "file unaffected on failure", and weaker wording than `ftruncate()` | Defect (inconsistency) | Low | No check of the file after a failure |
| SI-58 | XSH `umask()`: "only the permission bits" vs "other bits implementation-defined" | Defect (inconsistency) | Low | Only S_ISVTX is tested beyond the permission bits |
| SI-59 | XSH `futimens()` page: `utimes()` errors written in terms of tv_nsec; XSI shading incomplete | Defect (editorial) | Low | utimes() errors read through "equivalent to utimensat()" |
| SI-60 | XSH `utimes()`: "rounding towards the nearest second" vs XBD 4.12 | Defect (inconsistency) | Low | Accepts 1 s either way (XSH.futimens.018) |
| SI-61 | XSH `*at()` functions: [EACCES] for fd's directory is not limited to relative paths | Defect (omission) | Low | Absolute paths ignore fd (e.g. XSH.futimens.007) |
| SI-62 | XSH `readdir()`: RATIONALE says `fpathconf()` with _SC_NAME_MAX | Defect (editorial) | Low | None needed |
| SI-63 | XSH `alphasort()`/`scandir()`: RETURN VALUE wording | Defect (editorial) | Low | None needed |
| SI-64 | XBD `<signal.h>`: whether the listed si_code values are required | Defect (ambiguity) | Low | SEGV_ACCERR, SEGV_MAPERR disputed (XSH.2.4.3-signal-actions.015, .020) |
| SI-65 | XBD `<signal.h>`: no si_code for SIGBUS past the end of a mapped object | Defect (omission) | Low | Code not checked (XSH.2.4.3-signal-actions.021) |
| SI-66 | XBD `<signal.h>`: SI_MESGQ not shaded MSG | Defect (editorial) | Low | Test is MSG-conditional (XSH.2.4.3-signal-actions.017) |
| SI-67 | XSH `fflush()`: RATIONALE says the input-stream offset rule is not mandated | Defect (inconsistency) | Low | DESCRIPTION followed (XSH.fflush.004) |
| SI-68 | XSH `truncate()`: an application requirement where an error is required | Defect (inconsistency) | Low | EACCES required (XSH.truncate.011) |
| SI-69 | XSH `getrlimit()`: RATIONALE says RLIMIT_FSIZE stays XSI | Defect (editorial) | Low | Base, per the CHANGE HISTORY |
| SI-70 | XSH `sigaction()`: APPLICATION USAGE says longjmp() is not async-signal-safe | Defect (editorial) | Low | XSH 2.4.3 table followed |
| SI-71 | XSH `pthread_sigmask()`: an unconditional requirement on how | Defect (editorial) | Low | The specific text followed (XSH.pthread_sigmask.010) |
| SI-72 | XSH 2.5.1: "any handle to this open file descriptor" | Defect (editorial) | Low | Read as open file description |
| SI-73 | XSH fscanf(): "(except for %n)" and end-of-file | Defect (conflict with ISO C) | Low | Case avoided (XSH.fscanf.033) |
| SI-74 | XSH fopen() and freopen(): the two ENAMETOOLONG conditions are swapped | Defect (inconsistency) | Low | {NAME_MAX} case required for both |
| SI-75 | XSH strerror(): two statements about overwriting by strerror_l() | Defect (editorial) | Low | Only the forbidden overwrites are tested |
| SI-76 | XSH gmtime(): tm_zone is set only in the gmtime_r() text | Defect (editorial) | Low | Only gmtime_r() tested |
| SI-77 | XSH mktime(): "shall attempt to determine" whether DST is in effect | Defect (editorial) | Low | Not tested |
| SI-78 | XSH strftime(): %z with tm_isdst 0 and a DST tm_gmtoff | Defect (unclear) | Low | Both outputs accepted |
| SI-79 | XSH fmemopen(): the append position "one byte after the end of the buffer" | Defect (editorial) | Low | Position max_size taken |
| SI-80 | XSH pthread_mutex_lock(): NORMAL relock "deadlock" against the may-fail [EDEADLK] | Defect (unclear) | Low | Both outcomes accepted |
| SI-81 | XSH sysconf(): the table names _POSIX_SS_REPL_MAX for a limit | Defect (editorial) | Low | Checked under either name |
| SI-82 | XSH fchdir() and readlink(): wording slips | Defect (editorial) | Low | Not tested |
| SI-83 | XSH fprintf(): wording gaps | Defect (editorial) | Low | More digits and either sign accepted |
| SI-84 | XBD 3.253 against _Exit(): when a child's parent changes | Defect (conflict) | Low | Zombie window not tested |
| SI-85 | XSH getgroups(): CHANGE HISTORY says 0 is not permitted, DESCRIPTION allows it | Defect (conflict, informative) | Low | Zero accepted |
| SI-86 | XSH setreuid(), setregid(), setgid(): wording slips | Defect (editorial) | Low | Not tested |
| SI-87 | XSH nanosleep() against clock_getres(): measured by CLOCK_REALTIME, or independent of it | Defect (unclear) | Low | UNRESOLVED if the clocks disagree |
| SI-88 | XSH clock_getres() and clock_nanosleep(): wording slips and an overlap | Defect (editorial) | Low | Both errors accepted |
| Q-1 | XSH `write()`/`send()`: SIGPIPE after shutdown(SHUT_WR) | Question | Low | Does not require SIGPIPE (XSH.write.017) |
| Q-2 | XSH `lseek()`: offsets beyond the file system's largest file | Question | Medium | Asserts only the off_t overflow case (XSH.lseek.011) |
| Q-3 | XSH `fork()`/`sem_init()`: unnamed semaphore, pshared 0, in the child | Question | Low | Not tested |
| Q-4 | XBD orphaned process group: is a zombie still a member? | Question | Low | Avoids the case (XSH._Exit.010) |
| Q-5 | XSH `wait()`: which SIGCHLD paragraph applies | Question | Low | One child, where both agree (XSH.wait.019) |
| Q-6 | XSH `wait()`: when another child's status is "available" | Question | Low | Not tested |
| Q-7 | XSH `exit()`: streams with no file descriptor | Question | Low | Not tested |
| Q-8 | XSH 2.3: ENAMETOOLONG/ENOTDIR/ELOOP always meet ENOENT for lookups | Question | Medium | Accepts ENOENT too (XSH.exec.038, 041); disputed elsewhere |
| Q-9 | XSH exec [E2BIG]: which {ARG_MAX} | Question | Low | Uses sysconf()'s value (XSH.exec.043) |
| Q-10 | XSH `execvp()` sh fallback when argv[0] is null | Question | Low | Not tested |
| Q-11 | XSH `raise()`/`pthread_kill()`: delivery before return | Question | Low | raise() answered by its own rule (XSH.raise.001); pthread_kill() open |
| Q-12 | XSH `kill()` with the most negative pid_t | Question | Low | Not tested |
| Q-13 | XSH `alarm()`: rounding of the seconds returned | Question | Low | Expects 1 under a second left (XSH.alarm.002) |
| Q-14 | XSH `alarm()`: may SIGALRM come early? | Question | Low | Treats early as FAIL (XSH.alarm.001) |
| Q-15 | XSH `pause()`/`sigsuspend()`: delivery to the calling thread? | Question | Low | Directs the signal at the waiter (XSH.sigsuspend.006) |
| Q-16 | XBD `<limits.h>`/`<signal.h>`: {RTSIG_MAX} the range's size or a lower bound? | Question | Low | Checks the lower bound (XBD.signal.h.006) |
| Q-17 | XSH 2.4.2: kill() occurrences vs sigqueue() FIFO order | Question | Low | FIFO among sigqueue() ones only (XSH.2.4.2-realtime-signals.004) |
| Q-18 | XSH 2.4.1/2.4.3: SIGCONT and stopped processes | Question | Low | SIGCONT at SIG_DFL (XSH.2.4.3-signal-actions.006) |
| Q-19 | XSH `psiginfo()`: may the description carry siginfo_t details? | Question | Low | Disputed (XSH.psiginfo.003) |
| Q-20 | XSH `sigaltstack()`: must oss report SS_ONSTACK/SS_DISABLE? | Question | Low | Disputed (XSH.sigaltstack.009) |
| Q-21 | XSH `abort()`: caught but blocked SIGABRT: must the handler run? | Question | Low | Disputed (XSH.abort.006) |
| Q-22 | XBD `<signal.h>`: MINSIGSTKSZ constant vs CPU-dependent frames | Question | Medium | XSH.sigaltstack.010 FAILs on AVX-512 |
| Q-23 | XSH `signal()`: return value after failed calls or sigaction() | Question | Low | Successful calls only (XSH.signal.001) |
| Q-24 | XSH `link()`: what access to the existing file may be required, and with which error | Question | Medium | Disputed (XSH.link.019) |
| Q-25 | XSH `chown()`: chown(-1, -1) of another user's file | Question | Low | Disputed (XSH.chown.008) |
| Q-26 | XSH `truncate()`/`ftruncate()`: does the file size limit apply when shrinking? | Question | Low | Only growth tested |
| Q-27 | XSH `futimens()`: "write access to the file" for a descriptor | Question | Low | Requires the file's permission (XSH.futimens.021) |
| Q-28 | XSH `fflush()`: does fflush(NULL) set the offsets of input streams? | Question | Low | Disputed (XSH.fflush.011) |
| Q-29 | XSH `fflush()`/`fclose()`/`exit()`: what "not already at EOF" means | Question | Low | File grown after the reads (both readings met) |
| Q-30 | XSH `fflush()`: where does reading resume after pushback is discarded? | Question | Low | Only the discard is checked (XSH.fflush.005) |
| Q-31 | XSH `_exit()`: what happens to the other threads waiting for the last child? | Question | Low | Disputed (XSH._Exit.020) |
| Q-32 | XSH `_exit()`: asynchronous I/O at exit cannot be observed | Question | Low | Disputed (XSH._Exit.019) |
| Q-33 | XSH `setvbuf()`, `fopen()`: buffering is required, but support is implementation-defined | Question | Medium | Disputed (XSH.setvbuf.003, .004, .006; XSH.fopen.019, .020) |
| Q-34 | XSH `ungetc()`: the limit on repeated pushback omits fflush() | Question | Low | Not tested (permission) |
| Q-35 | XSH `fseek()`: the ERRORS preamble against errors that need no lseek() or write() | Question | Medium | Disputed (XSH.fseek.009 to .011, .014 to .017) |
| Q-36 | XSH `fdopen()`: the 'a' modes: position and O_APPEND | Question | Medium | Disputed (XSH.fdopen.013) |
| Q-37 | XBD `<limits.h>`: {STREAM_MAX} as a runtime invariant tied to {FOPEN_MAX} | Question | Low | NOTINUSE when it follows RLIMIT_NOFILE |
| Q-38 | XSH `open()`, `fopen()`: may a failed create leave a file behind? | Question | Low | Not tested |
| Q-39 | XSH `fopen()`: a trailing slash with "wx" | Question | Low | Not tested |
| Q-40 | XSH `fseek()`: the offset after fflush() against XSH 2.5.1 | Question | Medium | Disputed (XSH.fseek.004) |
| Q-41 | XSH `fseek()`: EBADF for a descriptor "not open for writing" | Question | Low | Not tested |
| Q-42 | XSH `getrlimit()`: RLIMIT_NOFILE below {_POSIX_OPEN_MAX} | Question | Low | Tests keep the limit at 20 or more |
| Q-43 | XSH `fcntl()`: F_SETOWN with the ID of a process that is gone | Question | Low | Disputed (XSH.fcntl.013) |
| Q-44 | XSH `rmdir()`: ENOENT for "a nonexistent directory" | Question | Low | Disputed (XSH.rmdir.008, .015) |
| Q-45 | XSH `bind()`: the file an AF_UNIX address creates | Question | Low | Required, by implication (XSH.fstatat.006) |
| Q-46 | XSH `utimes()`: an out-of-range tv_usec | Question | Low | Disputed (XSH.futimens.023) |
| Q-47 | XSH `ungetc()` against `fflush()`: when is pushback discarded? | Question | Medium | Tests use a large file (XSH.ungetc.010) |
| Q-48 | XSH `chmod()`: what "S_ISUID and S_ISGID bits in mode ... ignored" means | Question | Low | Directory S_ISGID masked (chmod, fchmod, umask) |
| Q-49 | XSH 2.5.1: process creation the application does not see | Question | Low | Not tested |
| Q-50 | XSH `mmap()` against XSH 2.8.3.3: references past a shrunk file | Question | Low | A one-page file mapped twice as long (XSH.2.4.3-signal-actions.021) |
| Q-51 | XSH `signal()`: what "prevent ... from occurring" is | Question | Low | Judged by behaviour (XSH.signal.004) |
| Q-52 | XSH `sigwaitinfo()`: siginfo for a signal without SA_SIGINFO | Question | Low | SA_SIGINFO set in the tests (XSH.sigtimedwait.004, .008) |
| Q-53 | XSH `_exit()`: status available before SIGCHLD; E2BIG against ENOMEM | Question | Low | Bounded waits (XSH._Exit.014, XSH.wait.002); ENOMEM accepted (XSH.exec.043) |
| Q-54 | XBD `<unistd.h>`: intptr_t without XSI shading | Question | Low | Conditional on XSI (Tier 0) |
| Q-55 | XBD `<signal.h>`: are the flag and code constants distinct? | Question | Low | Disputed (XBD.signal.h.016, .017) |
| Q-56 | XSH aio_read(): a pipe or FIFO | Question | Low | UNRESOLVED (XSH.fork.023) |
| Q-57 | XSH signal(): the return value after a reset on delivery | Question | Low | Not tested |
| Q-58 | XSH scandir(): is sel called once per entry? | Question | Low | Disputed (XSH.alphasort.010) |
| Q-59 | XSH _exit(): when the consequences of termination take effect | Question | Low | Bounded waits (XSH._Exit.016 to .018); disputed (XSH._Exit.021) |
| Q-60 | XSH fcntl(): "Locks may start and extend beyond the current end of a file" | Question | Low | Read as required of the implementation (XSH.fcntl.027) |
| Q-61 | XSH `write()`/`read()`: a stream socket whose peer has gone | Question | Medium | Disputed (XSH.write.026, XSH.read.022) |
| Q-62 | XSH msync(): flags 0, MS_INVALIDATE alone, MS_ASYNC|MS_SYNC | Question | Medium | Disputed (XSH.msync.002, .007, .008) |
| Q-63 | XSH strndup() and strnlen(): may bytes after the null byte be read? | Question | Low | Disputed (XSH.strdup.007, XSH.strnlen.005) |
| Q-64 | XSH open_memstream(): byte-oriented from the start, or unoriented until used? | Question | Low | Disputed (XSH.open_memstream.012, .014) |
| Q-65 | XSH fprintf(): infinity and NaN with e, g and a | Question | Low | Disputed (XSH.fprintf.058) |
| Q-66 | XSH getcwd(): is ENAMETOOLONG excluded for a long absolute name? | Question | Low | Disputed (XSH.getcwd.008) |
| Q-67 | XSH chdir(): ENOENT for a regular file | Question | Low | Disputed (XSH.chdir.004) |
| Q-68 | XSH mknod(): unspecified behaviour against the required EPERM | Question | Low | Disputed (XSH.mknod.018, .019) |
| Q-69 | XSH setreuid() and setregid(): is the saved ID otherwise left alone? | Question | Low | Disputed (XSH.setreuid.004, XSH.setregid.004) |
| Q-70 | XSH setregid(): which saved set-group-ID may be assigned to the real group ID? | Question | Low | Avoided (XSH.setregid.006) |
| Q-71 | XSH shm_unlink(): EPERM or EACCES when the caller may not remove the name | Question | Low | Disputed (XSH.shm_unlink.005) |
| Q-72 | XSH 2.9.5.1: asynchronous cancellation "may be acted upon at any time" | Question | Low | Disputed (XSH.pthread_setcancelstate.006) |
| Q-73 | XBD <limits.h> and pthread_create()/pthread_key_create(): how {PTHREAD_THREADS_MAX} and {PTHREAD_KEYS_MAX} count | Question | Low | Both readings accepted (XSH.pthread_create.012, XSH.pthread_key_create.005) |
| Q-74 | XSH sleep(): rounding of the unslept time | Question | Low | Both roundings accepted (XSH.sleep.003) |
| Q-166 | XBD <netinet/in.h>: no value given for INADDR_ANY or INADDR_BROADCAST | Question | Low | Expects 0 and all ones, `disputed` (XBD.netinet_in.h.010) |
| Q-167 | XBD <netdb.h>: IPPORT_RESERVED is "the highest reserved port", reserved ports undefined | Unclear | Low | Only the range of a port number is checked (XBD.netdb.h.004) |
| Q-169 | XBD headers: constants used as alternatives or return codes are never said to differ | Question | Low | Distinctness expected; `disputed` for <langinfo.h> (XBD.langinfo.h.002, .004) |

## Entries

### SI-1: write() gives ECONNRESET the meaning of ENOTCONN

- **Where:** XSH `write()`, ERRORS (the "shall fail" list), [ECONNRESET];
  compare `send()` and `read()` ERRORS.
- **What the text says:** the [ECONNRESET] entry describes a write on a
  socket that is not connected. There is no [ENOTCONN] entry, although the
  Issue 6 CHANGE HISTORY says both errors were added for sockets.
- **Why it is a problem:** in `send()` and `read()`, ECONNRESET means the
  peer forcibly closed the connection and ENOTCONN means not connected.
  `write()` on a socket is defined as `send()` with no flags, so the pages
  contradict each other, and read literally every system that reports
  ENOTCONN for `write()` on an unconnected socket is non-conforming.
- **Practice:** Linux gives ENOTCONN on an unconnected AF_UNIX stream socket.
- **posixcheck meanwhile:** XSH.write.027 accepts either error.
- **Suggested change:** split the entry as `read()` does: [ECONNRESET] for a
  connection forcibly closed by the peer, and a new [ENOTCONN] for a socket
  that is not connected.

### SI-2: write() and send() disagree on when SIGPIPE is sent

- **Where:** XSH `write()` ERRORS, the second [EPIPE] entry (sockets);
  XSH `send()` ERRORS, [EPIPE].
- **What the text says:** `write()` sends SIGPIPE for a socket that is no
  longer connected only if it is SOCK_STREAM; `send()` does so for
  SOCK_STREAM and SOCK_SEQPACKET unless MSG_NOSIGNAL is set.
- **Why it is a problem:** `write()` on a socket is `send()` with no flags,
  so the two should agree on SOCK_SEQPACKET.
- **posixcheck meanwhile:** XSH.write.026 requires SIGPIPE only for
  SOCK_STREAM; SOCK_SEQPACKET is untested.
- **Suggested change:** name SOCK_SEQPACKET in `write()` too, or refer to
  `send()` for the signal.

### SI-3: the "offset unchanged" sentence in the pread() and pwrite() errors

- **Where:** XSH `read()` ERRORS, [EINVAL] for `pread()`; XSH `write()`
  ERRORS, [EINVAL] for `pwrite()`.
- **What the text says:** a negative offset on a regular or block special
  file gives EINVAL, and the entry adds that the file offset stays
  unchanged.
- **Why it is a problem:** these functions never change the file offset
  (DESCRIPTION), so the sentence restates the general rule inside one error
  entry, which no other error entry does, and invites the reading that
  other errors might move the offset.
- **posixcheck meanwhile:** checks the offset anyway (XSH.read.017,
  XSH.write.024).
- **Suggested change:** drop the sentence from both entries, or state once
  that the offset is unchanged on any error.

### SI-4: F_DUPFD with arg at or above {OPEN_MAX} meets both EINVAL and EMFILE

- **Where:** XSH `fcntl()` ERRORS, [EINVAL] and [EMFILE].
- **What the text says:** EINVAL for F_DUPFD (and the CLOEXEC and CLOFORK
  variants) with arg negative or at least {OPEN_MAX}; EMFILE when no
  descriptor at or above arg is available.
- **Why it is a problem:** at arg of {OPEN_MAX} or more no such descriptor
  can ever be available, so both conditions always hold together, and
  XSH 2.3 lets either be reported. The EINVAL requirement for that case is
  then untestable, which is probably not the intent.
- **posixcheck meanwhile:** XSH.fcntl.005 accepts either at arg = {OPEN_MAX}.
- **Suggested change:** limit [EMFILE] to arg below {OPEN_MAX}, or say that
  EINVAL takes precedence.

### SI-5: true's usage note on "--" has no basis in XCU 1.4

- **Where:** XCU `true`, APPLICATION USAGE; XCU 1.4 Utility Description
  Defaults, OPTIONS; XCU `false`.
- **What the text says:** the informative note says `true` need only accept
  and discard a first "--" argument. XCU 1.4 requires "--" handling only of
  utilities that take operands but no options, and `true` takes neither.
- **Why it is a problem:** the informative text states a requirement the
  normative text does not make, and `false`, with the same OPTIONS and
  OPERANDS, has no such note.
- **posixcheck meanwhile:** no assertion; a low-priority todo for both.
- **Suggested change:** add the rule normatively (for both utilities, or in
  XCU 1.4), or reword the note so it claims no requirement.

### SI-6: fork() still resets XSI interval timers, which Issue 8 removed

- **Where:** XSH `fork()` (and `_Fork()`), DESCRIPTION, the XSI item on
  interval timers; XBD `<sys/time.h>` CHANGE HISTORY, Issue 8.
- **What the text says:** among what the child does not inherit, interval
  timers are reset in the child on XSI systems.
- **Why it is a problem:** Austin Group defect 1330, applied in Issue 8,
  removed `getitimer()` and `setitimer()`. The item now refers to a facility
  the standard no longer defines.
- **posixcheck meanwhile:** the unit is a todo with this note.
- **Suggested change:** delete the item (editorial clean-up after 1330).

### SI-7: fork()'s rule for process-shared locks is not TSH-shaded

- **Where:** XSH `fork()`, DESCRIPTION, the item on locks set to be
  process-shared.
- **What the text says:** such locks held by the parent's threads are not
  held by the child; the item has no option shading.
- **Why it is a problem:** a lock can be made process-shared only through
  the Thread Process-Shared Synchronization option (TSH), so the item
  cannot apply without it, and the convention elsewhere would shade it.
- **posixcheck meanwhile:** XSH.fork.014 is conditional on TSH.
- **Suggested change:** shade the item TSH (editorial).

### SI-8: wait() requires discarding a pending SIGCHLD; its RATIONALE treats it as optional

- **Where:** XSH `wait()`, DESCRIPTION (the paragraphs on a pending SIGCHLD,
  for implementations that queue it and for SIGCHLD blocked) and RATIONALE.
- **What the text says:** DESCRIPTION requires that, when a child's status
  is returned, its pending SIGCHLD is discarded, or (SIGCHLD blocked) the
  pending SIGCHLD is cleared unless another child's status is available.
  The RATIONALE speaks of allowing the wait functions to do this, and of
  implementations that do not permit it.
- **Why it is a problem:** the informative text reads as if discarding were
  optional, so a reader can conclude that leaving the signal pending
  conforms; the normative text says it does not.
- **Practice:** Linux (kernel; glibc and musl alike) leaves the SIGCHLD
  pending after wait() reports the only child. Linux does not queue SIGCHLD,
  and the leftover signal names the reaped child, so it fails under either
  paragraph.
- **posixcheck meanwhile:** XSH.wait.019 follows DESCRIPTION; Linux fails it.
- **Suggested change:** reword the RATIONALE to explain the requirement,
  without the language of permission.

### SI-9: SIG_IGN and SA_NOCLDWAIT for SIGCHLD: Base in XSH 2.12, XSI elsewhere

- **Where:** XSH 2.12 Status Information (new in Issue 8); XSH 2.4.3;
  XSH `_Exit()`, Consequences of Process Termination; XSH `wait()` and
  `waitid()`, APPLICATION USAGE.
- **What the text says:** 2.12, unshaded, says that with SIGCHLD set to
  SIG_IGN or SA_NOCLDWAIT set, a child's termination generates no new
  status and discards existing status. 2.4.3, the consequences item and the
  APPLICATION USAGE notes mark the same behaviour XSI.
- **Why it is a problem:** on a system without XSI, 2.12 requires the
  behaviour and the other places leave it unspecified.
- **posixcheck meanwhile:** XSH._Exit.009 follows the shading.
- **Suggested change:** shade the 2.12 item XSI, or unshade the others.

### SI-10: SA_NOCLDWAIT is said to have moved to Base but is still XSI-shaded

- **Where:** XBD `<signal.h>`, the SA_NOCLDWAIT entry and CHANGE HISTORY
  (Issue 7); XSH `sigaction()`, DESCRIPTION.
- **What the text says:** the Issue 7 history says SA_NOCLDWAIT (with
  SA_RESETHAND, SA_RESTART, SA_SIGINFO and SA_NODEFER) moved from XSI to
  Base, yet its entries in both places are still XSI-shaded.
- **Why it is a problem:** it is not said whether only the symbol is Base
  while its behaviour stays XSI, or whether the shading was left by
  mistake. This bears on SI-9.
- **posixcheck meanwhile:** XBD.signal.h.017 follows the shading (the symbol is checked only on XSI systems); the behaviour is not tested yet.
- **Related:** in the same list the names SA_ONSTACK, SS_ONSTACK, SS_DISABLE, MINSIGSTKSZ and SIGSTKSZ are unshaded and only their descriptions are XSI-shaded, while the sentence introducing the list is unshaded; read literally, the names are required on systems without the XSI option, although sigaltstack() is XSI. XBD.signal.h.017 and .018 treat them as XSI.
- **Suggested change:** remove the shading, or say that only the symbol is Base; shade the names together with their descriptions.

### SI-11: wait()'s WNOHANG return value is worded unlike the rest

- **Where:** XSH `wait()`, RETURN VALUE, the WNOHANG sentence.
- **What the text says:** the zero return for WNOHANG is stated without
  "shall", while the next sentence uses it; its two conditions (at least
  one child without status, and no status for any process pid selects)
  overlap.
- **Why it is a problem:** the second condition implies the first except
  when there are no such children, which is the ECHILD case anyway; and the
  missing "shall" makes the requirement read as descriptive.
- **posixcheck meanwhile:** treats it as a requirement (XSH.wait.010).
- **Suggested change:** if WNOHANG was given and status is not available
  for any process pid selects, 0 shall be returned (editorial).

### SI-12: exit() speaks of the active handle to a file descriptor

- **Where:** XSH `exit()`, DESCRIPTION, the CX sentence on stream offsets;
  compare XSH `fclose()` and XSH 2.5.1.
- **What the text says:** exit() sets the offset for each stream that is
  the active handle to its underlying file *descriptor*; `fclose()` says
  file *description*.
- **Why it is a problem:** 2.5.1 defines handles on an open file
  description, and a descriptor is itself one of the handles.
- **posixcheck meanwhile:** reads it as file description (XSH.exit.010).
- **Suggested change:** "open file description" (editorial).

### SI-13: XSH 2.5.1 counts abnormal termination as closing a stream

- **Where:** XSH 2.5.1 Interaction of File Descriptors and Standard I/O
  Streams; XSH `abort()`.
- **What the text says:** a handle counts as closed when its process ends
  by exit(), by abort() or by a signal. Closing satisfies the rule for
  leaving the first handle, and 2.5.1 promises no data is lost when the
  rules are followed.
- **Why it is a problem:** only exit() flushes and sets the offset; abort()
  need only attempt an fclose(), and a signal does neither, so buffered
  output can be lost although the rules count the stream as closed.
- **posixcheck meanwhile:** not tested.
- **Suggested change:** count only exit() (and return from main()) as
  closing for the handle rules, or say the rules give no guarantee after
  abort() or a signal.

### SI-14: fexecve()'s EBADF wording suggests O_EXEC is needed

- **Where:** XSH exec, ERRORS, [EBADF] for `fexecve()`; APPLICATION USAGE.
- **What the text says:** EBADF is for a descriptor that is not valid and
  open for executing. The APPLICATION USAGE says the descriptor need not
  have been opened with O_EXEC, and suggests opening it for reading so the
  file can be checked first.
- **Why it is a problem:** "open for executing" reads as O_EXEC; then an
  O_RDONLY descriptor must give EBADF, against the page's own advice.
- **posixcheck meanwhile:** XSH.exec.048 expects success with O_RDONLY;
  XSH.exec.049 tests EBADF only with a closed descriptor.
- **Suggested change:** "not a valid open file descriptor", leaving
  permission to an EACCES entry (see SI-17).

### SI-15: "saved resource limits" is not defined anywhere

- **Where:** XSH exec, DESCRIPTION, the last paragraph before RETURN VALUE.
- **What the text says:** the new image's saved resource limits are a copy
  of the process's hard and soft limits.
- **Why it is a problem:** no other page defines or uses saved resource
  limits; `getrlimit()` knows only hard and soft limits, and resource
  limits are already in the inherited-attribute list.
- **posixcheck meanwhile:** limit inheritance is tested (XSH.exec.025).
- **Suggested change:** delete the sentence, or define the term.

### SI-16: exec names PTHREAD_CANCEL_ENABLED, which does not exist

- **Where:** XSH exec, DESCRIPTION, the initial thread's cancellation state.
- **What the text says:** the state is given as PTHREAD_CANCEL_ENABLED.
- **Why it is a problem:** `<pthread.h>` defines PTHREAD_CANCEL_ENABLE; the
  type in the same sentence, PTHREAD_CANCEL_DEFERRED, is spelled right.
- **posixcheck meanwhile:** reads it as PTHREAD_CANCEL_ENABLE (XSH.exec.030).
- **Suggested change:** PTHREAD_CANCEL_ENABLE (editorial).

### SI-17: fexecve() and missing execute permission

- **Where:** XSH exec, ERRORS (the group that excludes `fexecve()`), and
  APPLICATION USAGE on `fexecve()` and O_EXEC.
- **What the text says:** the EACCES entry for a file that denies execute
  permission excludes `fexecve()`, and no `fexecve()` entry covers it. The
  APPLICATION USAGE says `fexecve()` checks execute permission when called
  and fails if the mode does not grant it.
- **Why it is a problem:** the informative text promises a failure the
  normative text does not require, with no error number.
- **posixcheck meanwhile:** not tested.
- **Suggested change:** add an `fexecve()` EACCES entry for a file that
  denies execute permission.

### SI-18: a "regular" image file against EACCES for other file types

- **Where:** XSH exec, DESCRIPTION (first paragraph) and the first [EACCES].
- **What the text says:** the image is built from a regular, executable
  file; the error is for a file that is not regular *and* of a type the
  implementation does not run.
- **Why it is a problem:** the error implies some non-regular files may be
  runnable, which the DESCRIPTION rules out.
- **posixcheck meanwhile:** XSH.exec.036 expects EACCES for a directory.
- **Suggested change:** drop "regular" from the DESCRIPTION, or make the
  error simply "not a regular file".
- **After the Linux-ism audit:** XSH.exec.036 is now `disputed`.

### SI-19: a file with "privileges"

- **Where:** XSH exec, ERRORS [EINVAL]; RATIONALE on ENOEXEC and EINVAL.
- **What the text says:** EINVAL is for a file with appropriate
  *privileges* in a recognized format the system cannot run.
- **Why it is a problem:** processes have privileges, files permissions;
  the neighbouring ENOEXEC entry says access permission.
- **posixcheck meanwhile:** not tested (needs a binary for another machine).
- **Suggested change:** "appropriate access permission" (editorial).

### SI-20: the inherited-attribute list repeats a limit and has a process signal mask

- **Where:** XSH exec, DESCRIPTION, the two lists of inherited attributes.
- **What the text says:** the process list has both the file size limit and
  resource limits, plus a process signal mask and a (singular) pending
  signal; the thread list has the signal mask and pending signals.
- **Why it is a problem:** the file size limit is a resource limit (left
  over from when `getrlimit()` was XSI; Issue 8 moved it to Base); the
  signal mask is a thread attribute.
- **posixcheck meanwhile:** XSH.exec.018 tests the calling thread's mask,
  XSH.exec.025 the limits.
- **Suggested change:** drop the file size limit and the process signal
  mask from the process list, and make it the process's pending signals.

### SI-21: the SA_RESETHAND note on SIGILL and SIGTRAP contradicts the rule it annotates

- **Where:** XSH `sigaction()`, DESCRIPTION, SA_RESETHAND, and the note
  inside it; the typographical conventions in the front matter.
- **What the text says:** with SA_RESETHAND the action is reset to SIG_DFL
  on entry to the handler, and without it the action is not modified on
  entry. A note inside the item says SIGILL and SIGTRAP cannot be reset
  this way and that the system silently enforces it.
- **Why it is a problem:** notes inside normative text are informative
  (front matter, typographical conventions), so the note cannot make an
  exception, yet it describes a restriction the normative rule does not
  have: read normatively, SIGILL and SIGTRAP must be reset like any other
  signal. Either the note is a requirement in the wrong form, or it is
  stale (it comes from older systems).
- **Practice:** Linux resets SIGILL and SIGTRAP like other signals.
- **posixcheck meanwhile:** follows the normative rule; no assertion about
  SIGILL or SIGTRAP.
- **Suggested change:** make the exception normative (and say whether the
  call fails or the flag is ignored), or delete the note.

### SI-22: is the signal blocked during an SA_RESETHAND handler?

- **Where:** XSH `sigaction()`, DESCRIPTION: SA_RESETHAND, SA_NODEFER, and
  the paragraph on the mask installed for a handler.
- **What the text says:** (1) under SA_NODEFER, if that flag is not set the
  signal is always added to the thread's mask on entry; (2) the paragraph
  on the handler's mask adds the signal unless SA_NODEFER *or SA_RESETHAND*
  is set; (3) under SA_RESETHAND, sigaction() may behave as if SA_NODEFER
  were also set.
- **Why it is a problem:** for a handler installed with SA_RESETHAND and
  without SA_NODEFER, (1) requires the signal to be blocked, (2) says it is
  not, and (3) makes it optional.
- **posixcheck meanwhile:** not tested.
- **Suggested change:** keep (3), and qualify (1) and (2) to match it.

### SI-23: sigaction()'s non-XSI SA_SIGINFO failure names no error number

- **Where:** XSH `sigaction()`, ERRORS (the paragraph after the "may fail" list); CHANGE HISTORY.
- **What the text says:** on systems without the XSI option, sigaction() may fail if SA_SIGINFO is set for a signal outside SIGRTMIN-SIGRTMAX. No errno is given; the CHANGE HISTORY records that [ENOTSUP] was added for this and later deleted when the X/Open UNIX functionality was folded into the Base.
- **Why it is a problem:** a "may fail" condition without an error number cannot be handled by an application, and the deletion left the condition behind. Either the condition went with the error, or it needs one.
- **posixcheck meanwhile:** the SA_SIGINFO tests use SIGRTMIN, so a system that uses this permission cannot fail them.
- **Suggested change:** name the error (EINVAL or ENOTSUP), or delete the paragraph.

### SI-24: queuing with SA_SIGINFO: any signal, or only realtime ones?

- **Where:** XSH `sigaction()`, DESCRIPTION (subsequent occurrences with SA_SIGINFO); XSH 2.4.2; XSH `sigqueue()`, DESCRIPTION and RETURN VALUE; XBD `<signal.h>` (SIGRTMIN and SIGRTMAX); XRAT B.2.4.2.
- **What the text says:** sigaction(), 2.4.2 and sigqueue() say that with SA_SIGINFO set, occurrences generated by sigqueue() are queued in FIFO order, for any signal; sigqueue()'s RETURN VALUE says that on success the signal has been queued. `<signal.h>` describes SIGRTMIN-SIGRTMAX as the range for which the realtime behaviour is supported, and the rationale keeps the range for queuing and values.
- **Why it is a problem:** for SIGUSR1 with SA_SIGINFO, one reading requires queuing and the other leaves it to the implementation. Linux queues only realtime signals, and sigqueue() returns 0 although the occurrence is merged, which contradicts sigqueue()'s RETURN VALUE under either reading.
- **posixcheck meanwhile:** XSH.sigaction.020 requires FIFO queuing for SIGRTMIN; XSH.sigaction.021 (SIGUSR1) is disputed, its FAIL logged as INSPECT.
- **Suggested change:** limit the queuing sentences to SIGRTMIN-SIGRTMAX (and say what sigqueue() returns when an occurrence is not queued), or require queuing for all signals in `<signal.h>`.

### SI-25: SA_NOCLDSTOP still allows for systems without SIGCHLD

- **Where:** XSH `sigaction()`, DESCRIPTION, SA_NOCLDSTOP.
- **What the text says:** SIGCHLD is generated when children stop "if the implementation supports the SIGCHLD signal".
- **Why it is a problem:** SIGCHLD has been mandatory since Issue 6, when job control became required; the condition is a leftover.
- **posixcheck meanwhile:** treats SIGCHLD as always supported (XSH.sigaction.007, since 2001).
- **Suggested change:** delete the condition (editorial).

### SI-26: SA_ONSTACK and the handler's mask speak of the process

- **Where:** XSH `sigaction()`, DESCRIPTION: SA_ONSTACK, and the paragraph on the mask installed for a handler.
- **What the text says:** with SA_ONSTACK the signal is delivered to the calling process on the alternate stack; the handler's mask lasts until the handler returns or until sigprocmask() or sigsuspend() is called.
- **Why it is a problem:** alternate stacks and signal masks are per thread (sigaltstack(), pthread_sigmask()), and the signal goes to the thread that receives it, not necessarily the caller of sigaction(). pthread_sigmask() also ends the handler's mask, but is not named.
- **posixcheck meanwhile:** reads both as the receiving thread (XSH.sigaction.009, 014).
- **Suggested change:** "delivered to the receiving thread on that stack"; add pthread_sigmask() to the list (editorial).

### SI-27: raise()'s errors do not match its pthread_kill() equivalence

- **Where:** XSH `raise()`, DESCRIPTION (equivalent to pthread_kill(pthread_self(), sig)) and ERRORS; XSH `pthread_kill()`, ERRORS.
- **What the text says:** raise() fails with EINVAL for an invalid signal number. pthread_kill() may fail with EINVAL for signal 0 and shall fail for a non-zero invalid *or unsupported* number.
- **Why it is a problem:** if the two are equivalent, raise(0) may fail and raise() of an unsupported number must fail; raise()'s own list allows neither.
- **posixcheck meanwhile:** XSH.raise.003 uses a number that is invalid on every system (100000).
- **Suggested change:** give raise() the same error entries as pthread_kill(), or refer to them.

### SI-28: kill()'s CHANGE HISTORY names the wrong process

- **Where:** XSH `kill()`, CHANGE HISTORY, Issue 6.
- **What the text says:** the saved set-user-ID of the calling process is checked in place of its effective user ID.
- **Why it is a problem:** the DESCRIPTION compares the sender's real or effective user ID with the *receiving* process's real or saved set-user-ID; the sender's saved ID plays no part.
- **posixcheck meanwhile:** follows the DESCRIPTION (XSH.kill.014).
- **Suggested change:** "the saved set-user-ID of the receiving process" (editorial).

### SI-29: kill()'s permission wording differs between pid values

- **Where:** XSH `kill()`, DESCRIPTION, the paragraphs for pid 0, pid -1 and pid below -1.
- **What the text says:** for pid 0 and below -1, the processes for which the sender has permission to send *a* signal; for -1, permission to send *that* signal.
- **Why it is a problem:** permission depends on the signal (the SIGCONT session rule), so for kill(0, SIGCONT) to a group member of another user, "a signal" reads as excluding it and "that signal" as including it.
- **posixcheck meanwhile:** not tested.
- **Suggested change:** "that signal" in all three (editorial).

### SI-30: killpg() is missing from the lists of signal-generating functions

- **Where:** XSH `pthread_sigmask()`, DESCRIPTION (SIGFPE, SIGILL, SIGSEGV and SIGBUS generated while blocked); XSH 2.4.3 (functions whose signal makes handler behaviour defined).
- **What the text says:** generating these signals while blocked is undefined unless they come from another process or from kill(), pthread_kill(), raise() or sigqueue().
- **Why it is a problem:** killpg() (XSI, equivalent to kill() with a negative pid) is left out, so killpg() to the caller's own group is undefined while the same kill() is not.
- **posixcheck meanwhile:** XSH.pthread_sigmask.011 uses kill().
- **Related:** two more 2.4.3 sentences (behaviour after ignoring SIGFPE, SIGILL, SIGSEGV or SIGBUS, and after a normal return from their handlers) exempt only signals from kill(), sigqueue() and raise(), leaving out pthread_kill() as well as killpg(), although the multithreaded handler rule and pthread_sigmask() include pthread_kill().
- **Suggested change:** add killpg() to both lists, and pthread_kill() and killpg() to the two 2.4.3 sentences (editorial).

### SI-31: the no-EINTR sentence covers sigprocmask(), which returns no error codes

- **Where:** XSH `pthread_sigmask()`, ERRORS.
- **What the text says:** these functions shall not return an error code of [EINTR].
- **Why it is a problem:** sigprocmask() returns -1 and sets errno; it returns no error code, so the sentence says nothing about it as worded.
- **posixcheck meanwhile:** not tested (the calls do not block).
- **Suggested change:** "shall not fail with [EINTR]" (editorial).

### SI-32: sigwaitinfo() and sigtimedwait() end the wait on a caught signal, but [EINTR] is only a may-fail error

- **Where:** XSH `sigtimedwait()` (which also defines sigwaitinfo()), DESCRIPTION and ERRORS.
- **What the text says:** with nothing in the set pending, the thread is suspended until a signal in the set becomes pending or until it is interrupted by an unblocked, caught signal; [EINTR] for that interruption is listed under "may fail", and whether it is used must be documented.
- **Why it is a problem:** the description ends the wait on the interruption, but an implementation that does not fail with [EINTR] has nothing left to return: no signal from the set was selected, and no other outcome is given. The likely intent is that such an implementation resumes the wait (as with SA_RESTART), which the description does not allow as written.
- **posixcheck meanwhile:** the interrupted case is not tested (the unit is `partial`; [EINTR] is `optional`).
- **Suggested change:** "...until one or more signals in set become pending or, if the implementation detects the [EINTR] condition, until it is interrupted by an unblocked, caught signal" (editorial).

### SI-33: sigwait(): "if the implementation supports queued signals" is left over

- **Where:** XSH `sigwait()`, DESCRIPTION.
- **What the text says:** the rule that the rest of a signal's queue stays queued is conditional on the implementation supporting queued signals.
- **Why it is a problem:** queued signals have been part of the Base since Issue 7 (the CHANGE HISTORY says the Realtime Signals Extension functionality moved to the Base), so the condition is always true and suggests an option that no longer exists (compare SI-25).
- **posixcheck meanwhile:** XSH.sigwait.003 tests the rule unconditionally.
- **Suggested change:** delete the condition (editorial).

### SI-34: sigwait(): the implementation-defined remainder sentence overlaps the queued-signal rule

- **Where:** XSH `sigwait()`, DESCRIPTION; XSH 2.4.1.
- **What the text says:** with several pending instances of one signal number before the call, it is implementation-defined whether any remain pending after a successful return; the next sentence requires the remainder of a queue to stay queued.
- **Why it is a problem:** read literally, the first sentence also covers queued instances, which the second sentence settles the other way. XSH 2.4.1 avoids this by limiting the implementation-defined case to circumstances other than those in which queuing is required.
- **posixcheck meanwhile:** XSH.sigwait.003 follows the queued-signal rule; the unit for the first sentence is `impl`.
- **Suggested change:** add the same limit to the first sentence (editorial).

### SI-35: sigqueue(): "the pending, unblocked signal" does not say which

- **Where:** XSH `sigqueue()`, DESCRIPTION; XSH `kill()`, DESCRIPTION.
- **What the text says:** when the signal is generated for the sender, either it or "at least the pending, unblocked signal" is delivered to the calling thread before sigqueue() returns. kill() says "at least one pending unblocked signal".
- **Why it is a problem:** the definite article points at a signal that has not been identified; kill()'s wording is what is meant.
- **posixcheck meanwhile:** XSH.sigqueue.003 tests the case where the queued signal is the only one pending.
- **Suggested change:** use kill()'s wording (editorial).

### SI-36: sigtimedwait() RATIONALE names a "value" argument that does not exist

- **Where:** XSH `sigtimedwait()`, RATIONALE (informative).
- **What the text says:** sigwaitinfo() is said to return the queued value "in the value argument", and not to return it when "the value argument" is NULL.
- **Why it is a problem:** the parameter is `info`, and it carries si_signo and si_code as well as the value.
- **posixcheck meanwhile:** nothing to test.
- **Suggested change:** say "in the siginfo_t structure pointed to by info" (editorial).

### SI-37: sigtimedwait() CHANGE HISTORY: wording restored to the wrong section

- **Where:** XSH `sigtimedwait()`, CHANGE HISTORY (Issue 6, XSH/TC2/D6/130).
- **What the text says:** the entry says wording was restored in the RETURN VALUE section, and quotes the sentence that an implementation should only check for the error when no signal is pending and it is necessary to wait.
- **Why it is a problem:** that sentence is under [EINVAL] in ERRORS, not in RETURN VALUE.
- **posixcheck meanwhile:** nothing to test.
- **Suggested change:** name the ERRORS section (editorial).

### SI-38: alarm() SEE ALSO starts with an empty link to itself

- **Where:** XSH `alarm()`, SEE ALSO (HTML edition).
- **What the text says:** the first entry is the page's own name, linked to "#".
- **Why it is a problem:** a page does not list itself; the entry looks left over from an edit.
- **posixcheck meanwhile:** nothing to test.
- **Suggested change:** delete the entry (editorial).

### SI-39: <signal.h> si_code table: option spans opened but not closed

- **Where:** XBD `<signal.h>`, the si_code table (HTML edition).
- **What the text says:** the table opens a CX span at SIGILL, an XSI span at the SIGTRAP rows and another CX span at SIGCHLD, but only one end-of-option marker follows (after SI_MESGQ); the page as a whole has two more option starts than ends.
- **Why it is a problem:** the markup does not say where the XSI shading of the SIGTRAP rows ends, so the CLD_ and SI_ rows could be read as XSI.
- **posixcheck meanwhile:** XBD.signal.h.024 treats only TRAP_BRKPT and TRAP_TRACE as XSI.
- **Suggested change:** close each span where its rows end (editorial).

### SI-40: {NSIG_MAX} has no Minimum Acceptable Value

- **Where:** XBD `<limits.h>`, Other Invariant Values; XBD `<signal.h>`.
- **What the text says:** {NSIG_MAX} is the largest value sysconf(_SC_NSIG) can return, bounded above by what sigset_t can represent; every signal macro must be below it. Unlike the other entries in that list it has no minimum.
- **Why it is a problem:** nothing ties {NSIG_MAX} to the signals an implementation must support, so a value below the largest required signal number is not excluded by the entry itself, only indirectly by `<signal.h>`.
- **posixcheck meanwhile:** XBD.signal.h.007 checks every table signal and SIGRTMAX against it.
- **Suggested change:** give a minimum, for example one more than the largest required signal number plus {_POSIX_RTSIG_MAX} (editorial or normative).

### SI-41: <signal.h>: SIGRTMIN and SIGRTMAX "declared"; the non-overlap rule has no "shall"

- **Where:** XBD `<signal.h>`, DESCRIPTION.
- **What the text says:** the header "shall declare" the SIGRTMIN and SIGRTMAX macros; the next sentences say the range is reserved for application use and that its numbers do not overlap the table's signals.
- **Why it is a problem:** macros are defined, not declared; and the non-overlap statement is worded as a fact, so it is unclear whether it is a requirement.
- **posixcheck meanwhile:** XBD.signal.h.006 treats non-overlap as required.
- **Suggested change:** "shall define", and "shall not overlap" (editorial).

### SI-42: XSH 2.4.3: si_addr still described as the faulting instruction's address

- **Where:** XSH 2.4.3, the siginfo_t member table; XBD `<signal.h>`.
- **What the text says:** 2.4.3 describes si_addr as the address of the faulting instruction; `<signal.h>` (changed by Defect 1775) gives it as the address that caused the fault, which for SIGSEGV and SIGBUS is the faulting memory reference.
- **Why it is a problem:** the two descriptions differ for memory faults, and only one of them was updated.
- **posixcheck meanwhile:** XSH.2.4.3-signal-actions.015 follows `<signal.h>` (an address inside the page written).
- **Suggested change:** align 2.4.3 with `<signal.h>` (editorial).
- **After the Linux-ism audit:** <signal.h> also says si_addr "may be inaccurate" for some implementations, with no bound; XSH.2.4.3-signal-actions.021 now notes a wrong address rather than failing it (Q-50).

### SI-43: XSH 2.4.3 SIG_DFL: the queued-values sentence's scope and wording

- **Where:** XSH 2.4.3, SIG_DFL and SIG_IGN.
- **What the text says:** under SIG_DFL, after the rule that setting SIG_DFL discards a pending default-ignored signal, any queued values pending are discarded and their resources "returned to the system for other use"; under SIG_IGN the same idea is worded "made available to queue other signals".
- **Why it is a problem:** it is unclear whether the SIG_DFL sentence applies only to the default-ignored case just before it, or to any signal set to SIG_DFL (a blocked, pending SIGRTMIN, say); and the two wordings differ for what is meant to be the same effect.
- **posixcheck meanwhile:** only the SIG_IGN case is tested (XSH.2.4.3-signal-actions.010).
- **Suggested change:** tie the sentence to the case before it and use one wording (editorial).

### SI-44: XSH 2.4.1: a thread in sigwait() is said to have a signal "delivered"

- **Where:** XSH 2.4.1.
- **What the text says:** signals generated for the process are delivered to exactly one thread that is in a sigwait() call for the signal or has not blocked it.
- **Why it is a problem:** by 2.4.1's own definitions a thread in sigwait() accepts the signal; it is not delivered.
- **posixcheck meanwhile:** XSH.sigwait.006 tests that only one waiter returns.
- **Suggested change:** "delivered to or accepted by exactly one of those threads" (editorial).

### SI-45: sigaddset(): "Applications shall call" where the other set functions say "should"

- **Where:** XSH `sigaddset()`, `sigdelset()`, `sigismember()`, DESCRIPTION.
- **What the text says:** sigaddset() says applications shall call sigemptyset() or sigfillset() at least once for each sigset_t object before other use; sigdelset() and sigismember() say should. sigaddset() and sigdelset() also describe their own effect without "shall" (the function adds, deletes), while sigfillset() and sigismember() use "shall".
- **Why it is a problem:** the same application requirement is stated with two strengths, and the core behaviour of two functions is worded as a statement rather than a requirement.
- **posixcheck meanwhile:** XSH.sigaddset.001 and XSH.sigdelset.001 treat the effect as required.
- **Suggested change:** use "shall" for the effect of each function and one strength for the application rule (editorial).

### SI-46: sigaltstack(): "In some implementations, ... shall always execute"

- **Where:** XSH `sigaltstack()`, DESCRIPTION.
- **What the text says:** in some implementations, a signal shall always execute on the alternate stack if it is delivered while another signal is being caught on it.
- **Why it is a problem:** a "shall" limited to some implementations is not a requirement anyone can test or rely on; it describes allowed behaviour.
- **posixcheck meanwhile:** the unit is classified `impl`.
- **Suggested change:** "In some implementations, a signal ... executes on the alternate stack ...", or make it implementation-defined (editorial).

### SI-47: sigaltstack(): ENOMEM has no exception for SS_DISABLE

- **Where:** XSH `sigaltstack()`, DESCRIPTION and ERRORS.
- **What the text says:** with SS_DISABLE, ss_sp and ss_size are ignored; ENOMEM is required when the size of the alternate stack area is below MINSIGSTKSZ, without exception.
- **Why it is a problem:** a call that disables the stack with ss_size 0 may be read as requiring ENOMEM, contradicting "ignored".
- **posixcheck meanwhile:** XSH.sigaltstack.011 is `disputed` (expects success); XSH.sigaltstack.003 disables with a valid size.
- **Suggested change:** limit ENOMEM to calls that enable a stack (editorial).

### SI-48: signal(): the errno rule is not marked as an extension to ISO C

- **Where:** XSH `signal()`, DESCRIPTION.
- **What the text says:** a successful signal() shall not change errno; the sentence is unshaded.
- **Why it is a problem:** ISO C does not require this (library functions may set errno even on success unless their description says otherwise), so the requirement is a POSIX extension and should carry CX shading like the page's other additions.
- **posixcheck meanwhile:** XSH.signal.006 tests it (since 2008).
- **Suggested change:** shade the sentence CX (editorial).

### SI-49: str2sig(): "a string containing" a name or number

- **Where:** XSH `str2sig()`, DESCRIPTION.
- **What the text says:** str2sig() recognizes a string containing the name of a table signal, "RTMIN+n" with n a decimal representation, or a decimal representation of a signal number.
- **Why it is a problem:** "containing" could be read as matching a substring ("xINTx"), and nothing says whether leading zeros, a plus sign or white space are accepted in the numbers.
- **posixcheck meanwhile:** the tests use plain names and shortest decimal numbers only.
- **Suggested change:** "a string that is the name ...", and say which decimal forms are accepted (editorial).

### SI-50: <stdio.h>: the dprintf() prototype has no semicolon

- **Where:** XBD `<stdio.h>`, SYNOPSIS (the CX-shaded declaration of dprintf()).
- **What the text says:** the declaration of dprintf() ends after its parameter list with no semicolon; every other function declaration on the page ends with one.
- **Why it is a problem:** the page lists declarations as C source; read literally, this one runs into the next, so a tool that reads the declarations (posixcheck's spec model) has to special-case it.
- **posixcheck meanwhile:** `gen/specmodel.py` splits declarations at a line break when the semicolon is missing and flags the item `no_semicolon`.
- **Suggested change:** add the semicolon (editorial).

### SI-51: <float.h>: two paragraphs are left unclosed in the HTML edition

- **Where:** XBD `<float.h>`, DESCRIPTION, the list of decimal-digit characteristics (DECIMAL_DIG, FLT_DIG and the rest).
- **What the text says:** in the HTML edition, one paragraph that holds a formula ends with `</p` without its `>`, and another is never closed, so the following list of constants sits inside the paragraph.
- **Why it is a problem:** browsers recover, but a tool that reads the pages (posixcheck's spec model) loses the constants unless it repairs the markup. This is a defect of the HTML rendition only.
- **posixcheck meanwhile:** `gen/specmodel.py` closes such paragraphs where the next block starts.
- **Suggested change:** fix the markup in the HTML edition (editorial).

### SI-52: rename(): no error number for a directory renamed to "new/"

- **Where:** XSH `rename()`, DESCRIPTION (CX) and ERRORS.
- **What the text says:** rename() shall fail if new does not resolve to an existing directory and ends with trailing slashes (XBD 4.16 permits this constraint and names rename() in its footnote). [ENOTDIR] covers the case only when old is not a directory.
- **Why it is a problem:** for rename("d", "nd/") with d a directory and nd not existing, the call must fail, but no listed error number describes the condition, so applications cannot tell what to expect. XCU mv, the other interface the footnote names, forbids the trailing slash only for non-directory sources, which makes the difference look unintended.
- **posixcheck meanwhile:** XSH.rename.014 requires the failure and accepts any error number.
- **Suggested change:** name the error for a directory (ENOTDIR or ENOENT), or say explicitly that the rule applies to directories too.

### SI-53: lchown() and fchown(): error lists out of step with chown()

- **Where:** XSH `chown()`, `fchown()` and `lchown()`, ERRORS and RETURN VALUE.
- **What the text says:** lchown() is "equivalent to chown()" except for symbolic links, yet its [EINVAL] for an unsupported owner or group ID is a "shall fail" error, where chown() and fchown() list it under "may fail". lchown()'s [EPERM] reads "does not match the owner of the file and the process does not have appropriate privileges", while chown()'s and fchown()'s read "does not match the owner of the file, or the calling process does not have appropriate privileges and _POSIX_CHOWN_RESTRICTED indicates that such privilege is required". fchown()'s "may fail" [EINVAL] entry runs two conditions together and says "pipe" after naming a pipe or socket, and its RETURN VALUE lacks chown()'s "no changes are made" sentence.
- **Why it is a problem:** lchown() is stricter about EINVAL than the chown() it is equivalent to; lchown() leaves out the restricted-chown case; and chown()'s "or", read literally, makes even a privileged process that does not own the file fail with EPERM, where lchown()'s "and" has the intended meaning.
- **posixcheck meanwhile:** lchown() is tested with chown()'s rules (XSH.lchown.003, XSH.lchown.007); privileged chown() of another user's file is required to work (XSH.chown.001); lchown()'s EINVAL entry is classified as implementation-defined.
- **Suggested change:** give all three pages chown()'s [EPERM] with "and" for the owner case and the restricted case as a second sentence; make lchown()'s [EINVAL] a "may fail" error; split fchown()'s [EINVAL] entry and add the RETURN VALUE sentence (editorial).

### SI-54: truncate(): [EFBIG] names a "SIGFSZ" signal

- **Where:** XSH `truncate()`, ERRORS, the [EFBIG] entry for the file size limit.
- **What the text says:** the XSI-shaded sentence says a "SIGFSZ" signal shall also be generated for the thread.
- **Why it is a problem:** no signal of that name exists. The DESCRIPTION of the same page, and the matching [EFBIG] entry of ftruncate(), name SIGXFSZ, the signal `<signal.h>` defines for exceeding the file size limit.
- **posixcheck meanwhile:** the entry is read as SIGXFSZ (XSH.truncate.008).
- **Suggested change:** replace "SIGFSZ" with "SIGXFSZ" (editorial).

### SI-55: opendir()/scandir(): [ENOENT] is worded so that it overlaps [ENOTDIR]

- **Where:** XSH `opendir()` and `alphasort()` (scandir()), ERRORS, [ENOENT] and [ENOTDIR].
- **What the text says:** [ENOENT] applies when a component of the name does not name an existing *directory* (other pages say "an existing file"). [ENOTDIR] applies when a component names an existing file that is not a directory.
- **Why it is a problem:** a regular file named as the directory, or a loop of symbolic links, also "does not name an existing directory", so [ENOENT] applies alongside [ENOTDIR] (and [ELOOP]) every time, and XSH 2.3 lets either be reported. The more specific errors become impossible to require. fdopendir()'s [EBADF] ("not open for reading") also covers an O_SEARCH descriptor only by implication.
- **posixcheck meanwhile:** XSH.opendir.008 and XSH.alphasort.006 are `disputed`; ENOENT there is logged as INSPECT.
- **Suggested change:** use "does not name an existing file" in [ENOENT], as the other pathname pages do.

### SI-56: ftruncate(): the DESCRIPTION leaves other file types unspecified, ERRORS requires EINVAL

- **Where:** XSH `ftruncate()`, DESCRIPTION and ERRORS ([EINVAL]).
- **What the text says:** the DESCRIPTION leaves the result unspecified when the descriptor is open for writing on a file that is neither a regular file nor a shared memory object. Since Defect 1381, [EINVAL] says ftruncate() shall fail for a file on which the operation is not possible, naming a pipe, FIFO or socket.
- **Why it is a problem:** for a pipe or socket open for writing, one sentence makes the outcome unspecified and the other requires EINVAL. Defect 1381 appears to have left the older sentence in place.
- **posixcheck meanwhile:** XSH.ftruncate.010 (pipe and socket) is `disputed`; the negative-length case (XSH.ftruncate.005) stays required.
- **Suggested change:** limit the "unspecified" sentence to file types the ERRORS section does not cover, or drop it.

### SI-57: truncate(): out of step with ftruncate() on failures and on wording

- **Where:** XSH `truncate()` and `ftruncate()`, DESCRIPTION.
- **What the text says:** ftruncate() says that when it fails the file is unaffected; truncate() has no such sentence, so XSH 2.3 ("the action requested may have been partially performed") applies to it. truncate() also states the effects of shrinking and extending as plain statements ("is discarded", "appears as if zero-filled") where ftruncate() uses "shall", and ftruncate() speaks of the "seek pointer" where truncate() says "file offset".
- **Why it is a problem:** the two functions do the same thing to a file, yet a failed truncate() may leave it partly changed, and the normative force of its effects reads weaker. The difference looks unintended.
- **posixcheck meanwhile:** the truncate() tests do not examine the file after a failure; the ftruncate() tests do.
- **Suggested change:** give truncate() ftruncate()'s sentence on failures and its "shall" wording; say "file offset" in both (editorial).

### SI-58: umask(): only the permission bits are used, yet other bits have implementation-defined meaning

- **Where:** XSH `umask()`, DESCRIPTION and RATIONALE.
- **What the text says:** only the file permission bits of cmask shall be used; then (after the XSI-shaded S_ISVTX clause) the meaning of the other bits is implementation-defined. The RATIONALE says the second clause lets historical implementations use extra bits.
- **Why it is a problem:** the first clause forbids using any other bit and the second allows it. The RETURN VALUE's round-trip rule ("including any unspecified use of those bits") supports the second reading.
- **posixcheck meanwhile:** the round trip is tested with S_ISUID, S_ISGID and S_ISVTX set (XSH.umask.004); only S_ISVTX is required to be ignored (XSH.umask.003, .005).
- **Suggested change:** say that only the permission bits are used *to clear bits of new files' modes*, and that other bits have implementation-defined meaning.

### SI-59: futimens() page: utimes()'s errors are given in tv_nsec terms, and its XSI shading covers only the SYNOPSIS

- **Where:** XSH `futimens()` (also utimensat(), utimes()), DESCRIPTION and ERRORS.
- **What the text says:** the ERRORS lists that apply to utimes() describe [EINVAL], [EACCES] and [EPERM] by the tv_nsec fields, UTIME_NOW and UTIME_OMIT, which a timeval does not have. Only utimes()'s SYNOPSIS line is XSI-shaded; its DESCRIPTION paragraph and the "utimes() shall fail" lead-ins are not.
- **Why it is a problem:** for utimes() the conditions can only be read through the sentence making it equivalent to utimensat(); in particular nothing says an out-of-range tv_usec gives EINVAL. The missing shading makes XSI-only text look like Base text.
- **posixcheck meanwhile:** XSH.futimens.023 requires EINVAL for tv_usec 1000000 and -1 (XSI); utimes() assertions are conditional on XSI.
- **Suggested change:** state the utimes() conditions in timeval terms (tv_usec, a null times argument) and shade all utimes() text XSI.
- **After the Linux-ism audit:** Raised to a question, Q-46: XSH.futimens.023 is now `disputed`.

### SI-60: utimes(): rounding to the nearest second conflicts with XBD 4.12

- **Where:** XSH `futimens()` page (utimes() paragraph); XBD 4.12 File Times Update.
- **What the text says:** utimes() is accurate to the microsecond and rounding towards the nearest second may occur. XBD 4.12 says a timestamp shall be set to the greatest value the file system supports that is not greater than the value asked for, and that resolution is no coarser than one second.
- **Why it is a problem:** rounding to the nearest second can give a time later than the one asked for, which XBD 4.12 forbids; and with sub-second resolution required, no rounding to seconds should be needed.
- **posixcheck meanwhile:** XSH.futimens.018 accepts times up to 1 s either side.
- **Suggested change:** drop the rounding clause, or say "towards zero" (truncation), which agrees with XBD 4.12.

### SI-61: *at() functions: the fd search [EACCES] is not limited to relative paths

- **Where:** XSH `faccessat()`, `fchmodat()`, `fchownat()`, `fstatat()`, `linkat()`, `mkdirat()`, `mkfifoat()`, `mknodat()`, `openat()`, `readlinkat()`, `renameat()`, `symlinkat()`, `unlinkat()`, `utimensat()`, ERRORS.
- **What the text says:** each lists [EACCES] when fd's open file description is not O_SEARCH and fd's directory does not permit searches. The neighbouring [EBADF] and [ENOTDIR] entries are limited to paths that are not absolute; this one is not.
- **Why it is a problem:** read literally, an absolute path with fd open on a directory that denies search shall fail, though the DESCRIPTIONs say fd is used only for relative paths.
- **posixcheck meanwhile:** absolute paths are required to ignore fd (for example XSH.futimens.007 with a closed descriptor).
- **Suggested change:** add "the path argument is not an absolute path and" to each [EACCES] entry, as for [EBADF].

### SI-62: readdir(): RATIONALE names _SC_NAME_MAX for fpathconf()

- **Where:** XSH `readdir()`, RATIONALE (the readdir_r() buffer size).
- **What the text says:** it refers to fpathconf() with _SC_NAME_MAX.
- **Why it is a problem:** fpathconf() takes _PC_ names; _SC_ names belong to sysconf(), and there is no _SC_NAME_MAX.
- **posixcheck meanwhile:** nothing depends on it.
- **Suggested change:** _PC_NAME_MAX (editorial).

### SI-63: alphasort()/scandir(): RETURN VALUE wording

- **Where:** XSH `alphasort()` page, RETURN VALUE.
- **What the text says:** alphasort() compares the name of the directory entry d1 points to with "the directory" d2 points to. scandir() returns -1 on failure, but the RETURN VALUE does not say errno is set, though ERRORS lists "shall fail" errors.
- **Why it is a problem:** d2 points to a directory entry, not a directory; and every other pathname function's RETURN VALUE says errno is set.
- **posixcheck meanwhile:** scandir() errors are required with errno set (XSH.alphasort.005 to .008).
- **Suggested change:** "the directory entry pointed to by d2"; "return -1 and set errno to indicate the error" (editorial).

### SI-64: <signal.h>: the listed si_code values are stated, not required

- **Where:** XBD `<signal.h>`, the CX paragraph after the table of signal-specific si_code values; XSH 2.4.3 Signal Actions.
- **What the text says:** the paragraph says implementations "do not generate" a different value for the circumstances the table describes, while allowing limitations that prevent some values being generated. XSH 2.4.3 says that for signals not generated by the listed functions and events, si_code is either a signal-specific value from `<signal.h>` or an implementation-defined value.
- **Why it is a problem:** "do not generate" is a statement, not a "shall", and the 2.4.3 sentence allows any implementation-defined value. Read together, it is unclear whether SEGV_MAPERR for an unmapped address or SEGV_ACCERR for a protection fault is ever required.
- **posixcheck meanwhile:** XSH.2.4.3-signal-actions.015 (SEGV_ACCERR) and .020 (SEGV_MAPERR) are `disputed`; another code is logged as INSPECT.
- **Suggested change:** make the paragraph normative ("shall not generate a different value ... for the circumstances described"), or say plainly that the values are recommended.

### SI-65: <signal.h>: no SIGBUS si_code describes a reference past a mapped object's end

- **Where:** XBD `<signal.h>` (the SIGBUS codes BUS_ADRALN, BUS_ADRERR, BUS_OBJERR); XSH `mmap()`, DESCRIPTION.
- **What the text says:** mmap() requires SIGBUS for references to whole pages following the end of the mapped object. The SIGBUS codes describe invalid address alignment, a nonexistent physical address and an object-specific hardware error.
- **Why it is a problem:** the most common SIGBUS on today's systems has no matching reason, so applications cannot know which code to expect (Linux uses BUS_ADRERR).
- **posixcheck meanwhile:** XSH.2.4.3-signal-actions.021 requires the SIGBUS and si_addr, not the code.
- **Suggested change:** name the code for a reference beyond the end of a mapped object (BUS_ADRERR or BUS_OBJERR), or add one.

### SI-66: <signal.h>: SI_MESGQ is shaded in XSH 2.4.3 but not in the header's table

- **Where:** XBD `<signal.h>`, the table of si_code values; XSH 2.4.3.
- **What the text says:** XSH 2.4.3 shades SI_MESGQ with the Message Passing option; the `<signal.h>` table lists it unshaded.
- **Why it is a problem:** a Base system without message queues must still define a code it can never generate, or the header and chapter disagree on whether the symbol is optional.
- **posixcheck meanwhile:** the SI_MESGQ assertion is conditional on MSG.
- **Suggested change:** shade SI_MESGQ MSG in `<signal.h>` too, or unshade it in 2.4.3 (editorial).

### SI-67: fflush(): the RATIONALE contradicts the input-stream offset rule

- **Where:** XSH `fflush()`, RATIONALE and DESCRIPTION.
- **What the text says:** the RATIONALE says that repositioning the file descriptor after fflush() on streams open for reading is not mandated, because buffered data may make the position impractical to determine. Since Issue 7 (Interpretation 1003.1-2001 #002), the DESCRIPTION requires the offset of a seekable input stream not at end of file to be set to the stream's position.
- **Why it is a problem:** the informative text says the opposite of the normative text, which invites implementations to skip the rule (as glibc does in part, O.26).
- **posixcheck meanwhile:** the DESCRIPTION is followed (XSH.fflush.004, .005, .016).
- **Suggested change:** rewrite the RATIONALE to explain the Issue 7 rule (editorial).

### SI-68: XSH truncate(): an application requirement where an error is required

- **Where:** XSH `truncate()`, DESCRIPTION and ERRORS.
- **What the text says:** the DESCRIPTION says the application shall ensure the process has write permission for the file, while ERRORS says truncate() shall fail with EACCES when write permission is denied.
- **Problem:** read as an application requirement, a call without permission is undefined, which contradicts the required error.
- **Suggested change:** delete the application sentence.
- **Also:** the same pattern occurs in `mmap()` and `mprotect()`: an application requirement that PROT_WRITE not be asked for on a descriptor open read-only, next to a required [EACCES]. XSH.mmap.010 and XSH.mprotect.005 require EACCES.

### SI-69: XSH getrlimit(): RATIONALE says RLIMIT_FSIZE stays XSI

- **Where:** XSH `getrlimit()`, RATIONALE and CHANGE HISTORY.
- **What the text says:** the RATIONALE says the RLIMIT_CPU and RLIMIT_FSIZE limits remain in the XSI option; the Issue 8 CHANGE HISTORY (Austin Group defects 51 and 1669) moves the functions to Base excluding only RLIMIT_CPU, and the DESCRIPTION shades only RLIMIT_CPU and the SIGXFSZ clause of RLIMIT_FSIZE.
- **Problem:** the RATIONALE disagrees with the normative text.
- **Suggested change:** say that only RLIMIT_CPU remains XSI.

### SI-70: XSH sigaction(): APPLICATION USAGE says longjmp() is not async-signal-safe

- **Where:** XSH `sigaction()`, APPLICATION USAGE; XSH 2.4.3.
- **What the text says:** the APPLICATION USAGE says longjmp() and siglongjmp() are not in the list of async-signal-safe functions; the table in XSH 2.4.3 lists both.
- **Problem:** the informative text is out of date.
- **Suggested change:** update the APPLICATION USAGE.

### SI-71: XSH pthread_sigmask(): an unconditional requirement on how

- **Where:** XSH `pthread_sigmask()`, DESCRIPTION and ERRORS.
- **What the text says:** the application shall ensure how is one of SIG_BLOCK, SIG_SETMASK and SIG_UNBLOCK, without exception, yet how "is not significant" when set is a null pointer, and an invalid how is a required EINVAL.
- **Problem:** the application requirement overlaps both the null-set rule and the required error.
- **Suggested change:** limit the sentence to a non-null set, or drop it in favour of EINVAL.

### SI-72: XSH 2.5.1: "any handle to this open file descriptor"

- **Where:** XSH 2.5.1, first handle.
- **What the text says:** the item for a handle whose only further action is to be closed speaks of "any handle to this open file descriptor"; handles are defined on the open file description.
- **Problem:** the wording names the wrong object.
- **Suggested change:** read "open file description".
- **After the variant retrofit:** the same sentence can also be read two ways on whose actions count: "the only further action ... on any handle" may mean no handle at all will do more than close (so a second handle that goes on writing makes the item inapplicable), or that this handle will only be closed. XSH.2.5.1-stream-handles.012 rests on the second reading and is now `disputed`.

### SI-73: XSH fscanf(): "(except for %n)" and end-of-file

- **Where:** XSH `fscanf()`, DESCRIPTION (end-of-file clause).
- **What the text says:** the exception for %n sits on the conversion specification that meets end-of-file; ISO C instead lets a %n directive that follows a conversion ended by end-of-file still run.
- **Problem:** for sscanf("abcde", "%*s%n") the two texts give different results for the %n (POSIX leaves it untouched, ISO C stores 5). The page says it defers to ISO C where they conflict, which settles it, but the wording invites the other reading.
- **Suggested change:** move the parenthetical to the following-directive clause, as ISO C has it.
- **posixcheck meanwhile:** the tests avoid the case (XSH.fscanf.033 ends its input in white space); XSH.fscanf.042 is `partial`.

### SI-74: XSH fopen() and freopen(): the two ENAMETOOLONG conditions are swapped

- **Where:** XSH `fopen()` and `freopen()`, ERRORS.
- **What the text says:** fopen() lists a pathname longer than {PATH_MAX} as a "shall fail" error and a component longer than {NAME_MAX} as "may fail"; freopen() lists the component case as "shall fail" and the whole-pathname case as "may fail". freopen()'s change history says a second [ENAMETOOLONG] was added as optional.
- **Problem:** the same two conditions have opposite strength on two pages that describe the same open.
- **Suggested change:** make freopen() match fopen() (or the reverse), whichever was meant.
- **posixcheck meanwhile:** the {NAME_MAX} cases are required for both (XSH.fopen.025, .030, XSH.freopen.020), taking the stronger listing for each.

### SI-75: XSH strerror(): two statements about overwriting by strerror_l()

- **Where:** XSH `strerror()`, DESCRIPTION.
- **What the text says:** the string "might be overwritten" by a later strerror_l() in the same thread, and, in a separately shaded sentence, "may be overwritten" by a later strerror_l() in the same thread.
- **Problem:** the same permission is given twice with different modal verbs, and which string "it" means is not clear (the one from strerror() or from strerror_l()).
- **Suggested change:** state the permission once, and say for which function's result.
- **posixcheck meanwhile:** XSH.strerror.006 and .007 check only what the page forbids (no overwriting by other threads or by strerror_l() with a different locale object).

### SI-76: XSH gmtime(): tm_zone is set only in the gmtime_r() text

- **Where:** XSH `gmtime()`, RETURN VALUE.
- **What the text says:** gmtime_r() sets the result's tm_zone to the string "UTC"; the text for gmtime() never mentions tm_zone, although the structure now has the member.
- **Problem:** it is unclear whether gmtime() (static result) must set tm_zone too.
- **Suggested change:** say that gmtime() sets tm_zone as gmtime_r() does, or that it is unspecified.
- **posixcheck meanwhile:** only gmtime_r() is tested (XSH.gmtime.003).

### SI-77: XSH mktime(): "shall attempt to determine" whether DST is in effect

- **Where:** XSH `mktime()`, DESCRIPTION (negative tm_isdst).
- **What the text says:** with a negative tm_isdst, mktime() "shall attempt to determine" whether daylight saving time is in effect.
- **Problem:** an attempt may fail, and the text does not say what result a failed attempt gives.
- **Suggested change:** say that mktime() determines it, or say what happens when it cannot.
- **posixcheck meanwhile:** no assertion depends on a failed attempt.

### SI-78: XSH strftime(): %z with tm_isdst 0 and a DST tm_gmtoff

- **Where:** XSH `strftime()`, DESCRIPTION (%z).
- **What the text says:** %z is built from "tm_isdst and tm_gmtoff" (and from the standard offset); the page does not say which wins when tm_isdst is 0 but tm_gmtoff still holds a summer offset.
- **Problem:** a structure whose members disagree can be read as asking for the standard offset or the summer one.
- **Suggested change:** say that %z uses tm_gmtoff when it is available, or the standard offset when tm_isdst is 0.
- **posixcheck meanwhile:** XSH.strftime.011 is required and accepts both outputs.

### SI-79: XSH fmemopen(): the append position "one byte after the end of the buffer"

- **Where:** XSH `fmemopen()`, DESCRIPTION.
- **What the text says:** in append mode, with no null byte in the buffer, the initial position is "one byte after the end of the buffer"; elsewhere the page says no position beyond max_size can be reached, and the application-usage note describes a buffer filled to max_size.
- **Problem:** a position one past the end would be max_size + 1 bytes from the start if "end" meant the last byte, and max_size if it meant the end position.
- **Suggested change:** say "at the end of the buffer (position max_size)".
- **posixcheck meanwhile:** XSH.fmemopen.010 takes max_size.

### SI-80: XSH pthread_mutex_lock(): NORMAL relock "deadlock" against the may-fail [EDEADLK]

- **Where:** XSH `pthread_mutex_lock()`, DESCRIPTION table and ERRORS.
- **What the text says:** the table says a relock of a NORMAL mutex by its owner deadlocks, while ERRORS says the call may fail with [EDEADLK] when a deadlock is detected.
- **Problem:** an implementation that detects the self-deadlock and returns EDEADLK may or may not conform; the table reads as if no error comes back.
- **Suggested change:** say in the table that EDEADLK is permitted for NORMAL mutexes, or that it is not.
- **posixcheck meanwhile:** XSH.pthread_mutex_lock.011 accepts both: no success, and either a block or EDEADLK.

### SI-81: XSH sysconf(): the table names _POSIX_SS_REPL_MAX for a limit

- **Where:** XSH `sysconf()`, the table of variables; RATIONALE.
- **What the text says:** the table row pairs _SC_SS_REPL_MAX with _POSIX_SS_REPL_MAX, the form used for options; the limit is {SS_REPL_MAX}, and the RATIONALE speaks of "_POSIX_SS_REPL_MAX" as a related variable of _POSIX_SPORADIC_SERVER.
- **Problem:** it is unclear whether the name is a typo for {SS_REPL_MAX}.
- **Suggested change:** name the limit as {SS_REPL_MAX}, or define _POSIX_SS_REPL_MAX.
- **posixcheck meanwhile:** XSH.sysconf.002 checks the value under either name.

### SI-82: XSH fchdir() and readlink(): wording slips

- **Where:** XSH `fchdir()`, ERRORS; XSH `readlink()`, RATIONALE.
- **What the text says:** the fchdir() "may fail" list is introduced by "The fchdir() may fail if" (no word "function"); the readlink() RATIONALE calls the parameter bufsiz where the SYNOPSIS says bufsize.
- **Problem:** wording only.
- **Suggested change:** correct both.
- **posixcheck meanwhile:** no test depends on it.

### SI-83: XSH fprintf(): wording gaps

- **Where:** XSH `fprintf()`, DESCRIPTION and ERRORS.
- **What the text says:** (a) the e/E exponent "shall always contain at least two digits" and ISO C adds "no more digits than necessary"; the page does not repeat the upper limit. (b) "If the value is zero, the exponent shall be zero" does not say whether it carries a sign. (c) dprintf() is told to take its errors from fputc(), but it has no stream.
- **Problem:** each leaves a conforming output or error unspecified or only fixed by reading ISO C.
- **Suggested change:** repeat the limit, say "+00" or either sign, and name write() for dprintf().
- **posixcheck meanwhile:** XSH.fprintf.026 accepts more digits and either sign for a zero exponent; dprintf's errors are tested with a bad descriptor only.

### SI-84: XBD 3.253 against _Exit(): when a child's parent changes

- **Where:** XBD 3.253 (Parent Process), 3.285 (Zombie Process); XSH `_Exit()`, DESCRIPTION.
- **What the text says:** the creator stays the parent for its whole lifetime, which includes the zombie state until it is reaped; _Exit() says children of the terminating process are adopted when it terminates.
- **Problem:** between termination and reaping a child's getppid() could be either the dead creator or the adopter.
- **Suggested change:** say which, for the zombie window.
- **posixcheck meanwhile:** XSH.getppid.002 tests only after the creator has been reaped, where both agree.

### SI-85: XSH getgroups(): CHANGE HISTORY says 0 is not permitted, DESCRIPTION allows it

- **Where:** XSH `getgroups()`, DESCRIPTION and CHANGE HISTORY.
- **What the text says:** the change history says a return value of 0 is no longer permitted; the description allows it when the effective group ID is not included in the list and there are no other groups.
- **Problem:** informative text contradicts normative text.
- **Suggested change:** correct the change history.
- **posixcheck meanwhile:** XSH.getgroups.001 to .003 accept 0 where the effective group ID is not reported.

### SI-86: XSH setreuid(), setregid(), setgid(): wording slips

- **Where:** XSH `setreuid()` DESCRIPTION; `setregid()` APPLICATION USAGE; `setgid()` RETURN VALUE.
- **What the text says:** setreuid(): "If ruid or euid is -1, the corresponding effective or real user ID" has the two the wrong way round; setregid() APPLICATION USAGE says "real user ID" and "saved-group-ID" where the real group ID and the saved set-group-ID are meant; setgid() RETURN VALUE "0 is returned" lacks a "shall".
- **Problem:** wording only.
- **Suggested change:** correct the three.
- **posixcheck meanwhile:** no test depends on it.

### SI-87: XSH nanosleep() against clock_getres(): measured by CLOCK_REALTIME, or independent of it

- **Where:** XSH `nanosleep()`, DESCRIPTION; XSH `clock_getres()`, DESCRIPTION.
- **What the text says:** nanosleep() measures the suspension by CLOCK_REALTIME; clock_getres() says that setting CLOCK_REALTIME does not affect relative sleeps, which expire independently of the clock's value.
- **Problem:** if the clock is stepped during a sleep the two statements give different durations.
- **Suggested change:** say which clock a relative sleep follows.
- **posixcheck meanwhile:** nanosleep tests measure with CLOCK_REALTIME and are UNRESOLVED when CLOCK_REALTIME and CLOCK_MONOTONIC disagree about the interval (XSH.nanosleep.001, .003, .006, .008).

### SI-88: XSH clock_getres() and clock_nanosleep(): wording slips and an overlap

- **Where:** XSH `clock_getres()`, DESCRIPTION; XSH `clock_nanosleep()`, DESCRIPTION and ERRORS.
- **What the text says:** (a) the process CPU-time paragraph says the values "represent" execution time, the thread paragraph says "shall represent"; (b) the truncation of set values is stated twice, once without "shall"; (c) for an absolute time already reached, "the calling process shall not be suspended" where the thread is meant; (d) the calling thread's CPU-time clock is named under [EINVAL], while [ENOTSUP] cites "a CPU-time clock" as its example, so XSH 2.3 lets either be returned.
- **Problem:** wording only, except (d), where two errors apply.
- **Suggested change:** make the modal verbs and the subject agree, and choose one error for (d).
- **posixcheck meanwhile:** XSH.clock_nanosleep.008 accepts both errors.

## Questions to raise

These are consistent as written, but the intent or the fit with practice
is unclear.

### Q-1: Should SIGPIPE follow EPIPE after shutdown(SHUT_WR)?

- **Where:** XSH `write()` and `send()`, ERRORS, [EPIPE].
- **What the text says:** a socket shut down for writing gives EPIPE, but
  SIGPIPE is required only when the socket is no longer connected.
- **Practice:** Linux sends SIGPIPE after `shutdown(SHUT_WR)` too; the BSDs
  are believed to (not yet verified).
- **Question:** is the narrower wording intended, or should the signal be
  required, or explicitly allowed, in both cases?
- **posixcheck meanwhile:** XSH.write.017 requires EPIPE only.

### Q-2: lseek() to an offset beyond the largest file the file system can hold?

- **Where:** XSH `lseek()`, DESCRIPTION and ERRORS; XSH `open()`; XSH 2.3.
- **What the text says:** offsets beyond end of file are allowed; the only
  errors are a bad whence or negative result (EINVAL) and a result that
  does not fit in off_t (EOVERFLOW). Size limits appear only as EFBIG on
  writes.
- **Practice:** Linux rejects offsets above the file system's maximum file
  size with EINVAL, and reports EINVAL, not EOVERFLOW, when SEEK_CUR
  overflows off_t.
- **Question:** is "beyond the file system's maximum" an extra error XSH 2.3
  allows, or is the offset valid until a write gives EFBIG? Either way the
  page could say so.
- **posixcheck meanwhile:** asserts only the off_t overflow case
  (XSH.lseek.011); Linux fails it.
- **After the Linux-ism audit:** XSH.lseek.011 is now `disputed`: the overflowing result is also beyond every file system's largest file, so XSH 2.3 lets either error be reported.

### Q-3: A fork child and an unnamed semaphore with pshared 0?

- **Where:** XSH `fork()`, DESCRIPTION; XSH `sem_init()`, DESCRIPTION.
- **Question:** the child gets a copy of the semaphore's memory. Is it a
  usable, independent semaphore, is using it undefined, or is it
  unspecified? The rule for process-private mutexes suggests an answer, but
  nothing states it for semaphores.
- **posixcheck meanwhile:** not tested.

### Q-4: Is a zombie still a member of its process group when deciding whether the group is orphaned?

- **Where:** XBD 3, definitions of orphaned process group, process lifetime
  and process group lifetime; XSH `_Exit()`, Consequences of Process
  Termination.
- **Question:** if the exiting process is the member whose parent links
  the group to the session, its zombie keeps that link until reaped, so
  read literally its exit orphans nothing and the SIGHUP/SIGCONT rule never
  applies in that case. Is the exiting process meant to be left out, as
  implementations do?
- **Practice:** Linux and the BSDs leave it out.
- **posixcheck meanwhile:** XSH._Exit.010 uses a layout that needs no reading.

### Q-5: Which of wait()'s two SIGCHLD paragraphs applies?

- **Where:** XSH `wait()`, DESCRIPTION (the queuing and the otherwise
  paragraphs); XSH 2.4.1.
- **Question:** wait() treats queuing SIGCHLD as a fixed property; 2.4.1
  makes repeated delivery implementation-defined and ties queuing to
  SA_SIGINFO elsewhere. May an implementation queue SIGCHLD only with
  SA_SIGINFO, and which paragraph then applies without it?
- **posixcheck meanwhile:** XSH.wait.019 uses one child, where both agree.

### Q-6: When is another child's status "available"?

- **Where:** XSH `wait()`, DESCRIPTION (the paragraph for SIGCHLD blocked).
- **Question:** does a stopped child count for a wait() that cannot report
  stops? Does a child outside waitpid()'s pid set count? And in that case
  the paragraph only exempts the signal from clearing; unlike the queuing
  paragraph, it does not say it stays pending.
- **posixcheck meanwhile:** not tested; needs two children.

### Q-7: What does exit() do for a stream that has no file descriptor?

- **Where:** XSH `exit()`, DESCRIPTION (the CX sentence closing each
  stream's descriptor); XSH `fmemopen()`, `open_memstream()`.
- **Question:** memory streams have no descriptor. The page no longer says,
  as ISO C does, that streams are closed, so it is unclear whether exit()
  must do for them what fclose() would (for `open_memstream()`, update the
  buffer pointer and size).
- **posixcheck meanwhile:** not tested.

### Q-8: Can ENAMETOOLONG or ENOTDIR be required when ENOENT also applies?

- **Where:** XSH 2.3 Error Numbers; ERRORS of exec, `stat()` and other lookups.
- **Question:** for a lookup, a component longer than {NAME_MAX} can never
  name an existing file, so ENOENT always applies too and XSH 2.3 lets
  either be reported; likewise a missing name under a non-directory prefix
  (ENOTDIR and ENOENT). Read so, ENAMETOOLONG is never required of such
  interfaces. Is that intended, or should ENOENT exclude these cases?
- **posixcheck meanwhile:** XSH.exec.038 and 041 accept ENOENT as well; for
  open() the suite avoids the overlap with O_CREAT (XSH.open.014, 017); the
  file and directory modules mark such assertions `disputed` (a FAIL is
  logged as INSPECT), for example XSH.fstatat.020, XSH.link.014,
  XSH.chmod.012.
- **After the Linux-ism audit:** Cases where only one error condition applies are now required assertions of their own (XSH.link.024, .025, XSH.unlink.023, XSH.rename.035, XSH.fstatat.026, XSH.chmod.022, XSH.chown.022, XSH.access.021, XSH.lchown.008, .009); only the overlapping cases stay disputed.
- **After the variant retrofit:** the same overlap arises with ELOOP: a path that resolves through a loop of symbolic links names no existing file, so ENOENT applies literally too. XSH.unlink.013 and XSH.rmdir.009 are now `disputed`, and the loop-in-the-first-path cases of link() and rename() are split out as XSH.link.026 and XSH.rename.036 (`disputed`). More lookups in the file and directory modules are `disputed` for the same overlap: XSH.chdir.005 and .006, XSH.realpath.006 to .008, XSH.readlink.008 to .010 (see also Q-67 for chdir() of a regular file). XSH.freopen.018 and .020 follow XSH.fopen.025 and take ENAMETOOLONG/ENOTDIR as required. A loop in the directory part of the second path (XSH.link.013, XSH.rename.021) stays required: the prefix component exists, as a symbolic link, so ENOENT for a missing prefix component does not apply.

### Q-9: Which {ARG_MAX} does E2BIG compare against?

- **Where:** XSH exec, ERRORS [E2BIG]; XBD `<limits.h>`, Runtime Invariant
  Values; XSH `sysconf()`.
- **Question:** when sysconf() reports less than the system enforces (musl
  reports 131072; Linux accepts about 2 MiB), exec succeeds above the
  reported {ARG_MAX}. `<limits.h>` makes sysconf() give the actual value,
  so the fault is sysconf()'s, but the exec page could say which value it
  means. Linux also derives the limit from the stack size limit, which fits
  badly with a runtime invariant value.
- **posixcheck meanwhile:** XSH.exec.043 uses sysconf()'s value; musl fails.
- **After the Linux-ism audit:** XSH.exec.043 is now `disputed`, and accepts ENOMEM when the arguments also exceed the stack limit (Q-53).

### Q-10: What does execvp()'s sh fallback pass when argv[0] is a null pointer?

- **Where:** XSH exec, DESCRIPTION (the sh fallback takes arguments from
  argv[1] on); RATIONALE (argc may be zero).
- **Question:** with argv[0] null, argv[1] is past the array. Should the
  fallback pass no arguments then?
- **posixcheck meanwhile:** not tested.

### Q-11: Must raise() deliver the signal before it returns?

- **Where:** XSH `raise()`, DESCRIPTION; XSH `pthread_kill()`; XSH `kill()`.
- **Question:** kill() to the calling process requires at least one unblocked pending signal to be delivered before it returns. raise() only says that, if a handler is called, raise() does not return before the handler does, and is equivalent to pthread_kill() on the calling thread, which has no delivery-before-return rule. Is a raise() that returns before the handler runs (the handler running later) conforming? ISO C expects the handler to run within raise().
- **posixcheck meanwhile:** tests that check a handler ran right away use kill(getpid(), sig).
- **Update:** raise() itself is settled by its own rule that it does not return until after a handler it causes has returned: for an unblocked, caught signal that handler will run, so it must run before raise() returns (XSH.raise.001). The question stays for pthread_kill() to the calling thread.

### Q-12: kill() with the most negative pid_t?

- **Where:** XSH `kill()`, DESCRIPTION (pid below -1: the group whose ID is the absolute value of pid); RATIONALE.
- **Question:** the absolute value of the most negative pid_t cannot be represented. Is ESRCH required (what most systems give), EINVAL, or is the result unspecified? The RATIONALE only mentions extensions for large values.
- **posixcheck meanwhile:** not tested.

### Q-13: How does alarm() round the seconds it returns?

- **Where:** XSH `alarm()`, RETURN VALUE.
- **Question:** a previous request with time left returns a non-zero value that is the number of seconds until it would have fired. The time left is rarely a whole number of seconds and no rounding is stated. With under a second left only rounding up gives a non-zero value; is rounding up (or to nearest, but at least 1) the intent for every fraction?
- **posixcheck meanwhile:** XSH.alarm.002 expects 1 with under a second left (the only non-zero value of at most 1); XSH.alarm.003 accepts 8 to 10 for about 9 seconds left.

### Q-14: May alarm() deliver SIGALRM early?

- **Where:** XSH `alarm()`, DESCRIPTION and RATIONALE; XSH `sleep()`, RATIONALE.
- **Question:** the DESCRIPTION generates SIGALRM after the given number of realtime seconds have elapsed. The RATIONALE notes that historical systems fire up to a second early and calls not firing early only the "most appropriate" choice. Is an early SIGALRM nonconforming, as the DESCRIPTION reads?
- **posixcheck meanwhile:** XSH.alarm.001 treats an early signal as a FAIL, with 10 ms tolerance for clock granularity.
- **sleep():** the same question for sleep(), whose RATIONALE extends the allowance; XSH.sleep.002 (`disputed`) asks for the full interval, the other sleep() tests only for 1 s of 2.
- **After the Linux-ism audit:** The early-signal checks moved to XSH.alarm.006 and .007, both `disputed`; XSH.alarm.001, .003 and .004 no longer depend on this question, and .004 no longer has an upper bound.
### Q-15: pause() and sigsuspend(): delivery to which thread?

- **Where:** XSH `pause()` and `sigsuspend()`, DESCRIPTION.
- **Question:** both suspend the calling thread until delivery of a signal whose action is to run a handler or terminate the process, without saying the signal is delivered to the calling thread. Read literally, a process-directed signal handled by another thread ends the wait. Implementations end it only for delivery to the calling thread; should the text say so?
- **posixcheck meanwhile:** the tests are single-threaded, or (XSH.sigsuspend.006) direct the signal at the waiting thread.

### Q-16: Is {RTSIG_MAX} the size of the realtime range, or a lower bound?

- **Where:** XBD `<limits.h>` ({RTSIG_MAX}); XBD `<signal.h>` (SIGRTMIN, SIGRTMAX).
- **Question:** `<limits.h>` describes {RTSIG_MAX} as the maximum number of realtime signals reserved for application use; `<signal.h>` says the range reserved for application use includes at least {RTSIG_MAX} numbers. Together they make the range exactly {RTSIG_MAX} long. Is that intended, or is one of them meant loosely?
- **posixcheck meanwhile:** XBD.signal.h.006 checks only the lower bound (at least {RTSIG_MAX}).

### Q-17: Are kill()-generated occurrences ordered with sigqueue() ones?

- **Where:** XSH 2.4.2.
- **Question:** occurrences generated by sigqueue() and other value-carrying functions are said to be queued in FIFO order (a statement without "shall"); whether kill() queues an occurrence is implementation-defined, and nothing orders such an occurrence relative to the sigqueue() ones. Is the FIFO statement a requirement, and does it cover a queued kill() occurrence?
- **posixcheck meanwhile:** XSH.2.4.2-realtime-signals.004 requires FIFO order only among the sigqueue() occurrences and lets an SI_USER one come anywhere.

### Q-18: SIGCONT and stopped processes in 2.4.3

- **Where:** XSH 2.4.1 and 2.4.3.
- **Question:** 2.4.3 says a stopped process gets no further signals until continued, naming only SIGKILL as an exception, though 2.4.1 makes SIGCONT continue the process whatever its action; the "continue" leg of the rule that a signal delivered to a thread continues the whole process can never apply, since continuing happens at generation; and the rule that pending unblocked signals are handled before execution resumes sits under SIG_DFL, so its order is unstated when SIGCONT is caught or ignored. Is the order meant to hold whatever SIGCONT's action?
- **posixcheck meanwhile:** XSH.2.4.3-signal-actions.006 uses SIGCONT at SIG_DFL.

### Q-19: May psiginfo() add details from the siginfo_t?

- **Where:** XSH `psiginfo()`.
- **Question:** psiginfo() writes the message, ": " and "the signal description string associated with ... the signal indicated by pinfo". glibc adds the cause and sender (for example "(Signal sent by kill() 759 0)") after the description. Is the description meant to be the same string psignal() writes for that signal number, or may it carry what the siginfo_t says?
- **posixcheck meanwhile:** XSH.psiginfo.003 is `disputed`: it expects psignal()'s line, and a difference is logged as INSPECT.

### Q-20: Must sigaltstack() report SS_ONSTACK and SS_DISABLE in oss?

- **Where:** XSH `sigaltstack()`, DESCRIPTION.
- **Question:** the oss ss_flags member "may contain" SS_ONSTACK (the process is executing on the stack) or SS_DISABLE. Is reporting the state required, with "may" only meaning that one value or neither applies? The SS_ONSTACK description also speaks of the process, while the stack is per thread (compare SI-26).
- **posixcheck meanwhile:** XSH.sigaltstack.009 (SS_ONSTACK reported inside the handler) is `disputed`; XSH.sigaltstack.002 and .003 require SS_DISABLE to be reported.
- **After the Linux-ism audit:** The same "may contain" wording governs SS_DISABLE. XSH.sigaltstack.002, .008 and the new .013 (SS_DISABLE reported) are now `disputed` too; XSH.sigaltstack.003, .012 and XSH.sigaction.009 keep only their behaviour.

### Q-21: abort() with SIGABRT caught but blocked: must the handler run?

- **Where:** XSH `abort()`, DESCRIPTION.
- **Question:** abort() overrides blocking or ignoring SIGABRT, and terminates the process unless a SIGABRT it generates is caught and the handler does not return. When SIGABRT is caught but blocked, is the handler required to run (glibc), or is terminating without it (musl) also overriding the block?
- **posixcheck meanwhile:** XSH.abort.006 is `disputed` (expects the handler to run).

### Q-22: How can MINSIGSTKSZ be a compile-time constant when the signal frame size depends on the CPU?

- **Where:** XBD `<signal.h>`; XSH `sigaltstack()`.
- **Question:** MINSIGSTKSZ is a symbolic constant (an integer constant expression) giving the minimum stack size for a signal handler, but on current hardware the frame the system builds depends on the CPU (vector registers such as AVX-512), known only at run time. Should MINSIGSTKSZ and SIGSTKSZ be allowed to be run-time values, or should a sysconf() name be added? (Check the Austin Group tracker for an existing report.)
- **posixcheck meanwhile:** XSH.sigaltstack.010 expects a handler to run on a stack of exactly MINSIGSTKSZ bytes; it FAILs on x86-64 Linux with AVX-512 (docs/linux-deviations.md O.16).

### Q-23: signal()'s return value: the most recent call, or the most recent successful one?

- **Where:** XSH `signal()`, RETURN VALUE.
- **Question:** signal() returns the value of func for the most recent call to signal() for the signal. A failed call, or a sigaction() in between, is not mentioned. Is the most recent successful call meant (as ISO C has it), and what is returned after sigaction() changed the action?
- **posixcheck meanwhile:** XSH.signal.001 uses successful signal() calls only.

### Q-24: link(): what "permission to access the existing file" may an implementation require?

- **Where:** XSH `link()`, DESCRIPTION and ERRORS ([EACCES]).
- **Question:** the implementation may require that the calling process has permission to access the existing file, and [EACCES] covers the case where it lacks that permission. "Access" is not defined: is read permission enough, or may an implementation demand ownership, or read and write permission? And when a security policy refuses (Linux `fs.protected_hardlinks`, FreeBSD `security.bsd.hardlink_check_uid`), is EPERM an allowed additional error, or must the error be [EACCES] (XSH 2.3 forbids a different error number for a condition the page describes)?
- **posixcheck meanwhile:** XSH.link.019 is `disputed`: linking another user's readable file must succeed or fail with EACCES; EPERM is logged as INSPECT.
- **After the Linux-ism audit:** When path1 is a directory and the implementation also requires access to the existing file, both EPERM and EACCES apply; should EPERM take precedence? XSH.link.012 gives the directory to the unprivileged child, so only EPERM applies there.

### Q-25: chown() with owner and group both -1 on another user's file

- **Where:** XSH `chown()`, DESCRIPTION and ERRORS ([EPERM]).
- **Question:** [EPERM] applies when "the effective user ID does not match the owner of the file". With owner (uid_t)-1 and group (gid_t)-1 nothing is changed (and st_ctim need not be marked). Must the call still fail with EPERM for a process that is not the owner, or may it succeed as a no-op, as Linux does?
- **Also:** Linux not only returns 0 but marks st_ctim of the other user's file for update, so the call is not quite a no-op there.
- **posixcheck meanwhile:** XSH.chown.008 is `disputed`: success is logged as INSPECT.

### Q-26: truncate()/ftruncate(): does the file size limit apply to a shrink that stays above it?

- **Where:** XSH `truncate()` and `ftruncate()`, DESCRIPTION and [EFBIG].
- **Question:** the request fails if it "would cause the file size to exceed" the soft limit, and [EFBIG] applies when the length "exceeds the file size limit". Shrinking a 10000-byte file to 8000 bytes under a 4096-byte limit leaves it above the limit. Must that fail, as a literal reading says, or does the limit apply only to growth, as write() and every reference system (which succeed) treat it?
- **posixcheck meanwhile:** only growth beyond the limit is tested (XSH.truncate.007, XSH.ftruncate.008).

### Q-27: futimens(): is write access the file's permission or the descriptor's mode?

- **Where:** XSH `futimens()`, DESCRIPTION (who may set the current time) and [EACCES].
- **Question:** for futimens(fd, NULL), is "write access to the file" decided by the file's permission bits, or by whether fd is open for writing? The Linux man page has long said the kernel checks the descriptor; Linux 6.18 decides by the file's permissions. The sentence on the O_SEARCH check also says "the function" where only utimensat() can be meant.
- **posixcheck meanwhile:** XSH.futimens.021 requires success for a read-only descriptor of a file the process may write.
- **After the Linux-ism audit:** The read-only-descriptor case is now XSH.futimens.026, `disputed`; XSH.futimens.021 keeps the rest, required.

### Q-28: fflush(NULL) and the input-stream offset rule

- **Where:** XSH `fflush()`, DESCRIPTION.
- **Question:** the page gives two behaviours: writing an output stream's unwritten data (from ISO C), and, as a POSIX addition, setting a seekable input stream's file offset to the stream's position. For a null pointer, fflush() performs "this flushing action on all streams for which the behavior is defined above". Does that include the input-stream rule, so that fflush(NULL) sets every seekable input stream's offset, or only the flushing of output, as in ISO C? Neither glibc nor musl touches input streams for fflush(NULL), though both do for fflush(stream).
- **posixcheck meanwhile:** XSH.fflush.011 is `disputed`: an untouched offset is logged as INSPECT.

### Q-29: "If the file is not already at EOF": the stream's indicator or the file offset?

- **Where:** XSH `fflush()`, `fclose()` and `exit()` (the input-stream offset rule); XSH 2.5.1.
- **Question:** the offset rule applies "if the file is not already at EOF". Does that mean the stream's end-of-file indicator (XSH 2.5.1 speaks of feof() being true), or the underlying file offset being at the end of the file? After one read of a small file, read-ahead has usually taken the whole file: the indicator is clear but the descriptor is at the end, so the two readings give different answers.
- **posixcheck meanwhile:** the tests read a 1 MiB file, so both readings require the rule; XSH.fflush.015 checks that a stream whose indicator is set is left alone.
- **After the Linux-ism audit:** Moving the descriptor while the stream is at end of file and then using the stream without fseek() is undefined (XSH 2.5.1, second handle), so the "offset" reading cannot be tested at end of file without undefined behaviour. XSH.fflush.015 no longer moves the descriptor.
- **After the variant retrofit:** correction: a 1 MiB file meets both readings only if the stream reads less than 1 MiB ahead; a conforming system may buffer the whole file. The tests now grow the file through a separate open() after the reads (a new open file description, so not a handle on the stream's), so the descriptor is not at the end under either reading and the rule is required: XSH.fflush.004, .005, .011, .016, XSH.fclose.003, .010, .011, XSH.ungetc.010, XSH.2.5.1-stream-handles.014. XSH.fflush.015 (a stream at end of file) is UNRESOLVED when the offset is not the file size, since XSH 2.5.1 lets stdio be built on something other than read().

### Q-30: fflush(): the stream's position after pushed-back bytes are discarded

- **Where:** XSH `fflush()`, DESCRIPTION; XSH `ungetc()`.
- **Question:** fflush() of an input stream sets the offset to the stream's position and discards pushed-back bytes "without further changing the file offset". With "01" read and 'x' pushed back (position 1), must the next read return '1' (the byte at the new offset), or may it resume where the stream was before the pushback ('2', as glibc does)? ungetc() keeps ISO C's rule for reading the pushed-back bytes but does not say what discarding them does to the position.
- **posixcheck meanwhile:** XSH.fflush.005 checks the offset and that the discarded byte is not read, not which byte comes next.

### Q-31: _exit(): the other waiting threads once the last child is reaped

- **Where:** XSH `_exit()`, Consequences of Process Termination; XSH 2.12.
- **Question:** when several threads of the parent are blocked in wait() or waitpid() for a child that exits, one of them obtains the status. Only the XSI case (SIGCHLD ignored or SA_NOCLDWAIT) then says a waiter with no remaining children fails with ECHILD. In the ordinary case, must the other threads fail with ECHILD once no children remain, or may they stay blocked? wait()'s ERRORS suggest ECHILD, but the text does not say so.
- **posixcheck meanwhile:** XSH._Exit.015 requires that exactly one thread gets the status; XSH._Exit.020 (the other gets ECHILD) is `disputed`.
- **After the Linux-ism audit:** XSH._Exit.015 and .020 no longer join the thread that may stay blocked; .020 FAILs (INSPECT) if it is still blocked 2 s later.

### Q-32: _exit(): completing or canceling outstanding asynchronous I/O

- **Where:** XSH `_exit()`, Consequences of Process Termination.
- **Question:** operations not canceled at exit "complete as if _exit() had not yet occurred" and their signal notifications are suppressed, while which operations are canceled is implementation-defined (and "cancelable" is defined only through aio_cancel()). After the process is gone, a dropped write cannot be told from a canceled one, and the suppression of a signal to a process that no longer exists cannot be observed. Is a write that never reaches the file conforming if the implementation documents that it cancels such writes? Should the text say which operations an implementation may cancel at exit?
- **posixcheck meanwhile:** XSH._Exit.019 is `disputed`: a short file is logged as INSPECT.

### Q-33: setvbuf(): required buffering modes against implementation-defined support

- **Where:** XSH `setvbuf()`, DESCRIPTION and RETURN VALUE; XSH 2.5.
- **Question:** setvbuf() says each of _IOFBF, _IOLBF and _IONBF shall cause that kind of buffering, and that a request that cannot be honoured makes it return non-zero. XSH 2.5 (following ISO C) makes support for the buffering characteristics implementation-defined. Together these appear to let an implementation refuse any mode, or accept it and buffer some other way. Must an accepted _IONBF stream send each byte at once and an accepted _IOLBF stream send each line at its newline? Is setvbuf() still permitted after an unsuccessful call, given that the text mentions this only as an allowance for applications?
- **posixcheck meanwhile:** XSH.setvbuf.001 checks only that data arrives intact under each accepted mode and notes refusals. XSH.setvbuf.003, .004 and .006 check the observable effect of an accepted mode and are `disputed`: a FAIL is logged as INSPECT, and a refusal is UNRESOLVED.
- **Also:** fopen() says a stream is fully buffered if and only if it can be determined not to refer to an interactive device, but "interactive device" is not defined and the same implementation-defined support applies. XSH.fopen.019 (a terminal, XSI) and XSH.fopen.020 (a regular file) are `disputed` under this question.
- **After the Linux-ism audit:** XSH 2.5.1's first-handle items for unbuffered and line-buffered streams rest on the same point: XSH.2.5.1-stream-handles.004 and .005 are both `disputed`.

### Q-34: ungetc(): the limit on repeated pushback and fflush()

- **Where:** XSH `ungetc()`, DESCRIPTION.
- **Question:** a successful fflush() discards pushed-back bytes, like the file-positioning functions (a CX addition, Austin Group defect 87). But the sentence allowing ungetc() to fail after too many calls names only an intervening read or file-positioning operation, not fflush(). After fflush() has discarded the pushback, may ungetc() still fail because of the calls made before it? The two sentences should probably name the same operations.
- **posixcheck meanwhile:** the permission is not tested. XSH.ungetc.010 checks that fflush() discards the byte.

### Q-35: fseek(): the conditions in the ERRORS preamble

- **Where:** XSH `fseek()`, ERRORS.
- **Question:** the "shall fail" list applies only when the stream is unbuffered or its buffer needed to be flushed, and the call invokes an underlying lseek() or write(). Read literally, a buffered stream with nothing to flush need not fail with EINVAL for an invalid whence or a negative position, or with ESPIPE on a pipe. Yet the same list names conditions that involve neither call: an fmemopen() position beyond the end of the buffer (EINVAL) and open_memstream() running out of memory (ENOMEM). Is the preamble meant to restrict only the errors that come from lseek() and write() (EAGAIN, EFBIG, EINTR, EIO, ENOSPC, EPIPE)? Should EINVAL and ESPIPE be required whatever the buffer state, as ISO C's "a request that cannot be satisfied" suggests?
- **Also:** does the DESCRIPTION's rule that fseek() writes unwritten data apply to a call that fails? If so, pending output always satisfies the preamble; if not, a system can avoid it by validating first, as glibc and musl do for an invalid whence (the data is not written). And fmemopen()'s page requires a seek beyond max_size to fail with no such preamble, while fseek()'s EINVAL for the same case sits under it; the two pages should agree (fmemopen() says "larger than the buffer size", fseek() "beyond the end of the underlying memory buffer").
- **posixcheck meanwhile:** XSH.fseek.009, .010, .011, .014, .015, .016 and .017 are `disputed`; the same preamble question applies to fsetpos() and rewind() and their notes cite this entry. The flush errors that need a write() (EBADF, EFBIG, EPIPE, EAGAIN: XSH.fseek.012, .013, .018, .020) stay required, and the fmemopen() failure itself is required by XSH.fseek.019.

### Q-36: fdopen(): the 'a' modes

- **Where:** XSH `fdopen()`, DESCRIPTION; XSH 2.5.
- **Question:** fdopen() sets the new stream's position from the descriptor's offset, but XSH 2.5 makes the initial position of a stream opened in append mode implementation-defined (beginning or end). Which governs fdopen(fd, "a"), and may fdopen() move the shared offset (glibc seeks the descriptor to the end; musl keeps it)? The 'a' paragraph also lacks the "O_APPEND set shall remain set" sentence that 'r' and 'w' have, and when O_APPEND stays clear (allowed) nothing says whether the stream forces its writes to the end of file, as fopen()'s append mode does.
- **posixcheck meanwhile:** XSH.fdopen.013 (position kept for "a" and "a+") is `disputed`; XSH.fdopen.010 uses descriptors that already have O_APPEND set.

### Q-37: {STREAM_MAX}

- **Where:** XBD `<limits.h>`, Runtime Invariant Values; XSH `fopen()`, `fdopen()`, ERRORS.
- **Question:** {STREAM_MAX} is a runtime invariant whose actual value sysconf() reports, yet BSD-family systems derive it from RLIMIT_NOFILE, which setrlimit() changes. When defined it equals {FOPEN_MAX}, ISO C's guaranteed minimum, and the EMFILE "shall fail" then turns that minimum into a hard ceiling. Nor does the text say whether stdin, stdout, stderr and streams the library opens for itself count.
- **posixcheck meanwhile:** XSH.fopen.032 and XSH.fdopen.009 count the three standard streams, and report NOTINUSE when the value follows RLIMIT_NOFILE (the descriptor case is XSH.fopen.031).

### Q-38: a failed create

- **Where:** XSH `open()`, `fopen()`; XSH 2.3.
- **Question:** XSH 2.3 allows a failed call to have been partly performed unless stated otherwise, and neither open() nor fopen() says that a failed O_CREAT leaves no new file: not for EMFILE (a system may create the name before it finds no free descriptor), nor for the trailing-slash errors. Is a file left behind conforming, or is "no file created" intended?
- **posixcheck meanwhile:** XSH.fopen.027, .028 and .031 check only the errno. XSH.fopen.009 and .011 do check the file, on the strength of open()'s atomic O_EXCL rule.
- **After the Linux-ism audit:** Correction: open()'s RETURN VALUE does say "If -1 is returned, no files shall be created or modified". The question narrows to fopen(): its file is created as if by open(), which covers a failure of that open(), but fopen() can also fail after a successful open() (no stream left, out of memory). Must it then remove the file it created?

### Q-39: fopen(): a trailing slash with "wx"

- **Where:** XSH `fopen()`, ERRORS; XSH `open()`, ERRORS.
- **Question:** open()'s trailing-slash ENOTDIR is qualified by O_CREAT and O_EXCL not being specified (it is unclear whether that means neither flag or not both), while fopen()'s ENOTDIR has no such qualifier. For fopen("f/", "wx") with f a regular file, EEXIST and ENOTDIR then both apply. XSH 2.3 lets either be reported, but the two pages should be worded alike.
- **posixcheck meanwhile:** not tested.

### Q-40: fseek(): the offset after fflush() against XSH 2.5.1

- **Where:** XSH `fseek()`, DESCRIPTION (CX); XSH 2.5.1.
- **Question:** when fflush() is the most recent operation (other than ftell()), fseek() shall adjust the offset of the underlying open file description. That can only be seen through another handle, but XSH 2.5.1 asks the application to fflush() (or close) the stream after its last use (here the fseek()) before using another handle. Is the fseek() sentence an exception to 2.5.1, so that fflush(); fseek(); and then use of the descriptor is defined, or does it have no observable effect?
- **posixcheck meanwhile:** XSH.fseek.004 reads the descriptor's offset after fflush() and fseek() and is `disputed`. glibc leaves it at the end of the read-ahead for an input stream (docs/linux-deviations.md O.33).

### Q-41: fseek(): EBADF for a descriptor "not open for writing"

- **Where:** XSH `fseek()`, ERRORS.
- **Question:** EBADF covers a descriptor "not open for writing" or a buffer that needed flushing when the file is not open. Read literally, every fseek() on an unbuffered read-only stream (the preamble holds, since lseek() is invoked) shall fail with EBADF. The first clause presumably means "not open for writing while output needed to be flushed". No implementation behaves the literal way.
- **posixcheck meanwhile:** XSH.fseek.012 tests only the closed-descriptor case (and fsetpos() has the same wording).

### Q-42: XSH getrlimit(): RLIMIT_NOFILE below {_POSIX_OPEN_MAX}

- **Where:** XSH `getrlimit()`, DESCRIPTION and APPLICATION USAGE; XBD `<limits.h>`.
- **Question:** setrlimit() sets the requested value, yet the APPLICATION USAGE warns that a soft or hard RLIMIT_NOFILE below {_POSIX_OPEN_MAX} may cause unexpected behaviour, and {OPEN_MAX} has {_POSIX_OPEN_MAX} (20) as its minimum. Must a conforming system honour a soft limit of, say, 8 (getrlimit() returning it, EMFILE at it), and what may sysconf(_SC_OPEN_MAX) return then?
- **posixcheck meanwhile:** the EMFILE tests (XSH.open.040, XSH.dup.005, XSH.pipe.006, .010, XSH.fcntl.006, XSH.fopen.031) never go below 20, and give UNRESOLVED if the limit does not take.

### Q-43: XSH fcntl(): F_SETOWN with the ID of a process that is gone

- **Where:** XSH `fcntl()`, ERRORS.
- **Question:** ESRCH (shall fail) is for a process ID that names no process; the may-fail EINVAL is for a positive arg "not valid as a process ID". Is an ID that names no process also "not valid", so that XSH 2.3 allows either error?
- **posixcheck meanwhile:** XSH.fcntl.013 requires ESRCH and is `disputed`.

### Q-44: XSH rmdir(): ENOENT for "a nonexistent directory"

- **Where:** XSH `rmdir()`, ERRORS.
- **Question:** ENOENT covers a path that "names a nonexistent directory". An existing regular file, named as "f" or "f/", names no directory, so ENOENT overlaps ENOTDIR and XSH 2.3 allows either. Should ENOENT read "names a nonexistent file", as on other pages?
- **posixcheck meanwhile:** XSH.rmdir.008 and .015 require ENOTDIR and are `disputed`.

### Q-45: XSH bind(): the file an AF_UNIX address creates

- **Where:** XSH `bind()`; XBD `<sys/stat.h>`.
- **Question:** bind() never says that binding an AF_UNIX address creates a file of type socket (S_ISSOCK through stat()); that is only implied by EACCES (the name needs writing in a directory) and EROFS (the name would reside on a read-only file system). Should bind() say so?
- **posixcheck meanwhile:** XSH.fstatat.006 requires S_ISSOCK for the bound name, with a note.

### Q-46: XSH utimes(): an out-of-range tv_usec

- **Where:** XSH `futimens()` (utimes()), ERRORS; SI-59.
- **Question:** utimes()'s errors are written in tv_nsec terms; nothing says a tv_usec outside [0, 999999] gives EINVAL. utimes() is equivalent to utimensat() with a timeval, so EINVAL follows if the value is carried over unchanged, but normalising the timeval is also a defensible reading. Which is intended?
- **posixcheck meanwhile:** XSH.futimens.023 requires EINVAL and is `disputed`.

### Q-47: XSH ungetc() against fflush(): when is pushback discarded?

- **Where:** XSH `ungetc()`, DESCRIPTION; XSH `fflush()`, DESCRIPTION.
- **Question:** ungetc() says a successful fflush() shall discard any pushed-back bytes, with no condition; fflush() discards them only for a stream open for reading on a seekable file that is not already at end of file. For a stream at end of file, or on a pipe, one page requires the discard and the other does not. Which governs?
- **posixcheck meanwhile:** XSH.ungetc.010 reads a 1 MiB file, so both pages require the discard.

### Q-48: XSH chmod(): what "S_ISUID and S_ISGID bits in mode ... ignored" means

- **Where:** XSH `chmod()`, DESCRIPTION.
- **Question:** implementation-defined restrictions may cause the S_ISUID and S_ISGID bits in mode to be ignored. Does that mean the file's existing bit is kept (for example a directory's inherited S_ISGID), or that the bit in mode is treated as zero? The answer decides whether a check of the whole mode is valid.
- **posixcheck meanwhile:** the directory mode checks ignore S_ISGID; XSH.chmod.020 accepts EPERM for a refused S_ISUID, as for an ignored one.

### Q-49: XSH 2.5.1: process creation the application does not see

- **Where:** XSH 2.5.1; XSH `popen()`, `system()`, `posix_spawn()`.
- **Question:** the rules ask the application to prepare for a fork() as for a change of handle, but popen(), system() and posix_spawn() create processes without the application calling fork(). Does the guarantee (no data lost or duplicated) still hold for data pending in the parent's streams? Separately, the parenthesis saying a process that only calls _exit() or an exec function never accesses the handle sits uneasily with the sentence that the state becomes undefined if the active handle "ceases to be accessible" during a fork() or _exit(); the parenthesis should be stated as an exception.
- **posixcheck meanwhile:** XSH.2.5.1-stream-handles.009 and .010 rely on the parenthesis; popen(), system() and posix_spawn() are not tested.

### Q-50: XSH mmap() against XSH 2.8.3.3: references past a shrunk file

- **Where:** XSH `mmap()`, DESCRIPTION; XSH 2.8.3.3.
- **Question:** mmap() leaves references to portions of a mapping that correspond to removed portions of the file unspecified, while 2.8.3.3 requires SIGBUS for references beyond the current length of the object. Which governs a file shrunk after mmap()?
- **posixcheck meanwhile:** XSH.2.4.3-signal-actions.021 maps two pages of a file that has one page from the start, which both texts cover.

### Q-51: XSH signal(): what "prevent ... from occurring" is

- **Where:** XSH `signal()`, DESCRIPTION; XSH `sigaction()`.
- **Question:** when a handler installed with signal() runs, the system either resets the action to SIG_DFL or prevents an implementation-defined set of signals from occurring. Must the prevention be the signal mask (visible to sigprocmask() in the handler)? And is the implicit reset "established by signal()", so that sigaction()'s oact is then unspecified?
- **posixcheck meanwhile:** XSH.signal.004 judges by behaviour: a reset action ends the child; a held-off signal does not re-enter the handler.

### Q-52: XSH sigwaitinfo(): siginfo for a signal without SA_SIGINFO

- **Where:** XSH `sigwaitinfo()`, `sigtimedwait()`; XSH 2.4.2, 2.4.3.
- **Question:** sigwaitinfo() stores the cause in si_code, but the rule that SI_USER signals carry si_pid and si_uid is stated for handlers entered with SA_SIGINFO, and 2.4.2 leaves queueing unspecified without SA_SIGINFO; some systems report SI_NOINFO for such signals. Must si_code, si_pid and si_uid be valid when the signal accepted has no SA_SIGINFO action?
- **posixcheck meanwhile:** XSH.sigtimedwait.004 and .008 give their signals an SA_SIGINFO handler (blocked, so it never runs).
- **After the variant retrofit:** sigqueue()'s RETURN VALUE says a successful call means the signal "shall have been queued", while its DESCRIPTION and XSH 2.4.2 leave queueing unspecified without SA_SIGINFO. XSH.sigqueue.009 requires only that the signal be sent at least once.

### Q-53: XSH _exit(): status available before SIGCHLD; E2BIG against ENOMEM

- **Where:** XSH `_exit()`, Consequences of Process Termination; XSH `wait()`; XSH exec, ERRORS.
- **Question:** once a parent can obtain a child's status (waitid() with WNOWAIT returns), must the SIGCHLD for that termination already have been generated? The consequences list does not order the two, and wait() covers only the other direction. Separately: when the arguments exceed {ARG_MAX} and the new image would also exceed a memory limit such as the stack limit, XSH 2.3 lets the may-fail ENOMEM be reported instead of the shall-fail E2BIG; should E2BIG take precedence (as with SI-4 and Q-8)?
- **posixcheck meanwhile:** XSH._Exit.014 waits up to 1 s for SIGCHLD; XSH.wait.002 establishes availability with waitid(WNOWAIT); XSH.exec.043 accepts ENOMEM when the arguments exceed the stack limit.

### Q-54: XBD <unistd.h>: intptr_t without XSI shading

- **Where:** XBD `<unistd.h>`; XBD `<stdint.h>`.
- **Question:** <unistd.h> (unshaded) shall define intptr_t "as described in <stdint.h>", where intptr_t is required only on XSI-conformant systems. Is intptr_t mandatory for every system, or should the <unistd.h> sentence be XSI-shaded?
- **posixcheck meanwhile:** Tier 0 checks <unistd.h>'s intptr_t only on XSI systems.

### Q-55: XBD <signal.h>: are the flag and code constants distinct?

- **Where:** XBD `<signal.h>`; XSH `sigaction()`.
- **Question:** the SA_* constants are only "symbolic constants"; sigaction() speaks of flags set or cleared in sa_flags, which implies non-zero values with no bits in common but does not say so. Likewise nothing states that SIG_BLOCK, SIG_UNBLOCK and SIG_SETMASK, the SIGEV_* values, or the si_code values within one signal's group are distinct. Should the header say so, as <fcntl.h> does for its flags?
- **posixcheck meanwhile:** XBD.signal.h.016 and .017 (SA_* flags) are `disputed`; .004, .015 and .023 note that distinctness is inferred.

### Q-56: XSH aio_read(): a pipe or FIFO

- **Where:** XSH `aio_read()`, DESCRIPTION and ERRORS; XSH `lseek()`.
- **Question:** aio_read() reads at aio_offset "as if" lseek() had been called with that offset, and a pipe or FIFO cannot be positioned (lseek() gives ESPIPE). May aio_read() on a pipe fail (EINVAL for an invalid offset, or ESPIPE, which aio_read() does not list), or must it read the pipe ignoring aio_offset?
- **posixcheck meanwhile:** XSH.fork.023, which needs a read that stays pending, is UNRESOLVED when aio_read() on a pipe fails with EINVAL or ESPIPE; any other outcome is judged.

### Q-57: XSH signal(): the return value after a reset on delivery

- **Where:** XSH `signal()`, DESCRIPTION and RETURN VALUE.
- **Question:** on an implementation that resets the action to SIG_DFL when the handler is entered ("the equivalent of signal(sig, SIG_DFL)"), what does the next signal() call return: SIG_DFL, or the handler given to the most recent explicit call? RETURN VALUE speaks of "the most recent call to signal()"; whether the reset counts as a call is not said.
- **posixcheck meanwhile:** no assertion depends on it.

### Q-58: XSH scandir(): is sel called once per entry?

- **Where:** XSH `alphasort()` (scandir()), DESCRIPTION.
- **Question:** scandir() calls sel "on each directory entry"; nothing says only once. An implementation that reads the directory twice (counting, then filling) would call it twice per entry with the same result. Is a single call required? It matters to a sel with side effects.
- **posixcheck meanwhile:** XSH.alphasort.002 (required) checks the sorted result and that every entry is offered; the single call is XSH.alphasort.010, `disputed`.

### Q-59: XSH _exit(): when the consequences of termination take effect

- **Where:** XSH `_exit()`, Consequences of Process Termination; XSH `wait()`.
- **Question:** the consequences (shared memory detached, semaphore adjustments applied, message queue notification registrations removed) are listed with no order and no time. Must they all have happened by the time the parent can obtain the status? Read strictly, any of them may lag, and no test can wait for ever. Related to Q-53 (SIGCHLD).
- **posixcheck meanwhile:** XSH._Exit.016, .017 and .018 poll for up to 10 s after the status is obtained (a practical limit). Only memory mappings are said to be unmapped before the process is destroyed, which happens when the status is obtained; whether an XSI shared-memory attachment is such a mapping is not said. XSH._Exit.021 (`disputed`) checks shm_nattch at once.

### Q-60: XSH fcntl(): "Locks may start and extend beyond the current end of a file"

- **Where:** XSH `fcntl()`, DESCRIPTION; XBD 1.5 (terminology).
- **Question:** under XBD 1.5, "may" describes an optional feature of the implementation, so the sentence could mean an implementation may support such locks, not that an application may request them. The l_len 0 rule (a lock to the largest possible offset) only works if they are supported. Should the sentence say "shall be allowed to"?
- **posixcheck meanwhile:** XSH.fcntl.027 reads it as a requirement on the implementation.

### Q-61: XSH write() and read(): a stream socket whose peer has gone

- **Where:** XSH `write()`, `read()`, `send()`, `recv()`, ERRORS; XSH 2.3; SI-1.
- **Question:** write() gives EPIPE for a socket that is "no longer connected" and then requires SIGPIPE for a stream socket; ECONNRESET ("forcibly closed by a peer") and ENOTCONN ("not connected") are separate entries. When the peer has reset the connection, the socket is both no longer connected and forcibly closed, so XSH 2.3 seems to allow ECONNRESET, and then no SIGPIPE is required; read as such, the SIGPIPE requirement can always be avoided. Likewise read() after a reset: ECONNRESET, or ENOTCONN because the socket is no longer connected? Which is intended?
- **posixcheck meanwhile:** XSH.write.026 requires EPIPE with SIGPIPE pending and XSH.read.022 requires ECONNRESET; both are `disputed`. The other readings are kept as undeclared variants (write.peer_gone_reset, read.reset_enotconn).

### Q-62: XSH msync(): flags 0, MS_INVALIDATE alone, MS_ASYNC|MS_SYNC

- **Where:** XSH `msync()`, DESCRIPTION and ERRORS.
- **Question:** the DESCRIPTION says that exactly one of MS_ASYNC and MS_SYNC is to be specified, and ERRORS requires [EINVAL] when flags is invalid. Does the first sentence define which values are invalid (so EINVAL is required for 0, for MS_INVALIDATE alone and for both together), or is it a requirement on the application whose breach is undefined?
- **posixcheck meanwhile:** XSH.msync.002, .007 and .008 expect EINVAL and are `disputed` (a FAIL is logged as INSPECT); the lenient reading is planted as READING msync.flags_lenient.

### Q-63: XSH strndup() and strnlen(): may bytes after the null byte be read?

- **Where:** XSH `strdup()` (strndup()), `strnlen()`, DESCRIPTION.
- **Question:** both functions examine at most size bytes and stop at the null byte, but nothing says the argument array must have size bytes, or that nothing after the null byte is read. May an implementation read ahead up to size bytes when the string ends sooner (as a word-at-a-time scan would), faulting on an unmapped page?
- **posixcheck meanwhile:** XSH.strdup.007 and XSH.strnlen.005 require that nothing past the null byte is touched and are `disputed`; the other reading is planted as a READING.

### Q-64: XSH open_memstream(): byte-oriented from the start, or unoriented until used?

- **Where:** XSH `open_memstream()`, DESCRIPTION; XSH 2.5.
- **Question:** open_memstream() makes the stream byte-oriented; XSH 2.5 says the orientation of a memory stream is determined as for a file stream, which starts with none until the first operation. What does fwide(f, 0) return right after the call: negative, or 0?
- **posixcheck meanwhile:** XSH.open_memstream.012 (and .014 for the wide function) take the page's wording and are `disputed`; a FAIL is logged as INSPECT. Both readings agree that a wide-oriented stream is wrong.

### Q-65: XSH fprintf(): infinity and NaN with e, g and a

- **Where:** XSH `fprintf()`, DESCRIPTION (f, F, e, E, g, G, a, A).
- **Question:** f and F leave the style of an infinity or NaN (and the n-char-sequence) implementation-defined; e, g and a say only "in the style of an f or F conversion". Is that the style f itself uses (so f and e agree), or any style f or F could use (so e may differ from f)? F is said to produce INF, INFINITY or NAN "respectively".
- **posixcheck meanwhile:** XSH.fprintf.058 requires e, g and a to write exactly as f does and is `disputed`.

### Q-66: XSH getcwd(): is ENAMETOOLONG excluded for a long absolute name?

- **Where:** XSH `getcwd()`, ERRORS and RATIONALE; XSH 2.3.
- **Question:** only the RATIONALE says the old ENAMETOOLONG for a name longer than {PATH_MAX} no longer applies when buf is large enough; the normative ERRORS lists no such error, but XSH 2.3 lets implementations add errors that no page forbids.
- **posixcheck meanwhile:** XSH.getcwd.008 requires the full name and is `disputed`.

### Q-67: XSH chdir(): ENOENT for a regular file

- **Where:** XSH `chdir()`, ERRORS; XSH 2.3; Q-8.
- **Question:** chdir()'s [ENOENT] is "does not name an existing directory", which a regular file also meets, while [ENOTDIR] says a component is not a directory. Read so, either error is allowed for chdir("file"); this is the Q-8 overlap with a different wording.
- **posixcheck meanwhile:** XSH.chdir.004 requires ENOTDIR and is `disputed`; the other reading is planted as a READING.

### Q-68: XSH mknod(): unspecified behaviour against the required EPERM

- **Where:** XSH `mknod()`, DESCRIPTION and ERRORS.
- **Question:** the DESCRIPTION makes the behaviour unspecified whenever mode is not S_IFIFO or dev is not 0, while ERRORS requires [EPERM] for a caller without appropriate privileges for any type but FIFO-special. Which governs an unprivileged mknod() of a regular file, directory or special file?
- **posixcheck meanwhile:** XSH.mknod.018 and .019 require EPERM and are `disputed`; the types are split only so each result is recorded separately.

### Q-69: XSH setreuid() and setregid(): is the saved ID otherwise left alone?

- **Where:** XSH `setreuid()` and `setregid()`, DESCRIPTION; setregid() APPLICATION USAGE.
- **Question:** the pages say when the saved set-ID becomes the new effective ID, not (in normative text) that it is otherwise unchanged. The setregid() APPLICATION USAGE says so for groups.
- **posixcheck meanwhile:** XSH.setreuid.004 and XSH.setregid.004 require it unchanged and are `disputed`.

### Q-70: XSH setregid(): which saved set-group-ID may be assigned to the real group ID?

- **Where:** XSH `setregid()`, DESCRIPTION.
- **Question:** an unprivileged process may set the real group ID to "the saved set-group-ID from one of the exec family of functions". Does an ID that a later setgid() or setregid() stored in the saved ID count?
- **posixcheck meanwhile:** XSH.setregid.006 obtains the saved ID from exec and avoids the question.

### Q-71: XSH shm_unlink(): EPERM or EACCES when the caller may not remove the name

- **Where:** XSH `shm_unlink()`, ERRORS; XSH 2.3; unlink() ERRORS.
- **Question:** the page never says what permission removing the name needs; where it is refused because only the owner may remove it (as with a sticky directory), unlink() allows EPERM or EACCES, but shm_unlink() lists only EACCES, and XSH 2.3 lets an implementation add errors for conditions the page does not describe.
- **posixcheck meanwhile:** XSH.shm_unlink.005 requires EACCES and is `disputed`; a system that lets the unlink happen is NOTINUSE.

### Q-72: XSH 2.9.5.1: asynchronous cancellation "may be acted upon at any time"

- **Where:** XSH 2.9.5.1; XSH `pthread_setcancelstate()`, RATIONALE.
- **Question:** with the asynchronous type a cancellation request "may be acted upon at any time". Read as a permission, an implementation may still wait for a cancellation point and never cancel a thread that computes without one. Is that intended?
- **posixcheck meanwhile:** XSH.pthread_setcancelstate.006 requires cancellation within 10 s and is `disputed`.

### Q-73: XBD <limits.h> and pthread_create()/pthread_key_create(): how {PTHREAD_THREADS_MAX} and {PTHREAD_KEYS_MAX} count

- **Where:** XBD `<limits.h>`; XSH `pthread_create()` and `pthread_key_create()`, ERRORS.
- **Question:** <limits.h> speaks of threads "created per process" and keys "created by a process"; the ERRORS say "total number of threads in a process" and "keys per process". Does the initial thread count, do terminated threads count, do deleted keys count?
- **posixcheck meanwhile:** XSH.pthread_create.012 accepts failure one creation early; XSH.pthread_key_create.005 counts live keys only.

### Q-74: XSH sleep(): rounding of the unslept time

- **Where:** XSH `sleep()`, RETURN VALUE.
- **Question:** an interrupted sleep() returns the unslept amount in seconds; how a fraction of a second is rounded is not said (compare Q-13 for alarm()).
- **posixcheck meanwhile:** XSH.sleep.003 accepts the rounded-down and rounded-up values.

### Q-166: XBD <netinet/in.h>: the values of INADDR_ANY and INADDR_BROADCAST

- **Where:** XBD `<netinet/in.h>`, DESCRIPTION (the paragraphs introducing INADDR_ANY and INADDR_BROADCAST) and RATIONALE.
- **Question:** the page calls INADDR_ANY the IPv4 wildcard address and INADDR_BROADCAST the IPv4 broadcast address and says their byte order is not specified because they do not depend on it, but it gives no value for either. The values 0.0.0.0 and 255.255.255.255 come from the IPv4 address definitions, which are outside this standard. Should the page state them?
- **posixcheck meanwhile:** XBD.netinet_in.h.010 expects 0 and all 32 bits set, unchanged by htonl(), and is `disputed`; a failure is reported for inspection.

### Q-167: XBD <netdb.h>: what IPPORT_RESERVED is

- **Where:** XBD `<netdb.h>`, DESCRIPTION (the IPPORT_RESERVED paragraph).
- **Question:** the value is described as that of the highest reserved Internet port number, but the standard never says which ports are reserved. Common practice names the first unreserved port rather than the highest reserved one, which differ by one. Is the description meant literally, and which ports are reserved?
- **posixcheck meanwhile:** XBD.netdb.h.004 checks only that the value is a possible port number (0 to 65535).

### Q-169: XBD headers: constants that must be told apart are not said to differ

- **Where:** XBD `<langinfo.h>` (the nl_item constants), `<regex.h>` (REG_ error codes and flags), `<netdb.h>` (EAI_ error values), `<netinet/in.h>` (IPPROTO_ levels beside SOL_SOCKET), `<locale.h>` (the LC_ mask macros), `<nl_types.h>` (NL_CAT_LOCALE beside 0), `<dlfcn.h>` (RTLD_ modes), `<wchar.h>` (WEOF beside the wide characters).
- **Question:** each page lists constants that a function takes as alternatives in one argument, ORs into a flags argument, or returns as codes that a second function turns into messages. As in Q-55, Q-165 and Q-168, no sentence says that two different names have different values, or that flags share no bit; the function pages imply it, since the alternatives could not be told apart otherwise. Should the header pages say so?
- **posixcheck meanwhile:** the tests expect distinct values (no common bit for flags); for the nl_langinfo items the assertions XBD.langinfo.h.002 and .004 are `disputed`.

## Tracking

Entries keep their ID for good; a resolved or withdrawn entry stays in the
list with its outcome. An assertion whose verdict depends on an entry names
its ID in its `note`. New entries are added when a test or a review finds
the text conflicting, wrong or unclear, after checking them against the
HTML edition.
