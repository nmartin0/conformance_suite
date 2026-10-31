# FreeBSD triage, group xbd

Counts: TEST-BUG 1, SYSTEM 78, STD-AMBIG 0, ENV 0, UNKNOWN 1.

Fix committed in the worktree: 152b17cc (XBD.unistd.h.008 split; the XSI-shaded symbols moved to the new conditional XBD.unistd.h.032).

| ID | result | class | reason |
|---|---|---|---|
| XBD.10.1-directory-structure-and-files.005 | FAIL | UNKNOWN | XBD 10.1 makes /dev/tty the process's controlling terminal. The helper confirmed the pty was the controlling terminal (-c), and pty output capture works in other tests on this run (e.g. XCU.sh-jobs.002), but the marker was missing from pty.out. The saved output is empty, so it is unclear whether opening or writing /dev/tty failed. A rerun that prints rc.txt, err.txt and pty.out would settle it |
| XBD.11.1-interface-characteristics.030 | FAIL | SYSTEM | XBD 11.1.7 case MIN>0, TIME>0: TIME is an inter-byte timer, restarted on each byte. Bytes sent 0.6 s apart with TIME 1.5 s gave one read of "abc" without "d", which fits a timer counted from the first byte only |
| XBD.11.1-interface-characteristics.045 | FAIL | SYSTEM | XBD 11.1.11: the last close sends pending output to the device. "xyz" written by the subsidiary side 150 ms before its last close was not readable at the manager afterwards (got nothing; poll gave no POLLIN) |
| XBD.11.2-terminal-parameters.023 | FAIL | SYSTEM | XBD 11.2.6 and 11.1.9: with ERASE etc. set to _POSIX_VDISABLE and IEXTEN clear, 0x08 still erased the byte before it. 11.1.9 and 11.2.5 say implementation-defined functions (such as an extra erase character) are recognised only when IEXTEN is set |
| XBD.11.2-terminal-parameters.024 | FAIL | SYSTEM | XBD 11.2.6: in non-canonical mode _POSIX_VDISABLE has no special meaning for VMIN. MIN 255 (within MAX_INPUT) must wait, but a read of one queued byte returned it |
| XBD.12.1-utility-argument-syntax.002 | FAIL | SYSTEM | XBD 12.1: numeric option-arguments are decimal. head -n 010 must give 10 lines (head requires a decimal integer) but gave a different result |
| XBD.2-conformance.001 | FAIL | SYSTEM | XBD 2.1.3 and getconf PATH: gettext, msgfmt and ngettext are unshaded mandatory utilities in Issue 8, and they were not found on the standard PATH |
| XBD.4.1-case-insensitive-comparisons.001 | FAIL | SYSTEM | XBD 4.1: since "aa" matches \(a\)\1, the case-variant "aA" must match under grep -i. It did not (status 1) |
| XBD.4.1-case-insensitive-comparisons.002 | FAIL | SYSTEM | XBD 4.1 and find -iname: "Beta" matches [[:upper:]]eta, so "beta" must match with -iname. Nothing was found |
| XBD.4.1-case-insensitive-comparisons.003 | FAIL | SYSTEM | XBD 4.1 and the sed s///i flag: the back-reference case "aA" against \(a\)\1 must match, but no substitution was made |
| XBD.4.16-pathname-resolution.011 | FAIL | SYSTEM | XBD 4.16 and the readlink() ERRORS: an empty path is an ENOENT shall-fail condition (it names no file, so EINVAL does not apply). The call returned EINVAL |
| XBD.5-file-format-notation.010 | FAIL | SYSTEM | XBD 5 lists the F conversion, and the awk printf statement follows File Format Notation. %F printed literally instead of 1.500000 |
| XBD.6.2-character-encoding.003 | FAIL | SYSTEM | XBD 6.2: a zero byte is never part of another character. In ja_JP.eucJP, mbrtowc() after lead byte 0x8E took the zero byte as completing a character (returned 1) |
| XBD.6.2-character-encoding.004 | INSPECT | SYSTEM | XBD 6.2: the bytes of period, slash, newline and CR are never part of another character. ja_JP.SJIS (no shift states) took '.' after 0x81 as a trail byte. Already `disputed` (Q-208), but that dispute concerns shift states only, and the test skips shifting locales, so this FAIL is clear |
| XBD.7.3.1-lc-ctype.005 | FAIL | SYSTEM | XBD 7.3.1 digit: in all locales only 0-9 are in the class. iswdigit(0x660) is 1 in C.UTF-8 |
| XBD.9.3-bre.002 | FAIL | SYSTEM | XBD 9.3.6: a back-reference with \{0,1\} repeats like any BRE. \(.\)\1\{0,1\}b must match "aab" in "xaab", but regexec gave REG_NOMATCH |
| XBD.9.3-bre.008 | FAIL | SYSTEM | XBD 9.3.5: a hyphen may be a starting range point when it comes first (the text's own example is [--@]). regcomp("[--/]") failed with 11 |
| XBD.9.5-grammar.003 | FAIL | SYSTEM | XBD 9.3.6 and 9.5: interval bounds may be 0 (0<=m<=n). regcomp("a\{0\}") failed with 14 |
| XBD.complex.h.008 | FAIL | SYSTEM | The <complex.h> SYNOPSIS (unshaded) declares ccoshl, ccosl, csinhl, etc. They are undeclared |
| XBD.dirent.h.003 | FAIL | SYSTEM | Unshaded <dirent.h> text: size_t and ssize_t (Issue 8) must be defined. Both are missing |
| XBD.dirent.h.004 | FAIL | SYSTEM | Unshaded <dirent.h> text: reclen_t must be defined. It is missing |
| XBD.dirent.h.005 | FAIL | SYSTEM | Unshaded <dirent.h> text: struct dirent must have member d_ino. It has none under the strict build |
| XBD.dirent.h.006 | FAIL | SYSTEM | Unshaded <dirent.h> text: struct posix_dent must be a complete type. It is incomplete |
| XBD.dirent.h.008 | FAIL | SYSTEM | Unshaded <dirent.h> text: posix_getdents() must be declared. It is not |
| XBD.dlfcn.h.002 | FAIL | SYSTEM | Unshaded <dlfcn.h> text (Issue 8): Dl_info_t must be defined. It is missing |
| XBD.dlfcn.h.004 | FAIL | SYSTEM | Unshaded <dlfcn.h> text: dladdr() must be declared. It is not |
| XBD.fcntl.h.006 | FAIL | SYSTEM | Unshaded <fcntl.h> text: F_OFD_GETLK, F_OFD_SETLK and F_OFD_SETLKW are required. F_OFD_GETLK is missing |
| XBD.fcntl.h.008 | FAIL | SYSTEM | Unshaded <fcntl.h> text: F_GETOWN_EX and F_SETOWN_EX are required. Both are missing |
| XBD.fcntl.h.013 | FAIL | SYSTEM | <fcntl.h> calls O_ACCMODE the mask for the access modes, and fcntl() F_GETFL says the modes are extracted with it. O_EXEC is 0x40000 while O_ACCMODE is 0x3 |
| XBD.fcntl.h.019 | FAIL | SYSTEM | Unshaded <fcntl.h> text: SEEK_SET, SEEK_CUR and SEEK_END must be defined. SEEK_SET is missing |
| XBD.fcntl.h.022 | FAIL | SYSTEM | Unshaded <fcntl.h> text: the file mode constants from <sys/stat.h> must be defined. S_IRWXU is missing |
| XBD.langinfo.h.003 | FAIL | SYSTEM | Unshaded <langinfo.h> table: ABALTMON_1 to ABALTMON_12 are required. They are undeclared |
| XBD.locale.h.006 | FAIL | SYSTEM | <locale.h> SYNOPSIS (CX, base): getlocalename_l() is undeclared |
| XBD.mqueue.h.005 | FAIL | SYSTEM | <mqueue.h> (MSG option, claimed): the O_ flags must be defined. O_RDONLY is missing |
| XBD.netdb.h.003 | FAIL | SYSTEM | Unshaded <netdb.h> text: all ten EAI_ values, EAI_SYSTEM and EAI_OVERFLOW included, are required. At least one is missing |
| XBD.netdb.h.012 | FAIL | SYSTEM | <netdb.h> prototype: getnameinfo() takes socklen_t lengths. The declared type is incompatible (top-level restrict does not affect compatibility) |
| XBD.netinet_in.h.008 | FAIL | SYSTEM | <netinet/in.h> IPv6 text (IP6 claimed): IN6ADDR_ANY_INIT and IN6ADDR_LOOPBACK_INIT must be defined. They are undeclared |
| XBD.pthread.h.015 | FAIL | SYSTEM | Unshaded <pthread.h> SYNOPSIS (Issue 8): pthread_cond_clockwait() and the other clock-lock functions are undeclared |
| XBD.pthread.h.016 | FAIL | SYSTEM | <pthread.h> prototype: pthread_mutexattr_getrobust() takes a const attribute pointer. The declared type is incompatible |
| XBD.pthread.h.020 | FAIL | SYSTEM | <pthread.h> [TPS] (claimed): pthread_setschedprio() is undeclared |
| XBD.regex.h.002 | FAIL | SYSTEM | Unshaded <regex.h> text (Issue 8): REG_MINIMAL is required. It is missing |
| XBD.semaphore.h.004 | FAIL | SYSTEM | Unshaded <semaphore.h> text: O_CREAT and O_EXCL must be defined. O_CREAT is missing |
| XBD.semaphore.h.005 | FAIL | SYSTEM | Unshaded <semaphore.h> SYNOPSIS: sem_clockwait() is undeclared |
| XBD.signal.h.011 | FAIL | SYSTEM | Unshaded <signal.h> table: all the listed signals, SIGWINCH (Issue 8) and SIGBUS included, are required. At least one is missing |
| XBD.signal.h.014 | FAIL | SYSTEM | <signal.h> (CX): struct sigaction must have member sa_sigaction. It is not visible as a member |
| XBD.signal.h.019 | FAIL | SYSTEM | <signal.h> (CX): stack_t must be defined. It is undeclared |
| XBD.signal.h.020 | FAIL | SYSTEM | <signal.h> (CX): ucontext_t has a uc_stack member of type stack_t. stack_t is unknown |
| XBD.stdlib.h.004 | FAIL | SYSTEM | <stdlib.h> (CX): WNOHANG and WUNTRACED must be defined. They are undeclared |
| XBD.stdlib.h.007 | FAIL | SYSTEM | <stdlib.h> (CX): mkostemp(), qsort_r() and the other Issue 8 functions are undeclared |
| XBD.sys_mman.h.011 | FAIL | SYSTEM | <sys/mman.h> [SHM\|TYM] (claimed): O_RDONLY and O_RDWR must be defined. O_RDONLY is missing |
| XBD.sys_mman.h.013 | FAIL | SYSTEM | <sys/mman.h> [SHM] (claimed): O_CREAT, O_EXCL and O_TRUNC must be defined. O_CREAT is missing |
| XBD.sys_resource.h.004 | FAIL | SYSTEM | <sys/resource.h>: the RLIM_ constants must be usable in #if. Their expansion is not a valid preprocessor expression |
| XBD.sys_socket.h.006 | FAIL | SYSTEM | Unshaded <sys/socket.h> text: SOCK_NONBLOCK, SOCK_CLOEXEC and SOCK_CLOFORK are required. SOCK_NONBLOCK is missing |
| XBD.sys_socket.h.007 | FAIL | SYSTEM | Unshaded <sys/socket.h> text: SO_DOMAIN (Issue 8) is required. It is missing |
| XBD.sys_socket.h.008 | FAIL | SYSTEM | Unshaded <sys/socket.h> text: MSG_CMSG_CLOEXEC is required. It is missing |
| XBD.sys_socket.h.012 | FAIL | SYSTEM | Unshaded <sys/socket.h> text: CMSG_SPACE is required, and it must be an integer constant expression for a constant argument. It is undeclared |
| XBD.sys_socket.h.024 | FAIL | SYSTEM | Unshaded <sys/socket.h> SYNOPSIS: accept4() is undeclared |
| XBD.sys_types.h.007 | FAIL | SYSTEM | Unshaded <sys/types.h> text: reclen_t is required. It is undeclared |
| XBD.sys_types.h.011 | FAIL | SYSTEM | Unshaded <sys/types.h> text: reclen_t is required. It is undeclared |
| XBD.sys_wait.h.008 | FAIL | SYSTEM | Unshaded <sys/wait.h> text: siginfo_t and union sigval must be defined. Both are missing |
| XBD.sys_wait.h.009 | FAIL | SYSTEM | <sys/wait.h> declares waitid(..., siginfo_t *, ...). siginfo_t is unknown |
| XBD.termios.h.015 | FAIL | SYSTEM | Unshaded <termios.h> text (Issue 8): struct winsize must be complete. It is incomplete |
| XBD.termios.h.017 | FAIL | SYSTEM | Unshaded <termios.h> SYNOPSIS: tcgetwinsize() and tcsetwinsize() are undeclared |
| XBD.tgmath.h.002 | FAIL | SYSTEM | <tgmath.h> real-argument dispatch fails to compile, because the generic macros refer to csinl and ccosl, which <complex.h> must declare and does not |
| XBD.tgmath.h.003 | FAIL | SYSTEM | Same cause as XBD.tgmath.h.002: csinl, ccosl and ctanl are undeclared, so the complex dispatch fails to compile |
| XBD.unistd.h.001 | FAIL | SYSTEM | Unshaded <unistd.h> text (Issue 8): POSIX_CLOSE_RESTART is required. It is missing |
| XBD.unistd.h.002 | FAIL | SYSTEM | Unshaded <unistd.h> text: SEEK_HOLE and SEEK_DATA must be defined as macros. They are missing |
| XBD.unistd.h.004 | FAIL | SYSTEM | <unistd.h>: _POSIX_VERSION and _POSIX2_VERSION must be 202405L. Neither is |
| XBD.unistd.h.006 | FAIL | SYSTEM | <unistd.h>: a defined option symbol must be -1, 0 or 202405L. Several (ADVISORY_INFO, CPUTIME, FSYNC, ...) have another positive value, which claims the option with an old version |
| XBD.unistd.h.007 | FAIL | SYSTEM | Unshaded <unistd.h> text: ASYNCHRONOUS_IO, BARRIERS and the rest must always be 202405L. They are not (no subprofile is defined) |
| XBD.unistd.h.008 | FAIL | TEST-BUG | _XOPEN_ENH_I18N and _XOPEN_SHM sit in the XSI-shaded part of the option list, but the test required them unconditionally, and XSI is not claimed on this run. Fixed: 008 keeps the three unshaded symbols, and the new XBD.unistd.h.032 (when XSI) checks the XSI pair |
| XBD.unistd.h.010 | FAIL | SYSTEM | Unshaded _SC_ list: _SC_DEVICE_CONTROL and _SC_THREAD_ROBUST_PRIO_* are required. They are undeclared |
| XBD.unistd.h.014 | FAIL | SYSTEM | Unshaded _SC_ list: _SC_ATEXIT_MAX, _SC_IOV_MAX and _SC_PAGE_SIZE are required. They are undeclared |
| XBD.unistd.h.015 | FAIL | SYSTEM | Unshaded _PC_ list: _PC_2_SYMLINKS and _PC_TIMESTAMP_RESOLUTION are required. They are undeclared |
| XBD.unistd.h.019 | FAIL | SYSTEM | Unshaded _SC_ list: _SC_NPROCESSORS_CONF/ONLN and _SC_DEVICE_CONTROL are required. They are undeclared |
| XBD.unistd.h.021 | FAIL | SYSTEM | Unshaded <unistd.h> text: O_CLOEXEC and O_CLOFORK must be defined. O_CLOEXEC is missing |
| XBD.unistd.h.024 | FAIL | SYSTEM | Unshaded <unistd.h> SYNOPSIS: dup3(), getentropy() and the other Issue 8 functions are undeclared |
| XBD.wchar.h.008 | FAIL | SYSTEM | <wchar.h> (CX): wcslcat() and wcslcpy() are undeclared |
| XBD.wordexp.h.003 | FAIL | SYSTEM | Unshaded <wordexp.h> text: size_t must be defined, since the we_wordc and we_offs members use it. It is unknown |
| XBD.wordexp.h.004 | FAIL | SYSTEM | Unshaded <wordexp.h> text: size_t must be defined. It is unknown |
