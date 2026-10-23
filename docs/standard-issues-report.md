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
| Q-1 | XSH `write()`/`send()`: SIGPIPE after shutdown(SHUT_WR) | Question | Low | Does not require SIGPIPE (XSH.write.017) |
| Q-2 | XSH `lseek()`: offsets beyond the file system's largest file | Question | Medium | Asserts only the off_t overflow case (XSH.lseek.011) |
| Q-3 | XSH `fork()`/`sem_init()`: unnamed semaphore, pshared 0, in the child | Question | Low | Not tested |
| Q-4 | XBD orphaned process group: is a zombie still a member? | Question | Low | Avoids the case (XSH._Exit.010) |
| Q-5 | XSH `wait()`: which SIGCHLD paragraph applies | Question | Low | One child, where both agree (XSH.wait.019) |
| Q-6 | XSH `wait()`: when another child's status is "available" | Question | Low | Not tested |
| Q-7 | XSH `exit()`: streams with no file descriptor | Question | Low | Not tested |
| Q-8 | XSH 2.3: ENAMETOOLONG/ENOTDIR always meet ENOENT for lookups | Question | Medium | Accepts ENOENT too (XSH.exec.038, 041) |
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
| Q-29 | XSH `fflush()`/`fclose()`/`exit()`: what "not already at EOF" means | Question | Low | Tests use a 1 MiB file (both readings met) |
| Q-30 | XSH `fflush()`: where does reading resume after pushback is discarded? | Question | Low | Only the discard is checked (XSH.fflush.005) |
| Q-31 | XSH `_exit()`: what happens to the other threads waiting for the last child? | Question | Low | Disputed (XSH._Exit.020) |
| Q-32 | XSH `_exit()`: asynchronous I/O at exit cannot be observed | Question | Low | Disputed (XSH._Exit.019) |
| Q-33 | XSH `setvbuf()`: each mode is required, but support is implementation-defined | Question | Medium | Disputed (XSH.setvbuf.003, .004, .006) |
| Q-34 | XSH `ungetc()`: the limit on repeated pushback omits fflush() | Question | Low | Not tested (permission) |

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

### Q-28: fflush(NULL) and the input-stream offset rule

- **Where:** XSH `fflush()`, DESCRIPTION.
- **Question:** the page gives two behaviours: writing an output stream's unwritten data (from ISO C), and, as a POSIX addition, setting a seekable input stream's file offset to the stream's position. For a null pointer, fflush() performs "this flushing action on all streams for which the behavior is defined above". Does that include the input-stream rule, so that fflush(NULL) sets every seekable input stream's offset, or only the flushing of output, as in ISO C? Neither glibc nor musl touches input streams for fflush(NULL), though both do for fflush(stream).
- **posixcheck meanwhile:** XSH.fflush.011 is `disputed`: an untouched offset is logged as INSPECT.

### Q-29: "If the file is not already at EOF": the stream's indicator or the file offset?

- **Where:** XSH `fflush()`, `fclose()` and `exit()` (the input-stream offset rule); XSH 2.5.1.
- **Question:** the offset rule applies "if the file is not already at EOF". Does that mean the stream's end-of-file indicator (XSH 2.5.1 speaks of feof() being true), or the underlying file offset being at the end of the file? After one read of a small file, read-ahead has usually taken the whole file: the indicator is clear but the descriptor is at the end, so the two readings give different answers.
- **posixcheck meanwhile:** the tests read a 1 MiB file, so both readings require the rule; XSH.fflush.015 checks that a stream whose indicator is set is left alone.

### Q-30: fflush(): the stream's position after pushed-back bytes are discarded

- **Where:** XSH `fflush()`, DESCRIPTION; XSH `ungetc()`.
- **Question:** fflush() of an input stream sets the offset to the stream's position and discards pushed-back bytes "without further changing the file offset". With "01" read and 'x' pushed back (position 1), must the next read return '1' (the byte at the new offset), or may it resume where the stream was before the pushback ('2', as glibc does)? ungetc() keeps ISO C's rule for reading the pushed-back bytes but does not say what discarding them does to the position.
- **posixcheck meanwhile:** XSH.fflush.005 checks the offset and that the discarded byte is not read, not which byte comes next.

### Q-31: _exit(): the other waiting threads once the last child is reaped

- **Where:** XSH `_exit()`, Consequences of Process Termination; XSH 2.12.
- **Question:** when several threads of the parent are blocked in wait() or waitpid() for a child that exits, one of them obtains the status. Only the XSI case (SIGCHLD ignored or SA_NOCLDWAIT) then says a waiter with no remaining children fails with ECHILD. In the ordinary case, must the other threads fail with ECHILD once no children remain, or may they stay blocked? wait()'s ERRORS suggest ECHILD, but the text does not say so.
- **posixcheck meanwhile:** XSH._Exit.015 requires that exactly one thread gets the status; XSH._Exit.020 (the other gets ECHILD) is `disputed`.

### Q-32: _exit(): completing or canceling outstanding asynchronous I/O

- **Where:** XSH `_exit()`, Consequences of Process Termination.
- **Question:** operations not canceled at exit "complete as if _exit() had not yet occurred" and their signal notifications are suppressed, while which operations are canceled is implementation-defined (and "cancelable" is defined only through aio_cancel()). After the process is gone, a dropped write cannot be told from a canceled one, and the suppression of a signal to a process that no longer exists cannot be observed. Is a write that never reaches the file conforming if the implementation documents that it cancels such writes? Should the text say which operations an implementation may cancel at exit?
- **posixcheck meanwhile:** XSH._Exit.019 is `disputed`: a short file is logged as INSPECT.

### Q-33: setvbuf(): required buffering modes against implementation-defined support

- **Where:** XSH `setvbuf()`, DESCRIPTION and RETURN VALUE; XSH 2.5.
- **Question:** setvbuf() says each of _IOFBF, _IOLBF and _IONBF shall cause that kind of buffering, and that a request that cannot be honoured makes it return non-zero. XSH 2.5 (following ISO C) makes support for the buffering characteristics implementation-defined. Together these appear to let an implementation refuse any mode, or accept it and buffer some other way. Must an accepted _IONBF stream send each byte at once and an accepted _IOLBF stream send each line at its newline? Is setvbuf() still permitted after an unsuccessful call, given that the text mentions this only as an allowance for applications?
- **posixcheck meanwhile:** XSH.setvbuf.001 checks only that data arrives intact under each accepted mode and notes refusals. XSH.setvbuf.003, .004 and .006 check the observable effect of an accepted mode and are `disputed`: a FAIL is logged as INSPECT, and a refusal is UNRESOLVED.

### Q-34: ungetc(): the limit on repeated pushback and fflush()

- **Where:** XSH `ungetc()`, DESCRIPTION.
- **Question:** a successful fflush() discards pushed-back bytes, like the file-positioning functions (a CX addition, Austin Group defect 87). But the sentence allowing ungetc() to fail after too many calls names only an intervening read or file-positioning operation, not fflush(). After fflush() has discarded the pushback, may ungetc() still fail because of the calls made before it? The two sentences should probably name the same operations.
- **posixcheck meanwhile:** the permission is not tested. XSH.ungetc.010 checks that fflush() discards the byte.

## Tracking

Entries keep their ID for good; a resolved or withdrawn entry stays in the
list with its outcome. An assertion whose verdict depends on an entry names
its ID in its `note`. New entries are added when a test or a review finds
the text conflicting, wrong or unclear, after checking them against the
HTML edition.
