# Second-system triage, group xsh1

Source lists: `scratchpad/triage/nb/list-xsh1.tsv` (129 non-PASS lines of the second-system run). Standard: the POSIX.1-2024 pages under the personal copy; only short phrases quoted.

| ID | second system | Linux | class | reason |
| --- | --- | --- | --- | --- |
| XSH.getgrgid.006 | FAIL | PASS | SYSTEM | getgrgid_r(): ERANGE shall be returned for a buffer too small (DESCRIPTION/ERRORS); the system returns 0 with a null result for the 1-byte buffer. |
| XSH.getgrnam.002 | FAIL | FAIL | SYSTEM | getgrnam RETURN VALUE: errno shall not be changed when no entry is found; the system sets it to 0 (same on both systems). |
| XSH.getgrnam.005 | FAIL | PASS | SYSTEM | getgrnam_r ERRORS [ERANGE] (shall fail): a 1-byte buffer cannot hold the entry; the system returns 0. |
| XSH.getgrnam.006 | FAIL | PASS | SYSTEM | Same cause as .005: with a too-small buffer the system returns 0 and a null *result, so the doubling loop stops at once. |
| XSH.getnameinfo.003 | FAIL | PASS | SYSTEM | getnameinfo ERRORS [EAI_OVERFLOW]: an argument buffer too small is EAI_OVERFLOW; the system returns EAI_MEMORY, which is for allocation failure. |
| XSH.getnameinfo.004 | FAIL | PASS | SYSTEM | Same as .003 for the service buffer. |
| XSH.getnameinfo.008 | FAIL | FAIL | SYSTEM | getnameinfo ERRORS [EAI_NONAME]: both node and service null is an error; the system returns 0 (same on both systems). |
| XSH.getnameinfo.011 | INSPECT | UNINITIATED | STD-AMBIG | Already disputed (Q-137): whether a flag bit outside the listed ones may be accepted as an extension. |
| XSH.getopt.006 | FAIL | PASS | SYSTEM | getopt DESCRIPTION: an optional leading '+' has no effect, so "+:a:" starts with ':' for the missing-argument return; the system treats '+' as an ordinary option character. |
| XSH.getopt.007 | FAIL | PASS | SYSTEM | Same cause: with "+:a:" the diagnostic must be suppressed (leading colon after the optional '+'); the system prints one. |
| XSH.getopt.008 | FAIL | PASS | SYSTEM | getopt DESCRIPTION: a leading '+' is not an option character, so "-+" is an unknown option; the system accepts '+' as an option. |
| XSH.getpeername.004 | FAIL | PASS | SYSTEM | getpeername DESCRIPTION: address_len on output is the length of the address (the actual length, compared with the supplied one); the system stores the truncated length. |
| XSH.getpeername.008 | FAIL | FAIL | SYSTEM | getpeername ERRORS [EINVAL] (shall): a shut-down socket; the system returns the peer address (same on both systems). |
| XSH.getpwnam.002 | FAIL | FAIL | SYSTEM | getpwnam RETURN VALUE: errno shall not be changed when no entry is found; the system sets it to 0 (same on both systems). |
| XSH.getrlimit.014 | FAIL | PASS | SYSTEM | getrlimit RLIMIT_FSIZE: a truncate that would exceed the limit fails with EFBIG; the system lets ftruncate() to 1001 succeed under a 1000-byte limit. |
| XSH.getsockname.003 | FAIL | PASS | SYSTEM | getsockname DESCRIPTION: address_len on output is the length of the socket address; the system stores the truncated length. |
| XSH.getsockopt.005 | UNRESOLVED | PASS | TEST-BUG | The target was a socket bound but still open, so no refusal is promised (the attempt may be dropped without an answer); a port with no endpoint is refused. |
| XSH.getsockopt.006 | UNRESOLVED | PASS | TEST-BUG | Same set-up as .005. |
| XSH.getsubopt.002 | FAIL | PASS | TEST-BUG | The test measured strlen() of the string after the call, but the call may overwrite the equals sign (only the comma replacement is specified), moving the expected end. |
| XSH.glob.006 | FAIL | PASS | SYSTEM | XCU 2.14.3: when nothing matches, the pattern string shall be left unchanged (glob GLOB_NOCHECK); the system drops the backslash. |
| XSH.glob.011 | FAIL | PASS | SYSTEM | XBD 9.3.5 / XCU 2.14.1: a character class expression selects by class; the system finds no match for "[[:digit:]]x". |
| XSH.glob.016 | FAIL | PASS | SYSTEM | XCU 2.14.3: a bracket that would contain a slash is ordinary and "a[b/c]d" matches the pathname a[b/c]d; the system finds no match. |
| XSH.glob.032 | UNRESOLVED | PASS | ENV | The guest refused to extend a file to just over 2 GiB (EFBIG); the test already reports UNRESOLVED. |
| XSH.iconv.006 | UNRESOLVED | FAIL | STD-AMBIG | Already Q-128 (setup of .006 to .009): the page lets a suffix be unsupported (EINVAL) while saying any of the three can be appended; the suffixed open failed so UNRESOLVED is the right verdict. |
| XSH.iconv.007 | UNRESOLVED | FAIL | STD-AMBIG | Same as .006 (Q-128). |
| XSH.iconv.008 | UNRESOLVED | FAIL | STD-AMBIG | Same as .006 (Q-128). |
| XSH.iconv.009 | UNRESOLVED | PASS | STD-AMBIG | Same as .006 (Q-128). |
| XSH.iconv_open.002 | INSPECT | PASS | STD-AMBIG | Already disputed (Q-128): "UTF-8//IGNORE" fails with EINVAL; the ERRORS list allows EINVAL for an unsupported suffix. |
| XSH.inet_ntop.002 | FAIL | FAIL | SYSTEM | inet_pton DESCRIPTION: leading zeros shall be allowed in the dotted-decimal form; the system rejects "001.002.003.004" (same on both systems). |
| XSH.isgreater.008 | FAIL | PASS | SYSTEM | isgreater DESCRIPTION: islessgreater() shall not evaluate x and y twice; the double form evaluates its first argument three times. |
| XSH.kill.005 | UNRESOLVED | UNRESOLVED | ENV | Needs the profile entry kill_uid, which this run did not give; UNRESOLVED is right. |
| XSH.kill.012 | UNRESOLVED | UNRESOLVED | ENV | Same as .005. |
| XSH.kill.014 | FAIL | PASS | SYSTEM | kill DESCRIPTION: the sender's real or effective ID must match the receiver's real or saved set-user-ID; a 65534 sender signalled a receiver with real and saved ID 0. |
| XSH.link.021 | FAIL | UNRESOLVED | SYSTEM | XSH 4 (at-functions) / open O_SEARCH: no search check through an O_SEARCH descriptor; here O_SEARCH is defined but the check is made (fstatat etc. give EACCES in sibling tests). |
| XSH.lio_listio.002 | FAIL | PASS | SYSTEM | lio_listio DESCRIPTION: NULL elements and LIO_NOP entries are ignored, so LIO_WAIT over a list of those and one good write returns 0; the system returns -1 with EIO. |
| XSH.lio_listio.006 | FAIL | PASS | SYSTEM | lio_listio RETURN VALUE: LIO_WAIT returns 0 only when all the I/O completed successfully, otherwise -1 (EIO when an individual request failed); the system returns 0 with a failed read. |
| XSH.listen.003 | FAIL | FAIL | SYSTEM | listen ERRORS: an unbound stream socket gives success or EDESTADDRREQ; EINVAL is only for a connected (or shut-down) socket; the system gives EINVAL for AF_UNIX (same on both systems). |
| XSH.lseek.011 | INSPECT | INSPECT | SYSTEM | lseek ERRORS [EOVERFLOW] (Q-2 only lets EINVAL stand in for it): the sum of the offset 1 and the largest off_t is not representable, yet the call succeeded and returned the largest off_t, which no reading allows; the assertion stays disputed. |
| XSH.lseek.012 | UNRESOLVED | UNRESOLVED | SYSTEM | Blocked by XBD.unistd.h.002 (SEEK_HOLE/SEEK_DATA not visible as macros in <unistd.h>); the verdict follows that XBD test, UNRESOLVED is right while they are missing. |
| XSH.lseek.013 | UNRESOLVED | UNRESOLVED | SYSTEM | Same as lseek.012. |
| XSH.lseek.014 | UNRESOLVED | UNRESOLVED | SYSTEM | Same as lseek.012. |
| XSH.malloc.002 | FAIL | PASS | TEST-BUG | malloc DESCRIPTION asks for alignment for an object of fundamental alignment "in the space allocated"; a 2-byte block cannot hold a 16-byte-aligned type, so the test over-reached for small sizes. |
| XSH.mbrtoc16.001 | FAIL | FAIL | SYSTEM | mbrtoc16 ERRORS [EILSEQ]: in the POSIX locale all byte values are valid characters; byte 128 gives an encoding error (same on both systems). |
| XSH.mbrtoc16.002 | FAIL | PASS | TEST-BUG | One mbstate_t was passed to mbrtoc32() and then mbrtoc16(); the text does not promise a state altered by one function can be used by the other (mbsinit page), and the second call saw a pending surrogate (-3). |
| XSH.mbrtoc16.003 | FAIL | PASS | UNKNOWN | Killed by SIGSEGV with a null s; the same one-state-for-both-functions set-up was a suspect, now removed, but a crash on a null s may also be real; a re-run on the second system settles it. |
| XSH.mbrtoc16.004 | FAIL | PASS | TEST-BUG | Same shared-state set-up as .002. |
| XSH.mbrtoc16.009 | FAIL | PASS | TEST-BUG | Same shared-state set-up as .002. |
| XSH.mkdir.018 | FAIL | UNRESOLVED | SYSTEM | Same as link.021 (mkdirat through an O_SEARCH descriptor). |
| XSH.mkdtemp.005 | FAIL | PASS | SYSTEM | mkdtemp ERRORS [EINVAL] (shall fail): a template not ending in "XXXXXX" ("dXXXXX") is an error; the system accepted a five-X template (as for truncate/SI-68 the explicit error is asserted). |
| XSH.mkdtemp.012 | FAIL | PASS | SYSTEM | Same as .005 for mkstemp(). |
| XSH.mkdtemp.015 | FAIL | UNINITIATED | SYSTEM | Same as .005 for mkostemp() (EINVAL on a five-X template). |
| XSH.mkfifo.016 | FAIL | UNRESOLVED | SYSTEM | Same as link.021 (mkfifoat). |
| XSH.mktime.007 | FAIL | PASS | SYSTEM | mktime DESCRIPTION: for a skipped or repeated local time with tm_isdst negative the result is the DST or the standard reading (it is unspecified which); the representable time must not be refused with -1. |
| XSH.mlock.001 | UNRESOLVED | PASS | SYSTEM | Blocked by XSH.msync.004: the lock state is observed through msync(MS_INVALIDATE) giving EBUSY over a locked page, which the system does not do; UNRESOLVED follows msync.004. |
| XSH.mlock.002 | UNRESOLVED | PASS | SYSTEM | Same as mlock.001. |
| XSH.mlock.003 | FAIL | PASS | SYSTEM | mlock ERRORS [ENOMEM] (shall fail): a range with an unmapped page; the system gives EINVAL although the address is page aligned. |
| XSH.mlock.004 | UNRESOLVED | FAIL | SYSTEM | Same as mlock.001 (blocked by msync.004). |
| XSH.mlock.005 | UNRESOLVED | PASS | SYSTEM | Same as mlock.001. |
| XSH.mlockall.001 | UNRESOLVED | PASS | SYSTEM | Same as mlock.001. |
| XSH.mlockall.002 | UNRESOLVED | PASS | SYSTEM | Same as mlock.001. |
| XSH.mlockall.003 | UNRESOLVED | PASS | SYSTEM | Same as mlock.001. |
| XSH.mmap.007 | FAIL | PASS | SYSTEM | mmap ERRORS [EINVAL]: len zero shall fail, with or without MAP_ANONYMOUS; the anonymous zero-length call succeeded. |
| XSH.mmap.008 | FAIL | PASS | SYSTEM | mmap ERRORS [EINVAL]: neither MAP_PRIVATE nor MAP_SHARED shall fail; the call with flags 0 succeeded. |
| XSH.mmap.017 | FAIL | PASS | SYSTEM | mmap (partial last page not written) with ftruncate (extended area appears zero-filled): the extension read back the stored 'z' bytes. |
| XSH.mprotect.003 | FAIL | PASS | SYSTEM | mprotect: PROT_WRITE on a shared mapping of a descriptor open for reading and writing violates no access permission; the system gives EACCES. |
| XSH.mprotect.006 | FAIL | PASS | SYSTEM | mprotect ERRORS [ENOMEM] (shall fail): a range with an unmapped middle page; the system returned 0. |
| XSH.mprotect.008 | FAIL | PASS | SYSTEM | mmap/mprotect: write permission on the file is needed only for shared mappings; mprotect(PROT_WRITE) of a private mapping of a read-only descriptor gave EACCES. |
| XSH.mq_close.003 | FAIL | PASS | SYSTEM | mq_close DESCRIPTION: the registration made through the closed descriptor is removed; a new registration through another descriptor still gave EBUSY. |
| XSH.mq_getattr.003 | FAIL | PASS | SYSTEM | mq_getattr / mq_open: each mq_open() creates its own open message queue description, and mq_flags belongs to it; the second description shows O_NONBLOCK too. |
| XSH.mq_notify.010 | FAIL | PASS | SYSTEM | mq_notify with SIGEV_THREAD must run the function when a message arrives at the empty queue; the registration was accepted but the function never ran. |
| XSH.mq_open.009 | FAIL | PASS | SYSTEM | Same cause as mq_getattr.003 (the flag is shared by the descriptions of one queue). |
| XSH.mq_receive.008 | UNRESOLVED | PASS | SYSTEM | Set-up message of length 0 was refused by mq_send (see mq_send.003), so the receive could not be tried; UNRESOLVED follows a system fault. |
| XSH.mq_send.003 | FAIL | PASS | SYSTEM | mq_send DESCRIPTION: msg_len only has to be at most mq_msgsize, so a zero-length message must be accepted; the system refused it. |
| XSH.mq_setattr.005 | FAIL | PASS | SYSTEM | mq_setattr DESCRIPTION: the attributes are those of the open message queue description; another description of the queue changed too. |
| XSH.msync.003 | FAIL | PASS | SYSTEM | msync ERRORS [ENOMEM] (shall fail): a range with an unmapped page; the system gives EFAULT, which names no such case. |
| XSH.msync.004 | FAIL | PASS | SYSTEM | msync ERRORS [EBUSY] (shall fail): MS_INVALIDATE over a locked page; the system returned 0. |
| XSH.msync.008 | INSPECT | INSPECT | STD-AMBIG | Already disputed (Q-62): MS_INVALIDATE alone, a success is logged INSPECT. |
| XSH.munmap.005 | FAIL | PASS | SYSTEM | munmap ERRORS [EINVAL] (shall fail): len is 0; the system returned 0. |
| XSH.nan.002 | FAIL | PASS | SYSTEM | nan RETURN VALUE: nanl("1") shall return a quiet NaN equivalent to strtold("NAN(1)"); the long double result is not recognised as a NaN by isnan(). |
| XSH.nan.003 | FAIL | PASS | SYSTEM | nan DESCRIPTION: a tag that is not an n-char sequence ("a b") gives the same value as strtod("NAN"); the representations differ. |
| XSH.open.016 | FAIL | PASS | SYSTEM | open O_NOFOLLOW: a symbolic link shall fail with ELOOP; the system reports another error number. |
| XSH.open.039 | FAIL | PASS | SYSTEM | open ERRORS [EISDIR]: O_CREAT without O_DIRECTORY on a directory shall fail; the system returned a descriptor. |
| XSH.open.050 | UNRESOLVED | UNRESOLVED | SYSTEM | Blocked by XBD.fcntl.h.007: O_TTY_INIT is not defined in <fcntl.h>; verdict follows that XBD test. |
| XSH.open_memstream.011 | FAIL | FAIL | SYSTEM | open_memstream ERRORS [EMFILE] (shall fail): with {STREAM_MAX} streams open the call must fail; the system opens more (see Q-37; same on both systems). |
| XSH.opendir.012 | FAIL | UNRESOLVED | SYSTEM | opendir ERRORS [EBADF]: fdopendir() needs a descriptor open for reading; an O_SEARCH descriptor (search only) was accepted. |
| XSH.perror.005 | FAIL | FAIL | SYSTEM | perror DESCRIPTION / XSH 2.5: perror() must not change the orientation of stderr; fwide(stderr, 0) is -1 afterwards (same on both systems). |
| XSH.poll.011 | FAIL | UNINITIATED | SYSTEM | poll POLLHUP: this event and POLLOUT are mutually exclusive; the read end of a pipe whose writer closed reported both. |
| XSH.poll.025 | FAIL | UNINITIATED | SYSTEM | poll ERRORS [EINVAL] (shall fail): nfds greater than {OPEN_MAX} (the sysconf value); the call with {OPEN_MAX}+1 entries returned 0. |
| XSH.poll.027 | FAIL | UNINITIATED | SYSTEM | poll POLLERR: an error that write() would report from the state of the object alone (EPIPE) is POLLERR; the write end of a pipe without reader reported POLLOUT/POLLHUP instead. |
| XSH.poll.028 | FAIL | UNINITIATED | SYSTEM | Same as .027 with no requested events (revents 0x10, POLLHUP only). |
| XSH.popen.011 | INSPECT | PASS | STD-AMBIG | Already disputed (Q-64): whether a fresh popen() stream is byte-oriented at once or undetermined (fwide returns 0). |
| XSH.popen.014 | FAIL | FAIL | SYSTEM | popen ERRORS [EMFILE] (shall fail): with {STREAM_MAX} streams open the call must fail; the system opens more (see Q-37; same on both systems). |
| XSH.posix_spawn.009 | FAIL | UNINITIATED | SYSTEM | posix_spawn DESCRIPTION: a relative path is interpreted in the child's working directory after all file actions; the caller's prog ran instead of w/prog. |
| XSH.posix_spawn.013 | FAIL | UNINITIATED | SYSTEM | posix_spawnp DESCRIPTION: the PATH search uses the child's working directory after all file actions; the caller's ./rel/prog ran. |
| XSH.posix_spawn.027 | FAIL | PASS | SYSTEM | posix_spawn_file_actions_adddup2: with equal descriptors the action ensures FD_CLOEXEC is clear in the new image; the descriptor was closed there. |
| XSH.posix_spawn.043 | UNRESOLVED | UNRESOLVED | SYSTEM | POSIX_SPAWN_SETSID is required in <spawn.h> by the standard and is not defined under the driver's feature macro; UNRESOLVED is right. |
| XSH.posix_spawn.052 | FAIL | PASS | SYSTEM | posix_spawn: SETSCHEDPARAM alone keeps the caller's policy and uses the attribute's parameters; the image had priority 0 instead of one above the minimum. |
| XSH.posix_spawn.058 | FAIL | PASS | SYSTEM | exec ERRORS [ENOENT] (via posix_spawn): an empty path names no file; the system returned EACCES. |
| XSH.posix_spawn.069 | FAIL | PASS | SYSTEM | posix_spawn RETURN VALUE: when an error number is returned no child process is created; EPERM was returned but a child was left to be waited for. |
| XSH.posix_spawn_file_actions_addclose.013 | FAIL | PASS | SYSTEM | posix_spawn_file_actions_addopen ERRORS [EBADF] (shall fail): a descriptor number equal to {OPEN_MAX} is refused; the system returned 0 (tested as written, see Q-90). |
| XSH.posix_spawn_file_actions_adddup2.004 | FAIL | PASS | SYSTEM | adddup2 DESCRIPTION: with fildes equal to newfildes the child gets the descriptor with FD_CLOEXEC clear; it was closed in the new image (same cause as posix_spawn.027). |
| XSH.posix_spawn_file_actions_adddup2.007 | FAIL | PASS | SYSTEM | adddup2 ERRORS [EBADF] (shall fail): a descriptor equal to {OPEN_MAX}; the system returned 0 (tested as written, see Q-90). |
| XSH.pow.012 | FAIL | PASS | SYSTEM | pow RETURN VALUE: an overflow returns +-HUGE_VAL with the sign of the true result; powl(-2, 16385) returned +inf. |
| XSH.pread.010 | UNRESOLVED | PASS | ENV | The guest refused to extend a file to just past 4 GiB (EFBIG); the test already reports UNRESOLVED. |
| XSH.psiginfo.002 | UNRESOLVED | PASS | SYSTEM | Blocked by psiginfo.001, whose build failed: psignal() is in <signal.h> in the standard but is not declared under _POSIX_C_SOURCE=202405L; the verdict follows that system header gap. |
| XSH.psiginfo.003 | UNRESOLVED | INSPECT | SYSTEM | Same as psiginfo.002. |
| XSH.psiginfo.004 | UNRESOLVED | FAIL | SYSTEM | Same as psiginfo.002. |
| XSH.psiginfo.005 | UNRESOLVED | PASS | SYSTEM | Same as psiginfo.002. |
| XSH.psiginfo.006 | UNRESOLVED | PASS | SYSTEM | Same as psiginfo.002. |
| XSH.psiginfo.007 | FAIL | FAIL | SYSTEM | psiginfo DESCRIPTION: on a write error the error indicator of stderr is set and errno is set (descriptor 2 closed); the indicator stays clear (same on both systems). |
| XSH.pthread_attr_getstack.003 | FAIL | PASS | SYSTEM | pthread_attr_getstack ERRORS [EINVAL] (shall fail): a stacksize below {PTHREAD_STACK_MIN}; the call returned 0. |
| XSH.pthread_condattr_getclock.001 | FAIL | PASS | SYSTEM | pthread_condattr_getclock: the default clock of a new attributes object is CLOCK_REALTIME and the call must succeed; it returned EINVAL. |
| XSH.pthread_condattr_init.002 | FAIL | PASS | SYSTEM | pthread_condattr_init must set the clock attribute to its default; getclock afterwards returned EINVAL (same cause as getclock.001). |
| XSH.pthread_create.012 | FAIL | NOTINUSE | SYSTEM | pthread_create ERRORS [EAGAIN]: creating more threads than the {PTHREAD_THREADS_MAX} that sysconf reports must fail (Q-73 only concerns how the initial thread counts); 1045 succeeded. |
| XSH.pthread_exit.005 | FAIL | PASS | SYSTEM | pthread_exit DESCRIPTION: when the last thread terminates the process exits as if exit(0) had been called, so atexit handlers run; the handler did not run. |
| XSH.pthread_exit.006 | FAIL | PASS | SYSTEM | Same as .005 in a child made by fork(). |
| XSH.pthread_key_create.005 | FAIL | PASS | SYSTEM | pthread_key_create ERRORS [EAGAIN]: more keys than the {PTHREAD_KEYS_MAX} that sysconf reports (128) must fail; 129 were created (Q-73). |
| XSH.pthread_kill.004 | FAIL | PASS | SYSTEM | pthread_kill DESCRIPTION: it shall not be an error if the thread is a zombie thread; the call returned ESRCH. |
| XSH.pthread_mutex_getprioceiling.002 | FAIL | PASS | SYSTEM | pthread_mutex_getprioceiling ERRORS [EINVAL] (shall fail): the mutex protocol is PTHREAD_PRIO_NONE; the call returned 0. |
| XSH.pthread_sigmask.008 | FAIL | PASS | SYSTEM | sigprocmask RETURN VALUE (via pthread_sigmask page): an invalid how gives -1 with errno EINVAL; the call returned the error number 22 itself. |
| XSH.putwc.003 | FAIL | FAIL | SYSTEM | fputwc RETURN VALUE / ERRORS [EILSEQ]: for a code that is not a valid character the call returns WEOF, sets errno and sets the stream's error indicator; here WEOF and EILSEQ are given but the indicator stays clear (Linux fails it differently). |
| XSH.pwrite.002 | FAIL | FAIL | SYSTEM | pwrite DESCRIPTION: it writes at the given offset regardless of O_APPEND; the data went to the end (same on both systems). |
| XSH.pwrite.011 | UNRESOLVED | PASS | ENV | The guest refused a file of 4294967302 bytes (EFBIG); the test already reports UNRESOLVED. |
| XSH.pwrite.013 | FAIL | PASS | SYSTEM | write/pwrite: when RLIMIT_FSIZE leaves room for fewer bytes than requested, only those are written and their count returned; here the count differed from 46. |
| XSH.quick_exit.005 | FAIL | PASS | SYSTEM | quick_exit DESCRIPTION: a function registered during quick_exit() is called (after the registering function, before earlier ones); at_quick_exit() succeeded yet h3 never ran (order "21"). |
| XSH.readlink.018 | FAIL | UNRESOLVED | SYSTEM | Same as link.021 (readlinkat). |
| XSH.readlink.019 | FAIL | PASS | SYSTEM | readlink DESCRIPTION / XBD 4.12: readlink marks the link's last data access time for update; the atime stayed at the old value although reads of regular files update it on the same file system (read.014 passes). |
| XSH.realloc.003 | FAIL | PASS | SYSTEM | realloc ERRORS [ENOMEM] (shall fail): realloc(p, SIZE_MAX) returned a null pointer with errno left at 0. |
| XSH.recv.017 | FAIL | PASS | SYSTEM | XSH 2.10.6: when a peer has been specified for a datagram socket only datagrams from that peer shall be received; one from another socket was received first. |

