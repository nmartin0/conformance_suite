# PureDarwin 26.5: the 241 miscellaneous failures, read against the standard

Copyright (c) 2026 Nicholas Martin. All rights reserved.

Companion to `2026-11-03-puredarwin-first-pass.md`. Four read-only readers each took about 60
of the FAIL results that did not fall into an obvious cause group and judged each one from the
standard's text alone (test source, assertion comment, failure note; no other test suite, no
system documentation). Nothing was run for this reading, so every verdict rests on the test
code and the standard, not on observing the guest. Verdicts: REAL (the system deviates), TESTBUG
(the test asks for more than the standard), ENV (compiler, headers, emulator, file system,
userland), UNSURE.

Totals: ENV 33, REAL 196, UNSURE 12 of 241.

Not TESTBUG: none of the 241 was judged a test bug. Several smaller test-quality points came out
of the reading; they are listed under "Test quality points" and none changes a verdict.

Caveats added after the readers finished:
- `XSH.wordexp.*` (19 results): the readers called these REAL ("shell helper not working"). The
  most likely cause is that the minimal image lacks the helper program the system's `wordexp`
  runs; that is an incomplete image rather than a library defect, but the system still fails the
  function. Probe on the guest before filing.
- `XSH.abort.005` and `XSH.raise.002` use `_Thread_local`, which the compiler accepts but compiles
  to code that crashes. The driver now probes this and reports such tests as a toolchain gap
  (commit e5cb03e0); both are ENV.
- `XSH.clock_getres.003` (outside this list): `clock_getres(clock, NULL)` crashes the process, so
  the library dereferences a null `res`. REAL.
- The ext4 driver is the guest's own (the root file system is ext4 too), but the file-system
  results (rewinddir, rename dot rules, append atomicity, size limit on appends, truncate and mmap
  SIGBUS) would be settled by a run on another file system.

## Verdict table

