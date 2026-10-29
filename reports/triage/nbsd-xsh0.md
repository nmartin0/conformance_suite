# Second-system triage, group xsh0 (XSH, 2.11 to getgrgid)

Source: the full run on the second system (BSD-derived, as root) against the
latest Linux run. Every line below is judged against the standard alone.
Table columns: assertion, second-system code, Linux code, class, reason.

| ID | 2nd | Linux | class | reason |
|----|-----|-------|-------|--------|
| XSH.2.11-data-types.002 | FAIL | FAIL | SYSTEM | <dirent.h> must define reclen_t (XBD dirent.h, via XSH 2.11); it is undeclared |
| XSH.2.11-data-types.004 | FAIL | INSPECT | TEST-BUG | MINOF(T) pasted a name after expanding T, and a type that is a macro (pid_t) made it max___pid_t; now one function per type (min_##T) so no expansion is pasted; the unsigned-id_t FAIL that may remain is the disputed Q-200 |
| XSH.2.2-compilation-environment.002 | FAIL | PASS | SYSTEM | XBD dlfcn.h: the header must declare dladdr() with Dl_info_t; the system header uses an undeclared type Dl_info, so including it alone fails |
| XSH.2.2-compilation-environment.003 | FAIL | PASS | SYSTEM | XBD dlfcn.h: the header must declare dladdr() with Dl_info_t; the system header uses an undeclared type Dl_info, so including it alone fails |
| XSH.2.2-compilation-environment.004 | FAIL | PASS | SYSTEM | XBD dlfcn.h: the header must declare dladdr() with Dl_info_t; the system header uses an undeclared type Dl_info, so including it alone fails |
| XSH.2.3-error-numbers.004 | FAIL | PASS | SYSTEM | link() ERRORS [EMLINK] (XSH 2.3, link): pathconf reports {LINK_MAX} 32767 yet 32777 links were made |
| XSH.2.4.2-realtime-signals.002 | FAIL | PASS | SYSTEM | XSH 2.4.2/timer_create: a SIGEV_THREAD timer armed for 100 ms never called its function in 5 s (timer_create succeeded) |
| XSH.2.5.1-stream-handles.014 | FAIL | PASS | SYSTEM | XSH 2.5.1 with fflush(): the stream is not at EOF, so fflush() must set the offset to the position; fflush() on this input stream fails or leaves the read-ahead offset (same family as fflush.004) |
| XSH.2.5.2-stream-orientation.003 | FAIL | PASS | SYSTEM | XSH 2.5.2 (byte I/O functions orient the stream, fwrite included): fwide() still returns 0 after fwrite() |
| XSH.2.5.2-stream-orientation.004 | FAIL | PASS | SYSTEM | XSH 2.5.2: fwprintf() applied to a new stream must make it wide-oriented; fwide() gives -1 (byte) |
| XSH.2.5.2-stream-orientation.006 | FAIL | UNRESOLVED | SYSTEM | XSH 2.5.2: the encoding rule is fixed when the stream becomes wide-oriented; the read followed the later single-byte locale (195 read instead of the UTF-8 character) |
| XSH.2.5.2-stream-orientation.007 | UNRESOLVED | UNRESOLVED | SYSTEM | blocked by .006 (SYSTEM) |
| XSH.2.5.2-stream-orientation.010 | UNRESOLVED | UNRESOLVED | SYSTEM | blocked by XSH.fputwc.010 (SYSTEM) |
| XSH.2.8-realtime.005 | UNRESOLVED | PASS | SYSTEM | blocked by XSH.msync.004 (not in this list): msync(MS_INVALIDATE) over a locked page must fail EBUSY (msync ERRORS, shall); it returned 0 |
| XSH.2.9-threads.014 | FAIL | UNINITIATED | SYSTEM | XSH 2.9.5.2: waitid() is a cancellation point; with a request pending (014) or while blocked (024) it returned without the thread being canceled (the other 15 functions were canceled); on Linux the build failed for lack of ppoll in <poll.h> (system header gap), so no Linux comparison |
| XSH.2.9-threads.024 | FAIL | UNINITIATED | SYSTEM | XSH 2.9.5.2: waitid() is a cancellation point; with a request pending (014) or while blocked (024) it returned without the thread being canceled (the other 15 functions were canceled); on Linux the build failed for lack of ppoll in <poll.h> (system header gap), so no Linux comparison |
| XSH.2.9-threads.034 | FAIL | PASS | SYSTEM | same cause as 2.4.2-realtime-signals.002: the SIGEV_THREAD timer function is never called |
| XSH._Exit.018 | FAIL | PASS | SYSTEM | _Exit/mq_close: the notification registration made through the exiting child must go with its descriptor; the parent still gets EBUSY for 10 s |
| XSH.accept.007 | FAIL | PASS | SYSTEM | accept(): the address_len output is the full length of the address (16) even when the address is truncated; it was left at the 4 supplied |
| XSH.access.018 | FAIL | UNRESOLVED | SYSTEM | open()/faccessat() family DESCRIPTION: with an O_SEARCH descriptor no search check is made; the *at call is refused (EACCES) after the directory lost search permission |
| XSH.aio_fsync.003 | FAIL | PASS | SYSTEM | aio_fsync() ERRORS [EBADF] (shall): descriptor -1 was accepted (returned 0) |
| XSH.aio_read.006 | FAIL | PASS | SYSTEM | aio_read/aio_write: a negative aio_offset is an invalid offset, EINVAL; the request ended with EFBIG, which these pages do not list |
| XSH.aio_suspend.001 | FAIL | PASS | SYSTEM | aio_suspend() waits until the timeout interval (CLOCK_MONOTONIC) has passed; it returned after 291 ms of 300 (the test starts its clock before the call, so the early return is real) |
| XSH.aio_suspend.002 | UNRESOLVED | PASS | ENV | 2.8.2 leaves the processing order of requests unspecified; the file read queued behind a blocked pipe read did not run in 5 s (it does when no pipe read is pending: .003); the test already reports UNRESOLVED, which is right; no change |
| XSH.aio_suspend.003 | FAIL | PASS | SYSTEM | aio_suspend(): a request already completed at the time of the call makes it return without suspending (0); it gave EAGAIN with a zero timeout |
| XSH.aio_write.005 | FAIL | PASS | SYSTEM | aio_read/aio_write: a negative aio_offset is an invalid offset, EINVAL; the request ended with EFBIG, which these pages do not list |
| XSH.aio_write.007 | UNRESOLVED | PASS | TEST-BUG | the wait counted 800 sleeps of 10 ms and a coarse timer made them last far more than the 10 s guard, so the guard ended the test as UNRESOLVED; the 8 s is now measured on the clock (on Linux still PASS); the SIGEV_THREAD notification itself looks unsupported here, which would now give FAIL (SYSTEM, as for the timers) |
| XSH.atexit.003 | FAIL | PASS | SYSTEM | atexit()/pthread_exit(): at the end of the last thread the process exits as if by exit(0), so the registered function must run; the child reported 0T and no H |
| XSH.atomic_fetch_add.006 | INSPECT | INSPECT | STD-AMBIG | already disputed, Q-156 (pointer arithmetic unit of atomic_fetch_add) |
| XSH.bind.004 | FAIL | PASS | SYSTEM | bind(): an AF_UNIX pathname that names an existing file, a symbolic link included, is in use; a dangling link was bound through |
| XSH.bind.018 | FAIL | FAIL | SYSTEM | bind() ERRORS: a null address gives [EDESTADDRREQ] or [EISDIR]; both systems give EFAULT |
| XSH.c16rtomb.002 | FAIL | PASS | TEST-BUG | c32rtomb() and c16rtomb() shared one mbstate_t object across the two functions; an object that one function has used is not guaranteed to be in the initial state of the other; it is now cleared before each call (PASS on Linux; if it still fails here the cause is the library) |
| XSH.calloc.002 | FAIL | PASS | TEST-BUG | demanded the greatest fundamental alignment for 4-byte blocks, but the text asks for the alignment of objects that fit "in the space allocated"; now a block shorter than that is checked for the largest power of two not above its size (Q-212) |
| XSH.catgets.003 | FAIL | PASS | SYSTEM | gencat/catgets: an octal escape \ddd in a message text stands for that character; the "A" of \101 was dropped |
| XSH.catopen.004 | FAIL | PASS | SYSTEM | catopen(): on failure errno is set to indicate the error (empty name: [ENOENT]); it returned (nl_catd)-1 with errno 0 |
| XSH.chmod.019 | FAIL | UNRESOLVED | SYSTEM | open()/faccessat() family DESCRIPTION: with an O_SEARCH descriptor no search check is made; the *at call is refused (EACCES) after the directory lost search permission |
| XSH.chown.004 | FAIL | PASS | SYSTEM | chown(): with owner/group not both -1 the file status change time is marked for update even if the IDs are unchanged; ctime never moved in 3 s |
| XSH.chown.008 | INSPECT | INSPECT | STD-AMBIG | already disputed, Q-25 |
| XSH.chown.021 | FAIL | UNRESOLVED | SYSTEM | open()/faccessat() family DESCRIPTION: with an O_SEARCH descriptor no search check is made; the *at call is refused (EACCES) after the directory lost search permission |
| XSH.clock_getres.004 | FAIL | PASS | TEST-BUG | a candidate ID that this system treats as the CPU-time clock of a nonexistent process gave EINVAL from clock_getres() but ESRCH from clock_gettime(); the text lets an implementation have additional clocks, so such a candidate is now skipped (as an accepted one is) and the next tried; mutants still all caught |
| XSH.clock_getres.009 | FAIL | PASS | SYSTEM | clock_getres.html: IDs from clock_getcpuclockid() shall be supported; clock_getres() rejects the ID (CPT claimed) |
| XSH.clock_nanosleep.007 | UNRESOLVED | PASS | TEST-BUG | blocked by clock_getres.004, fixed there |
| XSH.clock_nanosleep.008 | FAIL | PASS | SYSTEM | clock_nanosleep(): shall fail when the clock is the calling thread's CPU-time clock; it slept and returned 0 |
| XSH.close.007 | UNRESOLVED | UNRESOLVED | SYSTEM | blocked by a missing required name (F_OFD_GETLK/F_OFD_SETLK(W), POSIX_CLOSE_RESTART or F_GETOWN_EX/F_SETOWN_EX); UNRESOLVED is the right verdict |
| XSH.close.009 | UNRESOLVED | UNRESOLVED | SYSTEM | blocked by a missing required name (F_OFD_GETLK/F_OFD_SETLK(W), POSIX_CLOSE_RESTART or F_GETOWN_EX/F_SETOWN_EX); UNRESOLVED is the right verdict |
| XSH.close.010 | UNRESOLVED | UNRESOLVED | SYSTEM | blocked by a missing required name (F_OFD_GETLK/F_OFD_SETLK(W), POSIX_CLOSE_RESTART or F_GETOWN_EX/F_SETOWN_EX); UNRESOLVED is the right verdict |
| XSH.close.014 | FAIL | FAIL | SYSTEM | close(): an uncanceled outstanding aio operation completes as if close had not happened; the second write ended with EBADF |
| XSH.confstr.002 | FAIL | PASS | SYSTEM | confstr() returns the size needed including the terminating null (strlen+1); it returned 89 for an 87-byte string |
| XSH.confstr.006 | UNRESOLVED | UNRESOLVED | SYSTEM | a required name (_CS_V8_ENV, _PC_TIMESTAMP_RESOLUTION) is not defined by <unistd.h> |
| XSH.connect.020 | FAIL | PASS | SYSTEM | connect(): a datagram socket connected to an AF_UNSPEC address has its peer reset and the call succeeds; it failed (the old message printed a stale errno 0; now captured) |
| XSH.ctermid.004 | UNRESOLVED | PASS | ENV | how a session leader acquires a controlling terminal is implementation-defined (XBD 11.1.3); opening the subsidiary device did not make it the controlling terminal; UNRESOLVED is the designed verdict; no change |
| XSH.exec.010 | FAIL | UNRESOLVED | TEST-BUG | set FD_CLOFORK in the parent and then forked the child that calls exec(); the fork closed the descriptor; it is now set in the child just before execv (compiles; cannot run here, FD_CLOFORK is missing on Linux) |
| XSH.exec.019 | FAIL | PASS | SYSTEM | exec: pending signals are inherited by the new image; SIGUSR2 raised while blocked is no longer pending |
| XSH.exec.028 | FAIL | FAIL | SYSTEM | exec (TCT): the initial thread's CPU-time clock in the new image starts at zero; it carried on (2036 ms) |
| XSH.exec.034 | FAIL | PASS | SYSTEM | exec ERRORS [ENOENT] (shall) for an empty path; got EACCES |
| XSH.exec.056 | UNRESOLVED | UNRESOLVED | SYSTEM | blocked by a missing required name (F_OFD_GETLK/F_OFD_SETLK(W), POSIX_CLOSE_RESTART or F_GETOWN_EX/F_SETOWN_EX); UNRESOLVED is the right verdict |
| XSH.exit.010 | FAIL | PASS | SYSTEM | exit(): streams are closed as by fclose, so the offset of an active-handle input stream is set to its position; it was left at the read-ahead 4096 |
| XSH.fclose.003 | FAIL | FAIL | SYSTEM | fclose(): offset of the open file description set to the stream position for an input stream not at EOF; left at 4096 (grown file keeps it off EOF, Q-29) |
| XSH.fclose.010 | FAIL | FAIL | SYSTEM | fclose(): offset of the open file description set to the stream position for an input stream not at EOF; left at 4096 (grown file keeps it off EOF, Q-29) |
| XSH.fclose.011 | FAIL | FAIL | SYSTEM | fclose(): offset of the open file description set to the stream position for an input stream not at EOF; left at 4096 (grown file keeps it off EOF, Q-29) |
| XSH.fclose.012 | FAIL | FAIL | SYSTEM | fclose()/fflush() ERRORS [ENOSPC] (shall) for the fmemopen() buffer: EOF is returned or not, errno stays 0 |
| XSH.fcntl.016 | UNRESOLVED | UNRESOLVED | SYSTEM | blocked by a missing required name (F_OFD_GETLK/F_OFD_SETLK(W), POSIX_CLOSE_RESTART or F_GETOWN_EX/F_SETOWN_EX); UNRESOLVED is the right verdict |
| XSH.fcntl.036 | FAIL | PASS | SYSTEM | fcntl() F_UNLCK: an unlock whose last byte is the largest off_t inside an l_len 0 lock unlocks to the end; byte 200 stayed locked |
| XSH.fcntl.037 | UNRESOLVED | UNRESOLVED | SYSTEM | blocked by a missing required name (F_OFD_GETLK/F_OFD_SETLK(W), POSIX_CLOSE_RESTART or F_GETOWN_EX/F_SETOWN_EX); UNRESOLVED is the right verdict |
| XSH.fcntl.038 | UNRESOLVED | UNRESOLVED | SYSTEM | blocked by a missing required name (F_OFD_GETLK/F_OFD_SETLK(W), POSIX_CLOSE_RESTART or F_GETOWN_EX/F_SETOWN_EX); UNRESOLVED is the right verdict |
| XSH.fcntl.039 | UNRESOLVED | UNRESOLVED | SYSTEM | blocked by a missing required name (F_OFD_GETLK/F_OFD_SETLK(W), POSIX_CLOSE_RESTART or F_GETOWN_EX/F_SETOWN_EX); UNRESOLVED is the right verdict |
| XSH.fcntl.040 | UNRESOLVED | UNRESOLVED | SYSTEM | blocked by a missing required name (F_OFD_GETLK/F_OFD_SETLK(W), POSIX_CLOSE_RESTART or F_GETOWN_EX/F_SETOWN_EX); UNRESOLVED is the right verdict |
| XSH.fcntl.041 | UNRESOLVED | UNRESOLVED | SYSTEM | blocked by a missing required name (F_OFD_GETLK/F_OFD_SETLK(W), POSIX_CLOSE_RESTART or F_GETOWN_EX/F_SETOWN_EX); UNRESOLVED is the right verdict |
| XSH.fcntl.042 | UNRESOLVED | UNRESOLVED | SYSTEM | blocked by a missing required name (F_OFD_GETLK/F_OFD_SETLK(W), POSIX_CLOSE_RESTART or F_GETOWN_EX/F_SETOWN_EX); UNRESOLVED is the right verdict |
| XSH.fcntl.043 | UNRESOLVED | UNRESOLVED | SYSTEM | blocked by a missing required name (F_OFD_GETLK/F_OFD_SETLK(W), POSIX_CLOSE_RESTART or F_GETOWN_EX/F_SETOWN_EX); UNRESOLVED is the right verdict |
| XSH.fdopen.005 | FAIL | FAIL | SYSTEM | fdopen()/freopen() mode letter e: FD_CLOEXEC set (freopen: cleared for a mode without e) |
| XSH.fdopen.009 | FAIL | FAIL | SYSTEM | fopen/fdopen/fmemopen ERRORS [EMFILE] (shall): with {STREAM_MAX} streams open the call must fail; sysconf reports a bound that the library does not enforce |
| XSH.fegetexceptflag.005 | FAIL | PASS | SYSTEM | fesetexceptflag() sets the flags to the stored states, a clear flag included; the flag stayed set |
| XSH.fflush.004 | FAIL | PASS | SYSTEM | fflush() of an input stream returns 0 and sets the descriptor offset to the position when not at EOF; here it fails (004, 005, 015: return value; 016: offset stays 4096); the message shows errno only now |
| XSH.fflush.005 | FAIL | FAIL | SYSTEM | fflush() of an input stream returns 0 and sets the descriptor offset to the position when not at EOF; here it fails (004, 005, 015: return value; 016: offset stays 4096); the message shows errno only now |
| XSH.fflush.011 | INSPECT | INSPECT | STD-AMBIG | already disputed, Q-28 |
| XSH.fflush.013 | FAIL | FAIL | SYSTEM | fclose()/fflush() ERRORS [ENOSPC] (shall) for the fmemopen() buffer: EOF is returned or not, errno stays 0 |
| XSH.fflush.015 | FAIL | PASS | SYSTEM | fflush() of an input stream returns 0 and sets the descriptor offset to the position when not at EOF; here it fails (004, 005, 015: return value; 016: offset stays 4096); the message shows errno only now |
| XSH.fflush.016 | FAIL | PASS | SYSTEM | fflush() of an input stream returns 0 and sets the descriptor offset to the position when not at EOF; here it fails (004, 005, 015: return value; 016: offset stays 4096); the message shows errno only now |
| XSH.fgetpos.004 | FAIL | PASS | SYSTEM | fgetpos()/ftell() ERRORS [EBADF] (shall): the underlying descriptor is not valid; answered from the buffered position |
| XSH.fgets.006 | FAIL | FAIL | SYSTEM | fgets(): a read error (EAGAIN after "ab") gives a null pointer; it returned s |
| XSH.fileno.003 | FAIL | PASS | SYSTEM | fileno() of a stream with no file (fmemopen): -1 with errno [EBADF]; errno stayed 0 |
| XSH.fmemopen.007 | FAIL | PASS | SYSTEM | fmemopen DESCRIPTION: end position (max_size for r, tracked for w and a), the written null byte not counted in it and written only when the write advances the end; the library takes the first null as the end and writes nulls inside the data |
| XSH.fmemopen.008 | FAIL | PASS | SYSTEM | fmemopen DESCRIPTION: end position (max_size for r, tracked for w and a), the written null byte not counted in it and written only when the write advances the end; the library takes the first null as the end and writes nulls inside the data |
| XSH.fmemopen.011 | FAIL | PASS | SYSTEM | fmemopen DESCRIPTION: end position (max_size for r, tracked for w and a), the written null byte not counted in it and written only when the write advances the end; the library takes the first null as the end and writes nulls inside the data |
| XSH.fmemopen.014 | FAIL | PASS | SYSTEM | fmemopen DESCRIPTION: end position (max_size for r, tracked for w and a), the written null byte not counted in it and written only when the write advances the end; the library takes the first null as the end and writes nulls inside the data |
| XSH.fmemopen.015 | FAIL | PASS | SYSTEM | fmemopen DESCRIPTION: end position (max_size for r, tracked for w and a), the written null byte not counted in it and written only when the write advances the end; the library takes the first null as the end and writes nulls inside the data |
| XSH.fmemopen.016 | FAIL | PASS | SYSTEM | fmemopen DESCRIPTION: end position (max_size for r, tracked for w and a), the written null byte not counted in it and written only when the write advances the end; the library takes the first null as the end and writes nulls inside the data |
| XSH.fmemopen.020 | FAIL | PASS | SYSTEM | fmemopen DESCRIPTION: end position (max_size for r, tracked for w and a), the written null byte not counted in it and written only when the write advances the end; the library takes the first null as the end and writes nulls inside the data |
| XSH.fmemopen.021 | FAIL | FAIL | SYSTEM | fmemopen: the null byte goes at the new end only if it fits; the library keeps room for it and drops the 4th byte (fflush fails) |
| XSH.fmemopen.024 | FAIL | FAIL | SYSTEM | fopen/fdopen/fmemopen ERRORS [EMFILE] (shall): with {STREAM_MAX} streams open the call must fail; sysconf reports a bound that the library does not enforce |
| XSH.fnmatch.009 | FAIL | PASS | SYSTEM | fnmatch(): [[:class:]], collating symbol [[.a.]], a "[" that starts no bracket expression matches itself (XCU 2.14.1), and with FNM_PERIOD a leading period is not matched by [!x] (2.14.3); each gives the wrong answer |
| XSH.fnmatch.010 | FAIL | PASS | SYSTEM | fnmatch(): [[:class:]], collating symbol [[.a.]], a "[" that starts no bracket expression matches itself (XCU 2.14.1), and with FNM_PERIOD a leading period is not matched by [!x] (2.14.3); each gives the wrong answer |
| XSH.fnmatch.012 | FAIL | PASS | SYSTEM | fnmatch(): [[:class:]], collating symbol [[.a.]], a "[" that starts no bracket expression matches itself (XCU 2.14.1), and with FNM_PERIOD a leading period is not matched by [!x] (2.14.3); each gives the wrong answer |
| XSH.fnmatch.017 | FAIL | PASS | SYSTEM | fnmatch(): [[:class:]], collating symbol [[.a.]], a "[" that starts no bracket expression matches itself (XCU 2.14.1), and with FNM_PERIOD a leading period is not matched by [!x] (2.14.3); each gives the wrong answer |
| XSH.fopen.032 | FAIL | FAIL | SYSTEM | fopen/fdopen/fmemopen ERRORS [EMFILE] (shall): with {STREAM_MAX} streams open the call must fail; sysconf reports a bound that the library does not enforce |
| XSH.fpathconf.005 | FAIL | PASS | SYSTEM | XBD limits.h: a pathname-variable macro (NAME_MAX 511) defined in <limits.h> means the value does not vary, but pathconf says 255 |
| XSH.fpathconf.006 | UNRESOLVED | UNRESOLVED | SYSTEM | a required name (_CS_V8_ENV, _PC_TIMESTAMP_RESOLUTION) is not defined by <unistd.h> |
| XSH.fprintf.030 | FAIL | PASS | SYSTEM | fprintf %a: with FLT_RADIX a power of 2 the value is correctly rounded; 0x1.0800000001p+0 at one digit gave 0x1.0p+0 |
| XSH.fprintf.049 | FAIL | PASS | SYSTEM | dprintf(): a descriptor not open for writing gives [EBADF] (as fputc); EINVAL |
| XSH.fprintf.052 | FAIL | PASS | SYSTEM | snprintf() ERRORS [EOVERFLOW] (shall) for a result above INT_MAX; returned negative with errno 0 |
| XSH.fputc.007 | FAIL | PASS | SYSTEM | fputc()/fputwc() on a stream not open for writing: EOF/WEOF, EBADF and the error indicator set; the indicator is not set |
| XSH.fputwc.007 | FAIL | PASS | SYSTEM | fputc()/fputwc() on a stream not open for writing: EOF/WEOF, EBADF and the error indicator set; the indicator is not set |
| XSH.fputwc.010 | FAIL | FAIL | SYSTEM | fputwc()/fputws() [EILSEQ] (Issue 8): the error indicator is also set; it is not |
| XSH.fputws.006 | FAIL | FAIL | SYSTEM | fputwc()/fputws() [EILSEQ] (Issue 8): the error indicator is also set; it is not |
| XSH.freopen.008 | FAIL | FAIL | SYSTEM | fdopen()/freopen() mode letter e: FD_CLOEXEC set (freopen: cleared for a mode without e) |
| XSH.freopen.011 | FAIL | FAIL | SYSTEM | freopen(): the new descriptor is the lowest available (2.6); the old 6 is kept |
| XSH.fscanf.005 | FAIL | PASS | SYSTEM | fscanf family: %n$ conversions (base since Issue 7) are not supported (0 returned) |
| XSH.fscanf.013 | FAIL | PASS | SYSTEM | fscanf %d on "-x": the sign is consumed as the failed item and the next fgetc() is x; it gives back the "-" |
| XSH.fscanf.014 | FAIL | FAIL | SYSTEM | fscanf: the input item is the longest initial subsequence of a matching sequence ("100e", "0x"), which is not matching, so the directive fails with 0; the library backs up and returns 1 |
| XSH.fscanf.015 | FAIL | FAIL | SYSTEM | fscanf: the input item is the longest initial subsequence of a matching sequence ("100e", "0x"), which is not matching, so the directive fails with 0; the library backs up and returns 1 |
| XSH.fscanf.037 | FAIL | PASS | SYSTEM | fscanf family: the m assignment-allocation modifier (ms, mc, m[, mls, ml[) is not supported (0 returned) |
| XSH.fscanf.038 | FAIL | PASS | SYSTEM | fscanf family: the m assignment-allocation modifier (ms, mc, m[, mls, ml[) is not supported (0 returned) |
| XSH.fseek.004 | UNRESOLVED | INSPECT | SYSTEM | disputed (Q-40); the UNRESOLVED comes from fflush() failing on an input stream (see fflush.004) |
| XSH.fseek.014 | INSPECT | PASS | STD-AMBIG | already disputed, Q-35 |
| XSH.fseek.016 | INSPECT | INSPECT | STD-AMBIG | already disputed, Q-35 |
| XSH.fstatat.013 | FAIL | UNRESOLVED | SYSTEM | open()/faccessat() family DESCRIPTION: with an O_SEARCH descriptor no search check is made; the *at call is refused (EACCES) after the directory lost search permission |
| XSH.ftell.006 | FAIL | PASS | SYSTEM | fgetpos()/ftell() ERRORS [EBADF] (shall): the underlying descriptor is not valid; answered from the buffered position |
| XSH.ftruncate.008 | FAIL | PASS | SYSTEM | ftruncate() ERRORS [EFBIG] (shall): length beyond the process file size limit; it succeeded |
| XSH.ftruncate.010 | INSPECT | PASS | STD-AMBIG | already disputed, SI-56 (pipe: EOPNOTSUPP here) |
| XSH.ftruncate.011 | UNRESOLVED | PASS | ENV | EFBIG creating a 2^32+5 byte file although {FILESIZEBITS} allows it: the guest volume cannot hold it; UNRESOLVED is the designed verdict; no change |
| XSH.futimens.009 | FAIL | PASS | SYSTEM | utimensat()/futimens(): a tv_nsec outside 0..999999999 (not UTIME_NOW/OMIT) gives [EINVAL]; case 0 was accepted (the message printed a stale errno; now captured) |
| XSH.futimens.025 | FAIL | UNRESOLVED | SYSTEM | open()/faccessat() family DESCRIPTION: with an O_SEARCH descriptor no search check is made; the *at call is refused (EACCES) after the directory lost search permission |
| XSH.fwprintf.011 | FAIL | PASS | SYSTEM | XSH 2.5.2: fwprintf() makes a new stream wide-oriented (same as orientation.004) |
| XSH.fwprintf.014 | FAIL | FAIL | SYSTEM | swprintf() with n 0: [EOVERFLOW] (n or more wide characters requested); EINVAL |
| XSH.fwscanf.010 | FAIL | PASS | SYSTEM | swscanf(): a literal mismatch is a matching failure (0), not input failure (-1) |
| XSH.fwscanf.017 | FAIL | PASS | SYSTEM | fscanf family: the m assignment-allocation modifier (ms, mc, m[, mls, ml[) is not supported (0 returned) |
| XSH.fwscanf.020 | FAIL | PASS | SYSTEM | fscanf family: %n$ conversions (base since Issue 7) are not supported (0 returned) |
| XSH.getgrgid.005 | FAIL | PASS | SYSTEM | getgrgid_r(): a buffer too small for the entry gives [ERANGE] and a null *result; returned 0 (getgrgid.006, getgrnam.005/.006 fail alike) |

## TEST-BUG changes and re-runs (Linux, `--only ID`)

- XSH.2.11-data-types.004 (`t/004-id-range.c`): the minimum of each type is now a function made by the same macro as the maximum, so no name is pasted after a type that is itself a macro. Re-run: INSPECT, as before (the disputed Q-200 reading; id_t unsigned here), no build failure. The build error on the second system is gone by construction.
- XSH.c16rtomb.002 (`t/002-portable.c`): `memset(&st, 0, sizeof st)` before each of the c32rtomb() and c16rtomb() calls. Re-run: PASS. Not proven to be the cause on the second system (c16rtomb(32) failed there right after c32rtomb(32) with the same object); if it still fails the library is at fault.
- XSH.calloc.002 (`t/002-array.c`, `calloc.asr` note, Q-212 added): alignment of a block shorter than `_Alignof(max_align_t)` is checked at the largest power of two not above its size. Re-run: PASS.
- XSH.clock_getres.004 (`t/004-unknown-clock.c`, `.asr` note): a candidate ID reported with ESRCH by any of the calls (the CPU-time clock of no process) is skipped like a known clock. Re-run: PASS; `ci/mutate.sh`: 4 mutants caught, 2 variants tolerated.
- XSH.clock_nanosleep.007: only blocked by clock_getres.004; no change of its own.
- XSH.exec.010 (`t/010-clofork-kept.c`): FD_CLOFORK is set in the child just before execv(); set in the parent it was closed by the fork that made the child. Re-run on Linux: UNRESOLVED (FD_CLOFORK missing here, blocked by XBD.fcntl.h.002); the changed code was compiled with `-DFD_CLOFORK=2 -Werror` and is clean, but its run on a system that has FD_CLOFORK is still to be seen.
- XSH.aio_write.007 (`t/007-sigev-thread.c`): the 8 s wait is measured with `now_ns()` instead of counting 800 sleeps of 10 ms (a coarse timer made them outlast the 10 s guard, which ended the test as UNRESOLVED instead of a verdict). Re-run: PASS; mutants `value_lost` and `no_notify` caught.

Diagnostics only (verdicts unchanged): in XSH.fflush.004/.005/.015/.016, XSH.connect.020, XSH.clock_getres.009 and XSH.futimens.009 the errno shown in the message was read in the same call as the function under test (argument order is unspecified), so it printed a stale 0 on the second system; it is now saved right after the call. Re-runs: PASS (fflush.005 still FAIL on Linux: offset 4095, as before). `ci/mutate.sh` on these: every fault caught, no variant rejected (fflush.005 unchecked, its baseline fails on Linux).

Lint: 6373 problems before and after (all "declares no MUTANT").

## Counts

SYSTEM 113, TEST-BUG 7, ENV 3, STD-AMBIG 6 (all six already disputed with an entry: Q-156, Q-25, Q-28, Q-35 twice, SI-56), UNKNOWN 0; 129 lines. One new index row, Q-212 (calloc alignment), no change of class.

## Patterns

- Missing required names in headers (O_SEARCH honoured only as a name, F_OFD_*, POSIX_CLOSE_RESTART, F_GETOWN_EX, _CS_V8_ENV, _PC_TIMESTAMP_RESOLUTION, reclen_t, Dl_info_t): the dependent assertions are UNRESOLVED or FAIL by design; 24 lines.
- Input streams: fflush(), fclose() and exit() do not set the descriptor offset (both systems for fclose; fflush() even fails here); the tests already avoid the EOF reading (Q-29).
- SIGEV_THREAD does not work on the second system for timers or aio; its tests are FAIL there.
- fmemopen(), scanf (%n$, %m, longest-initial-subsequence rule), fnmatch classes and the wide-stream orientation rules are the library areas with the most deviations.
- Evaluation order: many tests print `pc_errname(errno)` as an argument beside the call under test; on this compiler it shows a stale 0. The core `pc_check()` cannot fix it; each test would have to save errno first (done for the lines above only).
- Timing loops that count sleeps rather than read the clock are wrong on a coarse timer (aio_write.007 fixed; others may exist).
