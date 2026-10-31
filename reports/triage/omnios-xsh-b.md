# illumos triage, group xsh-b

Counts: TEST-BUG 0, SYSTEM 96, STD-AMBIG 6, ENV 0, UNKNOWN 0.

| ID | result | class | reason |
|---|---|---|---|
| XSH.mkdtemp.012 | FAIL | SYSTEM | mkstemp() shall fail with EINVAL for a template not ending in six X (mkdtemp page, ERRORS); it failed with errno 0 (no function may set errno to 0, XSH 2.3). |
| XSH.mkdtemp.015 | FAIL | SYSTEM | mkostemp() shall fail with EINVAL for a template without six trailing X; the call did not give -1 with EINVAL. |
| XSH.mktime.009 | FAIL | SYSTEM | mktime() shall return -1 with EOVERFLOW when the resulting tm_year does not fit an int; with tm_year INT_MAX and tm_mon 12 it returned a wrapped value. |
| XSH.mlock.004 | FAIL | SYSTEM | A failed munlock() makes no change to locks (mlock page, RETURN VALUE); after the ENOMEM failure the first page no longer reads as locked (via msync EBUSY, a shall-fail in msync; XSH.msync.004 is UNRESOLVED here, so either munlock or the msync oracle deviates, both required). |
| XSH.mmap.011 | FAIL | SYSTEM | Mapping a pipe where the file type is unsupported shall fail with ENODEV (mmap ERRORS); ENOSYS is not an allowed code. |
| XSH.mmap.017 | FAIL | SYSTEM | Stores past EOF in the last page must never reach the file (mmap DESCRIPTION); after ftruncate() extension the bytes read 0x7a, not zero. |
| XSH.mq_notify.009 | FAIL | SYSTEM | EBUSY is only for a process already registered; with no registration the call may give 0 or EINVAL (mq_notify ERRORS), it gave EBUSY. |
| XSH.newlocale.005 | FAIL | SYSTEM | A category whose locale data is unavailable shall give ENOENT (newlocale ERRORS); the system gave EINVAL for LC_ALL_MASK (a valid mask). |
| XSH.nftw.022 | FAIL | STD-AMBIG | EACCES (search denied on a component of path) conflicts with FTW_NS, which says fn is called when stat fails for lack of permission; the system took the FTW_NS route (nftw returned success). Reading is honestly ambiguous; the assertion is not marked disputed and should be. |
| XSH.open.039 | FAIL | SYSTEM | O_CREAT without O_DIRECTORY on an existing directory shall fail with EISDIR (open ERRORS); open() returned a descriptor. |
| XSH.perror.007 | FAIL | SYSTEM | perror() shall set the stream error indicator on write failure (perror DESCRIPTION); with fd 2 read-only and stderr unbuffered the indicator stayed clear. |
| XSH.popen.008 | FAIL | SYSTEM | Mode "re"/"we" shall set FD_CLOEXEC on the stream descriptor (popen DESCRIPTION); the flag was clear. |
| XSH.popen.015 | FAIL | SYSTEM | The command is passed to sh after "--" (popen DESCRIPTION); a command word starting with "-" was not run as a command. |
| XSH.posix_fallocate.001 | FAIL | SYSTEM | Valid offset/length on a regular file must succeed (or ENOTSUP if the file system lacks it); EINVAL is only for negative values (and may-fail zero length). System claims the ADV option but returned EINVAL. |
| XSH.posix_fallocate.002 | FAIL | SYSTEM | As posix_fallocate.001: EINVAL returned for valid arguments. |
| XSH.posix_fallocate.003 | FAIL | SYSTEM | As posix_fallocate.001: EINVAL returned for valid arguments. |
| XSH.posix_fallocate.010 | FAIL | SYSTEM | EFBIG is required when offset+len exceeds the file size limit; EINVAL was returned instead. |
| XSH.posix_spawn.014 | FAIL | SYSTEM | posix_spawnp() shall fail rather than fall back to running a non-executable-format file with sh (posix_spawn DESCRIPTION); the script ran (marker file created). |
| XSH.posix_spawn.027 | FAIL | SYSTEM | A dup2 action with equal descriptors must leave the descriptor open with FD_CLOEXEC clear (adddup2 DESCRIPTION); in the child descriptor 4 was closed. |
| XSH.posix_spawn_file_actions_addclose.013 | FAIL | SYSTEM | addopen() shall fail with EBADF for a descriptor >= {OPEN_MAX} (addclose ERRORS); it returned 0 (docs Q-90 notes a rationale tension but the ERRORS text is explicit). |
| XSH.posix_spawn_file_actions_adddup2.004 | FAIL | SYSTEM | With fildes == newfildes the child must have the descriptor open with FD_CLOEXEC clear (adddup2 DESCRIPTION); it was closed in the child. |
| XSH.posix_spawn_file_actions_adddup2.007 | FAIL | SYSTEM | adddup2() shall fail with EBADF for a descriptor >= {OPEN_MAX} (adddup2 ERRORS); it returned 0 (see Q-90). |
| XSH.pthread_cancel.003 | FAIL | SYSTEM | Cancelling a zombie thread is not an error (pthread_cancel DESCRIPTION); ESRCH was returned. |
| XSH.pthread_create.009 | FAIL | SYSTEM | The new thread inherits the creator's floating-point environment (pthread_create DESCRIPTION), which includes status flags; FE_DIVBYZERO raised in the creator was clear in the new thread (rounding direction passed). |
| XSH.pthread_key_create.005 | FAIL | SYSTEM | Creating more than {PTHREAD_KEYS_MAX} keys shall fail with EAGAIN (pthread_key_create ERRORS); 129 keys were created against a reported limit of 128. |
| XSH.pthread_kill.004 | FAIL | SYSTEM | pthread_kill() of a zombie thread is not an error and implementations returning ESRCH do not conform (RATIONALE); ESRCH was returned. |
| XSH.pthread_mutex_getprioceiling.001 | FAIL | SYSTEM | After setprioceiling the mutex is released; a trylock by another thread must give 0 (no listed trylock error fits); it returned EPERM, not a documented trylock error. |
| XSH.pthread_mutex_getprioceiling.002 | FAIL | SYSTEM | getprioceiling() shall fail with EINVAL for a PTHREAD_PRIO_NONE mutex (ERRORS); it returned 0. |
| XSH.ptsname.008 | FAIL | STD-AMBIG | A non-manager descriptor "may" give EINVAL or ENOTTY; ENXIO is neither, but XSH 2.3 only forbids a different number for a condition whose error is required, so whether another code is allowed for a may-fail condition is open. Assertion not marked disputed. |
| XSH.realloc.004 | FAIL | SYSTEM | Obsolescent zero-size behaviour: a null return must come with errno EINVAL (realloc RETURN VALUE); errno was 0. |
| XSH.realloc.007 | FAIL | SYSTEM | As realloc.004 for reallocarray() with a zero count or size: null with errno 0. |
| XSH.regcomp.014 | FAIL | SYSTEM | Bracket expression "[--@]" is a valid range from hyphen to @ (XBD 9.3.5, the page gives this example); regcomp() returned an error (10). |
| XSH.regcomp.017 | FAIL | SYSTEM | BRE "\(a\(b\)*\)*\2" shall fail to match "abab" (XBD 9.3.6 example); it matched [0,4]. |
| XSH.regcomp.027 | FAIL | SYSTEM | With REG_ICASE a string matching case-sensitively still matches with case counterparts (XBD 4.1); "\(a\)\1" on "aA" gave no match. |
| XSH.regcomp.033 | FAIL | SYSTEM | The ERE repetition modifier "?" after a duplication symbol (XBD 9.4.6, Issue 8) must compile; ".*?c" returned an error (12). |
| XSH.remquo.002 | FAIL | SYSTEM | remquol(9,4) must store a value of magnitude congruent to 2 modulo 2^n (n>=3) with sign of x/y (remquo DESCRIPTION); 0 was stored. |
| XSH.rename.013 | FAIL | SYSTEM | rename("f","nf/") for a non-directory shall fail with ENOTDIR (rename ERRORS); it succeeded. |
| XSH.rename.014 | FAIL | SYSTEM | rename() of a directory to a nonexistent "nd/" is covered by the shall-fail for a new name with a trailing slash not resolving to an existing directory (rename DESCRIPTION); it succeeded. |
| XSH.setegid.004 | FAIL | SYSTEM | setegid() of an ID that is neither real nor saved, without privileges, shall fail with EPERM (setegid ERRORS); the unprivileged probe was refused but setegid(effective ID) returned 0. |
| XSH.seteuid.004 | FAIL | SYSTEM | As setegid.004: seteuid() to the current effective ID, which is neither real nor saved, returned 0 without privileges instead of EPERM. |
| XSH.sinh.004 | FAIL | SYSTEM | Overflow shall raise FE_OVERFLOW where MATH_ERREXCEPT is set (sinh ERRORS); sinhf() did not raise it. |
| XSH.strfmon.004 | FAIL | SYSTEM | With neither + nor ( in the POSIX locale the style is that of + with negative sign as "-" (strfmon DESCRIPTION); "%.0n" of -123 gave "(123)". |
| XSH.strfmon.005 | FAIL | SYSTEM | The + flag shall fail with EINVAL when both signs are empty, as in the POSIX locale (strfmon ERRORS); the call returned 3. |
| XSH.strfmon.006 | FAIL | SYSTEM | As strfmon.005 in a locale whose localeconv() signs are both empty: returned 3 with errno 0. |
| XSH.strfmon.010 | FAIL | SYSTEM | Default sign style (see strfmon.004) must put "-" before the fill; the system used parentheses "(**123)". |
| XSH.strfmon.011 | FAIL | SYSTEM | Positive and negative formats must be padded to equal length with a left precision (strfmon DESCRIPTION); "  123" vs "(  123)" differ (also due to the wrong default sign style). |
| XSH.strfmon.013 | FAIL | SYSTEM | strfmon_l() with a "C" locale object must equal strfmon() in the POSIX locale; it gave "(123)" for -123 (see strfmon.004). |
| XSH.strftime.007 | FAIL | SYSTEM | Issue 8 strftime flags 0 and + with a field width must work (strftime DESCRIPTION); "%+4Y" was output as literal text "...19704Y". |
| XSH.strftime.014 | FAIL | SYSTEM | strftime() shall fail with ERANGE when result plus NUL exceeds maxsize (strftime ERRORS); returned 0 with errno 0. |
| XSH.strptime.003 | FAIL | SYSTEM | %d accepts [01,31] only (strptime DESCRIPTION); "0," was accepted. |
| XSH.strptime.005 | FAIL | SYSTEM | %p and %I in either order set tm_hour on the 24-hour clock (strptime %I/%p); with "%p %I" "AM 12" gave 12 instead of 0. |
| XSH.strptime.007 | FAIL | SYSTEM | %Y accepts leading zeros (strptime %Y); "0070," returned null. |
| XSH.strptime.008 | FAIL | SYSTEM | With both %C and %y the year is built from both in either order (strptime %y/%C); "%y %C" gave 100 for "24 20". |
| XSH.strptime.009 | FAIL | SYSTEM | %F converts the year as %Y with unlimited width (strptime %F); "12345-06-07" returned null. |
| XSH.strptime.010 | FAIL | SYSTEM | %c is the locale date-time format, which in the POSIX locale ends in the year; input missing the year must fail, but it succeeded. |
| XSH.strptime.012 | FAIL | SYSTEM | An ordinary format character must equal the input character (strptime DESCRIPTION); empty input against "a" did not return null. |
| XSH.strptime.013 | FAIL | SYSTEM | %u (1..7) is a listed conversion (strptime %u); "1," returned null. |
| XSH.strptime.014 | FAIL | SYSTEM | %V [01,53] is a listed conversion (strptime %V); "01," returned null. |
| XSH.strptime.015 | FAIL | SYSTEM | %s needs a decimal number; "abc" is no match so strptime() must fail, but it did not. |
| XSH.strptime.017 | FAIL | SYSTEM | A field width limits the characters read by %Y (strptime DESCRIPTION); "%2Y" on "2024" returned null. |
| XSH.strptime.018 | FAIL | SYSTEM | %Ow is a listed modified conversion and falls back to %w where no alternative exists (strptime); "3," returned null. |
| XSH.strptime.019 | FAIL | SYSTEM | Empty input where a weekday name is read finds no match, so strptime() must fail; it did not. |
| XSH.sysconf.005 | FAIL | SYSTEM | sysconf(_SC_VERSION) must be 202405 for this edition (unistd.h version macros); the system reported 200112. |
| XSH.sysconf.007 | FAIL | SYSTEM | Option values must be -1 or 202405 (unistd.h); _SC_ADVISORY_INFO returned 200112. |
| XSH.sysconf.009 | FAIL | SYSTEM | Only _SC_OPEN_MAX may change after setrlimit() (sysconf RETURN VALUE); _SC_STREAM_MAX changed from 65536 to 65535. |
| XSH.system.006 | FAIL | SYSTEM | The command is passed to sh after "--" (system DESCRIPTION); "-pcdash" ended with status 2 (option error) instead of running. |
| XSH.tcdrain.005 | FAIL | SYSTEM | tcdrain() on a non-terminal shall fail with ENOTTY (tcdrain ERRORS); a pipe read end gave EINVAL. |
| XSH.tcdrain.007 | FAIL | SYSTEM | A background caller ignoring SIGTTOU is allowed to proceed (tcdrain DESCRIPTION) and the call must return 0; with SIGTTOU ignored (blocked variant passes) it failed with EINVAL, an unlisted error. Running the ignored variant first would rule out a leftover from the first session. |
| XSH.tcflow.007 | FAIL | SYSTEM | tcflow() on a non-terminal shall fail with ENOTTY; a pipe read end gave EINVAL. |
| XSH.tcflow.009 | FAIL | SYSTEM | As tcdrain.007 for tcflow(): EINVAL with SIGTTOU ignored in a background process group. |
| XSH.tcflush.008 | FAIL | SYSTEM | tcflush() on a non-terminal shall fail with ENOTTY; a pipe read end gave EINVAL. |
| XSH.tcflush.010 | FAIL | SYSTEM | As tcdrain.007 for tcflush(): EINVAL with SIGTTOU ignored in a background process group. |
| XSH.tcgetattr.003 | FAIL | SYSTEM | tcgetattr() on a non-terminal shall fail with ENOTTY; a pipe read end gave EINVAL. |
| XSH.tcgetpgrp.002 | FAIL | SYSTEM | tcgetpgrp() on a file that is not the controlling terminal shall fail with ENOTTY; the pipe read end gave EINVAL. |
| XSH.tcgetsid.004 | FAIL | SYSTEM | tcgetsid() on a file that is not the controlling terminal shall fail with ENOTTY; a pipe end gave EINVAL. |
| XSH.tcsendbreak.004 | FAIL | SYSTEM | tcsendbreak() on a non-terminal shall fail with ENOTTY; a pipe read end gave EINVAL. |
| XSH.tcsendbreak.006 | FAIL | SYSTEM | As tcdrain.007 for tcsendbreak(): EINVAL with SIGTTOU ignored in a background process group. |
| XSH.tcsetattr.008 | FAIL | SYSTEM | tcsetattr() on a non-terminal shall fail with ENOTTY; a pipe read end gave EINVAL. |
| XSH.tcsetpgrp.002 | FAIL | SYSTEM | tcsetpgrp() on a file that is not the controlling terminal shall fail with ENOTTY; the pipe read end gave EINVAL. |
| XSH.timer_getoverrun.001 | FAIL | SYSTEM | it_value from timer_gettime() is zero for a disarmed timer (timer_gettime DESCRIPTION); after timer_settime() with zero value it still reported time left. |
| XSH.ttyname.003 | FAIL | SYSTEM | ERANGE only when namesize is smaller than the string including NUL (ttyname ERRORS); namesize equal to length+1 returned ERANGE. |
| XSH.ungetc.008 | FAIL | SYSTEM | ungetc() converts c to unsigned char and returns the converted value (ungetc RETURN VALUE); ungetc(0x1ff) returned 0x1ff, not 0xff. |
| XSH.unlockpt.002 | FAIL | STD-AMBIG | Both error cases for a non-manager are "may" errors with EINVAL listed; the system gave ENOTTY (not listed). Same open question as ptsname.008 about other codes for may-fail conditions. Assertion not marked disputed. |
| XSH.vfwprintf.001 | FAIL | SYSTEM | vswprintf() must store wide characters (vfwprintf/fwprintf DESCRIPTION); the array held the output packed as bytes (10 characters made 3 elements). Probable library/header variant selection under the 2024 feature macro; the system must give the standard behaviour there. |
| XSH.vfwprintf.002 | FAIL | SYSTEM | As vfwprintf.001: vswprintf() output "b a" packed as bytes into one element. |
| XSH.vfwprintf.003 | FAIL | SYSTEM | As vfwprintf.001: the array holds one packed element then the untouched fill. |
| XSH.vfwprintf.004 | FAIL | SYSTEM | vswprintf() shall return a negative value (EOVERFLOW) when output does not fit n (fwprintf RETURN VALUE/ERRORS); it returned 6 for n 4. |
| XSH.vfwprintf.007 | FAIL | SYSTEM | As vfwprintf.001: vswprintf(%s) of a multibyte character did not fill one wide element. |
| XSH.waitid.018 | INSPECT | STD-AMBIG | Already class disputed: the page does not state that a call with none of WEXITED/WSTOPPED/WCONTINUED has an invalid options value; the system accepted it (returned 0). Stays INSPECT. |
| XSH.wcsftime.004 | FAIL | SYSTEM | wcsftime() stores a terminating null wide character even for an empty result (wcsftime DESCRIPTION/RETURN VALUE); with an empty format it was not written. |
| XSH.wcsftime.006 | FAIL | SYSTEM | Issue 8 flags and field width as in strftime apply to wcsftime(); "%010F" returned 3 instead of 10 (same cause as strftime.007). |
| XSH.wcsrtombs.005 | FAIL | SYSTEM | On an invalid code *src must point just past the last wide character converted (wcsrtombs DESCRIPTION); it was left elsewhere. |
| XSH.wordexp.003 | FAIL | SYSTEM | Word expansion removes an unquoted empty field (XCU 2.6 via wordexp); "${PCW_E-d}" with an empty set variable gave one word instead of none. |
| XSH.wordexp.008 | INSPECT | STD-AMBIG | Already class disputed (Q-205): wordexp failed with we_wordc left at its preset 99 rather than 0; this is the second reading of "number of expanded words is 0". Stays INSPECT. |
| XSH.wordexp.009 | INSPECT | STD-AMBIG | Already class disputed (Q-205): same as wordexp.008 for unquoted parentheses and braces. |
| XSH.wordexp.011 | FAIL | SYSTEM | An unterminated quote shall give WRDE_SYNTAX (wordexp ERRORS); the call returned 0. |
| XSH.wordexp.012 | FAIL | SYSTEM | With WRDE_NOCMD command substitution shall fail with WRDE_CMDSUB (wordexp DESCRIPTION/ERRORS); the call returned 0. |
| XSH.wordexp.014 | FAIL | SYSTEM | Unquoted empty expansion of an empty set variable is removed (XCU 2.6 via wordexp); "$PCW_E" gave one word instead of none. |
| XSH.wordexp.016 | FAIL | SYSTEM | A failed WRDE_APPEND call must return WRDE_SYNTAX for an unterminated quote; it returned 0 (same cause as wordexp.011). |
| XSH.wordexp.017 | FAIL | SYSTEM | As wordexp.014: with WRDE_DOOFFS and an empty expansion we_wordc must be 0 but was 1. |
| XSH.write.008 | FAIL | SYSTEM | XSI: a write failing for the file size limit also generates SIGXFSZ for the thread (write ERRORS); EFBIG was returned but no SIGXFSZ was pending (blocked and handled in the test child). Possible delivery timing on the system is not excluded. |
| XSH.y0.004 | FAIL | SYSTEM | yn(n,0) with n negative and odd shall return +HUGE_VAL (y0 RETURN VALUE); yn(-1,0.0) returned -inf. |
