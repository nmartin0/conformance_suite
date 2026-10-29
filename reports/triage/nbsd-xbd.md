# Second-system triage, XBD group

Every non-PASS line of the XBD list (127) was decided against the standard,
not against either system. "2nd" is the verdict on the second system, "Linux"
the verdict of the latest Linux run. Test sources are under modules/xbd.

| ID | 2nd | Linux | class | reason (and for TEST-BUG / STD-AMBIG what changed, with the Linux re-run) |
|---|---|---|---|---|
| XBD.10.1-directory-structure-and-files.005 | FAIL | PASS | TEST-BUG | Relied on the pty helper giving the child a controlling terminal, which is implementation-defined (XBD 11.1.3, 10.1 /dev/tty "if any"); added pcpty -c / pc_pty_run -c so a refusal is UNRESOLVED. Re-run on Linux: PASS. |
| XBD.11.1-interface-characteristics.003 | UNRESOLVED | PASS | ENV | Acquiring a controlling terminal by opening the subsidiary side is implementation-defined (XBD 11.1.3); the test already reports UNRESOLVED, no change. |
| XBD.11.1-interface-characteristics.004 | UNRESOLVED | PASS | ENV | Acquiring a controlling terminal by opening the subsidiary side is implementation-defined (XBD 11.1.3); the test already reports UNRESOLVED, no change. |
| XBD.11.1-interface-characteristics.005 | UNRESOLVED | PASS | ENV | Acquiring a controlling terminal by opening the subsidiary side is implementation-defined (XBD 11.1.3); the test already reports UNRESOLVED, no change. |
| XBD.11.1-interface-characteristics.006 | UNRESOLVED | PASS | ENV | Acquiring a controlling terminal by opening the subsidiary side is implementation-defined (XBD 11.1.3); the test already reports UNRESOLVED, no change. |
| XBD.11.1-interface-characteristics.007 | UNRESOLVED | PASS | ENV | Acquiring a controlling terminal by opening the subsidiary side is implementation-defined (XBD 11.1.3); the test already reports UNRESOLVED, no change. |
| XBD.11.1-interface-characteristics.008 | UNRESOLVED | PASS | ENV | Acquiring a controlling terminal by opening the subsidiary side is implementation-defined (XBD 11.1.3); the test already reports UNRESOLVED, no change. |
| XBD.11.1-interface-characteristics.009 | UNRESOLVED | PASS | ENV | Acquiring a controlling terminal by opening the subsidiary side is implementation-defined (XBD 11.1.3); the test already reports UNRESOLVED, no change. |
| XBD.11.1-interface-characteristics.010 | UNRESOLVED | PASS | ENV | Acquiring a controlling terminal by opening the subsidiary side is implementation-defined (XBD 11.1.3); the test already reports UNRESOLVED, no change. |
| XBD.11.1-interface-characteristics.011 | UNRESOLVED | PASS | ENV | Acquiring a controlling terminal by opening the subsidiary side is implementation-defined (XBD 11.1.3); the test already reports UNRESOLVED, no change. |
| XBD.11.1-interface-characteristics.012 | UNRESOLVED | PASS | ENV | Acquiring a controlling terminal by opening the subsidiary side is implementation-defined (XBD 11.1.3); the test already reports UNRESOLVED, no change. |
| XBD.11.1-interface-characteristics.013 | UNRESOLVED | PASS | ENV | Acquiring a controlling terminal by opening the subsidiary side is implementation-defined (XBD 11.1.3); the test already reports UNRESOLVED, no change. |
| XBD.11.1-interface-characteristics.014 | UNRESOLVED | PASS | ENV | Acquiring a controlling terminal by opening the subsidiary side is implementation-defined (XBD 11.1.3); the test already reports UNRESOLVED, no change. |
| XBD.11.1-interface-characteristics.015 | UNRESOLVED | PASS | ENV | Acquiring a controlling terminal by opening the subsidiary side is implementation-defined (XBD 11.1.3); the test already reports UNRESOLVED, no change. |
| XBD.11.1-interface-characteristics.016 | UNRESOLVED | PASS | ENV | Acquiring a controlling terminal by opening the subsidiary side is implementation-defined (XBD 11.1.3); the test already reports UNRESOLVED, no change. |
| XBD.11.1-interface-characteristics.035 | UNRESOLVED | PASS | ENV | Acquiring a controlling terminal by opening the subsidiary side is implementation-defined (XBD 11.1.3); the test already reports UNRESOLVED, no change. |
| XBD.11.1-interface-characteristics.036 | UNRESOLVED | PASS | ENV | Acquiring a controlling terminal by opening the subsidiary side is implementation-defined (XBD 11.1.3); the test already reports UNRESOLVED, no change. |
| XBD.11.1-interface-characteristics.037 | UNRESOLVED | PASS | ENV | Acquiring a controlling terminal by opening the subsidiary side is implementation-defined (XBD 11.1.3); the test already reports UNRESOLVED, no change. |
| XBD.11.1-interface-characteristics.045 | FAIL | FAIL | SYSTEM | The output written before the last close never reaches the manager side (XBD 11.1.11: the last close sends pending output); Linux fails the other half (input not discarded), so the test is exercised on both sides. |
| XBD.11.2-terminal-parameters.021 | UNRESOLVED | PASS | ENV | Same implementation-defined acquisition of a controlling terminal; the test already reports UNRESOLVED, no change. |
| XBD.11.2-terminal-parameters.022 | UNRESOLVED | PASS | ENV | Same implementation-defined acquisition of a controlling terminal; the test already reports UNRESOLVED, no change. |
| XBD.12.1-utility-argument-syntax.002 | FAIL | PASS | UNKNOWN | cut -f 2147483647 gave nothing; XBD 12.1 only says such numerals are "syntactically recognized", so a resource failure inside cut is not excluded. Need cut stderr and exit status; the failure message now shows both. Re-run on Linux: PASS. |
| XBD.2-conformance.001 | FAIL | FAIL | SYSTEM | ngettext (mandatory, XCU ngettext) is not found on the getconf PATH; the only name missing. |
| XBD.4.1-case-insensitive-comparisons.001 | FAIL | PASS | SYSTEM | grep -i must match the case counterparts of a string that matches (XBD 4.1, 9.2); a back-reference such as \(a\)\1 against aA is refused. |
| XBD.4.1-case-insensitive-comparisons.002 | FAIL | FAIL | SYSTEM | find -iname is equivalent to -name but case-insensitive per XBD 4.1, so [[:lower:]]ulu must match Zulu; fails on both systems. |
| XBD.4.1-case-insensitive-comparisons.003 | FAIL | PASS | SYSTEM | sed s///i must apply XBD 4.1 including to back-references (\(a\)\1 on aA). |
| XBD.4.4-directory-operations.004 | FAIL | PASS | ENV | The writer needed over 100 s on the slow guest, so the readers were ended by their own alarm (status 99). A time limit must not FAIL: a reader killed by its alarm and the main alarm are now UNRESOLVED; real violations (codes 1-3) still FAIL. Re-run on Linux: PASS. |
| XBD.5-file-format-notation.010 | FAIL | FAIL | SYSTEM | awk printf must handle F (XBD 5 f,F; XCU awk printf follows the File Format Notation); the awk wrote the conversion text unchanged. |
| XBD.6.2-character-encoding.003 | FAIL | PASS | SYSTEM | The zero byte shall be the null character independent of shift state and never part of another character (XBD 6.2); mbrtowc consumed it as a continuation. |
| XBD.6.2-character-encoding.004 | FAIL | PASS | STD-AMBIG | The four bytes are stated unconditionally "in any locale", but byte meaning is a function of the shift state and locking-shift use is implementation-defined (XBD 6.2); Q-208, disputed. Test now skips the after-lead part for locales with shift states (new pcl_shifting()). Re-run on Linux: PASS. |
| XBD.6.3-wide-character-codes.002 | FAIL | PASS | STD-AMBIG | iswxdigit() of a digit fails only in a locale with shift states, where use of the classification functions is implementation-defined (XBD 6.2); Q-208, disputed. Locales with shift states are now skipped. Re-run on Linux: PASS. |
| XBD.7.3.1-lc-ctype.005 | FAIL | PASS | SYSTEM | The digit class holds only 0-9 in all locales (XBD 7.3.1.1 digit); iswdigit(0x660) is true in a UTF-8 locale. |
| XBD.9.1-definitions.005 | UNRESOLVED | PASS | SYSTEM | grep rejects the valid collating symbol [[.a.]] (XBD 9.3.5), so the invalid-RE check cannot be judged; the test correctly gives UNRESOLVED. |
| XBD.9.3-bre.002 | FAIL | PASS | SYSTEM | \(.\)\1\{0,1\}b must match xaab (XBD 9.3.6: interval after a back-reference); the matcher does not backtrack into it. |
| XBD.9.3-bre.003 | FAIL | PASS | STD-AMBIG | Whether a repeated group makes an extra empty iteration after its last non-empty one (group 1 of \(a*\)* on aab) is not stated (XBD 9.1, 9.3.6); Q-207, disputed, both readings now accepted for the three affected cases. Re-run on Linux: PASS. |
| XBD.9.3-bre.008 | FAIL | PASS | SYSTEM | [--/] is a valid range starting with hyphen (XBD 9.3.5 gives [--@] as an example); regcomp rejects it. |
| XBD.9.4-ere.004 | FAIL | PASS | STD-AMBIG | Same extra-empty-iteration question for EREs ((a*)* on aab); Q-207, disputed, both readings accepted for the four affected cases. Re-run on Linux: PASS. |
| XBD.9.5-grammar.003 | FAIL | PASS | SYSTEM | The interval a\{0\} is valid (0 <= m, XBD 9.3.6, 9.5); regcomp returns REG_BADBR. |
| XBD.aio.h.008 | FAIL | FAIL | STD-AMBIG | lio_listio is declared with "struct aiocb *const" list elements on both systems, not "*restrict const"; the page does not say a nested qualifier must match; Q-209, disputed. Test accepts both forms. Re-run on Linux: PASS. |
| XBD.complex.h.007 | FAIL | PASS | SYSTEM | CMPLX, CMPLXF, CMPLXL are required macros of complex.h; not defined. |
| XBD.dirent.h.001 | UNRESOLVED | UNRESOLVED | SYSTEM | The DT_ file type constants are not visible under the strict macro. |
| XBD.dirent.h.002 | UNRESOLVED | UNRESOLVED | SYSTEM | The DT_ constants (DT_MQ, DT_SEM, DT_SHM) are not visible. |
| XBD.dirent.h.004 | FAIL | FAIL | SYSTEM | reclen_t (Issue 8) is missing. |
| XBD.dirent.h.005 | FAIL | FAIL | SYSTEM | struct dirent has no member d_ino (d_fileno instead). |
| XBD.dirent.h.006 | FAIL | FAIL | SYSTEM | struct posix_dent (Issue 8) is missing. |
| XBD.dirent.h.007 | FAIL | PASS | SYSTEM | dirfd() is not declared. |
| XBD.dirent.h.008 | FAIL | FAIL | SYSTEM | posix_getdents() (Issue 8) is not declared. |
| XBD.dlfcn.h.001 | FAIL | PASS | SYSTEM | Dl_info_t / dladdr() (Issue 8) is missing; the header itself fails to compile its own declaration. |
| XBD.dlfcn.h.002 | FAIL | FAIL | SYSTEM | Dl_info_t / dladdr() (Issue 8) is missing; the header itself fails to compile its own declaration. |
| XBD.dlfcn.h.003 | FAIL | PASS | SYSTEM | Dl_info_t / dladdr() (Issue 8) is missing; the header itself fails to compile its own declaration. |
| XBD.dlfcn.h.004 | FAIL | FAIL | SYSTEM | Dl_info_t / dladdr() (Issue 8) is missing; the header itself fails to compile its own declaration. |
| XBD.fcntl.h.006 | FAIL | FAIL | SYSTEM | F_OFD_GETLK, F_OFD_SETLK, F_OFD_SETLKW (Issue 8) are missing. |
| XBD.fcntl.h.007 | FAIL | FAIL | SYSTEM | O_TTY_INIT is a base constant of fcntl.h; missing. |
| XBD.fcntl.h.008 | FAIL | FAIL | SYSTEM | F_GETOWN_EX and F_SETOWN_EX (Issue 8) are missing. |
| XBD.fcntl.h.011 | UNRESOLVED | UNRESOLVED | SYSTEM | O_CLOEXEC, O_DIRECTORY, ..., O_TTY_INIT are base constants of fcntl.h; some are not visible. |
| XBD.fcntl.h.013 | FAIL | PASS | SYSTEM | O_EXEC lies outside O_ACCMODE, though fcntl() says F_GETFL access modes are extracted with the mask; Q-165 already records the inference. |
| XBD.fcntl.h.022 | FAIL | PASS | SYSTEM | fcntl.h must define the file mode constants of sys/stat.h (S_IRWXU...); not visible. |
| XBD.float.h.001 | FAIL | PASS | SYSTEM | FLT_DECIMAL_DIG, DBL_DECIMAL_DIG, LDBL_DECIMAL_DIG are ISO C macros the page requires; missing. |
| XBD.float.h.002 | FAIL | PASS | SYSTEM | FLT_TRUE_MIN, DBL_TRUE_MIN, LDBL_TRUE_MIN (ISO C) are missing. |
| XBD.float.h.004 | FAIL | PASS | SYSTEM | The *_DECIMAL_DIG macros (ISO C) are missing. |
| XBD.float.h.005 | FAIL | PASS | SYSTEM | The *_TRUE_MIN macros (ISO C) are missing. |
| XBD.fnmatch.h.002 | UNRESOLVED | UNRESOLVED | SYSTEM | FNM_IGNORECASE (Issue 8) is not defined. |
| XBD.langinfo.h.003 | FAIL | FAIL | SYSTEM | ALTMON_ and ABALTMON_ items (Issue 8) are missing. |
| XBD.langinfo.h.004 | FAIL | FAIL | TEST-BUG | A test of value distinctness failed to build when ALTMON_ names are missing, double-counting the presence assertion (langinfo.h.003) instead of reporting UNRESOLVED; now guarded on every constant it uses. Re-run on Linux: UNRESOLVED (glibc hides them under the strict macro; before: FAIL). |
| XBD.langinfo.h.005 | FAIL | PASS | SYSTEM | langinfo.h must define locale_t (since 2008); missing. |
| XBD.limits.h.001 | FAIL | PASS | SYSTEM | _POSIX_RTSIG_MAX, _POSIX_SEM_VALUE_MAX and _POSIX_CLOCKRES_MIN are base minima and are missing. |
| XBD.limits.h.007 | FAIL | FAIL | SYSTEM | LONG_BIT and WORD_BIT are base (CX) macros of limits.h and are missing. |
| XBD.limits.h.008 | FAIL | FAIL | SYSTEM | LONG_BIT and WORD_BIT missing (see limits.h.007). |
| XBD.limits.h.014 | FAIL | FAIL | SYSTEM | NSIG_MAX (Issue 8) is missing. |
| XBD.locale.h.006 | FAIL | FAIL | SYSTEM | getlocalename_l() (Issue 8) is not declared. |
| XBD.locale.h.007 | FAIL | PASS | SYSTEM | uselocale() and the other 2008 locale functions are not declared. |
| XBD.netdb.h.001 | FAIL | PASS | SYSTEM | AI_V4MAPPED and AI_ALL are unshaded constants of netdb.h; missing. |
| XBD.netdb.h.004 | FAIL | PASS | SYSTEM | IPPORT_RESERVED is required by the netdb.h page; missing. |
| XBD.poll.h.008 | FAIL | FAIL | SYSTEM | <poll.h> must define struct timespec (Issue 8); incomplete type on both systems. |
| XBD.pthread.h.005 | UNRESOLVED | UNRESOLVED | SYSTEM | PTHREAD_MUTEX_ROBUST and PTHREAD_MUTEX_STALLED are unshaded base constants and are missing. |
| XBD.pthread.h.012 | UNRESOLVED | UNRESOLVED | SYSTEM | PTHREAD_NULL (Issue 8) is missing. |
| XBD.pthread.h.015 | FAIL | FAIL | SYSTEM | pthread_cond_clockwait() and the other clock-lock functions (Issue 8) are not declared. |
| XBD.pthread.h.016 | FAIL | PASS | SYSTEM | pthread_mutex_consistent(), pthread_mutexattr_getrobust() and setrobust() are not declared. |
| XBD.regex.h.002 | FAIL | FAIL | SYSTEM | REG_MINIMAL (Issue 8) is missing. |
| XBD.semaphore.h.004 | FAIL | FAIL | SYSTEM | semaphore.h must define O_CREAT and O_EXCL (Issue 8); missing. |
| XBD.semaphore.h.005 | FAIL | FAIL | SYSTEM | sem_clockwait() (Issue 8) is not declared. |
| XBD.signal.h.003 | FAIL | PASS | SYSTEM | sigev_notify_attributes has a type other than pthread_attr_t * (base member since 2008). |
| XBD.signal.h.007 | FAIL | FAIL | SYSTEM | _SC_NSIG (Issue 8) is not defined. |
| XBD.signal.h.008 | FAIL | FAIL | SYSTEM | SIG2STR_MAX (Issue 8) is not defined. |
| XBD.signal.h.020 | FAIL | PASS | SYSTEM | ucontext_t is a base (CX) type of signal.h and is missing. |
| XBD.signal.h.027 | FAIL | PASS | SYSTEM | psignal() is CX (base) and is not declared. |
| XBD.signal.h.028 | FAIL | FAIL | SYSTEM | sig2str() and str2sig() (Issue 8) are missing. |
| XBD.spawn.h.001 | UNRESOLVED | UNRESOLVED | SYSTEM | The POSIX_SPAWN_* flags are unshaded (except the PS pair) and are not visible. |
| XBD.stdio.h.012 | FAIL | PASS | SYSTEM | renameat() is CX (base) and is not declared. |
| XBD.stdlib.h.004 | FAIL | PASS | SYSTEM | WNOHANG and WUNTRACED are CX macros of stdlib.h and are missing. |
| XBD.stdlib.h.007 | FAIL | FAIL | SYSTEM | secure_getenv() (CX, base) is not declared. |
| XBD.stdlib.h.009 | FAIL | FAIL | SYSTEM | realpath() (CX, base) is not declared. |
| XBD.sys_mman.h.011 | FAIL | FAIL | SYSTEM | sys/mman.h must define O_RDONLY, O_RDWR for SHM|TYM; header-only modules run on the compile-time claim, which is yes. |
| XBD.sys_mman.h.013 | FAIL | FAIL | SYSTEM | sys/mman.h must define O_CREAT, O_EXCL, O_TRUNC for SHM; missing on both systems. |
| XBD.sys_resource.h.004 | FAIL | PASS | SYSTEM | RLIM_INFINITY etc. must have values usable in #if (XBD sys/resource.h); the macro contains a cast. |
| XBD.sys_socket.h.007 | FAIL | FAIL | SYSTEM | SO_DOMAIN (Issue 8) is missing. |
| XBD.sys_socket.h.008 | FAIL | FAIL | SYSTEM | MSG_CMSG_CLOEXEC (Issue 8) is missing. |
| XBD.sys_socket.h.016 | FAIL | FAIL | SYSTEM | CMSG_NXTHDR(mhdr, NULL) must equal CMSG_FIRSTHDR(mhdr) (XBD sys/socket.h, Issue 8); the macro dereferences the null pointer (signal 11). |
| XBD.sys_stat.h.004 | UNRESOLVED | UNRESOLVED | TEST-BUG | S_IFBLK..S_IFSOCK are shaded XSI on the sys/stat.h page, but the test is built without the XSI macro; assertion made conditional on XSI (sys_stat.h.013 already is). Re-run on Linux: PASS. |
| XBD.sys_stat.h.005 | UNRESOLVED | UNRESOLVED | TEST-BUG | The crafted modes use the XSI-shaded S_IF* constants; assertion made conditional on XSI. Re-run on Linux: PASS. |
| XBD.sys_stat.h.007 | UNRESOLVED | PASS | SYSTEM | S_TYPEISMQ, S_TYPEISSEM, S_TYPEISSHM are base macros of sys/stat.h and are missing. |
| XBD.sys_stat.h.020 | FAIL | PASS | SYSTEM | fchmod() is unshaded on the sys/stat.h page and is not declared. |
| XBD.sys_types.h.007 | FAIL | FAIL | SYSTEM | reclen_t (Issue 8) is missing. |
| XBD.sys_types.h.011 | FAIL | FAIL | SYSTEM | reclen_t (Issue 8) is missing. |
| XBD.sys_wait.h.006 | UNRESOLVED | UNRESOLVED | SYSTEM | WCOREDUMP is an unshaded macro of sys/wait.h and is missing. |
| XBD.termios.h.004 | UNRESOLVED | PASS | SYSTEM | The c_iflag constants are unshaded on the termios.h page; at least one is not visible under the strict feature macro. |
| XBD.termios.h.016 | FAIL | PASS | SYSTEM | termios.h must define pid_t; missing. |
| XBD.termios.h.018 | FAIL | PASS | SYSTEM | tcgetsid() is unshaded on the termios.h page and is not declared. |
| XBD.tgmath.h.001 | FAIL | PASS | SYSTEM | tgmath.h must make the complex.h contents (including CMPLX) available; CMPLX is missing. |
| XBD.tgmath.h.004 | FAIL | PASS | SYSTEM | The test uses CMPLX, which the system does not define (see complex.h.007). |
| XBD.time.h.001 | FAIL | PASS | SYSTEM | CLOCKS_PER_SEC shall be an expression with type clock_t (XBD time.h); it has type int. |
| XBD.unistd.h.001 | FAIL | FAIL | SYSTEM | POSIX_CLOSE_RESTART (Issue 8) is missing. |
| XBD.unistd.h.002 | FAIL | FAIL | SYSTEM | SEEK_HOLE and SEEK_DATA are not defined. |
| XBD.unistd.h.004 | FAIL | FAIL | SYSTEM | _POSIX_VERSION and _POSIX2_VERSION are 200112L, not 202405L. |
| XBD.unistd.h.006 | FAIL | FAIL | SYSTEM | Option symbols such as _POSIX_CPUTIME have the value 200112L, which the page does not allow (-1, 0 or 202405L). |
| XBD.unistd.h.007 | FAIL | FAIL | SYSTEM | The always-supported option symbols are not 202405L. |
| XBD.unistd.h.008 | FAIL | PASS | SYSTEM | _POSIX_VDISABLE is an execution-time constant that must be usable in #if, and _XOPEN_ENH_I18N and _XOPEN_SHM must be defined and not -1 (XBD unistd.h); the system uses a cast and omits them. |
| XBD.unistd.h.010 | FAIL | FAIL | SYSTEM | The _SC_ names of unsupported options (_SC_ADVISORY_INFO...) are not defined, though the page lists them without shading. |
| XBD.unistd.h.011 | FAIL | FAIL | SYSTEM | _SC_RAW_SOCKETS is not defined. |
| XBD.unistd.h.014 | FAIL | FAIL | SYSTEM | Several listed _SC_ names (_SC_ADVISORY_INFO, _SC_IPV6, _SC_SEM_VALUE_MAX...) are not defined. |
| XBD.unistd.h.015 | FAIL | FAIL | SYSTEM | Several listed _PC_ names (_PC_ALLOC_SIZE_MIN, _PC_ASYNC_IO...) are not defined. |
| XBD.unistd.h.016 | FAIL | FAIL | STD-AMBIG | The _CS_POSIX_V7_* names are only "reserved for compatibility with Issue 7" (OB), not listed as ones the header shall define; Q-210, disputed; V7 names now optional in the test (and in unistd.h.019). Re-run on Linux: PASS. |
| XBD.unistd.h.019 | FAIL | FAIL | SYSTEM | _SC_NPROCESSORS_CONF and other 2024 names are missing; V7 _CS_ names made optional while here (Q-210). |
| XBD.unistd.h.021 | FAIL | FAIL | SYSTEM | unistd.h must define O_CLOEXEC and O_CLOFORK (Issue 8); missing. |
| XBD.unistd.h.024 | FAIL | FAIL | SYSTEM | _Fork() and posix_close() (Issue 8) are not declared. |
| XBD.unistd.h.026 | FAIL | PASS | SYSTEM | fchdir(), fchown(), getpgid(), lchown(), symlink() are unshaded and not declared under the strict macro. |
| XBD.wchar.h.009 | FAIL | PASS | SYSTEM | wcpcpy() and the other CX wide-string functions are not declared. |

