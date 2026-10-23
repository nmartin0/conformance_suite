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
| SI-10 | XBD `<signal.h>`: SA_NOCLDWAIT moved to Base, yet XSI-shaded | Defect (editorial) | Low | Not tested yet |
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
- **posixcheck meanwhile:** SA_NOCLDWAIT is not tested yet.
- **Suggested change:** remove the shading, or say that only the symbol is Base.

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
  open() the suite avoids the overlap with O_CREAT (XSH.open.014, 017).

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

## Tracking

Entries keep their ID for good; a resolved or withdrawn entry stays in the
list with its outcome. An assertion whose verdict depends on an entry names
its ID in its `note`. New entries are added when a test or a review finds
the text conflicting, wrong or unclear, after checking them against the
HTML edition.
