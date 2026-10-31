# illumos triage, group xsh-a

Counts: TEST-BUG 0, SYSTEM 88, STD-AMBIG 14, ENV 0, UNKNOWN 0.

| ID | result | class | reason |
|---|---|---|---|
| XSH.2.11-data-types.002 | FAIL | SYSTEM | <dirent.h> must define reclen_t as an unsigned type (XBD sys/types.h, dirent.h); the name is undeclared, so the test does not compile |
| XSH.2.11-data-types.004 | INSPECT | STD-AMBIG | already disputed (Q-200): id_t max 2147483647 is below uid_t max; width reading vs range reading of "at least the largest of" |
| XSH.2.2-compilation-environment.004 | FAIL | SYSTEM | with _XOPEN_SOURCE 800, XSH 2.2.1 forbids extra symbols; <strings.h> (no index/rindex/bcopy in the 2024 header) still declares index etc., so a file-scope object of that name clashes |
| XSH.2.2-compilation-environment.006 | FAIL | SYSTEM | XSH 2.2.1.2: _XOPEN_SOURCE 800 enables all _POSIX_C_SOURCE 202405L functionality; _POSIX_VERSION stays below 202405L |
| XSH.2.4.3-signal-actions.019 | FAIL | SYSTEM | XBD signal.h: for SIGCHLD, si_uid is the real user ID of the sending (child) process; si_uid was 1 for a root child (real uid 0) |
| XSH.2.4.3-signal-actions.021 | FAIL | SYSTEM | mmap: a reference to whole pages past the end of the object must raise SIGBUS; the read of the second page of a one-page file raised nothing (confirm with the build/run output, which is empty) |
| XSH.fwide.002 | FAIL | STD-AMBIG | fwide() "first attempts" to make an unoriented stream wide; the text never says when the attempt may fail, so a result of 0 (still no orientation) cannot be called wrong; not currently disputed, should be |
| XSH.fwide.003 | FAIL | STD-AMBIG | same as fwide.002 for byte orientation; not currently disputed, should be |
| XSH.fgetwc.007 | FAIL | SYSTEM | fgetwc RETURN VALUE: on an error other than an encoding error the error indicator must be set; EBADF on a write-only stream left it clear |
| XSH.2.7-xsi-ipc.001 | INSPECT | STD-AMBIG | already disputed: XSH 2.7 says identifiers are positive, msgget() says non-negative; the system returned 0 |
| XSH.msync.007 | INSPECT | STD-AMBIG | already disputed (Q-62): flags 0 returned 0 instead of EINVAL; "exactly one of MS_ASYNC/MS_SYNC" can be a rule on the application |
| XSH.msync.008 | INSPECT | STD-AMBIG | already disputed (Q-62): MS_INVALIDATE alone returned 0 instead of EINVAL |
| XSH.2.8-realtime.005 | FAIL | SYSTEM | XSH 2.8.3.1: memory locks are not inherited across fork(); msync EBUSY ("addresses are locked") still fired (or msync failed) in the child; slight doubt because EBUSY on a shared page could be read as page-level, and the output file is empty |
| XSH.2.9-threads.029 | FAIL | SYSTEM | XSH 2.9.7: write() calls on one descriptor are atomic with respect to each other, including the shared offset; blocks overwrote each other (file 20230144 bytes, not 24576000) |
| XSH.2.9-threads.030 | FAIL | SYSTEM | XSH 2.9.7: read() calls on one descriptor are atomic with respect to each other; records were read more than once (97985 for 60000) |
| XSH.2.9-threads.035 | FAIL | SYSTEM | XSH 2.9.9: a process-shared rwlock may be used through another mapping; pthread_rwlock_unlock through the second mapping returned EPERM |
| XSH._Exit.019 | INSPECT | STD-AMBIG | already disputed (Q-32): outstanding aio at exit may be canceled; file held 0 bytes |
| XSH.aio_suspend.003 | FAIL | SYSTEM | aio_suspend: if a listed request is already complete the call returns without suspending (and returns 0); it returned -1/EAGAIN with a zero timeout |
| XSH.atomic_fetch_add.004 | FAIL | SYSTEM | the atomic fast16 types do not wrap at the limit given by UINT_FAST16_MAX (type is wider than the macro says); <stdint.h> limit macros must match the types (max + 1 gave 0x10000) |
| XSH.atomic_fetch_add.005 | FAIL | SYSTEM | same cause as .004 for int_fast16_t (INT_FAST16_MAX does not match the type: 0x7fff + 1 gave 0x8000) |
| XSH.atomic_fetch_add.006 | INSPECT | STD-AMBIG | already disputed (Q-156): pointer arithmetic units; the system moved a short pointer by bytes, not objects |
| XSH.catopen.008 | FAIL | SYSTEM | XBD 8.2 NLSPATH: a trailing colon is the same as %N; leading and doubled colons worked but "pc-no-dir/%N:" failed with ENOENT |
| XSH.catopen.010 | FAIL | SYSTEM | catopen() failure must set errno (RETURN VALUE) and ENOENT is the error for files of invalid format; errno was left 0 |
| XSH.chown.008 | INSPECT | STD-AMBIG | already disputed (Q-25): chown(-1,-1) of another user's file succeeded |
| XSH.cosh.005 | FAIL | SYSTEM | cosh: overflow must raise FE_OVERFLOW where math_errhandling has MATH_ERREXCEPT; the float variant did not |
| XSH.exec.010 | FAIL | SYSTEM | exec: FD_CLOFORK, if set, shall remain set across exec; it was clear in the new image |
| XSH.fcntl.026 | FAIL | SYSTEM | fcntl: negative l_len covers l_start+l_len through l_start-1 (10..19 here); byte 10 was not locked |
| XSH.fdopen.005 | FAIL | SYSTEM | fdopen: mode letter "e" must set FD_CLOEXEC; it stayed clear for "re" |
| XSH.fdopen.013 | INSPECT | STD-AMBIG | already disputed (Q-36): "a" stream's initial ftell() was 6 (end), not the descriptor offset 2 |
| XSH.fgetc.005 | FAIL | SYSTEM | fgetc RETURN VALUE: on any error the error indicator is set; EBADF on a write-only stream left it clear |
| XSH.fgets.006 | FAIL | SYSTEM | fgets: an error (EAGAIN after "ab" on a non-blocking pipe) must give a null pointer; s was returned |
| XSH.fgetws.007 | FAIL | SYSTEM | fgetws RETURN VALUE: on error the error indicator is set; EBADF on a write-only stream left it clear |
| XSH.fgetws.009 | FAIL | SYSTEM | fgetws: an encoding error must give a null pointer; a pointer was returned for "ab" then 0xff |
| XSH.fileno.003 | FAIL | SYSTEM | fileno shall fail with EBADF for a stream not associated with a file (memory stream); it returned -1 but left errno 0 |
| XSH.fmemopen.014 | FAIL | SYSTEM | fmemopen: a read may not go beyond the end position (3); a read at 5 returned data (97) instead of EOF |
| XSH.fmemopen.021 | FAIL | SYSTEM | fmemopen: the terminating null is written at the new end only if it fits; at max_size it overwrote the last data byte |
| XSH.fmtmsg.006 | FAIL | SYSTEM | fmtmsg: the text and action strings are not limited to a specific size; the 3000-byte text was not written in full |
| XSH.fnmatch.009 | FAIL | SYSTEM | XCU 2.14.1 character class expressions: fnmatch("[[:alpha:]]","a",0) must match; returned no-match |
| XSH.fnmatch.010 | FAIL | SYSTEM | XBD 9.3.5 collating symbol: fnmatch("[[.a.]b]","a",0) must match; returned no-match |
| XSH.fnmatch.021 | FAIL | SYSTEM | FNM_CASEFOLD with XBD 4.1: "A" must match "[[:lower:]]"; returned no-match |
| XSH.fnmatch.022 | FAIL | SYSTEM | same as fnmatch.021 for FNM_IGNORECASE |
| XSH.fprintf.038 | FAIL | SYSTEM | %.3ls must not read past the 3 wide characters (no null needed when precision is reached); the call read the PROT_NONE page and was killed by SIGSEGV |
| XSH.fprintf.058 | INSPECT | STD-AMBIG | already disputed (Q-65): %a of infinity gave "infinity" while %f gave "inf" |
| XSH.fscanf.015 | FAIL | SYSTEM | fscanf: "0x" is a prefix of a match but not a match, so %x on "0xg" is a matching failure (0); it returned 1 |
| XSH.fscanf.037 | FAIL | SYSTEM | fscanf: the m modifier with s, c and [ must allocate; sscanf returned 0 (not 3) |
| XSH.fscanf.038 | FAIL | SYSTEM | fscanf: the m modifier with ls and l[ must allocate wide strings; sscanf returned 0 (not 2) |
| XSH.fscanf.043 | FAIL | SYSTEM | fscanf RETURN VALUE: EOF only if input ends before the first conversion completes; %*d completed, so 0 is required; -1 returned |
| XSH.fseek.016 | INSPECT | STD-AMBIG | already disputed (Q-35): fseeko() beyond off_t failed with EINVAL, not EOVERFLOW |
| XSH.fwprintf.001 | FAIL | SYSTEM | swprintf must store the wide characters of the formatted text in the array; the array held garbage while fwprintf/wprintf passed (output files empty, cause not seen) |
| XSH.fwprintf.002 | FAIL | SYSTEM | swprintf must store the wide characters of the formatted text in the array; the array held garbage while fwprintf/wprintf passed (output files empty, cause not seen) |
| XSH.fwprintf.003 | FAIL | SYSTEM | swprintf must store the wide characters of the formatted text in the array; the array held garbage while fwprintf/wprintf passed (output files empty, cause not seen) |
| XSH.fwprintf.004 | FAIL | SYSTEM | swprintf must store the wide characters of the formatted text in the array; the array held garbage while fwprintf/wprintf passed (output files empty, cause not seen) |
| XSH.fwprintf.005 | FAIL | SYSTEM | swprintf must store the wide characters of the formatted text in the array; the array held garbage while fwprintf/wprintf passed (output files empty, cause not seen) |
| XSH.fwprintf.006 | FAIL | SYSTEM | swprintf must store the wide characters of the formatted text in the array; the array held garbage while fwprintf/wprintf passed (output files empty, cause not seen) |
| XSH.fwprintf.007 | FAIL | SYSTEM | swprintf must store the wide characters of the formatted text in the array; the array held garbage while fwprintf/wprintf passed (output files empty, cause not seen) |
| XSH.fwprintf.009 | FAIL | SYSTEM | swprintf must store the wide characters of the formatted text in the array; the array held garbage while fwprintf/wprintf passed (output files empty, cause not seen) |
| XSH.fwprintf.012 | FAIL | SYSTEM | swprintf must store the wide characters of the formatted text in the array; the array held garbage while fwprintf/wprintf passed (output files empty, cause not seen) |
| XSH.fwprintf.013 | FAIL | SYSTEM | swprintf must return a negative value when the output would exceed n; it returned 6 |
| XSH.fwprintf.014 | FAIL | SYSTEM | swprintf with n 0 must fail with EOVERFLOW; errno was 0 |
| XSH.fwprintf.015 | FAIL | SYSTEM | swprintf must store the wide characters of the formatted text in the array; the array held garbage while fwprintf/wprintf passed (output files empty, cause not seen) |
| XSH.fwprintf.019 | FAIL | SYSTEM | swprintf of %s with a multibyte string must give the converted wide character; the array content was wrong |
| XSH.fwprintf.020 | FAIL | SYSTEM | swprintf("[%3ls]") must pad with spaces in wide characters; the array content was wrong |
| XSH.fwprintf.021 | FAIL | SYSTEM | swprintf must store the wide characters of the formatted text in the array; the array held garbage while fwprintf/wprintf passed (output files empty, cause not seen) |
| XSH.fwprintf.022 | FAIL | SYSTEM | swprintf must store the wide characters of the formatted text in the array; the array held garbage while fwprintf/wprintf passed (output files empty, cause not seen) |
| XSH.fwscanf.017 | FAIL | SYSTEM | swscanf with the m modifier and ls must allocate a wide string; returned 0 (m is required) |
| XSH.fwscanf.018 | FAIL | SYSTEM | swscanf(%ls%n) must consume 2 wide characters; n was 1 (the wide input seems read as bytes) |
| XSH.getdate.006 | FAIL | SYSTEM | getdate: an impossible date such as 31 February is an invalid input specification (getdate_err 8); a pointer was returned |
| XSH.getdate.007 | FAIL | SYSTEM | getdate: with no time given the current time of day is used; 00:00:00 was returned |
| XSH.getdate.009 | FAIL | SYSTEM | getdate: %y 69 means 1969 (pre-Epoch is representable in a 64-bit time_t); fails with getdate_err 8 |
| XSH.getdate.011 | FAIL | SYSTEM | getdate: %% must be supported; "10%11" with "%d%%%m" failed with getdate_err 8 |
| XSH.getdate.012 | FAIL | SYSTEM | getdate: %I:%M:%S %p with no date must work (hour taken from now into the future); failed with getdate_err 8 |
| XSH.getdate.013 | FAIL | SYSTEM | getdate: %C with %y gives the century given (1905); the result was 2005 (tm_year 105) |
| XSH.getdate.014 | FAIL | SYSTEM | getdate: a weekday alone is valid (page examples); failed with getdate_err 8 |
| XSH.getdate.015 | FAIL | SYSTEM | getdate: a month alone is valid (page examples); failed with getdate_err 8 |
| XSH.getdate.016 | FAIL | SYSTEM | getdate: a time of day alone is valid (page examples); failed with getdate_err 8 |
| XSH.getdate.017 | FAIL | SYSTEM | getdate: extra white space in the input is ignored; leading white space gave getdate_err 7 |
| XSH.getdate.018 | FAIL | SYSTEM | getdate: %X (time only in the POSIX locale) must work; failed with getdate_err 8 |
| XSH.getdate.019 | FAIL | SYSTEM | getdate: %Z must accept the expected zone (TZ names UTC); failed with getdate_err 8, like the other time-only inputs |
| XSH.getgrgid.002 | FAIL | SYSTEM | not-found lookups must leave errno unchanged (RETURN VALUE); errno was set to 0 |
| XSH.getgrnam.002 | FAIL | SYSTEM | not-found lookups must leave errno unchanged (RETURN VALUE); errno was set to 0 |
| XSH.getopt.006 | FAIL | SYSTEM | getopt: a leading "+" has no effect, so "+:a:" still means a leading colon (return ":"); returned "?" |
| XSH.getopt.007 | FAIL | SYSTEM | getopt: a leading colon after an optional "+" suppresses the diagnostic; a diagnostic was written |
| XSH.getopt.008 | FAIL | SYSTEM | getopt: "+" is only ignored at the start of optstring; "-+" with "+ab" must give "?" but "+" was accepted as an option |
| XSH.getpwnam.002 | FAIL | SYSTEM | not-found lookups must leave errno unchanged (RETURN VALUE); errno was set to 0 |
| XSH.getpwuid.002 | FAIL | SYSTEM | not-found lookups must leave errno unchanged (RETURN VALUE); errno was set to 0 |
| XSH.getrlimit.014 | FAIL | SYSTEM | getrlimit/ftruncate: exceeding RLIMIT_FSIZE makes the operation fail with EFBIG; ftruncate to 1001 with a limit of 1000 succeeded |
| XSH.glob.006 | FAIL | SYSTEM | glob GLOB_NOCHECK: the list is the pattern itself, unchanged; "no\?x" came back as "no?x" |
| XSH.glob.016 | FAIL | SYSTEM | XCU 2.14.3: a slash inside brackets makes the bracket ordinary; "a[b/c]d" must match the literal path a[b/c]d; returned GLOB_NOMATCH |
| XSH.glob.029 | FAIL | SYSTEM | glob: with GLOB_ERR a failure to read a directory returns GLOB_ABORTED even with a null errfunc; it did not |
| XSH.grantpt.003 | FAIL | SYSTEM | XSH 2.3: the error for a described condition is the listed one (EINVAL: not a manager pty); a regular file gave ENOTTY (slight doubt: "may fail" error with another code) |
| XSH.iconv_open.002 | INSPECT | STD-AMBIG | already disputed (Q-128): //IGNORE suffix refused with EINVAL |
| XSH.inet_ntop.010 | FAIL | SYSTEM | inet_ntop: a buffer too small for the text (size 15 for 255.255.255.255, needs 16) fails with ENOSPC; it returned a pointer |
| XSH.link.011 | FAIL | SYSTEM | link: path2 with a trailing slash naming a new file must fail with ENOENT or ENOTDIR; link("f","new/") succeeded |
| XSH.localeconv.001 | FAIL | SYSTEM | XBD 7.3.3/7.3.4 tables: in the POSIX locale localeconv() grouping and mon_grouping are empty strings; they are not (CHAR_MAX-style values) |
| XSH.localeconv.002 | FAIL | SYSTEM | XBD 7.3.3/7.3.4 tables: in the POSIX locale localeconv() grouping and mon_grouping are empty strings; they are not (CHAR_MAX-style values) |
| XSH.localeconv.004 | FAIL | SYSTEM | XBD 7.3.3/7.3.4 tables: in the POSIX locale localeconv() grouping and mon_grouping are empty strings; they are not (CHAR_MAX-style values) |
| XSH.localeconv.006 | FAIL | SYSTEM | XBD 7.3.3/7.3.4 tables: in the POSIX locale localeconv() grouping and mon_grouping are empty strings; they are not (CHAR_MAX-style values) |
| XSH.localtime.005 | FAIL | SYSTEM | XBD 8.3 TZ: rule times may be negative or up to 167 hours; "M3.2.0/-1,M11.1.0/26" was not honoured (result as if no offset) |
| XSH.localtime.006 | FAIL | SYSTEM | XBD 8.3 TZ: "0/0,J365/25" means DST all year; standard time was used at the year boundary |
| XSH.lseek.011 | INSPECT | STD-AMBIG | already disputed (Q-2): offset overflow gave EINVAL, not EOVERFLOW |
| XSH.mkdir.003 | FAIL | SYSTEM | mkdir (XSI): S_ISVTX of the mode is applied to the new directory; mode 01777 gave 0777 |
| XSH.mkdtemp.005 | FAIL | SYSTEM | mkdtemp: a template not ending in XXXXXX fails with EINVAL; mkdtemp("") left errno 0 |