## Counts

- ENV: 20
- STD-AMBIG: 6
- SYSTEM: 96
- TEST-BUG: 4
- UNKNOWN: 1
- total: 127

## Changes made

- TEST-BUG: core/pcpty.c and core/pcpty.sh (new -c flag: status 125 unless the opened subsidiary side became the controlling terminal; pc_pty_run -c turns that into UNRESOLVED), used by XBD.10.1-directory-structure-and-files.005. XBD.langinfo.h.004 (every constant guarded, UNRESOLVED when one is missing). XBD.sys_stat.h.004 and .005 (class conditional, when XSI, because S_IFBLK to S_IFSOCK are XSI-shaded).
- ENV: XBD.4.4-directory-operations.004 now reports UNRESOLVED when a reader hits its own alarm or the test runs out of time; a violation seen by a reader still FAILs.
- UNKNOWN: XBD.12.1-utility-argument-syntax.002 now prints the status and standard error of the cut call so the next run on the second system settles it.
- STD-AMBIG (Q-207 to Q-210 appended to docs/standard-issues.md; class disputed; tests tolerant): 9.3-bre.003 and 9.4-ere.004 (extra empty iteration, Q-207); 6.2-character-encoding.004 and 6.3-wide-character-codes.002 (locales with shift states, Q-208; new pcl_shifting() in both pcl.h); aio.h.008 (restrict on the list elements, Q-209); unistd.h.016 (V7 _CS_ names, Q-210; unistd.h.019 made tolerant too).
- lint: python3 ci/lint.py reports 6373 problems before and after, all of them "declares no MUTANT".
- Not done: no VARIANT was added for the new tolerated readings (no planting library exists for regcomp group iteration, locale shift states or declaration qualifiers).

