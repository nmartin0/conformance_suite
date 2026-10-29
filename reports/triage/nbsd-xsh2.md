# Triage of the second-system run, group xsh2 (XSH.recvfrom to XSH.write)

Second system: a BSD-derived system run as root; Linux column from the latest Linux run. The standard is the oracle; neither system is. Codes: S = second system, L = Linux.

Counts: SYSTEM 83, ENV 38, TEST-BUG 6, STD-AMBIG 2, UNKNOWN 0 (total 129).

| ID | S | L | class | reason |
|---|---|---|---|---|
| XSH.recvfrom.004 | FAIL | PASS | SYSTEM | address_len left at the truncated length; "on output specifies the length of the sending address" (recvfrom DESCRIPTION) |
| XSH.recvmsg.007 | FAIL | PASS | SYSTEM | msg_namelen left at truncated length; recvmsg DESCRIPTION msg_namelen on output |
| XSH.recvmsg.010 | FAIL | FAIL | SYSTEM | EMSGSIZE is a shall-fail entry for msg_iovlen <= 0 on recvmsg (no "zero or more buffers" conflict on this page); both systems return 0 |
| XSH.recvmsg.023 | FAIL | PASS | SYSTEM | control data that cannot be stored (msg_controllen 0) is truncated control data; MSG_CTRUNC required in msg_flags (recvmsg DESCRIPTION) |
| XSH.regcomp.014 | FAIL | PASS | SYSTEM | [--@] is explicitly valid and matches '-' through '@' (XBD 9.3.5); regcomp rejected it with REG_ERANGE |
| XSH.regcomp.017 | FAIL | FAIL | SYSTEM | XBD 9.3.6 states \(a\(b\)*\)*\2 fails to match abab; matched [0,4] on both |
| XSH.regcomp.027 | FAIL | PASS | SYSTEM | XBD 4.1: case counterparts of any characters must also match, so \(a\)\1 matches aA under REG_ICASE |
| XSH.regcomp.033 | FAIL | FAIL | SYSTEM | XBD 9.4.6 repetition modifier ? (shortest match) is required; second system rejects .*? with REG_BADRPT, Linux matches greedily |
| XSH.rename.012 | FAIL | FAIL | SYSTEM | either pathname with final component dot gives EINVAL (rename ERRORS); e is a directory so EISDIR does not apply |
| XSH.rename.013 | FAIL | PASS | SYSTEM | old existing non-directory and new nonexistent ending in slash is ENOTDIR (rename ERRORS) |
| XSH.rename.014 | FAIL | FAIL | SYSTEM | DESCRIPTION: new ending in slash not resolving to an existing directory makes rename() fail; it succeeded on both |
| XSH.rename.027 | FAIL | UNRESOLVED | SYSTEM | O_SEARCH descriptor must not be checked for search permission (rename DESCRIPTION); same failure in all eight *at O_SEARCH tests on this system |
| XSH.sched_getparam.001 | FAIL | PASS | TEST-BUG | -1 is the failure return of sched_get_priority_min/max; test took -1 with errno 0 as a real bound (range -1..-1) |
| XSH.sched_setparam.002 | FAIL | PASS | SYSTEM | priority one below the minimum (-1) is out of range and must give EINVAL; accepted (apparently -1 is a special value there) |
| XSH.sched_setscheduler.004 | FAIL | PASS | SYSTEM | same: priority min-1 = -1 accepted |
| XSH.select.009 | FAIL | FAIL | SYSTEM | regular files always select true for error conditions (select DESCRIPTION); error set not set on both |
| XSH.select.012 | FAIL | FAIL | SYSTEM | nfds greater than FD_SETSIZE is EINVAL (select ERRORS); call succeeded on both |
| XSH.sendmsg.003 | INSPECT | INSPECT | STD-AMBIG | already disputed (Q-148); INSPECT is the intended outcome, nothing to change |
| XSH.sendmsg.013 | FAIL | FAIL | SYSTEM | EAFNOSUPPORT shall-fail when the address family cannot be used with the socket; got ENOENT (path read from the AF_INET address) here, EINVAL on Linux |
| XSH.sendto.007 | FAIL | FAIL | SYSTEM | same as sendmsg.013 |
| XSH.setegid.005 | FAIL | PASS | SYSTEM | (gid_t)-1 is never a valid group ID (XBD 3.165), EINVAL required |
| XSH.seteuid.005 | FAIL | PASS | SYSTEM | (uid_t)-1 never valid (XBD 3.408), EINVAL required |
| XSH.setgid.005 | FAIL | PASS | SYSTEM | same |
| XSH.setpgid.005 | FAIL | PASS | SYSTEM | setpgid on a session leader shall fail with EPERM even when pgid equals its own ID |
| XSH.setpgid.009 | FAIL | PASS | SYSTEM | pgid naming a group of another session must give EPERM; returned 0 |
| XSH.setpgid.010 | FAIL | PASS | SYSTEM | pid that is a grandchild is not the caller or a child: ESRCH required; returned 0 |
| XSH.setsockopt.011 | FAIL | PASS | STD-AMBIG | recv gave up 1 ms early; the text calls the timeout a limit and states no lower bound; assertion made disputed, Q-211 |
| XSH.setsockopt.019 | FAIL | FAIL | SYSTEM | EINVAL "the socket has been shut down" is shall-fail; returned 0 on both |
| XSH.setuid.005 | FAIL | PASS | SYSTEM | same |
| XSH.shutdown.008 | FAIL | FAIL | SYSTEM | ENOTCONN shall-fail for a socket that is not connected; AF_UNIX fresh socket succeeded on both |
| XSH.sigaction.015 | FAIL | PASS | SYSTEM | with SA_NODEFER a signal in sa_mask is still blocked (sigaction DESCRIPTION); not blocked here |
| XSH.sigaction.023 | FAIL | FAIL | SYSTEM | SA_RESETHAND must clear SA_SIGINFO on handler entry; still set on both |
| XSH.strdup.002 | UNRESOLVED | PASS | ENV | the 224 MiB string could not be allocated under the 384 MiB address-space limit (setup), correctly UNRESOLVED |
| XSH.strftime.007 | FAIL | FAIL | SYSTEM | flag and width in conversion specifications (+ and 0 flags) are required by the 2024 text; "%+" read as a date format then literal 4Y |
| XSH.strsignal.001 | UNRESOLVED | PASS | SYSTEM | psignal is part of the base <signal.h> but undeclared under the driver's _POSIX_C_SOURCE=202405L; test (and psiginfo.001 it uses) do not build |
| XSH.strsignal.002 | UNRESOLVED | FAIL | SYSTEM | same |
| XSH.strtod.005 | FAIL | PASS | SYSTEM | "nan(!)" is not of the expected form, subject sequence is "nan": end pointer must be 3 |
| XSH.symlink.020 | FAIL | UNRESOLVED | SYSTEM | same O_SEARCH rule (symlink DESCRIPTION) |
| XSH.sysconf.014 | UNRESOLVED | PASS | SYSTEM | _SC_NPROCESSORS_ONLN/CONF are base symbols of <unistd.h> (2024) not defined under _POSIX_C_SOURCE=202405L |
| XSH.sysconf.015 | UNRESOLVED | UNRESOLVED | SYSTEM | _SC_NSIG/NSIG_MAX not provided |
| XSH.tcdrain.002 | FAIL | PASS | TEST-BUG | pty output may count as transmitted only when the manager reads it: the test never read before expecting tcdrain to end; also count-based wait ran 2x longer than 10 s |
| XSH.tcdrain.005 | FAIL | PASS | SYSTEM | /dev/null is not a terminal: ENOTTY required, got EOPNOTSUPP |
| XSH.tcdrain.006 | UNRESOLVED | PASS | ENV | opening the subsidiary side does not make it the controlling terminal; XBD 11.1.3 leaves acquisition implementation-defined, test correctly UNRESOLVED |
| XSH.tcdrain.007 | UNRESOLVED | PASS | ENV | opening the subsidiary side does not make it the controlling terminal; XBD 11.1.3 leaves acquisition implementation-defined, test correctly UNRESOLVED |
| XSH.tcdrain.008 | UNRESOLVED | PASS | ENV | opening the subsidiary side does not make it the controlling terminal; XBD 11.1.3 leaves acquisition implementation-defined, test correctly UNRESOLVED |
| XSH.tcflow.007 | FAIL | PASS | SYSTEM | same |
| XSH.tcflow.008 | UNRESOLVED | PASS | ENV | opening the subsidiary side does not make it the controlling terminal; XBD 11.1.3 leaves acquisition implementation-defined, test correctly UNRESOLVED |
| XSH.tcflow.009 | UNRESOLVED | PASS | ENV | opening the subsidiary side does not make it the controlling terminal; XBD 11.1.3 leaves acquisition implementation-defined, test correctly UNRESOLVED |
| XSH.tcflow.010 | UNRESOLVED | PASS | ENV | opening the subsidiary side does not make it the controlling terminal; XBD 11.1.3 leaves acquisition implementation-defined, test correctly UNRESOLVED |
| XSH.tcflush.008 | FAIL | PASS | SYSTEM | same |
| XSH.tcflush.009 | UNRESOLVED | PASS | ENV | opening the subsidiary side does not make it the controlling terminal; XBD 11.1.3 leaves acquisition implementation-defined, test correctly UNRESOLVED |
| XSH.tcflush.010 | UNRESOLVED | PASS | ENV | opening the subsidiary side does not make it the controlling terminal; XBD 11.1.3 leaves acquisition implementation-defined, test correctly UNRESOLVED |
| XSH.tcflush.011 | UNRESOLVED | PASS | ENV | opening the subsidiary side does not make it the controlling terminal; XBD 11.1.3 leaves acquisition implementation-defined, test correctly UNRESOLVED |
| XSH.tcgetattr.004 | UNRESOLVED | PASS | ENV | opening the subsidiary side does not make it the controlling terminal; XBD 11.1.3 leaves acquisition implementation-defined, test correctly UNRESOLVED |
| XSH.tcgetpgrp.002 | FAIL | PASS | SYSTEM | pipe is not the controlling terminal: ENOTTY required; returned 0 |
| XSH.tcgetpgrp.003 | UNRESOLVED | PASS | ENV | opening the subsidiary side does not make it the controlling terminal; XBD 11.1.3 leaves acquisition implementation-defined, test correctly UNRESOLVED |
| XSH.tcgetpgrp.004 | UNRESOLVED | PASS | ENV | opening the subsidiary side does not make it the controlling terminal; XBD 11.1.3 leaves acquisition implementation-defined, test correctly UNRESOLVED |
| XSH.tcgetpgrp.005 | UNRESOLVED | PASS | ENV | opening the subsidiary side does not make it the controlling terminal; XBD 11.1.3 leaves acquisition implementation-defined, test correctly UNRESOLVED |
| XSH.tcgetpgrp.007 | UNRESOLVED | PASS | ENV | opening the subsidiary side does not make it the controlling terminal; XBD 11.1.3 leaves acquisition implementation-defined, test correctly UNRESOLVED |
| XSH.tcgetsid.001 | UNRESOLVED | PASS | ENV | opening the subsidiary side does not make it the controlling terminal; XBD 11.1.3 leaves acquisition implementation-defined, test correctly UNRESOLVED |
| XSH.tcgetsid.002 | UNRESOLVED | PASS | ENV | opening the subsidiary side does not make it the controlling terminal; XBD 11.1.3 leaves acquisition implementation-defined, test correctly UNRESOLVED |
| XSH.tcgetsid.003 | UNRESOLVED | PASS | ENV | opening the subsidiary side does not make it the controlling terminal; XBD 11.1.3 leaves acquisition implementation-defined, test correctly UNRESOLVED |
| XSH.tcgetsid.004 | UNRESOLVED | PASS | ENV | opening the subsidiary side does not make it the controlling terminal; XBD 11.1.3 leaves acquisition implementation-defined, test correctly UNRESOLVED |
| XSH.tcgetsid.006 | UNRESOLVED | PASS | ENV | opening the subsidiary side does not make it the controlling terminal; XBD 11.1.3 leaves acquisition implementation-defined, test correctly UNRESOLVED |
| XSH.tcgetwinsize.004 | FAIL | UNINITIATED | SYSTEM | same |
| XSH.tcsendbreak.001 | FAIL | PASS | SYSTEM | pseudo-terminal subsidiary is a terminal; tcsendbreak must return 0 (send or do nothing), not ENOTTY |
| XSH.tcsendbreak.002 | FAIL | PASS | SYSTEM | same |
| XSH.tcsendbreak.004 | FAIL | PASS | SYSTEM | same |
| XSH.tcsendbreak.005 | UNRESOLVED | PASS | ENV | opening the subsidiary side does not make it the controlling terminal; XBD 11.1.3 leaves acquisition implementation-defined, test correctly UNRESOLVED |
| XSH.tcsendbreak.006 | UNRESOLVED | PASS | ENV | opening the subsidiary side does not make it the controlling terminal; XBD 11.1.3 leaves acquisition implementation-defined, test correctly UNRESOLVED |
| XSH.tcsendbreak.007 | UNRESOLVED | PASS | ENV | opening the subsidiary side does not make it the controlling terminal; XBD 11.1.3 leaves acquisition implementation-defined, test correctly UNRESOLVED |
| XSH.tcsetattr.004 | FAIL | PASS | TEST-BUG | same pty drain issue as tcdrain.002 |
| XSH.tcsetattr.008 | FAIL | PASS | SYSTEM | same |
| XSH.tcsetattr.011 | UNRESOLVED | PASS | ENV | opening the subsidiary side does not make it the controlling terminal; XBD 11.1.3 leaves acquisition implementation-defined, test correctly UNRESOLVED |
| XSH.tcsetattr.012 | UNRESOLVED | PASS | ENV | opening the subsidiary side does not make it the controlling terminal; XBD 11.1.3 leaves acquisition implementation-defined, test correctly UNRESOLVED |
| XSH.tcsetattr.013 | UNRESOLVED | PASS | ENV | opening the subsidiary side does not make it the controlling terminal; XBD 11.1.3 leaves acquisition implementation-defined, test correctly UNRESOLVED |
| XSH.tcsetpgrp.002 | FAIL | PASS | SYSTEM | same |
| XSH.tcsetpgrp.003 | UNRESOLVED | PASS | ENV | opening the subsidiary side does not make it the controlling terminal; XBD 11.1.3 leaves acquisition implementation-defined, test correctly UNRESOLVED |
| XSH.tcsetpgrp.004 | UNRESOLVED | PASS | ENV | opening the subsidiary side does not make it the controlling terminal; XBD 11.1.3 leaves acquisition implementation-defined, test correctly UNRESOLVED |
| XSH.tcsetpgrp.005 | UNRESOLVED | PASS | ENV | opening the subsidiary side does not make it the controlling terminal; XBD 11.1.3 leaves acquisition implementation-defined, test correctly UNRESOLVED |
| XSH.tcsetpgrp.006 | UNRESOLVED | PASS | ENV | opening the subsidiary side does not make it the controlling terminal; XBD 11.1.3 leaves acquisition implementation-defined, test correctly UNRESOLVED |
| XSH.tcsetpgrp.007 | UNRESOLVED | FAIL | ENV | opening the subsidiary side does not make it the controlling terminal; XBD 11.1.3 leaves acquisition implementation-defined, test correctly UNRESOLVED |
| XSH.tcsetpgrp.009 | UNRESOLVED | PASS | ENV | opening the subsidiary side does not make it the controlling terminal; XBD 11.1.3 leaves acquisition implementation-defined, test correctly UNRESOLVED |
| XSH.tcsetpgrp.010 | UNRESOLVED | PASS | ENV | opening the subsidiary side does not make it the controlling terminal; XBD 11.1.3 leaves acquisition implementation-defined, test correctly UNRESOLVED |
| XSH.tcsetwinsize.002 | UNRESOLVED | UNINITIATED | ENV | opening the subsidiary side does not make it the controlling terminal; XBD 11.1.3 leaves acquisition implementation-defined, test correctly UNRESOLVED |
| XSH.tcsetwinsize.003 | UNRESOLVED | UNINITIATED | ENV | opening the subsidiary side does not make it the controlling terminal; XBD 11.1.3 leaves acquisition implementation-defined, test correctly UNRESOLVED |
| XSH.tcsetwinsize.005 | FAIL | UNINITIATED | SYSTEM | same |
| XSH.tcsetwinsize.006 | UNRESOLVED | UNINITIATED | ENV | opening the subsidiary side does not make it the controlling terminal; XBD 11.1.3 leaves acquisition implementation-defined, test correctly UNRESOLVED |
| XSH.tcsetwinsize.007 | UNRESOLVED | UNINITIATED | ENV | opening the subsidiary side does not make it the controlling terminal; XBD 11.1.3 leaves acquisition implementation-defined, test correctly UNRESOLVED |
| XSH.tcsetwinsize.008 | UNRESOLVED | UNINITIATED | ENV | opening the subsidiary side does not make it the controlling terminal; XBD 11.1.3 leaves acquisition implementation-defined, test correctly UNRESOLVED |
| XSH.tgamma.010 | FAIL | PASS | SYSTEM | math_errhandling includes MATH_ERREXCEPT yet the overflow exception is not raised for tgamma(1024) (tgamma ERRORS, range error) [XSH tgamma] |
| XSH.thrd_exit.005 | FAIL | PASS | SYSTEM | last thread's thrd_exit must act as exit(0): atexit routines run; none ran (XSH thrd_exit DESCRIPTION) |
| XSH.thrd_sleep.002 | FAIL | FAIL | SYSTEM | a negative return must set errno (EINTR); errno was left stale (EALREADY) |
| XSH.thrd_sleep.003 | FAIL | FAIL | SYSTEM | same |
| XSH.thrd_sleep.004 | FAIL | FAIL | SYSTEM | EINVAL not set for tv_nsec out of range (errno untouched) |
| XSH.timer_create.007 | FAIL | PASS | SYSTEM | SIGEV_THREAD timer creation and arming succeed but the notification function never runs (timer_create DESCRIPTION, XSH 2.4.1); other thread tests run, so not a missing library (probable; a run with the thread library named on the compile line would confirm) |
| XSH.timer_create.010 | FAIL | PASS | SYSTEM | _POSIX_CPUTIME claimed, so timer_create must accept CLOCK_PROCESS_CPUTIME_ID; call failed (test also fixed to read errno after the call) |
| XSH.timer_create.011 | FAIL | PASS | SYSTEM | same for CLOCK_THREAD_CPUTIME_ID (_POSIX_THREAD_CPUTIME) |
| XSH.timer_getoverrun.012 | FAIL | PASS | SYSTEM | overrun count of a timer whose signal waited 1 s must be >= 1 when accepted by sigtimedwait; returned 0 |
| XSH.truncate.007 | FAIL | PASS | SYSTEM | EFBIG required when the length exceeds the soft RLIMIT_FSIZE |
| XSH.truncate.012 | FAIL | PASS | SYSTEM | truncate() of a FIFO fails with EINVAL (named example in ERRORS); returned 0 |
| XSH.ungetc.010 | UNRESOLVED | PASS | SYSTEM | fflush() of a read stream failed (fflush DESCRIPTION defines it for such streams, EBADF only for an invalid descriptor); the test correctly reports UNRESOLVED |
| XSH.ungetwc.004 | FAIL | PASS | SYSTEM | fseek must discard pushed-back characters; L'Z' read back |
| XSH.ungetwc.005 | FAIL | PASS | SYSTEM | same for fsetpos |
| XSH.ungetwc.006 | UNRESOLVED | PASS | SYSTEM | same fflush failure as ungetc.010 |
| XSH.unlink.021 | FAIL | UNRESOLVED | SYSTEM | same O_SEARCH rule (unlink DESCRIPTION) |
| XSH.vfprintf.007 | FAIL | PASS | SYSTEM | value to be returned above INT_MAX must give EOVERFLOW; errno left 0 |
| XSH.vfscanf.004 | FAIL | PASS | SYSTEM | %n$ conversions are in the base; returned 0 |
| XSH.vfwprintf.006 | FAIL | PASS | SYSTEM | successful vfwprintf must make the stream wide-oriented (2.5.2) |
| XSH.vfwscanf.006 | FAIL | PASS | SYSTEM | %n$ conversions in wide scanf |
| XSH.wait.019 | FAIL | FAIL | SYSTEM | pending SIGCHLD must be cleared/discarded by wait() when no other child has status (wait DESCRIPTION); remained pending on both |
| XSH.waitid.015 | FAIL | FAIL | SYSTEM | ECHILD only when there are no unwaited-for children; a terminated unreaped child exists, so WSTOPPED/WNOHANG must return 0 with si_pid 0; both systems give ECHILD |
| XSH.wcsftime.004 | FAIL | PASS | SYSTEM | an empty format still stores the terminating null wide character (wcsftime RETURN VALUE) |
| XSH.wcsftime.006 | FAIL | FAIL | SYSTEM | width with the %F conversion: %010F is 10 wide |
| XSH.wcstod.005 | FAIL | PASS | SYSTEM | "nan(!)" subject sequence is "nan": end pointer at 3 |
| XSH.wcstoimax.003 | FAIL | PASS | SYSTEM | "0x" with base 0: subject is "0", end pointer after it |
| XSH.wcstoimax.004 | FAIL | PASS | SYSTEM | same with base 16 |
| XSH.wcstoimax.009 | UNRESOLVED | UNINITIATED | TEST-BUG | used snprintf/strcpy on wchar_t arrays (setup failed on one system, build failed on Linux) |
| XSH.wcstoimax.016 | FAIL | PASS | SYSTEM | wcstoumax "0x" base 0 |
| XSH.wcstoimax.017 | FAIL | PASS | SYSTEM | wcstoumax "0x" base 16 |
| XSH.wcstol.003 | FAIL | PASS | SYSTEM | "0x" base 0 |
| XSH.wcstol.004 | FAIL | PASS | SYSTEM | "0x" base 16 |
| XSH.wcstol.009 | UNRESOLVED | UNINITIATED | TEST-BUG | same as wcstoimax.009 |
| XSH.wcstoul.003 | FAIL | PASS | SYSTEM | "0x" base 0 |
| XSH.wcstoul.004 | FAIL | PASS | SYSTEM | "0x" base 16 |
| XSH.wcstoul.009 | UNRESOLVED | UNINITIATED | TEST-BUG | same as wcstoimax.009 |
| XSH.wordexp.016 | FAIL | PASS | SYSTEM | after a non-NOSPACE failure with WRDE_APPEND we_wordc and we_wordv shall not be modified |
| XSH.write.006 | FAIL | PASS | SYSTEM | write beyond RLIMIT_FSIZE with room for 20 bytes must write 20 and return 20; returned -1 EFBIG |
| XSH.write.022 | FAIL | FAIL | SYSTEM | pwrite writes at offset regardless of O_APPEND; appended on both |

