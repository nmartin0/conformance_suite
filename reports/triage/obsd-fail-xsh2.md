# Triage obsd-fail-xsh2

Second-system run, group fail-xsh2 (50 lines). Output files under the run's out/ were empty for these, so verdicts rest on the run log, the test sources and the standard.

| ID | this system | Linux | class | reason |
|---|---|---|---|---|
| XSH.rename.027 | UNRESOLVED | UNRESOLVED | SYSTEM | <fcntl.h> lacks the required O_SEARCH; the test reports UNRESOLVED correctly (XBD fcntl.h). |
| XSH.rewind.003 | UNRESOLVED | PASS | SYSTEM | rewind() restores errno, so the failure the page tells applications to detect through errno is invisible and the precondition cannot be established (rewind DESCRIPTION, ERRORS: as fseek). |
| XSH.rewind.004 | FAIL | PASS | SYSTEM | flushing pending output on a closed descriptor must set EBADF (fseek ERRORS); errno is left 0 (rewind DESCRIPTION: clear errno, call, test errno). |
| XSH.rewind.008 | FAIL | PASS | SYSTEM | same: EFBIG from the failed flush is not left in errno. |
| XSH.rewind.009 | FAIL | PASS | SYSTEM | same: EPIPE/ESPIPE not left in errno. |
| XSH.rewind.010 | FAIL | PASS | SYSTEM | same: EAGAIN/ESPIPE not left in errno. |
| XSH.scalbn.002 | FAIL | PASS | SYSTEM | scalbnl overflow must raise the overflow exception where math_errhandling selects exceptions (scalbn RETURN VALUE/ERRORS); double and float cases pass. |
| XSH.scalbn.011 | FAIL | PASS | SYSTEM | same for scalblnl. |
| XSH.setegid.004 | FAIL | FAIL | SYSTEM | setegid(<effective, neither real nor saved>) without privileges must fail EPERM (setegid ERRORS). |
| XSH.seteuid.004 | FAIL | FAIL | SYSTEM | seteuid ERRORS/EPERM: the effective ID itself is not exempt. |
| XSH.setgid.004 | FAIL | PASS | SYSTEM | setgid ERRORS/EPERM: without privileges the ID must match real or saved; the effective ID is accepted. |
| XSH.setsockopt.006 | FAIL | PASS | SYSTEM | an option a protocol does not support must fail ENOPROTOOPT (setsockopt ERRORS; XSH 2.3 forbids another number for a described condition); EOPNOTSUPP is not listed. |
| XSH.setsockopt.021 | FAIL | NOTINUSE | SYSTEM | IPV6_V6ONLY has default 0 (2.10.20) so 0 must be settable; setting 0 gives EINVAL (and getsockopt.017 shows the default is 1). |
| XSH.setuid.004 | FAIL | PASS | SYSTEM | setuid ERRORS/EPERM: effective ID that is neither real nor saved is accepted. |
| XSH.shm_open.002 | FAIL | PASS | SYSTEM | shm_open O_CREAT: permission bits are mode minus the umask bits; group bits are dropped (0600 for 0666 under 027). |
| XSH.shm_open.008 | FAIL | PASS | SYSTEM | same: an object created with mode 0640 and umask 0 has 0600. |
| XSH.shm_unlink.005 | INSPECT | PASS | STD-AMBIG | already class disputed (Q-71): EPERM versus EACCES for a refused unlink; nothing to change. |
| XSH.sigaddset.001 | UNRESOLVED | PASS | TEST-BUG | blocked only because sigismember.001 failed to build on a missing SIGRTMIN; see sigismember.001. |
| XSH.sigaddset.002 | UNRESOLVED | PASS | TEST-BUG | unblocked by the sigismember.001 fix; the test itself needs SIGRTMIN and now reports UNRESOLVED cleanly instead of a build failure when it is absent. |
| XSH.sigdelset.001 | UNRESOLVED | PASS | TEST-BUG | blocked by sigismember.001 (see there). |
| XSH.sigdelset.002 | UNRESOLVED | PASS | TEST-BUG | as sigaddset.002. |
| XSH.sigemptyset.001 | UNRESOLVED | PASS | TEST-BUG | blocked by sigismember.001 (see there). |
| XSH.sigemptyset.002 | UNRESOLVED | PASS | TEST-BUG | as sigaddset.002. |
| XSH.sigfillset.001 | UNRESOLVED | PASS | TEST-BUG | blocked by sigismember.001 (see there). |
| XSH.sigfillset.002 | UNRESOLVED | PASS | TEST-BUG | as sigaddset.002. |
| XSH.sigsuspend.006 | UNRESOLVED | PASS | TEST-BUG | blocked by sigpending.002, which only used sigtimedwait() for cleanup (fixed); the test's own sigtimedwait() use is legitimate, so on this system it will now be a build failure (sigtimedwait absent: SYSTEM). |
| XSH.sigtimedwait.001 | UNRESOLVED | PASS | SYSTEM | needs sigqueue() and SIGRTMIN (base interfaces in <signal.h>/sigqueue) which are absent; test correct. |
| XSH.sigwait.003 | UNRESOLVED | PASS | SYSTEM | needs sigqueue()/SIGRTMIN, absent; test correct. |
| XSH.socket.011 | FAIL | PASS | SYSTEM | protocol 0 with an unsupported family must give EAFNOSUPPORT; EPROTONOSUPPORT is for a non-zero protocol (socket ERRORS). |
| XSH.socketpair.009 | FAIL | FAIL | SYSTEM | same rule for socketpair(). |
| XSH.strftime.014 | FAIL | FAIL | SYSTEM | strftime ERRORS/ERANGE (shall fail) and RETURN VALUE: errno set when the result with its NUL exceeds maxsize; errno left 0. |
| XSH.symlink.020 | UNRESOLVED | UNRESOLVED | SYSTEM | <fcntl.h> lacks O_SEARCH. |
| XSH.sysconf.001 | FAIL | FAIL | SYSTEM | sysconf must support every table name incl. _SC_RTSIG_MAX; it fails EINVAL. |
| XSH.sysconf.002 | FAIL | FAIL | SYSTEM | same name: -1 with errno changed, not the allowed no-limit form. |
| XSH.sysconf.005 | FAIL | FAIL | SYSTEM | _SC_VERSION is 200809, not 202405 (unistd.h version macros). |
| XSH.sysconf.006 | FAIL | PASS | SYSTEM | Asynchronous I/O is mandatory in the 2024 base; _SC_ASYNCHRONOUS_IO returns -1. |
| XSH.sysconf.007 | FAIL | FAIL | SYSTEM | option values must be 202405L; _SC_BARRIERS gives 200112. |
| XSH.sysconf.009 | FAIL | FAIL | SYSTEM | sysconf RETURN VALUE: only _SC_OPEN_MAX may change after setrlimit(); _SC_STREAM_MAX moved 128 to 127. |
| XSH.system.006 | FAIL | PASS | SYSTEM | system() must behave as sh -c -- command; the shell here parses -pcdash as options (system DESCRIPTION). |
| XSH.tcgetpgrp.006 | FAIL | PASS | SYSTEM | no controlling terminal: tcgetpgrp() must fail ENOTTY; it returned a group ID. |
| XSH.ungetc.011 | FAIL | PASS | SYSTEM | ungetc decrements the file position, fseek SEEK_CUR counts from it (ungetc DESCRIPTION, fseek DESCRIPTION); the system seeks from the position before the pushback. |
| XSH.ungetc.013 | FAIL | PASS | SYSTEM | same cause: write after fseek(0,SEEK_CUR) lands one byte late. |
| XSH.unlink.021 | UNRESOLVED | UNRESOLVED | SYSTEM | <fcntl.h> lacks O_SEARCH. |
| XSH.vfwprintf.001 | FAIL | PASS | SYSTEM | aborts (signal 6) on the %n conversion, which wide printf must support (fprintf.024, fwprintf.006 abort likewise). |
| XSH.wcsftime.007 | FAIL | PASS | SYSTEM | %V for 2000-01-01 must be the last ISO week of 1999, 52; the system says 53. |
| XSH.wcstod.002 | FAIL | PASS | SYSTEM | wcstod must accept the hexadecimal form (wcstod DESCRIPTION); strtod does here, wcstod stops after the 0. |
| XSH.wcstod.003 | FAIL | PASS | SYSTEM | same: "-0x1p1" gives -0. |
| XSH.wcstod.007 | FAIL | PASS | SYSTEM | same: "0x1p" gives 0 instead of 1. |
| XSH.wcstod.009 | FAIL | PASS | SYSTEM | same: "0x1p100000" not converted. |
| XSH.wcstod.010 | FAIL | PASS | SYSTEM | same: "0x1p-100000" gives no ERANGE. |