## Recurring patterns

1. The second system claims the 2001 edition (_POSIX_VERSION 200112L) and lacks almost everything new in Issue 8 (clock-lock functions, reclen_t and posix_getdents, sig2str, ALTMON_, FNM_IGNORECASE, O_CLOEXEC in unistd.h, F_OFD_*, getlocalename_l, ISO C float/complex macros): 96 SYSTEM lines, almost all "compile-time presence".
2. It also hides base 2008 interfaces under the strict _POSIX_C_SOURCE=202405L (uselocale, locale_t, fchown, fchdir, symlink, renameat, realpath, WNOHANG in stdlib.h, LONG_BIT, tcgetsid, d_ino). These were checked one by one against the page shading; none is XSI-only, so they stay SYSTEM. Some lines fail on both systems for the same reason (poll.h timespec, LONG_BIT, NSIG_MAX, F_OFD_*).
3. The only header lines that were wrong tests were XSI-shaded names used without the XSI macro (S_IF*), a distinctness test that failed when its names were absent, and a prototype check stricter than the page needs.
4. Case-insensitive matching (4.1) and regular expressions (9.3, 9.5): the matcher misses back-reference case folding, backtracking into an interval after a back-reference, hyphen-first ranges and \{0\}; real deviations.
5. Terminal tests: the second system does not give a controlling terminal on open, which is implementation-defined; the 11.1/11.2 tests already say UNRESOLVED, only /dev/tty (10.1.005) needed a guard.