## TEST-BUG fixes and re-runs (this Linux machine)

- XSH.tcdrain.002, XSH.tcsetattr.004 (t/002-blocks.c, t/004-tcsadrain.c): both held a byte in a pseudo-terminal's output, restarted output with tcflow(TCOON) and then expected the blocked call to end while the manager side was not reading. Where output counts as transmitted only once the manager has read it, tcdrain() and tcsetattr(TCSADRAIN) legitimately stay blocked until then (the standard does not say when pseudo-terminal output is "transmitted"). Changed: after TCOON the test reads the manager side (collect()) before waiting for the child; the byte and attribute checks are unchanged. reap_within() also counted 10 ms sleeps instead of reading the clock, so on a coarse-tick system the "10 s" wait took 20 s and met the child's own alarm (status 0xe); it now uses CLOCK_MONOTONIC. Re-run: both PASS.
- XSH.wcstoimax.009, XSH.wcstol.009, XSH.wcstoul.009 (t/009-limits.c): the set-up built wide strings with snprintf()/strcpy() on wchar_t arrays (a bad conversion of a narrow template; it did not even build on Linux). Changed to swprintf()/wcscpy(). Re-run: all three PASS (were UNINITIATED on Linux, UNRESOLVED on the second system).
- XSH.sched_getparam.001 (t/001-self.c, .asr note): a sched_get_priority_min/max result of -1 with errno 0 was used as a real bound (range -1..-1), so any priority "failed". -1 is the failure value, so the test is now UNRESOLVED when either bound is -1. Re-run: PASS.
- XSH.timer_create.010, XSH.timer_create.011 stay SYSTEM, but the failing call was in the argument list of pc_check() beside pc_errname(errno), whose evaluation order is unspecified (the message printed "failed: 0"). The call and errno are now captured first. Re-run: both PASS.