| ID | Verdict | Confidence | Requirement or reason |
|---|---|---|---|
| XBD.10.2-output-devices-and-terminal-types.001 | ENV | low | The test is sound: od page, -t a, table of named characters, gives soh for byte 001. An empty result for the very first byte means od (or the shell printf) did not run as expected. Likely the toybox od/tr/printf. Settle: run `printf '\001' \| od -An -t a` on the guest and read stderr. |
| XBD.12.2-utility-syntax-guidelines.011 | UNSURE | low | The test is sound: XBD 7.2/12.2 let an operand or option-argument hold any byte in the POSIX locale, and the grep page defines -e pattern matching. cat of the odd file name worked, grep -e did not. Which grep is in use (Apple BSD grep or toybox) and how it treats byte 0351 in the C locale is unknown. Settle: `grep --version`, and retry with LC_ALL=C. If Apple grep fails on an 8-bit byte in the C locale it is REAL. |
| XBD.4.11-file-system-cache.002 | UNSURE | medium | 2.9.7 (read/write atomic with each other on file descriptors) plus XBD 4.11 (cache obeys the read-write serializability rule of write()) clearly forbid a mixed read. The test is sound. The cause (XNU does not serialize reads against writes, or the guest ext4 driver) is not visible. Leans REAL. Settle: rerun on a native volume (HFS+/APFS image or ram disk). |
| XBD.5-file-format-notation.013 | ENV | medium | The test is sound (XBD 5 defines %a/%A, awk printf follows that notation; the digit before the point and the exponent for 0 are as the regex allows). The empty output points to no awk or a crashing awk (toybox has no awk). Settle: `command -v awk; awk 'BEGIN{printf "%a\n",0}'`. |
| XBD.7.4-locale-grammar.006 | ENV | medium | Status 127 means localedef was not found. The guest claims _SC_2_LOCALEDEF yet ships no localedef (trimmed PureDarwin userland). Strictly a claim without the utility, but the cause is the stripped userland. See "Test bugs" for a message fix. |
| XBD.inttypes.h.004 | UNSURE | low | The test is valid C11 (L"%" PRId8 relies on the mixed narrow/wide literal concatenation that C11 defines). PRId8 vs %jd must print the same text on a conforming system. Candidates: TinyCC mishandling the mixed literal (ENV), or Apple's wide printf with hh (REAL). Settle: print g and w with a 3-line program built with another compiler. |
| XBD.stdlib.h.011 | ENV | medium | stdlib.h page and setstate page give `char *setstate(char *state)`. Apple's header uses a const-qualified parameter (the failing line). Counts as "Apple header form" per the brief; strictly it is a header that differs from the synopsis, though harmless to callers. |
| XSH.2.10-sockets.019 | REAL | medium | XSH 2.10 (IPv4-mapped addresses): when an AF_INET6 socket receives a UDP packet from an IPv4 node, recvfrom() shall return the peer as a sockaddr_in6 in mapped form; binding to a mapped address is also allowed. The datagram arrived (an earlier NEED passed) but the returned address was not AF_INET6/full length. BSD-derived stacks tend to turn such a socket into an IPv4 one on bind. |
| XSH.2.4.3-signal-actions.021 | UNSURE | medium | mmap page: a reference to whole pages after the end of the object shall raise SIGBUS. The test is sound (2-page mapping of a 1-page file). Reading did not fault. Could be the guest ext4 pager zero-filling (ENV) or XNU (REAL). Settle: repeat on a native volume. |
| XSH.2.9-threads.016 | REAL | medium | 2.9.5.2: tcdrain is a cancellation point; with a request pending and cancelability enabled the request is acted on before the function returns. It blocked instead. pccp.h harness is sound (cancel pending, enable, call). |
| XSH.2.9-threads.017 | REAL | medium | 2.9.5.2: fsync is a cancellation point. It returned with the request pending. |
| XSH.2.9-threads.019 | REAL | medium | 2.9.5.2: msync is a cancellation point. It returned. |
| XSH.2.9-threads.021 | REAL | medium | 2.9.5.2: aio_suspend is a cancellation point. It returned (the null-entry list does not excuse it: the rule is "before the function returns"). |
| XSH.2.9-threads.022 | REAL | medium | 2.9.5.2: msgsnd and msgrcv are cancellation points. Both returned (IPC_NOWAIT does not change the rule). |
| XSH.2.9-threads.023 | REAL | medium | 2.9.5.2: lockf is a cancellation point when the function argument is F_LOCK (the footnote). It returned. |
| XSH.2.9-threads.028 | REAL | medium | 2.9.5.2: a thread suspended in write() is woken and the request acted on; the effects are those of a signal interruption (partial count). The thread never ran its cleanup handler, so it was not canceled after a partial transfer. |
| XSH.2.9-threads.031 | UNSURE | medium | 2.9.7 lists pread and pwrite as atomic with each other; mixed blocks are a violation. Same open question as XBD.4.11.002: the system or the ext4 driver. Leans REAL. |
| XSH.2.9-threads.033 | UNSURE | medium | 2.9.7 lists symlink, unlink, readlink and lstat as atomic with each other. Rounds 1 and 2 passed; round 3 saw a readlink/lstat error other than ENOENT, i.e. a half-made symlink. Leans REAL; file system suspect. Settle: native volume. |
| XSH.fopen.013 | REAL | medium | fopen page (mode string): characters after the first are accepted in any order, and x with w or a adds O_EXCL, so "wex" on an existing file must fail with EEXIST. It succeeded. |
| XSH.fscanf.013 | REAL | high | fscanf page (input item): the item is the longest initial subsequence of a matching sequence and the first byte after it stays unread. "-" is the item, 'x' must be next. The system pushed the sign back (next byte was '-'). |
| XSH.fscanf.014 | REAL | high | Same rule: the item "100e" is a prefix of a matching sequence, it is not itself a number, so the directive is a matching failure (return 0). The system backtracked to "100". |
| XSH.fscanf.015 | REAL | high | Same rule: the item "0x" is not a matching sequence for %x/%i; result must be 0, %n not reached. The system returned 1. |
| XSH.fscanf.037 | REAL | high | fscanf page: the m assignment-allocation character is a base POSIX (CX) feature for s, c and [ ; `%ms %3mc %m[xyz]` is valid (width precedes m). The system matched 0 items. |
| XSH.fscanf.038 | REAL | high | Same, with the l length modifier (%mls, %ml[..]) allocating wide strings. |
| XSH.fscanf.043 | REAL | medium | fscanf page, RETURN VALUE: EOF only if input ends before the first conversion has completed. The suppressed %*d completed, so the failing %d gives 0, not EOF. Medium only because one could argue "conversion" means "assignment"; the text says conversion. |
| XSH.getpriority.006 | REAL | high | getpriority page: the returned value is in -{NZERO} to {NZERO}-1 and setpriority clamps to the supported maximum. The system reports 20 (= NZERO), outside the range. |
| XSH.getpwuid.002 | REAL | medium | getpwuid page: if the entry is not found, errno shall not be changed. errno became ENOENT. Caveat: if the lookup truly failed (no directory service on PureDarwin) an error return with errno would be allowed, and the test cannot tell. Reading is that ENOENT leaked from an internal probe. |
| XSH.lio_listio.001 | REAL | medium | XSH 2.8 async I/O: the offset applies only to seekable devices and is ignored otherwise, so an aio_read on a pipe with aio_offset 0 is valid. lio_listio may fail only with EAGAIN, EINVAL, EINTR, EIO (ERRORS); ESPIPE is not one. |
| XSH.lio_listio.004 | REAL | medium | Same cause (pipe read in the list; ESPIPE). |
| XSH.lio_listio.006 | REAL | high | lio_listio RATIONALE: a bad descriptor for one request is reported as EIO from lio_listio and EBADF in that aiocb's status. The system returned EBADF itself, which is not a listed error. |
| XSH.openlog.001 | REAL | high | closelog/openlog/setlogmask page: the default mask allows all priorities, and setlogmask(0) leaves the mask alone and returns it. The query returned 0. |
| XSH.openlog.003 | REAL | high | setlogmask shall return the previous mask. The first call returned 0x3f while the query said 0x0 (and the default should be all priorities). The two answers are inconsistent with each other and with the page. |
| XSH.openlog.006 | REAL | high | A syslog() rejected by the mask must leave the mask alone, and setlogmask(0) must report it (here LOG_MASK(LOG_EMERG)). It reported 0xff. |
| XSH.psiginfo.005 | REAL | medium | psiginfo/psignal page: on a write error the error indicator of stderr shall be set and errno set. With fd 2 closed the indicator was not set, so the output probably bypasses the stream. |
| XSH.pthread_join.006 | REAL | medium | 2.9.5.2: pthread_join is a cancellation point; a thread blocked in it is woken and canceled. It stayed blocked 20 s. The harness is sound. |
| XSH.realpath.001 | REAL | medium | realpath page (2024 text): the result resolves to the same directory entry, not merely the same file. d/g is a hard link to d/f; the system returned d/f. |
| XSH.realpath.005 | REAL | high | realpath ERRORS / XBD 4 pathname resolution: a trailing slash after a non-directory gives ENOTDIR. The system returned the path of f. |
| XSH.rewinddir.001 | ENV | low | rewinddir page: the stream is repositioned to the start. Reading again gave nothing at all, which a native file system would not do. Suspect the ext4 driver's directory seek. Settle: run on another volume. If it still fails, REAL. |
| XSH.rewinddir.002 | ENV | low | Same cause as rewinddir.001. |
| XSH.sem_init.001 | REAL | high | sem_init page (unmarked, so mandatory): shall return 0 on success. The system returns ENOSYS. ENOSYS is not an allowed error. |
| XSH.sem_init.005 | REAL | high | Value above {SEM_VALUE_MAX} must give EINVAL. Same missing implementation (ENOSYS). |
| XSH.sem_init.006 | REAL | high | Value {SEM_VALUE_MAX} must be accepted. Same ENOSYS. |
| XSH.strftime.007 | REAL | high | strftime page (CX): the 0 and + flags with a minimum width are required for %C %F %G %Y. The system treated `%+` as a date format and printed "4Y" literally. |
| XSH.strftime.014 | REAL | high | strftime ERRORS: shall fail with ERANGE when the result plus NUL exceeds maxsize. It returned 0 but errno was ENOENT (a stale value, probably from a missing time zone file). |
| XSH.system.006 | REAL | medium | system page: the command is run as `sh -c -- command`, so a leading - or + is a command. The status 1 looks like the shell took "-pcdash" as options, i.e. "--" is missing. Depends on the guest shell's message, which is not visible. |
| XSH.system.014 | REAL | medium | system page: system() ignores SIGINT/SIGQUIT only in the caller while it waits; the command behaves as after fork+exec, so a caught signal is back to default. SIGINT behaved, SIGQUIT stayed ignored in the command (status exit 5). |
| XSH.tcsendbreak.005 | REAL | medium | tcsendbreak page: a background process group on its controlling terminal gets SIGTTOU. The call returned 0 and the caller was not stopped. The test sets up the session correctly. |
| XSH.tcsendbreak.007 | REAL | medium | tcsendbreak ERRORS: EIO from an orphaned group unless SIGTTOU is blocked or ignored. It returned 0. |
| XSH.umask.003 | REAL | medium | umask page [XSI]: the S_ISVTX bit is ignored in cmask and is clear in the returned value. It was returned set (01077). Applies if the system claims XSI, which Darwin does. |
| XSH.umask.005 | REAL | medium | umask page [XSI]: S_ISVTX in cmask must not be applied. mkdir with 01777 lost the sticky bit (0755). Same XSI condition. |
| XSH.waitid.008 | REAL | high | waitid page: with WNOHANG and no status, si_signo and si_pid shall be set to zero. They were left untouched (-1 from the test's fill). |
| XSH.waitid.014 | REAL | high | Same defect. si_pid -1 is the untouched fill value, so the stop was not reported again; the required zeroing is missing. The message "reported again" is misleading (see Test bugs). |
| XSH.waitid.015 | REAL | high | waitid page: WEXITED reports only terminated children. A stopped child was reported (si_pid set, si_signo 20 = SIGCHLD). |
| XSH.wordexp.001 | ENV | medium | wordexp returns 6 (WRDE_SYNTAX in Apple's header) even for "a b  c". The pass of the BADCHAR/NOCMD error tests (008 009 011 012, not in this list) shows only the pre-checks work. Pattern: the shell helper that wordexp starts does not run correctly (toybox or reduced sh). |
| XSH.wordexp.002 | ENV | medium | Same shell-helper cause. |
| XSH.wordexp.003 | ENV | medium | Same. |
| XSH.wordexp.004 | ENV | medium | Same. |
| XSH.wordexp.005 | ENV | medium | Same. |
| XSH.wordexp.006 | ENV | medium | Same. |
| XSH.wordexp.007 | ENV | medium | Same. |
| XSH.wordexp.010 | ENV | medium | Same. |
| XSH.wordexp.013 | ENV | medium | Same (WRDE_NOCMD, "a b"). |
| XSH.wordexp.014 | ENV | medium | Returned WRDE_BADVAL (2) for a variable that is set: the helper shell failed under its -u option, consistent with the same cause. |
| XSH.wordexp.015 | ENV | medium | Same. |
| XSH.wordexp.016 | ENV | medium | Same. |
| XSH.wordexp.017 | ENV | medium | Same. |
| XSH.wordexp.018 | ENV | medium | Same. |
| XSH.wordexp.019 | ENV | medium | Same. |
| XSH.wordexp.020 | ENV | medium | Same. |
| XSH.wordexp.021 | ENV | medium | Same. |
| XSH.wordexp.022 | ENV | medium | Same. |
| XSH.wordexp.023 | ENV | medium | Same. |
| XSH.wordexp.024 | ENV | medium | Same. |
| XBD.11.1-interface-characteristics.002 | REAL | medium | XBD 11.1.3: a process that is not a session leader opening a terminal must not make it its controlling terminal. Here tcgetpgrp() in that process returned a foreground group (717, apparently the leader's), which only happens if the open associated the terminal with the session. tcgetpgrp page: ENOTTY if the file is not the caller's controlling terminal. Oddity: the call returned a value and also left errno ENOTTY. |
| XBD.11.1-interface-characteristics.004 | REAL | medium | XBD 11.1.3: a terminal is the controlling terminal of at most one session. A process in a second session got a valid foreground group (740, evidently the first session's) instead of -1/ENOTTY. Either the second session acquired it or tcgetpgrp() does not check "is the caller's controlling terminal"; both break the tcgetpgrp page (ENOTTY shall). Cannot tell which from the note. |
| XBD.11.1-interface-characteristics.005 | REAL | medium | XBD 11.1.3: setsid() relinquishes the controlling terminal. After setsid() in a child, tcgetpgrp() on the old descriptor still returned the old foreground group (750) rather than -1/ENOTTY (tcgetpgrp page, ENOTTY shall). Same root cause as .004: tcgetpgrp() on a terminal fd does not enforce "controlling terminal of the caller". |
| XBD.11.1-interface-characteristics.011 | REAL | low | The foreground read, write and signal checks all passed. Only the master read-back of "xyz" was empty. The test reads the master after the slave side has been closed (session ended). XBD 11.1.11: on last close, pending output shall be sent to the device. So loss of the data is the same defect as .045. The assertion under test (11.1.4, foreground access) is not what failed. See Patterns. |
| XBD.11.1-interface-characteristics.013 | REAL | low | Same as .011. The write returned 1 and no SIGTTOU was seen (11.1.4 satisfied); only the master data was missing after the writer's session ended (11.1.11). |
| XBD.11.1-interface-characteristics.014 | REAL | low | Same as .011/.013 for the TOSTOP-set, SIGTTOU blocked or ignored variants (11.1.4 passes, 11.1.11 data lost). |
| XBD.11.1-interface-characteristics.016 | REAL | low | XBD 11.1.4: tcsetattr, tcdrain, tcflow, tcflush, tcsendbreak from a background group are treated like writes with TOSTOP set, whatever TOSTOP is, so SIGTTOU (default: stop) must be sent. One of the five returned 0 with no stop. The note does not say which (the test prints it only as a pc_note). Test conditions are sound: S has the controlling terminal (checked by tcgetpgrp), the caller is in its own non-orphaned group, SIGTTOU is default. Confidence limited by not knowing which function. |
| XBD.11.1-interface-characteristics.039 | REAL | medium | XBD 11.1.9 (IEXTEN text, marked XSI): with IEXTEN set, ERASE, KILL, EOF can be escaped by a preceding backslash and then do nothing special. Got "a b NL" (61 62 0a): the ERASE character and the backslash both vanished, so ERASE acted on the backslash. Required only if the system claims XSI; PureDarwin presumably does. |
| XBD.11.1-interface-characteristics.045 | REAL | medium | XBD 11.1.11: the last close sends pending output and discards pending input. "xyz" was written to the slave, 150 ms passed, slave closed, master read returned nothing. The output was lost on close. |
| XSH.abort.005 | ENV | low | Child died by signal 11 (status 0xb), not by exit 7 from the handler. The only unusual construct is the `_Thread_local int is_b` read in the SIGABRT handler. TinyCC on Mach-O very probably emits no working TLS access (compiler issue), which segfaults in the handler. abort page: SIGABRT goes to the calling thread, so the test's logic is right. Would settle it: replace the TLS flag by pthread_equal(pthread_self(), saved_tid) in the handler, or look at the fault address. If it still dies with SIGSEGV, change to REAL. |
| XSH.aio_fsync.004 | REAL | high | aio_fsync page, ERRORS: shall fail with EINVAL for an op other than O_DSYNC or O_SYNC. op 0 returned 0. |
| XSH.aio_write.007 | REAL | medium | signal.h/XSH 2.4: SIGEV_THREAD is a required notification type (function run in a new detached thread with sigev_value). aio_write page lists EINVAL only for bad offset, priority or nbytes, none applies (zeroed aiocb, 2 bytes, offset 0). EINVAL here suggests the aio notification code rejects SIGEV_THREAD. |
| XSH.catopen.004 | REAL | low | catopen page: ENOENT is named for "name is an empty string". The call failed with EINVAL. Weak because the whole error list is "may fail": the system may also have chosen not to fail. When it does fail for this condition, the listed code is the one the standard gives. |
| XSH.catopen.010 | REAL | medium | catopen page (XSI NLSPATH): files that are not valid catalogs are ignored, and if only such files exist the error is ENOENT. Got errno 79, which the test cannot name (pc_errname printed E?79); that is the Darwin number for "inappropriate file type or format". Not the listed code. Cosmetic test gap: pc_errname does not know it. |
| XSH.fchdir.004 | REAL | high | fchdir page: ENOTDIR (shall) when the descriptor does not refer to a directory. A pipe gave EINVAL. The regular-file case before it passed. |
| XSH.fflush.005 | REAL | medium | fflush page: for a seekable input stream not already at EOF, the descriptor offset becomes the stream position and pushed-back characters are discarded. File is 1 MiB, stream read 2 bytes and pushed 1 back (position 1). Offset after fflush was 4095, not 1. The `fflush()==0` and discard-'x' checks passed; only the offset is wrong. |
| XSH.fflush.013 | REAL | medium | fflush page: on failure return EOF and set errno; ENOSPC (shall fail) when there is no room in the fmemopen buffer. The 4-byte fmemopen buffer, 10 bytes buffered via setvbuf, fflush returned EOF (that check passed) but errno stayed 0. |
| XSH.fmemopen.003 | REAL | high | fmemopen page: a stream opened read-only must never modify the buffer. fputc on the "r" stream wrote 'Q' and a null into the buffer ("aQ\0def"). (Writing to an "r" stream is allowed to fail but must not touch the buffer.) |
| XSH.fmemopen.014 | REAL | high | fmemopen page: reads never go past the end position and reaching it is EOF. "w+" with "abc" (end position 3), seek to 5 (legal, within max_size), fgetc returned 'x' (120) from the untouched buffer instead of EOF. |
| XSH.fmemopen.015 | REAL | high | fmemopen page: a null byte is written only when a write advances the end position. In "r+" the end position is max_size and fixed, so a write at 2..3 must not add a null. The buffer became "abXY\0f". |
| XSH.fmemopen.020 | REAL | high | Same rule. "w+": "abcdef" written (end 6), then a 1-byte write at position 1 does not move the end, yet a null was stored at position 2 ("aZ\0def\0"). |
| XSH.fmemopen.022 | REAL | medium | fmemopen page: SEEK_END is relative to the end position (3 here, after writing "abc"), and a seek is legal up to max_size (10). fseek(7, SEEK_END) is position 10 and failed with EINVAL. fseek(10, SEEK_SET) before it passed, so the system seems to compute SEEK_END from something other than the end position. |
| XSH.fwscanf.017 | REAL | medium | fwscanf page: the 'm' assignment-allocation character (c, s, [ conversions, with l for wide) shall be accepted and allocate the buffer. swscanf with %mls returned 0 matches (the first, string form, failed). |
| XSH.getgrgid.002 | REAL | high | getgrgid page: if no entry is found, errno shall not be changed. errno became ENOENT. The test excuses only the may-fail codes (EIO, EINTR, EMFILE, ENFILE), ENOENT is not one. |
| XSH.getlogin.005 | REAL | high | getlogin page: getlogin_r shall fail with ERANGE when namesize is smaller than the name length including the terminator. With namesize equal to the strlen it returned 0. |
| XSH.getpwnam.002 | REAL | high | getpwnam page: same rule as getgrgid, errno shall not change when the name is absent. Got ENOENT (likely a missing database file inside libc, but the observable rule is still violated). |
| XSH.glob.011 | REAL | medium | glob page uses the pattern matching notation (XCU 2.14), whose bracket expressions include character classes. `[[:digit:]]x` with a file `1x` present returned -3 (GLOB_NOMATCH). |
| XSH.glob.016 | REAL | high | XCU 2.14.3 (cited by glob): a slash found after an unescaped '[' before its ']' makes the '[' an ordinary character, so `a[b/c]d` matches the path `a[b/c]d` (directory `a[b`, file `c]d`, both created). Got GLOB_NOMATCH. |
| XSH.mkdir.003 | REAL | medium | mkdir page: S_ISVTX is initialized from mode (marked XSI) after umask on the permission bits. umask 0, mode 01777, result 0777. Only required if XSI is claimed. Running as root or the ext4 test directory do not explain a dropped sticky bit. |
| XSH.msgrcv.003 | REAL | high | msgrcv page (XSI): with negative msgtyp, take the first message of the lowest type that is <= the absolute value. Queue 5,3,4,3 with msgtyp -4 must deliver 3,3,4. System delivered a type-4 message where a 3 was due, i.e. it takes the first eligible message in queue order. |
| XSH.pause.003 | REAL | medium | pause page: it returns only after a handler for a caught signal returns (or the process dies). The only handler is for SIGUSR1; the count of handler runs was 0 when pause returned at 151 ms, just after the ignored SIGUSR2 (sent at 100 ms plus fork time) and the helper's exit (default-ignored SIGCHLD). So an ignored or default-ignored signal woke pause(). Cannot tell from the note which of the two signals did it; both are violations. |
| XSH.posix_spawn.014 | REAL | high | posix_spawn page: if exec would fail with ENOEXEC, posix_spawnp shall fail and not fall back to running the file under sh. The script (no shebang) was run by sh (marker file created). |
| XSH.posix_spawn.017 | REAL | medium | posix_spawn page: with null file_actions, descriptors with FD_CLOFORK set are closed in the child. FD_CLOFORK is defined in the guest headers, so this is not a header problem. The child still had descriptor 4 open. |
| XSH.posix_spawn.018 | REAL | medium | Same rule with an empty file_actions object. Descriptor 6 (the FD_CLOFORK one; the FD_CLOEXEC one is closed by exec anyway) was open in the child. posix_spawn evidently ignores FD_CLOFORK. |
| XSH.posix_spawn.022 | REAL | high | posix_spawn_file_actions_addclose page: a close of an in-range, not-open descriptor must be ignored, not fail with EBADF. Descriptor was 30 (well under OPEN_MAX); posix_spawn returned EBADF. |
| XSH.posix_spawn.027 | REAL | high | posix_spawn_file_actions_adddup2 page: with fildes equal to newfildes the action must leave the descriptor open in the child with FD_CLOEXEC clear, even if it was set. The descriptor was closed in the child (dup2 no-op semantics). |
| XSH.posix_spawn.061 | REAL | medium | posix_spawn page, RETURN VALUE: on failure no child process shall be created (the pid value is unspecified, but a child must not exist). ENOTDIR was returned correctly yet waitpid(-1, WNOHANG) found a child. The test's own helper child (copy_self) is reaped before, so I see no stray child. Same cause for .062 and .069. |
| XSH.posix_spawn.062 | REAL | medium | As .061: ELOOP returned, but a child existed (path errors found after the child was created, error returned to the parent). |
| XSH.posix_spawn.069 | REAL | medium | As .061: EPERM from the failed process-group change returned, but a child existed. Note the page also allows the error to be delivered as child exit status 127 only if posix_spawn returned 0. Here it returned non-zero and kept the child. |
| XSH.sem_open.004 | REAL | high | sem_getvalue is in the base (moved out of an option in Issue 7) and shall return 0 and set sval; the only may-fail error is EINVAL for an invalid semaphore. It failed on a valid named semaphore (the sem_open, sem_post, sem_trywait steps before it passed). |
| XSH.seteuid.005 | REAL | medium | XBD 3 (User ID): (uid_t)-1 shall not be a valid user ID. seteuid page: EINVAL shall be returned for an invalid uid that is not supported. As root, seteuid((uid_t)-1) returned 0. The phrase "invalid and not supported" leaves room for an implementation that treats -1 as a supported value, but the definition says it is not a valid ID. Darwin appears to treat -1 as "no change". |
| XSH.setuid.005 | REAL | medium | Same as seteuid.005, from the setuid page. |
| XSH.strptime.002 | REAL | high | strptime page: %Ob, %OB, %Oh are equivalent to %b. `%Ob` on "Jan,x" returned null (plain %b, %B, %h passed first). |
| XSH.strptime.003 | REAL | medium | strptime page: %d is the day [01,31]; no match means strptime fails. "0" and "00" are outside the range, so they must fail. "0," with "%d," was accepted. The page does not state the range rule in a separate sentence, hence medium. |
| XSH.strptime.005 | REAL | high | strptime page: %I with %p sets tm_hour from the combined results; nothing makes the order matter. "12 AM" with "%I %p" passed, but "AM 12" with "%p %I" gave hour 12, not 0. The %p-before-%I order is mishandled. |
| XSH.strptime.009 | REAL | high | strptime page: %F reads the year as %Y with unlimited width. "12345-06-07" with %F returned null (4-digit year limit). |
| XSH.strptime.010 | REAL | medium | strptime page: %c reads the locale's date-and-time format, which in the POSIX locale ends with the year. The three full %c, %x, %X, %r positive cases passed. The input "Thu Feb 29 13:45:59" lacks the year and must fail but a non-null pointer came back. Assumes the guest runs in the POSIX/C locale (default; if not, the same failure would still apply as the year is part of any d_t_fmt of that kind). |
| XSH.strptime.011 | REAL | high | strptime page: a run of %n, %t and white-space directives scans up to the first non-white-space, or until nothing more can be scanned (including zero characters). "%n%t " on white-space-then-x returned null. |
| XSH.strptime.014 | REAL | high | strptime page: %G reads the week-based year (and %g, %V, %U, %W, %O variants); the effect on tm is unspecified but it must parse. "1998," with "%G," returned null. The earlier %U, %W, %V cases passed. |
| XSH.strptime.015 | REAL | medium | strptime page: %s reads a decimal number of seconds. "abc" is not a match and the call must fail; it returned non-null (apparently strtol-style conversion that accepts no digits). The positive %s and %z cases ran before it and passed. |
| XSH.strptime.017 | REAL | high | strptime page: field widths are allowed for C, F, G, Y, Z; and %G is a required specifier. "1998" with "%4G" returned null; the %Y/%C/%F width cases before it passed, so this is the same missing %G as .014. |
| XSH.wcstod.005 | REAL | medium | wcstod page: the NaN subject sequence is NAN or NAN(n-wchar-sequence) where the sequence is digits and non-digits (letters and underscore). "nan(!)" is not of that form, so the longest valid subject is "nan" (end offset 3). The system consumed all 6. The meaning of the n-wchar-sequence is implementation-defined; its syntax is not. The other nan() cases passed. |
| XSH.wcstoimax.003 | REAL | high | wcstoimax is equivalent to wcstol (cited page). With base 0 the longest initial subsequence of expected form in "0x" is the octal constant "0", so value 0 and end offset 1. Got offset 0 (treated as no conversion). |
| XSH.wcstoimax.004 | REAL | high | wcstol: for base 16 the 0x prefix is optional, so "0" is a valid subject in "0x"; end offset must be 1. Got 0. |
| XSH.wcstoimax.016 | REAL | high | Same as .003, for wcstoumax. |
| XSH.wcstoimax.017 | REAL | high | Same as .004, for wcstoumax. |
| XBD.11.2-terminal-parameters.014 | REAL | medium | XBD 11.2.3 Output Modes: without ONLRET a newline leaves the column unchanged, and ONOCR only suppresses a CR at column 0. The test writes "ab\n\rc", so the column is 2 when the CR comes and the CR must be sent. The guest dropped it, so it seems to treat the column as 0 after NL. The ONLRET half of the test passed. |
| XBD.11.2-terminal-parameters.016 | REAL | medium | XBD 11.2.3: with OFILL set, a delay is sent as fill characters, and a newline delay (NL1) is two fill characters (NUL, or DEL with OFDEL). The test checks `odelay_kept` first, so NL1 was accepted, yet no fill came out ([61 0a]). The pty does not implement fill. |
| XBD.4.16-pathname-resolution.002 | REAL | medium | XBD 4.16 Pathname Resolution: each name must be located in the directory named by its predecessor, otherwise resolution fails. In `d/f/..`, f is a regular file, so ".." cannot be located in it and stat must fail (ENOTDIR). The guest resolved ".." lexically. The earlier cases (`d/f/x`, `d/f/.`) apparently passed. |
| XBD.7.3.1-lc-ctype.010 | ENV | high | Status 127 means localedef was not found, which fits the toybox userland. The test needs the localedef utility. Nothing here concerns the standard. |
| XBD.fcntl.h.013 | REAL | medium | XBD fcntl.h and XSH fcntl (F_GETFL): file access modes are extracted from F_GETFL with the O_ACCMODE mask. The header page does not say outright that each mode lies inside the mask, but O_EXEC = 0x40000000 outside a mask of 3 cannot be extracted, so the extraction rule breaks for O_EXEC. This is a header deviation (Apple form). |
| XBD.iso646.h.001 | ENV | medium | The sibling test 002 fails to build because `and` in `#if` evaluates as the identifier 0, so no iso646 macros are defined. The header the patched TinyCC finds defines nothing (the clang resource-dir header is probably missing). The requirement (XBD iso646.h: the spellings expand to the operators) holds, but the cause is the compiler/header setup. |
| XBD.sys_socket.h.007 | REAL | high | XBD sys/socket.h lists SO_DOMAIN (and SO_PROTOCOL) among the required socket-level option constants. The headers lack SO_DOMAIN. |
| XSH.sigpending.002 | REAL | medium | XSH sigpending: the set returned holds signals blocked for the calling thread and pending on the process or on the calling thread. SIGUSR2 is sent to the process and blocked in both threads (the new thread inherits the mask), so the second thread must see it. The first thread saw it and the second did not, so the guest ties a process signal to one thread. |
| XSH.2.7-xsi-ipc.002 | REAL | medium | XSH 2.7.1 IPC General Description: a process whose effective UID matches neither uid nor cuid but whose effective GID matches gid or cgid is judged by the group bits only. With mode 006 and the process in the owner group, group bits are 0, so access is refused. The guest allowed it, so it fell to the "other" bits. Either the group match is skipped, or IPC_SET did not store msg_perm.gid. Both are deviations. The report shows only the first failing case. |
| XSH.2.7-xsi-ipc.003 | REAL | medium | Same rule and same cause (semaphores, "alter" bits). |
| XSH.2.7-xsi-ipc.004 | REAL | medium | Same rule and same cause (shmat). |
| XSH.aligned_alloc.001 | REAL | low | XSH aligned_alloc defers to ISO C. A null pointer is allowed only for an alignment that is not valid/supported. Alignment 1 is `_Alignof(char)`, a fundamental alignment, which ISO C requires to be supported. This rests on the ISO C notion of "valid alignment", which this tree does not contain. If the guest's own supported set excludes alignments below sizeof(void*), the page reads ambiguous. |
| XSH.close.014 | REAL | medium | XSH aio_write: a descriptor of a device that cannot seek is handled as append, and the synchronous errors are only EAGAIN, EBADF, EINVAL. ESPIPE is not one of them, so a pipe write should queue. Residual doubt: the page says "device", and a pipe is not strictly a device. |
| XSH.connect.020 | REAL | medium | XSH connect: on a connectionless socket an AF_UNSPEC address resets the peer address. The call fails with EAFNOSUPPORT. The test passes a zeroed `sockaddr` of full size with only sa_family set, which is portable. |
| XSH.fclose.011 | REAL | high | XSH fclose: if the file is not at EOF, is seekable, and the stream is the active handle, the descriptor offset is set to the stream position. The test grows the file through another open file description, so the descriptor is not at EOF. The stream position is 1 after the pushback, and the offset was 4095. |
| XSH.fclose.012 | REAL | medium | XSH fclose ERRORS: ENOSPC covers "no free space in the buffer used by fmemopen()". fclose returned EOF (that check passed, since the failing check is the errno one) but left errno unset. The test reads errno right after the call, so this is a missing errno. |
| XSH.fgets.006 | REAL | medium | XSH fgets: success means n-1 bytes, a newline, or EOF was reached; on an error the indicator is set, a null pointer is returned and errno is set. With "ab" in a non-blocking pipe, none of the three stop conditions was met and the read hit EAGAIN (see fgetc ERRORS). Returning s is therefore wrong. Some libcs return the partial data, so the page is not as explicit as it could be. |
| XSH.fmtmsg.005 | REAL | medium | XSH fmtmsg: a component is shown only when its value is not the null value; the page's usage note says null values omit components. "TO FIX:" is the prefix of the action string, so with a null action it should not appear. The guest printed it. The normative support is indirect, and the usage note is informative. |
| XSH.fpathconf.001 | REAL | high | XSH fpathconf table: implementations shall support every listed variable, and for _PC_FILESIZEBITS on a directory the value applies to files in it. EINVAL means the name is unsupported. |
| XSH.fpathconf.004 | REAL | high | Same cause as .001 (pathconf on a directory with _PC_FILESIZEBITS fails with EINVAL, not "no limit"). |
| XSH.fpathconf.005 | REAL | medium | XSH fpathconf table: on a directory, _PC_PIPE_BUF applies to FIFOs creatable in it. It returned -1 with errno changed, so it is an error and not "no limit". Only PIPE_BUF is named by the first failure. |
| XSH.ftruncate.007 | REAL | low | XSH ftruncate: whole pages beyond the new end of a mapped file are discarded, and references to them give SIGBUS. The child's read of the second page ended normally. Possible ENV: if the ext4 test file system does not support mappings properly, the page stays resident. Check on a native file system. |
| XSH.getaddrinfo.021 | REAL | high | XSH getaddrinfo RETURN VALUE: with AI_CANONNAME and a non-null node, ai_canonname of the first entry points to the canonical name, or to the nodename string if none is available. A null pointer is not allowed. |
| XSH.getaddrinfo.024 | REAL | medium | XSH getaddrinfo ERRORS: EAI_SOCKTYPE for an unrecognized socket type. The guest returned 12, a non-standard "bad hints" code. |
| XSH.getnameinfo.008 | REAL | medium | XSH getnameinfo ERRORS: EAI_NONAME when both node and service are null. The guest returned 0. The second check (zero lengths) is more debatable, but the first already fails. |
| XSH.getnameinfo.010 | REAL | medium | XSH getnameinfo ERRORS: EAI_FAMILY when the address length is invalid for the family. A 1- or 2-byte length is invalid for AF_INET, and the guest returned 0. |
| XSH.grantpt.003 | REAL | medium | XSH grantpt ERRORS: the condition "not a manager pseudo-terminal" is EINVAL. The guest fails with ENOTTY, a code the page does not list. Weaker point: the page says "may fail", and a strict reader could allow other codes. |
| XSH.listen.003 | REAL | high | XSH listen ERRORS: unbound is either success or EDESTADDRREQ; EINVAL means already connected (or may mean shut down). An unbound AF_UNIX socket is neither. |
| XSH.mkdtemp.012 | REAL | high | XSH mkdtemp ERRORS: mkstemp shall fail with EINVAL when the template does not end in "XXXXXX". "fXXXXX" has five X and the guest did not fail. |
| XSH.nftw.015 | REAL | low | XSH nftw: the object may be a non-directory file (type FTW_F), and fn is called once. The walk then ends and nftw returns 0. It returned -1. The errno is not shown, so the cause is not known. |
| XSH.perror.007 | REAL | high | XSH perror: on error perror sets the stream's error indicator and errno. With descriptor 2 read-only and stderr unbuffered, the indicator was not set. The test goes UNRESOLVED if stderr cannot be made unbuffered, so the write was attempted. |
| XSH.posix_spawn_file_actions_addclose.003 | REAL | high | XSH addclose: a close of a non-negative descriptor below {OPEN_MAX} that is already closed is ignored, not EBADF. posix_spawn returned EBADF. |
| XSH.posix_spawn_file_actions_addclose.005 | REAL | high | Same rule, for a descriptor that was never open. |
| XSH.posix_spawn_file_actions_addclose.013 | REAL | medium | XSH addopen ERRORS: EBADF when fildes is negative or greater than or equal to {OPEN_MAX}. addopen(OPEN_MAX) returned 0. The test uses sysconf(_SC_OPEN_MAX) as {OPEN_MAX}. |
| XSH.pthread_kill.004 | REAL | high | XSH pthread_kill: it is not an error if the thread is a zombie, and the RATIONALE says implementations that return ESRCH there do not conform. |
| XSH.recv.017 | REAL | medium | XSH connect (SOCK_DGRAM): the peer address limits the remote sender for later recv. The guest delivered a datagram from the unconnected socket c first. |
| XSH.rename.012 | REAL | high | XSH rename: it fails when the final component of either path is dot or dot-dot (EINVAL). `rename("e","d2/.")` returned 0. The two "old" cases passed. |
| XSH.rename.013 | REAL | medium | XSH rename ERRORS: ENOTDIR when old is an existing non-directory and new names a nonexistent file with a trailing slash. The guest gave ENOENT. |
| XSH.rename.014 | REAL | high | XSH rename DESCRIPTION: if new does not resolve to an existing directory and ends with a slash after a non-slash character, rename fails. `rename(d,"nd/")` succeeded. |
| XSH.semop.016 | REAL | medium | XSH sys/sem.h: semval is an unsigned short, and semop gives ERANGE if semval would overflow the system limit. Three additions of 32767 are 98301, which cannot be represented, so the third must fail. All three succeeded. |
| XSH.setgid.005 | REAL | medium | XBD 3.165 Group ID: (gid_t)-1 shall not be a valid group ID. XSH setgid: EINVAL for an invalid gid. The call returned 0. |
| XSH.sigaction.018 | REAL | medium | XSH sigaction: catching or ignoring SIGKILL fails with EINVAL and installs nothing. The check `sigaction(sig,NULL,&o)==0 && handler==SIG_DFL` joins two things. Either the query failed (SIGKILL is a valid signal number, so examining it must work) or the handler changed. Both deviate, but see the diagnostic issue in Test bugs. |
| XSH.strsignal.001 | REAL | high | XSH strsignal: it uses the same set of messages as psignal. psignal wrote "Abort trap" and strsignal gave "Abort trap: 6". |
| XSH.symlink.022 | REAL | medium | XSH symlink: all interfaces behave as if the contents of a symbolic link can always be read, whatever its mode bits. After fchmodat set the link's mode to 0, readlink by another user failed. |
| XSH.tcgetpgrp.006 | REAL | medium | XSH tcgetpgrp ERRORS: ENOTTY if the caller has no controlling terminal. A process group ID was returned (errno ENOTTY is stale). The test does not prove the process has no controlling terminal, only that it used O_NOCTTY. Test 007 shows the same defect with the controlling terminal proven. |
| XSH.tcgetpgrp.007 | REAL | high | XSH tcgetpgrp ERRORS: ENOTTY if the file is not the controlling terminal. The session acquired a controlling terminal (checked), and the call was made on a second pty. A group ID was returned. |
| XSH.tcsetpgrp.008 | REAL | medium | XSH tcsetpgrp ERRORS: ENOTTY without a controlling terminal. It returned 0. Same caveat as tcgetpgrp.006. |
| XSH.tcsetpgrp.009 | REAL | high | XSH tcsetpgrp ERRORS: ENOTTY if the file is not the controlling terminal. It returned 0 on another pty. |
| XSH.ungetc.002 | REAL | medium | XSH ungetc: each successful ungetc decrements the file position. After two reads (position 2) and two pushbacks it should be 0, and it was 2. |
| XSH.ungetc.007 | REAL | low | XSH ungetc: pushed-back bytes are returned by later reads. The first check (ftell is 1) seems to pass and the read of 'Z' then fails. ftell is not a position-discarding call in the page, so ftell must not drop the byte. The cause is unclear: either ftell discards the pushback or ungetc stores it badly. |
| XSH.ungetc.011 | REAL | medium | XSH ungetc and its RATIONALE: fseek with SEEK_CUR is relative to the position on entry (after the pushback, here 1), and the pushback is discarded. The read gave '3', as if the position were 3. |
| XSH.ungetc.013 | REAL | medium | Same defect: after the pushback and fseek(0,SEEK_CUR) the write landed at offset 3 ("012W" instead of "W123"). The first (already-read 'Z') part of the test passed. |
| XSH.wcstol.003 | REAL | high | XSH wcstol: the subject sequence is the longest initial part of the expected form. For "0x" the longest valid form is "0", so the end pointer is at offset 1. The guest left it at 0. |
| XSH.wcstol.004 | REAL | high | Same rule for base 16: the optional 0x prefix needs digits after it, so "0x" converts the "0" and ends at the x. |
| XSH.write.005 | UNSURE | low | XSH write (O_APPEND): the offset is set to the end before each write with no intervening modification, so concurrent appenders must not lose data. 700 of 1000 records survived. This is a real violation if the file system is native. The test file system is ext4 under PureDarwin, whose driver may not implement atomic append. A rerun on the native file system would settle it. |
| XSH.write.007 | REAL | medium | XSH write: a write with no room under the soft file size limit fails (EFBIG), and with the XSI option raises SIGXFSZ. A 1-byte O_APPEND write to a 10-byte file with limit 10 returned 1. The limit is probably skipped for append writes. |
| XSH.write.008 | REAL | medium | Same defect as write.007 (the first check, EFBIG, fails). |
| XBD.12.1-utility-argument-syntax.003 | ENV | medium | pr is XSI; `-e` with no argument must expand tabs with gap 8 (pr page). Output is empty: the toybox pr does not implement `-e` (or fails). Test expectation `a`+7 spaces+`b` is correct. |
| XBD.4.1-case-insensitive-comparisons.001 | REAL | medium | XBD 4.1: a string that matches case-sensitively must also match with any characters swapped to their counterparts, so `\(a\)\1` must match `aA`. Same defect as XSH.regcomp.027 (back-reference with REG_ICASE); toybox grep -i most likely goes through libc regcomp. Cause could be toybox instead; would be ENV then. |
| XBD.4.1-case-insensitive-comparisons.002 | UNSURE | low | find page (-iname) refers to XBD 4.1, so `[[:upper:]]eta` must match `beta`; the test is right. Could be libc fnmatch FNM_CASEFOLD (fnmatch page, also XBD 4.1) not folding inside classes (REAL), or toybox find's own matching (ENV). A direct `fnmatch("[[:upper:]]eta","beta",FNM_CASEFOLD)` call on the guest settles it. |
| XBD.4.1-case-insensitive-comparisons.003 | REAL | medium | sed page (s flag `i`) plus XBD 4.1: `s/\(a\)\1/X/i` must change `aA`. Same root cause as 001 and XSH.regcomp.027 if toybox sed uses libc regcomp. |
| XBD.float.h.003 | ENV | medium | float.h page: LDBL_MIN is radix^(min_exp-1). The test rebuilds it by exact halving; FLT and DBL pass, only long double fails. On x86 the value is 2^-16382 and exact in x87, so the likely cause is TinyCC's handling of long double literals (the LDBL_MIN literal in the header) or of the volatile long double loop. Compare `LDBL_MIN` against `0x1p-16382L` printed with `%La` to confirm. |
| XBD.locale.h.007 | REAL | medium | freelocale page: the function is declared `void freelocale(locale_t)`. The header in use declares another return type (not void). If the headers in use are the zig fallback headers (system.txt says so) this is really a header-provenance issue, so ENV is possible; the brief lists "Apple header form" as ENV. Which header file declares it settles it. |
| XBD.tgmath.h.001 | ENV | high | tgmath.h page: <tgmath.h> includes <complex.h>; the complex macros do not exist because TinyCC has no `_Complex` (system.txt). |
| XBD.tgmath.h.002 | UNSURE | low | tgmath.h page: with a generic argument of integer type and the other float, the type is double, so `atan2(int,float)` is double and the test is right. All other lines passed first, so only the mixed int/float case of atan2 is wrong; either the header's type-generic machinery (ENV, TinyCC `_Generic` or header form) or the libc header. Look at the guest's atan2 definition in <tgmath.h>. |
| XSH.clock_getres.007 | REAL | high | clock_getres/clock_settime page: tv_nsec >= 1e9 shall fail with EINVAL (EPERM only for lack of privilege, "may"). Call returned 0, i.e. the clock was set (test runs as root). |
| XSH.access.004 | REAL | medium | access page: failure for a mode 0 file is EACCES; X_OK may succeed for a privileged process, but EPERM is not a permitted error for this condition. Could come from the ext4 test file system instead of the kernel (then ENV); rerun on another file system. |
| XSH.aio_read.009 | REAL | high | aio_read page: only EAGAIN, EBADF, EINVAL, EOVERFLOW may come back at queuing time; XSH 2.8 (Asynchronous I/O): for a device incapable of seeking aio_offset is not applied. A pipe read must be queued and stay in progress; ESPIPE is wrong. |
| XSH.bind.004 | REAL | high | bind page (AF_UNIX): a pathname naming an existing file, including a symbolic link, is "already in use" -> EADDRINUSE. Regular file and link to a file passed; the dangling link was followed and the socket created at its target. |
| XSH.confstr.005 | ENV | medium | confstr page / _CS_PATH must find all standard utilities. No directory of the value has `getconf`; toybox userland does not ship getconf (a missing utility is not a confstr defect). `sh env cat ls` passed. |
| XSH.exec.026 | REAL | medium | exec page: tms_cutime and tms_cstime are inherited across exec. New image sees 0 where 31 ticks were before. |
| XSH.exec.058 | REAL | medium | fcntl page (record locks): process-owned locks go away when any descriptor of the file is closed by the process; exec page: FD_CLOEXEC descriptors are closed at exec. The lock stayed held. Test logic is sound (prober is the parent). Medium only because "closed" at exec is implied rather than stated for process-owned locks. |
| XSH.fgetpos.004 | REAL | high | fgetpos page: shall fail with EBADF if the underlying descriptor is not valid. Position came from the stream's cached state. |
| XSH.fileno.003 | REAL | high | fileno page: shall fail with EBADF for a stream with no file. Returned -1 (that check passed) but errno stayed 0. |
| XSH.fnmatch.012 | REAL | high | XCU 2.14.1: a `[` that starts no valid bracket expression matches itself; fnmatch must return 0. Got 2 (neither match nor FNM_NOMATCH). |
| XSH.fprintf.030 | REAL | high | fprintf page: for a/A with FLT_RADIX a power of two the value shall be correctly rounded to the precision. 1.03125+tiny to one hex digit must be 0x1.1p+0; got 0x1.0p+0 (truncation or ties-down). |
| XSH.fprintf.031 | REAL | high | fprintf page: infinity under a/A is printed like f/F (inf or infinity). `%f %e %g` passed with the same argument; `%a` gave "nan". |
| XSH.ftell.006 | REAL | high | ftell page: shall fail with EBADF when the underlying descriptor is not open. Returned the buffered position (1). |
| XSH.fwprintf.014 | REAL | high | fwprintf page: swprintf shall fail with EOVERFLOW when n or more wide characters were requested; with n 0 that is every output. Got EINVAL. |
| XSH.getdate.017 | REAL | medium | getdate page: extra white space in the template file or the input is ignored. Leading blanks before `Nov` yield error 7 (no template matched). Medium: the other table entries are not visible. |
| XSH.getgrnam.002 | REAL | medium | getgrnam page: when the entry is not found errno shall not change. It became ENOENT. Medium because a missing group database would be an "error occurred" case; /etc/group presence on the guest settles it. |
| XSH.getopt.006 | REAL | high | getopt page: a leading `+` has no effect and the colon after it still selects ':' for a missing argument. Got '?': `+` is not skipped. |
| XSH.getopt.007 | REAL | high | getopt page: no diagnostic when the first character after the optional `+` is a colon. Diagnostic printed, same root cause as 006. |
| XSH.getopt.008 | REAL | high | getopt page: `+` is not an option character; `-+` with optstring "+ab" must return '?'. Got '+' as a valid option. |
| XSH.localtime.005 | REAL | high | XBD 8.3 (TZ): rule time may be negative and up to 167 hours. Result is the UTC time (offset 0, 5 h off): the whole TZ string was rejected and UTC used. |
| XSH.mktime.009 | REAL | high | mktime page: when the result or tm_year cannot be represented it shall return -1, set EOVERFLOW and leave tm_wday. -1 returned (checked first) but errno 0. |
| XSH.open.039 | REAL | high | open page: EISDIR for a directory when oflag includes O_CREAT without O_DIRECTORY. Descriptor 4 returned. |
| XSH.popen.008 | REAL | high | popen page: mode "re" sets FD_CLOEXEC on the stream's descriptor. EINVAL means the `e` mode character is unsupported. |
| XSH.popen.015 | UNSURE | medium | popen page: the shell is invoked as `sh -c -- command`, so a command word starting with `-` must work. Either popen omits `--` (REAL) or the guest sh cannot run a file without interpreter line (XCU 2.9.1.6; toybox/zsh shell, ENV). Run `sh -c -- -pcdash` and `sh -c -pcdash` by hand to separate them. |
| XSH.posix_spawn_file_actions_adddup2.007 | REAL | medium | adddup2 page: descriptors >= {OPEN_MAX} -> EBADF. The test takes {OPEN_MAX} from sysconf (here the soft limit, 256 probably); adddup2 accepts it. The system gives two different values of one limit. Compare header OPEN_MAX with sysconf on the guest. |
| XSH.pthread_cond_wait.008 | REAL | medium | pthread_cond_wait page: a thread cancelled in the wait acts on the request (cancellation point) and runs cleanup handlers with the mutex held. In the run log pthread_cancel.001 (blocked read) and pthread_cleanup_pop.003 pass, so cleanup handlers work; cancellation of threads blocked in cond_wait, write (2.9-threads.028) and pthread_join (pthread_join.006) does not arrive. Could be a QEMU/kernel issue, so medium; rerun elsewhere to be sure. |
| XSH.quick_exit.005 | REAL | medium | quick_exit and at_quick_exit pages: a registration made during quick_exit may fail (unspecified), but a successful one must run, after the already-called handlers. at_quick_exit returned 0 (no "F") but h3 never ran. Test already handles the refuse case. |
| XSH.regcomp.027 | REAL | high | regcomp page REG_ICASE and XBD 4.1: `\(a\)\1` must match "aA". Not matched. |
| XSH.regcomp.033 | REAL | high | XBD 9.4 (ERE): a duplication symbol may be suffixed by `?` to give the leftmost shortest match (repetition modifier, Issue 8). regcomp rejected `.*?c` with code 13 (REG_BADRPT). |
| XSH.rmdir.005 | UNSURE | low | rmdir page: no new entries may be created in a removed directory that is still open. File was created. Probably decided by the file system (test dir is on the ext4 test file system, an ENV factor); repeat on a tmpfs or native volume. Test is sound. |
| XSH.sem_close.002 | REAL | high | sem_getvalue page: returns 0 on success; no ENOSYS error is allowed (semaphores are base in Issue 8). Run log shows sem_init also gives ENOSYS. |
| XSH.setegid.005 | REAL | high | XBD 3 (Group ID): (gid_t)-1 is not a valid group ID; setegid page: EINVAL for an invalid, unsupported gid. Returned 0. |
| XSH.setpgid.010 | REAL | medium | setpgid page: ESRCH when pid is neither the caller nor a child of the caller. A grandchild was accepted (any descendant seems to be allowed). Medium: depends on the grandchild still being alive, which the test setup arranges. |
| XSH.strfmon.005 | REAL | high | strfmon page (Issue 8): `+` flag with empty positive_sign and negative_sign (POSIX locale) -> -1/EINVAL. Returned 3. |
| XSH.strtod.005 | REAL | high | strtod page: NAN(n-char-sequence) takes only digits and nondigits (letters, underscore). `nan(!)` has subject "nan", end offset 3; end was set to 6. |
| XSH.sysconf.001 | REAL | high | sysconf page: for a <limits.h> maximum without a limit return -1 with errno unchanged. TZNAME_MAX query set ENOENT. |
| XSH.sysconf.002 | REAL | high | Same defect as 001 (the only failing name). |
| XSH.sysconf.005 | REAL | high | unistd.h page: _POSIX_VERSION is 202405L for POSIX.1-2024 implementations. The system reports 200112, i.e. it claims an older edition. A version claim, not a behaviour defect. |
| XSH.sysconf.006 | REAL | high | unistd.h page: _POSIX_BARRIERS is always 202405L (barriers mandatory). sysconf(_SC_BARRIERS) is -1. |
| XSH.sysconf.007 | REAL | high | Same as 006. |
| XSH.sysconf.009 | REAL | high | sysconf page: values do not change during the process lifetime except _SC_OPEN_MAX across setrlimit(RLIMIT_NOFILE). STREAM_MAX moved from 256 to 255. |
| XSH.unlockpt.002 | UNSURE | low | unlockpt page lists EBADF (not open for writing) and EINVAL (not a manager) as "may fail". ENOTTY is listed nowhere; XSH 2.3 forbids a different error number for a described condition, so this leans REAL, but the condition is only a "may". The pipe read end is also not writable, so EBADF would be accepted by the test. |
| XSH.wcsftime.004 | REAL | medium | wcsftime page: the result array always gets a terminating null wide character when it fits; an empty format returns 0 and `buf[0]` must be L'\0'. It was left unterminated. |
| XSH.wcsftime.006 | REAL | high | strftime page (Issue 8 flags and width): `%010F` pads to 10 with zeros, giving 1970-01-02 (the page allows width for C, F, G, Y). Returned 3. |
| XSH.wcstoul.003 | REAL | high | wcstoul page: subject sequence is the longest initial part of the expected form; "0x" without hex digits gives subject "0" and end pointer at offset 1. End pointer left at 0. |
| XSH.wcstoul.004 | REAL | high | Same as 003, base 16. |
| XSH.y0.004 | REAL | medium | y0 page: x = 0 with n negative and odd -> +HUGE_VAL. yn(-1, 0.0) gave NaN. Depends on the [MXX] option being supported, which the test's IEC guard should assume. |

## Patterns (from the four readers)

### Reader 1
- Shell helper not working (19 assertions): XSH.wordexp.001-007, 010, 013-024. All fail on the first expansion needing a shell run; error-path tests that never start a shell pass.
- Darwin cancellation points not honored when the request is already pending (7): 2.9-threads.016, 017, 019, 021, 022, 023, plus blocked cases pthread_join.006 and 2.9-threads.028. The sibling tests for read, write, sockets, sleep and sync (012-015, 018, 020, 024) are not on this list, so cancellation works for those calls only.
- File-operation atomicity between threads (4): XBD.4.11.002, 2.9-threads.031, 033, and SIGBUS 2.4.3.021. All touch the file system under test; one rerun on a non-ext4 volume decides ENV against REAL for the group. Rewinddir 001/002 belong to the same "ext4 driver" suspicion.
- fscanf backtracking and missing features (6): 013, 014, 015 (BSD-style push-back of more than one character), 037, 038 (no %m), 043 (suppressed conversion not counted).
- POSIX 2024 additions missing from the libc (6): strftime 007 and 014, system 006 (no "--"), fopen 013 ('x' after 'e'), realpath 001 (hard-link entry) and 005 (trailing slash).
- Job-control and terminal checks absent in the tty layer (2): tcsendbreak 005 and 007.
- waitid does not zero infop on WNOHANG, and WEXITED matches a stop (3): 008, 014, 015.
- syslog mask handling (3): openlog 001, 003, 006 (default mask, setlogmask(0), previous-mask return).
- AIO (3): lio_listio 001 and 004 (ESPIPE on pipes), 006 (EBADF instead of EIO).
- Unnamed semaphores unimplemented, ENOSYS (3): sem_init 001, 005, 006.
- XSI umask sticky bit (2): umask 003, 005.
- Missing userland tools (4): od, awk, grep (unsure), localedef.

### Reader 2
1. Pseudo-terminal output lost on last close of the slave (XBD 11.1.11): .045 (direct), .011, .013, .014 (they only fail on the final master read-back after the slave side is gone; the assertion proper passed). One defect, four failures.
2. tcgetpgrp() does not enforce "caller's controlling terminal" and/or the terminal is acquired too liberally (XBD 11.1.3 plus the tcgetpgrp page): .002, .004, .005.
3. Terminal parameter calls from a background group are not stopped (11.1.4): .016.
4. fmemopen position and end-position rules (null byte only when the end advances; read-only never writes; reads stop at the end; SEEK_END relative to the end): fmemopen .003, .014, .015, .020, .022 (fflush.013 is the ENOSPC-with-errno side).
5. posix_spawn behaviour that follows older Darwin semantics: sh fallback on ENOEXEC (.014), FD_CLOFORK ignored (.017, .018), close of unopened fd gives EBADF (.022), dup2 with equal fds does not clear CLOEXEC (.027), and a child is left behind when an error is returned (.061, .062, .069).
6. strptime lacks Issue 8 features and has older-BSD quirks: %G (.014, .017), unlimited %F year (.009), %O modifiers on b (.002), %p before %I (.005), white-space runs (.011), range check on %d (.003), %s accepts non-numbers (.015), %c accepts a missing year (.010).
7. wcstoimax/wcstoumax with a bare "0x" prefix and base 0 or 16 report no conversion instead of converting "0": .003, .004, .016, .017 (all the same bug; wcstol-family siblings in the chunk list for other chunks likely share it).
8. Error codes or errno rules that differ from the page: fchdir on a pipe EINVAL (fchdir.004), catopen EINVAL / EFTYPE-like 79 (catopen.004, .010), aio_fsync accepts bad op (aio_fsync.004), aio_write rejects SIGEV_THREAD (aio_write.007), getlogin_r no ERANGE (getlogin.005), getpwnam/getgrgid clobber errno with ENOENT (.002 both), sem_getvalue fails (sem_open.004), seteuid/setuid accept (uid_t)-1 (seteuid.005, setuid.005).
9. Option-conditional (XSI) items: mkdir.003 (S_ISVTX), msgrcv.003, XBD .039 (backslash escape), catopen.010. These are REAL only if the system claims XSI; the project should confirm that PureDarwin claims it.
10. Environment: abort.005 only (TinyCC and `_Thread_local`).

### Reader 3
1. Pseudo-terminal output processing (XBD.11.2.014, .016): column tracking after NL and OFILL fill characters are not implemented in the pty line discipline.
2. Controlling-terminal checks missing in tcgetpgrp/tcsetpgrp (XSH.tcgetpgrp.006, .007, tcsetpgrp.008, .009): the calls succeed on a terminal that is not the caller's controlling terminal.
3. Stdio pushback position handling (XSH.ungetc.002, .007, .011, .013; probably also the XSH.fclose.011 offset): the file position is not adjusted for pushed-back bytes, and fseek/fclose act on the wrong offset.
4. posix_spawn file actions (XSH.posix_spawn_file_actions_addclose.003, .005, .013): closing an already-closed descriptor fails with EBADF, and addopen does not enforce the {OPEN_MAX} bound.
5. rename() with dot and trailing-slash rules (XSH.rename.012, .013, .014) and pathname resolution through a non-directory (XBD.4.16.002): the BSD-style path handling is laxer than the standard.
6. Missing or non-standard error codes: getaddrinfo (.024), getnameinfo (.008, .010), grantpt (.003), listen (.003), mkstemp (.012), setgid (.005), connect AF_UNSPEC (.020), pathconf names (fpathconf.001, .004, .005), semop ERANGE (.016).
7. File size limit not applied to O_APPEND writes (XSH.write.007, .008).
8. Per-thread bookkeeping of process-directed signals (XSH.sigpending.002), and IPC group-class permissions (XSH.2.7-xsi-ipc.002, .003, .004).
9. Environment: XBD.7.3.1-lc-ctype.010 (no localedef in the toybox userland) and XBD.iso646.h.001 (TinyCC with a header setup that defines no alternative spellings). Possible file-system effects: XSH.write.005 (atomic append) and XSH.ftruncate.007 (mapping behavior).
10. Header deviations from Apple: XBD.fcntl.h.013 (O_EXEC outside O_ACCMODE) and XBD.sys_socket.h.007 (no SO_DOMAIN).

### Reader 4
- Issue 8 additions not implemented: open O_CREAT on directory (open.039), popen "e" (popen.008), regex `?` repetition modifier (regcomp.033), strfmon `+` EINVAL (strfmon.005), strftime flags and width via wcsftime (wcsftime.006), swprintf EOVERFLOW (fwprintf.014), clock_settime tv_nsec check (clock_getres.007), barriers (sysconf.006/.007), _POSIX_VERSION (sysconf.005), (gid_t)-1 invalid (setegid.005), quick_exit registration (quick_exit.005).
- Case-insensitive back-reference: regcomp.027 is the root of XBD.4.1.001 and .003 (toybox grep and sed through libc regcomp). XBD.4.1.002 (find -iname with a class) may be the same family via fnmatch.
- getopt treats a leading `+` as an ordinary character: getopt.006, .007, .008.
- sysconf: TZNAME_MAX errno (001, 002), version (005), barriers (006, 007), STREAM_MAX changes with rlimit (009).
- errno or error number wrong, return value right: fileno.003, mktime.009, getgrnam.002, access.004, unlockpt.002, aio_read.009.
- Stdio operations answered from cache instead of checking the descriptor: fgetpos.004, ftell.006.
- Cancellation not delivered to threads blocked in some cancellation points: pthread_cond_wait.008 (same family as 2.9-threads.028 and pthread_join.006 in the run log, outside this list).
- ENV from the toolchain: tgmath.h.001 (no `_Complex`), float.h.003 and maybe tgmath.h.002 (TinyCC). ENV from userland: pr -e, confstr.005 (no getconf), and possibly find -iname and popen.015.
- File-system dependent, possible ENV: rmdir.005, access.004.
- Printf family: hex float rounding (fprintf.030) and %a of infinity (fprintf.031).
- Numeric parsing: wcstoul.003/.004 ("0x" prefix without digits), strtod.005 (n-char set).


## Test quality points

### Reader 1
No assertion is a definite TESTBUG: each requirement matched the standard's text. Small improvements, none changing a verdict:

- XSH.waitid.014: the message "the stop was reported again (si_pid -1)" is wrong when si_pid is still the 0xff fill value (-1). Report separately "infop left untouched on WNOHANG" so the same defect is not counted as a second status report.
- XBD.7.4-locale-grammar.006 (ldef.h, ld_ok_copy): status 127 is "utility not found", not "refused a source". Report it as such (or UNRESOLVED if the project decides a missing utility cannot be judged), instead of FAIL with the grammar message.
- XBD.5-file-format-notation.013: awk errors are discarded with 2>/dev/null and an empty result is judged as a bad format. Check `command -v awk` (or status 127) first so a missing awk is not reported as a %a failure. Same idea for XBD.10.2.001 (od) and XBD.12.2.011 (grep).
- XSH.getpwuid.002: ENOENT is not in xc_mayfail. If the standard's "an error occurs" case (lookup service unreachable) is to be told apart from a plain leak, the test could also confirm that getpwuid on a known user works; otherwise keep as is.
- XBD.inttypes.h.004: the failing case is only named by the macro (all messages are alike). Print both wide strings with %ls so the cause (compiler or libc) can be seen from the log.

### Reader 2
No assertion is a definite TESTBUG. Points worth fixing or double checking (none changes a verdict above):

- XBD 11.1 .011/.013/.014: the data read-back from the master happens after the session leader has exited, so these assertions (11.1.4 access control) also depend on 11.1.11. Suggest reading the master before end_session() (or while the slave is still open), so a system with the .045 defect does not also fail four unrelated assertions. The standard allows the current design, so it is not unsound.
- XBD 11.1 .016: the failure note does not say which of tcsetattr, tcdrain, tcflow, tcflush, tcsendbreak failed (the pc_note is not in the FAIL line). Suggest putting names[which] into the failure message.
- catopen.010 (and any test printing errors): pc_errname has no name for EFTYPE (errno 79), printing "E?79". Suggest adding it, or printing the number with a hint.
- strptime.003: the "0" rejection rests on reading the [01,31] range as the set of matching values. If the project wants no dependence on that reading, it could mark the "0"/"00" cases as a separate assertion, or note the reading in the ASSERTION text. I judged the reading correct.
- abort.005: depends on TLS in the handler (`_Thread_local`), which the TinyCC/Mach-O toolchain probably cannot do. Suggest replacing by pthread_equal(pthread_self(), tid saved in a global) so the test is not toolchain-dependent.
- XBD 11.1 .002/.004/.005: the check demands both return -1 and errno ENOTTY. The guest returned a value together with errno ENOTTY, which is itself odd; the test could print errno separately from the return value to help diagnose (it already does). No change needed.

### Reader 3
No assertion was judged TESTBUG. Two points on test quality, neither of which changes a verdict:

- XSH.sigaction.018 (`t/018-einval-uncatchable.c`): one `pc_check` joins "the query of the old action succeeded" and "the old action is still SIG_DFL", and the message says only "the action changed". Split it into two checks (query returns 0; handler equals SIG_DFL) so a failing query is not reported as a changed action.
- XSH.tcgetpgrp.006 and XSH.tcsetpgrp.008 (`M_NOCTTY` mode): the test never proves the leader has no controlling terminal, and it reads errno even when the call succeeded (stale ENOTTY in the note). Add a check that opening `/dev/tty` fails (or that the first call fails at all), and record errno only when the return value is -1.
- XSH.ungetc.007: the two checks run in sequence, so the note does not show which one failed. Report each condition with its own message (the ftell value and the byte read).

### Reader 4
None. Every test that fails here asks for what the cited page requires. Points I checked in particular:
- quick_exit.005 already allows a refused registration via the "F" marker.
- aio_read.009 is consistent with XSH 2.8 for a non-seeking descriptor.
- fnmatch.012 and tgmath.h.002 expectations follow the text exactly.
- confstr.005 depends on a `getconf` utility being installed; this is a precondition worth noting in its comment, but it follows the confstr requirement and is not a bug.
- adddup2.007 takes {OPEN_MAX} from sysconf; sysconf(_SC_OPEN_MAX) may legitimately change with setrlimit, so if a guest reports a lowered soft limit the test should perhaps read it only once before any change (it does). No change suggested.

Every one of the 55 assertions was judged; nothing was skipped.

## Guest probes confirming readers' verdicts (second round)

Programs `puredarwin-probes/wx.c` and `rn.c`, built with the same compiler and flags, run on the
guest while the full re-run was going.

- **wordexp: confirmed failing, cause inferred.** Every expansion that does not hit an error path
  returns 6 with no words (`a b  c`, `'a b'`, `$HOME`, `x*y`, `~`, empty), while the command
  substitution refusal under WRDE_NOCMD returns its correct code 3. `system("echo ...")` works, so
  the shell is fine. No file with "word" in its name is in `/usr/libexec`, which fits a missing
  helper in the minimal image (inference, not observed). All 19 `XSH.wordexp.*` results stand as
  failures of this system.
- **rewinddir: confirmed.** On a fresh stream a full read gives 5 entries, `rewinddir()` then a
  second read gives 0; rewinding before reading gives 5; after reading two entries and rewinding a
  read gives the remaining 3. `rewinddir()` does not reset the position. `seekdir()` to a saved
  start position does work (5). Both the root file system and the scratch disk are ext4, so this is
  the system's own behaviour with that driver.
- **rename: confirmed.** `rename("e", "d2/.")` returns 0 (expected EINVAL); `rename(file, "nf/")`
  gives ENOENT (expected ENOTDIR).
- **Pathname through a non-directory: confirmed.** `stat("f/..")` for a regular file `f` returns 0.
- **open(dir, O_RDONLY|O_CREAT): confirmed.** Returns a descriptor (4) instead of failing.