## Changes (TEST-BUG)

- **XSH.getsockopt.005**: The target was a socket bound but still open, so no refusal is promised (the attempt may be dropped without an answer); a port with no endpoint is refused. Closed the bound socket after taking its port (t/005-error-pending.c); re-run PASS.
- **XSH.getsockopt.006**: Same set-up as .005. Same change (t/006-error-cleared.c); re-run PASS.
- **XSH.getsubopt.002**: The test measured strlen() of the string after the call, but the call may overwrite the equals sign (only the comma replacement is specified), moving the expected end. Compute the end pointer before the call (t/002-token-value.c); re-run PASS.
- **XSH.malloc.002**: malloc DESCRIPTION asks for alignment for an object of fundamental alignment "in the space allocated"; a 2-byte block cannot hold a 16-byte-aligned type, so the test over-reached for small sizes. need_align(n): the alignment of the widest basic type that fits in n; updated the .asr says (t/002-aligned.c, malloc.asr); re-run PASS.
- **XSH.mbrtoc16.002**: One mbstate_t was passed to mbrtoc32() and then mbrtoc16(); the text does not promise a state altered by one function can be used by the other (mbsinit page), and the second call saw a pending surrogate (-3). Separate zeroed state per function (t/002-portable.c); re-run PASS.
- **XSH.mbrtoc16.004**: Same shared-state set-up as .002. Separate states (t/004-null-pc-ps.c); re-run PASS.
- **XSH.mbrtoc16.009**: Same shared-state set-up as .002. Separate states (t/009-*.c); re-run PASS.