## STD-AMBIG

- XSH.setsockopt.011: class set to disputed with a note; row Q-211 appended to docs/standard-issues.md. Re-run: PASS (INSPECT only on an early return).
- XSH.sendmsg.003: already disputed (Q-148).

Lint: 6373 problems before and after (all "declares no MUTANT").

## Patterns

- 37 pseudo-terminal lines are UNRESOLVED because opening the subsidiary side does not make it the controlling terminal. The text leaves acquisition implementation-defined (XBD 11.1.3), so UNRESOLVED is the right outcome and no test is wrong. Those tests lose all their coverage on such a system; an optional fallback (an ioctl-based request for the terminal, found by building, RULES 12) in the shared set-up code of the roughly 65 copies would recover it. Not done here: it cannot be checked without running on that system, a wrong header guess would turn UNRESOLVED into a build failure, and about 28 of the copies are in modules outside this list.
- tty calls on /dev/null give EOPNOTSUPP instead of ENOTTY (7 lines), on a pipe tcgetpgrp()/tcsetpgrp() succeed (2), and tcsendbreak() on a pseudo-terminal gives ENOTTY (2): all SYSTEM.
- Features new in the 2024 text that the second system lacks: strftime/wcsftime flags and widths, %n$ in scanf, _SC_NPROCESSORS_*, _SC_NSIG, psignal() visible under the strict feature macro, regcomp repetition modifier, thrd_sleep() errno. Several fail on both systems (strftime.007, wcsftime.006, thrd_sleep.002 to .004, regcomp.017/.033, select.009/.012, rename.012/.014, sendto.007, sendmsg.013, shutdown.008, setsockopt.019, sigaction.023, wait.019, waitid.015, write.022): the text is explicit in each, so they stay SYSTEM.
- All eight O_SEARCH descriptor tests in this group and nearby (rename.027, symlink.020, unlink.021, ...) fail: the system checks search permission on an O_SEARCH descriptor.
- Wide-character conversions "0x" with base 0/16 (8 lines) and "nan(!)" (strtod.005, wcstod.005) are consistent SYSTEM deviations; the narrow strtol/strtoul pass on the same system.
- Unverifiable: XSH.setpgid.009 (second call) and XSH.timer_create.007 are judged SYSTEM from the output alone; XSH.timer_create.010/.011 failure errno is unknown (needs a rerun of the amended tests there).
