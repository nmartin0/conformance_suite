# FreeBSD triage, group xsh

Counts: TEST-BUG 6, SYSTEM 149, STD-AMBIG 12, ENV 0, UNKNOWN 0.

| ID | result | class | reason |
|---|---|---|---|
| XSH.2.11-data-types.002 | FAIL | SYSTEM | XSH 2.11 and XBD sys/types.h make rlim_t unsigned and dirent.h/sys/types.h define reclen_t (unshaded); the build shows rlim_t signed and reclen_t undeclared. |
| XSH.2.2-compilation-environment.002 | FAIL | SYSTEM | libintl.h is an unshaded required header in POSIX.1-2024 (XBD libintl.h, XSH 2.2.2); the build stops on 'libintl.h' not found. |
| XSH.2.2-compilation-environment.003 | FAIL | SYSTEM | Same header set in reverse order; the build stops on the missing libintl.h. |
| XSH.2.2-compilation-environment.004 | FAIL | SYSTEM | Same: the required libintl.h is missing, so the program cannot build. |
| XSH.2.3-error-numbers.004 | FAIL | TEST-BUG | The link loop up to {LINK_MAX} ran past the test's own 100 s alarm and was reported FAIL; no time is bounded for link(). Fixed: the alarm now gives UNRESOLVED (cebf7524). |
| XSH.2.5.1-stream-handles.014 | FAIL | SYSTEM | fflush() of an "r+" stream last used for input must set the offset to the stream position (fflush DESCRIPTION, 2.5.1); the file is grown so it is not at EOF on either Q-29 reading; the write landed past the read-ahead. |
| XSH.2.5.2-stream-orientation.003 | FAIL | SYSTEM | 2.5.2: a byte I/O function on an unoriented stream makes it byte-oriented; after fprintf() fwide() still returned 0. |
| XSH.2.5.2-stream-orientation.004 | FAIL | SYSTEM | 2.5.2: a wide I/O function makes the stream wide-oriented; after fwprintf() fwide() returned 0. |
| XSH.2.5.2-stream-orientation.006 | FAIL | SYSTEM | 2.5.2: the encoding rule is fixed by LC_CTYPE when the stream becomes wide-oriented; the read followed the later single-byte locale (got 195, not the one UTF-8 character). |
| XSH.2.9-threads.010 | FAIL | TEST-BUG | 2.9.4's range guarantee covers only the option's priority-based policies; the test used the creator's policy (SCHED_OTHER here). Fixed to use SCHED_FIFO/SCHED_RR (4d3ed3ea). |
| XSH.2.9-threads.025 | FAIL | SYSTEM | 2.9.5.2 forbids cancellation points in unlisted functions; gmtime_r() is in neither list and is required thread-safe, yet the thread was canceled in it. |
| XSH.2.9-threads.026 | FAIL | SYSTEM | 2.9.5.3: on pthread_exit() the thread first sets cancelability DISABLE/DEFERRED; the cleanup handler did not see DISABLE. |
| XSH.2.9-threads.028 | FAIL | SYSTEM | 2.9.5.2: write() is a required cancellation point and a thread suspended in it shall be woken and the request acted on; the blocked writer's cleanup handler had not run 10 s after the request. |
| XSH.2.9-threads.034 | FAIL | SYSTEM | 2.4.2/2.9.8: a SIGEV_THREAD function runs as if created by pthread_create() with sigev_notify_attributes, so on the application stack given; it ran on another stack. |
| XSH.aio_read.009 | FAIL | STD-AMBIG | aio_read() on a pipe failed ENOTSUP; whether asynchronous I/O must work on a pipe is the suite's open Q-56 (and 2.3 allows additional errors). Not disputed; should be, like XSH.fork.023's handling. |
| XSH.aio_suspend.004 | FAIL | SYSTEM | aio_suspend(): null entries are ignored, and if no listed operation completes in the timeout it shall fail EAGAIN; a list of only nulls returned 0. |
| XSH.bind.018 | FAIL | SYSTEM | bind() ERRORS for AF_UNIX: a null address shall fail with EDESTADDRREQ or EISDIR; got EFAULT (2.3 forbids a different number for a described condition). |
| XSH.c16rtomb.008 | FAIL | SYSTEM | c16rtomb() RETURN VALUE: a code that is not a valid character is an encoding error ((size_t)-1, EILSEQ); with __STDC_UTF_32__ in a UTF-8 locale c32rtomb(0x110000) returned 4. |
| XSH.c16rtomb.009 | INSPECT | SYSTEM | Disputed (Q-121) only for errno; what failed is the return value: c16rtomb() of the null code must store a null byte and return 1 (DESCRIPTION, RETURN VALUE), it returned 0. |
| XSH.catgets.003 | FAIL | SYSTEM | gencat EXTENDED DESCRIPTION: \ddd gives the byte of that octal value; message 1 lost the \101 ('A'). |
| XSH.catgets.004 | FAIL | SYSTEM | gencat: a number with a blank separator and empty text stores an empty string; catgets() returned the default instead. |
| XSH.chown.008 | INSPECT | STD-AMBIG | Already disputed (Q-25): chown(-1,-1) of another user's file meets the EPERM condition but changes nothing; the system returned 0. |
| XSH.close.014 | FAIL | STD-AMBIG | aio_write() to a pipe failed ENOTSUP before close() was reached; whether AIO must be supported on a pipe is open (Q-56, 2.3 additional errors). Not disputed; should be. |
| XSH.cosh.005 | FAIL | SYSTEM | cosh() of a huge argument is an overflow range error; with MATH_ERREXCEPT in math_errhandling FE_OVERFLOW shall be raised (cosh RETURN VALUE, XBD 4.23); for double it was not. |
| XSH.creat.018 | FAIL | SYSTEM | open() RETURN VALUE (creat() is open()): if -1 is returned no file shall be created; the EMFILE failure left "new" created. |
| XSH.exec.010 | FAIL | SYSTEM | exec DESCRIPTION: a descriptor left open keeps FD_CLOFORK set; the new image found it cleared. |
| XSH.exec.028 | FAIL | SYSTEM | exec (TCT): the initial thread's CPU-time clock of the new image starts at zero; it read 2043 ms after the caller had used 2000 ms. |
| XSH.exec.032 | FAIL | SYSTEM | exec: success marks the image file's last data access time for update; a read updates atime on this file system, exec did not. |
| XSH.exit.004 | FAIL | SYSTEM | exit(): all atexit() functions are called, a late-registered one after its registrant; atexit(b) inside a handler succeeded but b was never called ("ax"). |
| XSH.exit.010 | FAIL | SYSTEM | exit() (CX): for an active seekable input stream not at EOF the offset is set to the stream position; the offset stayed at the 4096-byte read-ahead. |
| XSH.fchdir.004 | FAIL | SYSTEM | fchdir() ERRORS: a descriptor not referring to a directory shall fail ENOTDIR; a pipe gave EINVAL. |
| XSH.fclose.003 | FAIL | SYSTEM | fclose() DESCRIPTION: for an input stream not at EOF on a seekable file the offset is set to the stream position; it stayed 4096, not 1 (file grown, so Q-29 does not arise). |
| XSH.fclose.010 | FAIL | SYSTEM | Same rule for an "r+" stream last used for input; offset 4096, expected 1. |
| XSH.fclose.011 | FAIL | SYSTEM | Same rule with ungetc() pushback (position 1); offset 4096. |
| XSH.fclose.012 | FAIL | SYSTEM | fclose() shall fail ENOSPC when the fmemopen() buffer has no room; it returned EOF but left errno 0. |
| XSH.fclose.013 | FAIL | TEST-BUG | SIGXFSZ is XSI and hidden in the base build, but the signal is still sent; the child ignored it only when named and was killed. It now blocks every signal when the name is absent |
| XSH.fflush.004 | FAIL | SYSTEM | fflush() DESCRIPTION: an input stream on a seekable file not at EOF gets the offset set to its position; offset 4096, not 1. |
| XSH.fflush.005 | FAIL | SYSTEM | fflush() shall discard ungetc() pushback (fflush, ungetc pages; file not at EOF); the next fgetc() returned the pushed-back 'x'. |
| XSH.fflush.010 | FAIL | TEST-BUG | SIGXFSZ is XSI and hidden in the base build, but the signal is still sent; the child ignored it only when named and was killed. It now blocks every signal when the name is absent |
| XSH.fflush.011 | INSPECT | STD-AMBIG | Already disputed (Q-28): whether fflush(NULL) applies the input-stream offset rule; offset 4096. |
| XSH.fflush.013 | FAIL | SYSTEM | fflush() shall fail ENOSPC when the fmemopen() buffer is full; it returned EOF with errno 0. |
| XSH.fflush.016 | FAIL | SYSTEM | fflush() of an "r+" stream last used for input sets the offset to its position; 4096, expected 1. |
| XSH.fgetpos.004 | FAIL | SYSTEM | fgetpos() ERRORS (CX, shall fail): EBADF when the underlying descriptor is not valid; it returned 0 with the descriptor closed. |
| XSH.fmemopen.014 | FAIL | SYSTEM | fmemopen(): a read shall not advance past the end position and reaching it is EOF; a read at 5 with end 3 returned a byte. |
| XSH.fmemopen.015 | FAIL | SYSTEM | fmemopen(): a null byte is written only when a write advances the end position; "r+" (fixed end) got a null inside ("abXY\0f"). |
| XSH.fmemopen.020 | FAIL | SYSTEM | Same rule for "w+": a write at 1 that does not move the end position stored a null after it. |
| XSH.fmemopen.022 | FAIL | SYSTEM | fmemopen(): only seeks to a negative position or beyond max_size fail; fseek(7, SEEK_END) to 10 (= max_size) failed EINVAL. |
| XSH.fputws.006 | FAIL | SYSTEM | fputws() RETURN VALUE: on error return -1 and set the stream's error indicator; after the EILSEQ failure ferror() was 0. |
| XSH.freopen.004 | FAIL | SYSTEM | freopen(): a failure to close the old descriptor is ignored and the stream is associated with the new file; it returned the stream but the first read failed. |
| XSH.freopen.011 | FAIL | SYSTEM | freopen() (CX): the new descriptor is allocated as if by open(), i.e. the lowest free (XSH 2.6); it got 6 (the old one), not 4. |
| XSH.fscanf.005 | FAIL | SYSTEM | fscanf() (CX, base since Issue 7): "%n$" numbered conversions; sscanf("10 20","%2$d %1$d") returned 0. |
| XSH.fscanf.013 | FAIL | SYSTEM | fscanf(): the input item is the longest prefix of a matching sequence and the next byte stays unread; for "-x" the '-' was pushed back too. |
| XSH.fscanf.014 | FAIL | SYSTEM | fscanf(): on "100ergs" the %f item is "100e", not a matching sequence, so the conversion fails; it returned 1. |
| XSH.fscanf.015 | FAIL | SYSTEM | Same rule: "0x" alone is not a matching sequence for %x; it returned 1. |
| XSH.fscanf.019 | FAIL | SYSTEM | fscanf(): white space is skipped before every conversion except [, c, C, n, so %% skips it; " \t%7" with "%%%d" returned 0. |
| XSH.fscanf.037 | FAIL | SYSTEM | fscanf() (CX): the 'm' assignment-allocation character for s, c and [; returned 0. |
| XSH.fscanf.038 | FAIL | SYSTEM | Same 'm' feature with ls and l[; returned 0. |
| XSH.fseek.004 | INSPECT | STD-AMBIG | Already disputed (Q-40): the offset is only visible through another handle; "r" stream offset 10, expected 7. |
| XSH.ftell.006 | FAIL | SYSTEM | ftell() ERRORS (CX, shall fail): EBADF when the descriptor is not open; it returned 1 with the descriptor closed. |
| XSH.ftruncate.008 | FAIL | SYSTEM | ftruncate() ERRORS: length beyond the process file size limit shall fail EFBIG; ftruncate(4097) under a 4096 limit succeeded. |
| XSH.fwprintf.011 | FAIL | SYSTEM | 2.5.2: fwprintf() is a wide I/O function, so the stream becomes wide-oriented; it was not. |
| XSH.fwprintf.014 | FAIL | SYSTEM | fwprintf() ERRORS: swprintf() shall fail EOVERFLOW when n or more wide characters are requested; n 0 gave EINVAL. |
| XSH.fwscanf.017 | FAIL | SYSTEM | fwscanf() (CX): the 'm' allocation character with ls; %mls returned 0. |
| XSH.fwscanf.020 | FAIL | SYSTEM | fwscanf() (CX): "%n$" numbered conversions; returned 0. |
| XSH.getaddrinfo.004 | FAIL | SYSTEM | getaddrinfo(): each entry's family, type and protocol shall be usable with socket(); an SCTP/SOCK_SEQPACKET entry was returned and socket() refused it (EPROTONOSUPPORT). |
| XSH.getaddrinfo.030 | FAIL | SYSTEM | Same: a returned entry (2, 5, 132) is not accepted by socket(). |
| XSH.gethostent.004 | FAIL | SYSTEM | endhostent() shall close the connection, releasing any open descriptor; a descriptor opened by sethostent(1)/gethostent() stayed open. |
| XSH.getnameinfo.008 | FAIL | SYSTEM | getnameinfo() ERRORS: EAI_NONAME when both the node and service are null; null pointers gave 0. |
| XSH.getnameinfo.011 | INSPECT | STD-AMBIG | Already disputed (Q-137): whether an unlisted flag bit must give EAI_BADFLAGS; it gave 0. |
| XSH.getopt.006 | FAIL | SYSTEM | getopt() (Issue 8): a leading '+' is skipped before the ':' check; with "+:a:" a missing argument gave '?' instead of ':'. |
| XSH.getopt.007 | FAIL | SYSTEM | Same: with "+:" no diagnostic may be written, yet one was. |
| XSH.getopt.008 | FAIL | SYSTEM | getopt(): a leading '+' has no effect, so '+' on the command line is an unknown option; it was accepted as an option. |
| XSH.getpeername.008 | FAIL | SYSTEM | getpeername() ERRORS (shall fail): EINVAL when the socket has been shut down (ENOTCONN also accepted); it succeeded after SHUT_RDWR. |
| XSH.getrlimit.005 | FAIL | SYSTEM | setrlimit() shall fail EINVAL when the new rlim_cur exceeds rlim_max; it returned 0. |
| XSH.getrlimit.006 | FAIL | SYSTEM | RLIM_INFINITY is larger than any limit, so cur RLIM_INFINITY with a finite max shall fail EINVAL; returned 0. |
| XSH.getrlimit.014 | FAIL | SYSTEM | RLIMIT_FSIZE / ftruncate() ERRORS: extending past the file size limit shall fail EFBIG; ftruncate to 1001 under 1000 returned 0. |
| XSH.glob.011 | FAIL | SYSTEM | glob() uses XCU 2.14 patterns, where bracket expressions take character classes (XBD 9.3.5); "[[:digit:]]x" gave GLOB_NOMATCH. |
| XSH.glob.016 | FAIL | SYSTEM | XCU 2.14.3: a '[' whose bracket would contain '/' is ordinary, so "a[b/c]d" matches the literal name; got GLOB_NOMATCH. |
| XSH.iconv.007 | FAIL | SYSTEM | iconv() with //NON_IDENTICAL_DISCARD: a character with no identical counterpart is discarded (but counted); the output was not "ab". |
| XSH.inet_ntop.002 | FAIL | SYSTEM | inet_pton(AF_INET): one to three decimal digits per part, leading zeros allowed; "001.002.003.004" returned 0. |
| XSH.kill_dependency.001 | FAIL | SYSTEM | XBD stdatomic.h: with __STDC_NO_ATOMICS__ undefined the header shall define kill_dependency(); it did not (under -D_POSIX_C_SOURCE=202405L). |
| XSH.kill_dependency.002 | FAIL | SYSTEM | Same missing kill_dependency macro. |
| XSH.lio_listio.001 | FAIL | STD-AMBIG | The list includes a read of a pipe; lio_listio() gave EIO, most likely from that entry (aio on a pipe gives ENOTSUP here), which is the open Q-56. Per-entry aio_error() values would confirm. Not disputed. |
| XSH.lio_listio.002 | FAIL | SYSTEM | lio_listio(): null entries and LIO_NOP entries are ignored; a list of a NOP (fd -1) and one file write returned EIO. |
| XSH.lio_listio.004 | FAIL | STD-AMBIG | Depends on a pending aio read of a pipe (Q-56); lio_listio returned EIO. Not disputed. |
| XSH.localeconv.001 | FAIL | SYSTEM | XBD 7.3.4.1: in the POSIX locale localeconv() grouping is ""; it was not empty. |
| XSH.localeconv.002 | FAIL | SYSTEM | XBD 7.3.3.1: in the POSIX locale mon_grouping is ""; it was not empty. |
| XSH.localeconv.004 | FAIL | SYSTEM | Same POSIX-locale value via a thread-local POSIX locale object; grouping not empty. |
| XSH.localeconv.006 | FAIL | SYSTEM | With only LC_NUMERIC changed the monetary members keep their POSIX values; mon_grouping not empty (the POSIX value itself is wrong, as in 002). |
| XSH.mblen.004 | FAIL | SYSTEM | mblen() acts as a reset then mbtowc(), so each call starts afresh; a complete two-byte UTF-8 character with n 2 returned -1 after earlier short calls. |
| XSH.mbrtoc16.001 | FAIL | SYSTEM | XBD 6.2: the POSIX locale has 256 single-byte characters (mbrtoc16 ERRORS: no EILSEQ there); mbrtoc32() of byte 128 returned (size_t)-2. |
| XSH.mbrtoc16.008 | FAIL | SYSTEM | mbrtoc16() RETURN VALUE: an invalid sequence gives (size_t)-1/EILSEQ; a lone continuation byte gave (size_t)-2. |
| XSH.mbrtoc16.009 | FAIL | SYSTEM | mbrtoc16() shall not change errno on success and mbrtoc32() behaves the same; mbrtoc32("A") set ENOENT. |
| XSH.mkdtemp.005 | FAIL | SYSTEM | mkdtemp() ERRORS (shall fail): EINVAL when the template does not end in "XXXXXX"; "dXXXXX" succeeded. |
| XSH.mkdtemp.012 | FAIL | SYSTEM | Same shall-fail EINVAL for mkstemp(); "fXXXXX" succeeded. |
| XSH.mktime.009 | FAIL | SYSTEM | mktime() (CX): when tm_year would not fit in int return -1 with EOVERFLOW; errno was not set. |
| XSH.nan.002 | FAIL | SYSTEM | nan("n-char-sequence") shall be equivalent to strtod("NAN(...)"); for "A_9" the two NaNs differ. |
| XSH.nan.003 | FAIL | SYSTEM | nan() of a non n-char sequence is equivalent to strtod("NAN"); nan("a b") differs. |
| XSH.nanosleep.003 | FAIL | STD-AMBIG | rmtp was 5 ms below 5 s minus the measured call time (the system counts more sleep than elapsed). The page gives rmtp as requested minus time slept but no accuracy; whether it may be off by the sleep or clock-tick granularity is not said. Not disputed. |
| XSH.nanosleep.004 | FAIL | STD-AMBIG | Same 5 ms shortfall in the remaining time with rmtp == rqtp. Not disputed. |
| XSH.newlocale.007 | INSPECT | STD-AMBIG | Already disputed (Q-135): whether a mask bit outside LC_ALL_MASK may be a valid category. |
| XSH.open.016 | FAIL | SYSTEM | open() O_NOFOLLOW: a path naming a symbolic link shall fail ELOOP; got EMLINK. |
| XSH.perror.007 | FAIL | SYSTEM | perror(): on error it shall set the error indicator of stderr and errno; the indicator was not set after the failed write. |
| XSH.poll.027 | FAIL | SYSTEM | poll(): every error read()/write() would report from the object's state is POLLERR; a pipe with no reader (write gives EPIPE) reported only POLLHUP. |
| XSH.poll.028 | FAIL | SYSTEM | Same, with events 0 (POLLERR is always reported); only POLLHUP. |
| XSH.popen.011 | INSPECT | STD-AMBIG | Already disputed (Q-64): whether a popen() stream starts byte-oriented or unoriented; fwide() returned 0. |
| XSH.posix_spawn.014 | FAIL | SYSTEM | posix_spawnp(): where exec would give ENOEXEC it shall fail rather than fall back to sh; the script was run by sh. |
| XSH.posix_spawn_file_actions_addclose.013 | FAIL | SYSTEM | addopen() ERRORS (shall fail): EBADF for fildes >= {OPEN_MAX}; addopen(OPEN_MAX) returned 0. |
| XSH.posix_spawn_file_actions_adddup2.007 | FAIL | SYSTEM | adddup2() ERRORS: EBADF for a descriptor >= {OPEN_MAX}; returned 0. |
| XSH.pow.012 | FAIL | SYSTEM | pow() overflow is a range error; FE_OVERFLOW shall be raised under MATH_ERREXCEPT; not raised for long double. |
| XSH.psiginfo.005 | FAIL | SYSTEM | psignal() on error shall set the error indicator of stderr; with fd 2 closed it was not set. |
| XSH.pthread_attr_getstacksize.004 | FAIL | SYSTEM | A thread with a {PTHREAD_STACK_MIN} stack running a routine that only returns must work (limits.h: minimum stack size); the process died of SIGSEGV. |
| XSH.pthread_cancel.005 | FAIL | SYSTEM | 2.9.5.3: acting on a cancellation first sets DISABLE/DEFERRED; the cleanup handler saw ENABLE. |
| XSH.pthread_getschedparam.002 | FAIL | SYSTEM | 2.9.4: a new process's single thread has the process's policy and parameters; thread priority 0, process 167. |
| XSH.pthread_getschedparam.006 | FAIL | SYSTEM | pthread_setschedparam() shall fail ENOTSUP for an unsupported policy (EINVAL/EPERM also accepted); policy INT_MAX succeeded. |
| XSH.pthread_kill.004 | FAIL | SYSTEM | pthread_kill(): a zombie (terminated, unjoined) thread is not an error; it returned ESRCH. |
| XSH.pthread_mutexattr_settype.003 | FAIL | SYSTEM | settype ERRORS (shall fail): EINVAL for an invalid type; -0x7ffffff0 was accepted. |
| XSH.pwrite.002 | FAIL | SYSTEM | write() page: pwrite() writes at the given position and leaves the offset alone whether or not O_APPEND is set; it appended. |
| XSH.quick_exit.005 | FAIL | SYSTEM | quick_exit(): all at_quick_exit() functions are called, a late one after its registrant; the late registration succeeded but was not called ("21"). |
| XSH.readdir.005 | FAIL | SYSTEM | readdir() shall mark the directory's atime for update when it is actually read; file reads update atime here, the directory's did not change. |
| XSH.readlink.006 | FAIL | SYSTEM | readlink() ERRORS: ENOENT for an empty path; got EINVAL. |
| XSH.readlink.019 | FAIL | SYSTEM | readlink() shall mark the link's last data access time for update; st_atim unchanged. |
| XSH.regcomp.014 | FAIL | SYSTEM | XBD 9.3.5: a hyphen first in the list may start a range, so "[--@]" is '-' to '@'; regcomp() gave REG_ERANGE. |
| XSH.regcomp.017 | FAIL | SYSTEM | XBD 9.3.6 gives this very example: "\(a\(b\)*\)*\2" does not match "abab"; it matched. |
| XSH.regcomp.027 | FAIL | SYSTEM | REG_ICASE follows XBD 4.1: "aa" matches \(a\)\1, so "aA" must match case-insensitively; it did not. |
| XSH.regcomp.033 | FAIL | SYSTEM | XBD 9.4.6 (Issue 8): '?' after a duplication symbol is the shortest-match modifier; ".*?c" was rejected (REG_BADRPT). |
| XSH.remquo.002 | FAIL | SYSTEM | remquo(): the stored quotient has the sign of x/y and magnitude congruent mod 2^n (n >= 3) to the quotient; for 9/4 (quotient 2) it stored 0. |
| XSH.rename.013 | FAIL | SYSTEM | rename() ERRORS: an existing non-directory old and a nonexistent new ending in '/' shall fail ENOTDIR; got ENOENT. |
| XSH.rename.014 | FAIL | SYSTEM | rename() DESCRIPTION: a new path ending in '/' that does not resolve to an existing directory shall fail; renaming d to "nd/" succeeded. |
| XSH.rmdir.005 | FAIL | SYSTEM | rmdir(): an open removed directory stays until closed (no entries, none creatable); fdopendir() of its descriptor failed ENOENT, which fdopendir() does not list. |
| XSH.sched_setscheduler.001 | FAIL | SYSTEM | sched_setscheduler() RETURN VALUE: the former policy; it returned 0 where the former policy was 1 (SCHED_FIFO). |
| XSH.select.009 | FAIL | SYSTEM | select(): regular files always select true for read, write and error; it returned 2 (no error condition). |
| XSH.select.012 | FAIL | SYSTEM | select() ERRORS: nfds greater than FD_SETSIZE shall fail EINVAL; FD_SETSIZE+1 returned 2. |
| XSH.setegid.005 | FAIL | SYSTEM | XBD 3.165: (gid_t)-1 is never a valid group ID; setegid() shall fail EINVAL for an invalid ID; it returned 0. |
| XSH.seteuid.005 | FAIL | SYSTEM | Same for seteuid((uid_t)-1) (XBD 3.408 User ID): returned 0. |
| XSH.setgid.004 | FAIL | SYSTEM | setgid() ERRORS: without privilege, a gid that is neither real nor saved gives EPERM; setgid(<effective>) returned 0. |
| XSH.setgid.005 | FAIL | SYSTEM | setgid((gid_t)-1) shall fail EINVAL (not a valid ID); returned 0. |
| XSH.setpgid.010 | FAIL | SYSTEM | setpgid() ERRORS: ESRCH when pid is neither the caller nor a child; a grandchild's ID succeeded. |
| XSH.setuid.004 | FAIL | SYSTEM | setuid() ERRORS: without privilege, a uid matching neither real nor saved ID gives EPERM; setuid(<effective>) returned 0. |
| XSH.setuid.005 | FAIL | SYSTEM | setuid((uid_t)-1) shall fail EINVAL; returned 0. |
| XSH.signal.005 | FAIL | TEST-BUG | The "above every signal" number came from sysconf(_SC_NSIG) = 128, which sigaction() accepts here, so signal() rightly accepted it; the fault is sysconf()'s (sysconf.015). Fixed: setup checks the number is no signal, else UNRESOLVED (c83ec9b9). |
| XSH.strftime.007 | FAIL | SYSTEM | strftime() (Issue 8): '+' and '0' flags with a width for %Y etc.; "%+4Y" was not recognised ("...19704Y"). |
| XSH.strftime.009 | FAIL | SYSTEM | strftime() %s is computed as mktime() does, in the TZ in force; the value ignored TZ=PCT3 (3 h off). |
| XSH.strftime.013 | FAIL | SYSTEM | strftime() RETURN VALUE (CX): on success errno is unchanged; it became EOVERFLOW. |
| XSH.strftime.014 | FAIL | SYSTEM | strftime() ERRORS (CX, shall fail): ERANGE when the result with NUL exceeds maxsize; errno was ENOENT. |
| XSH.strsignal.002 | FAIL | SYSTEM | strsignal() uses the same messages as psignal(); for SIGRTMIN they differ ("Unknown signal " vs "Unknown signal: 65"). |
| XSH.strtod.005 | FAIL | SYSTEM | strtod(): "nan(!)" has no valid n-char sequence, so the subject is "nan" and the end pointer is at 3; it was at 6. |
| XSH.sysconf.015 | FAIL | SYSTEM | sysconf(_SC_NSIG) is the highest supported signal number + 1; it returned 128, which sigaction() accepts as a signal. |
| XSH.system.014 | FAIL | SYSTEM | system(): the command starts with the caller's dispositions as after fork and exec (caught become default); the command survived its own SIGQUIT and exited 5. |
| XSH.tcgetpgrp.002 | FAIL | SYSTEM | tcgetpgrp() ERRORS: ENOTTY when the file is not the controlling terminal; a pipe returned 0. |
| XSH.tcsendbreak.001 | FAIL | SYSTEM | tcsendbreak(): on a non-serial terminal it either sends a break or returns without action; ENOTTY is for a non-terminal, but a pseudo-terminal subsidiary gave ENOTTY. |
| XSH.tcsendbreak.002 | FAIL | SYSTEM | Same with a nonzero duration: -1/ENOTTY on a pseudo-terminal. |
| XSH.tcsetpgrp.002 | FAIL | SYSTEM | tcsetpgrp() ERRORS: ENOTTY for a file that is not the controlling terminal; a pipe returned 0. |
| XSH.tgamma.010 | FAIL | SYSTEM | tgamma() overflow is a range error raising FE_OVERFLOW under MATH_ERREXCEPT; not raised for double. |
| XSH.truncate.007 | FAIL | SYSTEM | truncate() ERRORS: past the file size limit shall fail EFBIG; truncate(4097) under 4096 succeeded. |
| XSH.truncate.012 | FAIL | SYSTEM | truncate() ERRORS (shall fail): EINVAL for a FIFO or socket; a FIFO returned 0. |
| XSH.ungetc.010 | FAIL | SYSTEM | ungetc() and fflush(): a successful fflush() discards pushback (file grown, so both pages agree, Q-47); the pushed-back 'Z' was read. |
| XSH.ungetwc.006 | FAIL | TEST-BUG | Read a 3-byte file (descriptor at EOF, where the fflush() page does not require the discard: Q-29/Q-47) and demanded L'b' next (open, Q-30). Fixed to a 1 MiB grown file and "not L'Z'" (1130b45f); the system still reads back 'Z', so it should then FAIL as SYSTEM. |
| XSH.vfscanf.004 | FAIL | SYSTEM | vfscanf() as fscanf(): "%n$" numbered conversions; returned 0. |
| XSH.vfwprintf.006 | FAIL | SYSTEM | 2.5.2: vfwprintf() makes an unoriented stream wide-oriented; it did not. |
| XSH.vfwscanf.006 | FAIL | SYSTEM | vswscanf() with "%n$" numbered conversions (CX); returned 0. |
| XSH.waitid.015 | FAIL | SYSTEM | waitid() ERRORS: ECHILD only when no unwaited-for children exist; with a zombie child and WSTOPPED\|WNOHANG it must return 0 with si_pid 0, it gave ECHILD. |
| XSH.wcsftime.004 | FAIL | SYSTEM | wcsftime(): a result of 0 characters that fits is a success and is null-terminated; an empty format left the array unterminated. |
| XSH.wcsftime.006 | FAIL | SYSTEM | wcsftime() follows strftime()'s Issue 8 flags and width; "%010F" returned 3. |
| XSH.wcstod.005 | FAIL | SYSTEM | wcstod(): "nan(!)" converts only "nan"; end pointer at 6, not 3. |
| XSH.wordexp.016 | FAIL | SYSTEM | wordexp() RETURN VALUE: on an error other than WRDE_NOSPACE with WRDE_APPEND, we_wordc and we_wordv are not modified; WRDE_BADCHAR changed them. |
| XSH.write.022 | FAIL | SYSTEM | pwrite() writes at the offset given regardless of O_APPEND (write() page); it wrote at the end. |