## TEST-BUG changes

- XSH.sigismember.001 (t/001-one-or-zero.c, .asr note): SIGRTMIN used unconditionally, so a header lacking it gave a build failure and blocked the eight set tests that depend on it. Now `#ifdef SIGRTMIN`, else the real-time non-member is skipped with pc_note (XBD.signal.h.006 reports the missing macro). Re-run on Linux: PASS.
- XSH.sigpending.002 (t/002-thread.c): the closing sigtimedwait() was only cleanup; replaced by signal(SIG_IGN) on the two signals, which discards pending instances. Re-run: PASS (and sigsuspend.006, which depends on it: PASS).
- XSH.sigaddset.002, sigdelset.002, sigemptyset.002, sigfillset.002: guarded with `#if !defined(SIGRTMIN) || !defined(SIGRTMAX)` giving pc_unresolved, like the O_SEARCH tests, instead of failing to build. Re-run: PASS each. Syntax of the no-macro branch checked by compiling with the macro names renamed.
- Lint: 6373 problems before and after (only 'declares no MUTANT').

## Counts

STD-AMBIG 1, SYSTEM 40, TEST-BUG 9

## Patterns

- BSD-style rewind() restores errno (5 lines); fseek SEEK_CUR ignores pushback (2); wcstod lacks hex floats (5); %n aborts (1).
- Missing base interfaces: O_SEARCH (3), SIGRTMIN/sigqueue/sigtimedwait, _SC_RTSIG_MAX, 2024 version values and mandatory options (sysconf, 6).
- Privilege checks accept the effective ID (4), shm_open drops group bits (2), unsupported-family errno (2), SO_DONTROUTE/V6ONLY (2).
- Only TEST-BUG cluster: a test needing an unneeded optional-looking feature blocking primitive-verifying assertions (sigismember/sigpending).