Also changed: `modules/xsh/malloc/malloc.asr` (says of XSH.malloc.002). Lint: 6373 problems before and after (all "declares no MUTANT"). None of the changed tests declares a MUTANT or VARIANT.

## Counts

- TEST-BUG: 7
- SYSTEM: 108
- STD-AMBIG: 8
- ENV: 5
- UNKNOWN: 1
- total: 129

## Patterns

- Set-ups that relied on behaviour beyond the text (TEST-BUG): a TCP port bound but still open as the "nothing listening" target (some systems drop the SYN instead of refusing); strlen() of a string the call may modify (getsubopt may overwrite the equals sign); alignment demanded for blocks too small to hold the widest type; one mbstate_t shared between mbrtoc32() and mbrtoc16().
- The second system deviates from "shall fail" errors in many places: zero/invalid arguments accepted (mmap len 0 and flags 0, munmap len 0, mkdtemp/mkstemp five-X templates, poll nfds, pthread stack size, mutex ceiling protocol), wrong error numbers (ELOOP, ENOMEM, EAI_OVERFLOW, ENOENT), and limits reported by sysconf() but not enforced ({STREAM_MAX}, {PTHREAD_THREADS_MAX}, {PTHREAD_KEYS_MAX}, {OPEN_MAX}).
- O_SEARCH is defined but does not skip the search check or exclude reading (link, mkdir, mkfifo, readlink, opendir tests; sibling modules fail the same way).
- Headers hide standard names under _POSIX_C_SOURCE=202405L (psignal, SEEK_HOLE/SEEK_DATA, O_TTY_INIT, POSIX_SPAWN_SETSID): verdicts UNINITIATED/UNRESOLVED that follow the system.
- mq_* flags are shared by all descriptors of a queue instead of belonging to the open description; mq_notify with SIGEV_THREAD never calls back; zero-length messages are refused.
- posix_spawn resolves the path before the file actions, leaves a child after returning an error, and mishandles equal-descriptor dup2, SETSCHEDPARAM alone and empty paths.
- Environment: file extension beyond 2 GiB/4 GiB refused (EFBIG) in the guest, kill_uid absent from the profile.
- Already disputed and left alone: Q-137 (getnameinfo.011), Q-128 (iconv.006 to .009, iconv_open.002), Q-64 (popen.011), Q-62 (msync.008), Q-2 (lseek.011). No new standard issue was needed.
- Open: XSH.mbrtoc16.003 (SIGSEGV with a null s) is UNKNOWN; re-run on the second system after the state split settles it.
