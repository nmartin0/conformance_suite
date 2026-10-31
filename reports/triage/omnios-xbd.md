# illumos triage, group xbd

Counts: TEST-BUG 5, SYSTEM 83, STD-AMBIG 1, ENV 0, UNKNOWN 0.

| ID | result | class | reason |
|---|---|---|---|
| XBD.10.2-output-devices-and-terminal-types.001 | FAIL | SYSTEM | od -t a must name byte 7 as bel (od page, named-characters table); it wrote a truncated name. |
| XBD.11.1-interface-characteristics.035 | FAIL | TEST-BUG | INTR flushes the input queue (11.1.9, ISIG/NOFLSH); the byte after it was written in the same write, so surviving the flush was timing. Now written after a pause; signals counts were fine. |
| XBD.11.1-interface-characteristics.036 | FAIL | TEST-BUG | Same as 035 for QUIT: follow-up byte now sent in a separate write after the flush. |
| XBD.11.1-interface-characteristics.037 | FAIL | TEST-BUG | Same as 035 for SUSP: follow-up byte now sent in a separate write after the flush. |
| XBD.11.2-terminal-parameters.009 | FAIL | SYSTEM | 11.2.2: with IXANY any input character restarts suspended output; the held byte never arrived after an ordinary byte (flag was kept by tcsetattr). |
| XBD.11.2-terminal-parameters.023 | FAIL | SYSTEM | 11.1.9/11.2.6: a special character set to the disabling value is not recognised; ERASE still erased (the other three were data) . |
| XBD.12.2-utility-syntax-guidelines.010 | FAIL | SYSTEM | tail OPERANDS: a - operand is stdin or the file named -; tail -n 1 - wrote more than the last line, so -n was not honoured. |
| XBD.2-conformance.001 | FAIL | SYSTEM | lp and realpath are base utilities (unshaded pages) and are not found under the getconf PATH; lp is absent from the guest, which is still a missing required utility. |
| XBD.2-conformance.002 | FAIL | SYSTEM | POSIX2_C_DEV is reported but c17 is absent; POSIX2_UPE is reported but talk is absent; both required by 2.1.6.2. |
| XBD.2-conformance.003 | FAIL | SYSTEM | XSI requires c17 (2.1.4.2); it is not installed (the guest has only gcc). |
| XBD.2-conformance.005 | FAIL | SYSTEM | With _XOPEN_REALTIME not -1, _POSIX_MEMLOCK etc. must be 202405L (2.1.5.2); the value differs. |
| XBD.4.1-case-insensitive-comparisons.001 | FAIL | SYSTEM | 4.1: a case-insensitive match accepts case-swapped strings, including for back-references; grep -i of \(a\)\1 did not select aA. |
| XBD.4.1-case-insensitive-comparisons.002 | FAIL | SYSTEM | find -iname (4.1, find page): [[:lower:]] against Z in Zulu must match case-insensitively; found none. |
| XBD.4.1-case-insensitive-comparisons.003 | FAIL | SYSTEM | sed s///i is in the sed page (case-insensitive flag); the flag is rejected as bad. |
| XBD.5-file-format-notation.005 | FAIL | SYSTEM | XBD 5: # on o increases precision so zero with precision 0 gives 0; printf wrote nothing. |
| XBD.5-file-format-notation.006 | FAIL | SYSTEM | XBD 5: a precision makes the 0 flag ignored for d,i,o,u,x,X; printf zero-padded to the width. |
| XBD.5-file-format-notation.007 | FAIL | SYSTEM | XBD 5: zero padding follows the 0x prefix and fills the width including it (%#06x of 255 is 0x00ff); printf gave 0x0000ff. |
| XBD.5-file-format-notation.008 | FAIL | SYSTEM | XBD 5: # on x gives a prefix only to a non-zero result; printf wrote 0x0 for zero. |
| XBD.5-file-format-notation.013 | FAIL | SYSTEM | awk printf follows XBD 5, which defines a and A; awk left %a unconverted. |
| XBD.7.3.1-lc-ctype.005 | FAIL | SYSTEM | 7.3.1: class digit holds only the ten digits in every locale; a UTF-8 locale classifies U+0660 as a digit. |
| XBD.9.3-bre.002 | FAIL | SYSTEM | 9.3.6: a back-reference may take an interval; \(.\)\1\{0,1\}b must match aab in xaab, regexec reported no match. |
| XBD.9.3-bre.008 | FAIL | SYSTEM | 9.3.5: [--/] is a valid range starting at hyphen-minus (like the page example [--@]); regcomp returned REG_ERANGE. |
| XBD.9.4-ere.003 | INSPECT | STD-AMBIG | Already disputed (Q-180) so the result is INSPECT; lone ) is ordinary per 9.4.3 but REG_EPAREN wording conflicts; regcomp("a)") returned REG_EPAREN. No change needed. |
| XBD.9.5-grammar.003 | FAIL | SYSTEM | 9.5/9.3.6: a count of 0 is within 0<=m<=RE_DUP_MAX; regcomp rejected a\{0\} (status 14). |
| XBD.complex.h.004 | FAIL | SYSTEM | complex.h: _Imaginary_I and imaginary exist iff imaginary types are supported; the header defines them but the compiler has no _Imaginary type, so use fails to compile. |
| XBD.complex.h.007 | FAIL | SYSTEM | complex.h requires CMPLX, CMPLXF, CMPLXL; not defined. |
| XBD.dirent.h.004 | FAIL | SYSTEM | dirent.h (2024) defines reclen_t; unknown type. |
| XBD.dirent.h.006 | FAIL | SYSTEM | dirent.h (2024) defines struct posix_dent; incomplete type. |
| XBD.dirent.h.007 | FAIL | SYSTEM | dirent.h declares alphasort, dirfd, fdopendir, scandir (unshaded); not declared with _POSIX_C_SOURCE=202405L. |
| XBD.dirent.h.008 | FAIL | SYSTEM | dirent.h (2024) declares posix_getdents(); not declared. |
| XBD.dlfcn.h.002 | FAIL | SYSTEM | dlfcn.h (2024) defines Dl_info_t; missing. |
| XBD.dlfcn.h.004 | FAIL | SYSTEM | dlfcn.h (2024) declares dladdr(); not declared. |
| XBD.fcntl.h.006 | FAIL | SYSTEM | fcntl.h (2024) defines F_OFD_GETLK, F_OFD_SETLK, F_OFD_SETLKW; not defined under the strict feature macro (documented deviation 1.8). |
| XBD.fcntl.h.007 | FAIL | SYSTEM | fcntl.h defines O_TTY_INIT (unshaded, since 2008); not defined. |
| XBD.fcntl.h.008 | FAIL | SYSTEM | fcntl.h (2024) defines F_GETOWN_EX/F_SETOWN_EX; not defined. |
| XBD.fcntl.h.024 | FAIL | SYSTEM | fcntl.h declares openat() (unshaded); not declared. |
| XBD.inttypes.h.004 | FAIL | SYSTEM | SCN macros must work in swscanf (inttypes.h defers to ISO C); swscanf with SCNd8 on valid text returned -1 instead of 1. |
| XBD.langinfo.h.003 | FAIL | SYSTEM | langinfo.h (2024) defines ALTMON_1..12 and ABALTMON_1..12; missing. |
| XBD.limits.h.010 | FAIL | SYSTEM | limits.h: every definition is usable in #if; PTHREAD_STACK_MIN expands to a function call. |
| XBD.limits.h.011 | FAIL | SYSTEM | Same cause as limits.h.010: PTHREAD_STACK_MIN is not usable in #if. |
| XBD.limits.h.014 | FAIL | SYSTEM | limits.h Other Invariant Values must be defined; GETENTROPY_MAX and NSIG_MAX (2024) are missing. |
| XBD.math.h.014 | FAIL | SYSTEM | math.h (XSI, 2024) defines M_EGAMMA, M_PHI, M_1_SQRTPI, M_SQRT3, M_SQRT1_3; missing. |
| XBD.math.h.015 | FAIL | SYSTEM | math.h (XSI, 2024) defines the long double constants M_El etc.; missing. |
| XBD.netdb.h.002 | FAIL | SYSTEM | netdb.h defines NI_NUMERICSCOPE (unshaded); not defined. |
| XBD.netinet_in.h.007 | FAIL | SYSTEM | netinet/in.h (IP6) defines IPV6_V6ONLY; not defined under the strict feature macro. |
| XBD.netinet_in.h.008 | FAIL | TEST-BUG | in6addr_any/in6addr_loopback are among interfaces needing the networking library of the compile utility; module listed no libs so link failed. Added libs (driver keeps only linkable ones); now links and passes here. Needs a re-run on the system. |
| XBD.netinet_in.h.009 | FAIL | TEST-BUG | Same link failure (in6addr_any) and same fix as netinet_in.h.008. |
| XBD.netinet_in.h.014 | FAIL | SYSTEM | netinet/in.h (IP6) defines struct ipv6_mreq; incomplete type. |
| XBD.poll.h.007 | FAIL | SYSTEM | poll.h (2024) defines sigset_t; unknown type. |
| XBD.poll.h.008 | FAIL | SYSTEM | poll.h (2024) defines struct timespec completely; incomplete. |
| XBD.poll.h.009 | FAIL | SYSTEM | poll.h (2024) declares ppoll(); not declared. |
| XBD.regex.h.002 | FAIL | SYSTEM | regex.h (2024) defines REG_MINIMAL; missing. |
| XBD.signal.h.007 | FAIL | SYSTEM | unistd.h (2024) defines _SC_NSIG (and limits.h NSIG_MAX); missing. |
| XBD.signal.h.008 | FAIL | SYSTEM | signal.h (2024) defines SIG2STR_MAX; missing. |
| XBD.signal.h.027 | FAIL | SYSTEM | signal.h declares psiginfo() and psignal() (CX, required); not declared. |
| XBD.signal.h.028 | FAIL | SYSTEM | signal.h (2024) declares sig2str() and str2sig(); not declared. |
| XBD.stdint.h.002 | FAIL | SYSTEM | stdint.h (ISO C): 8/16-bit exact-width limits have the integer-promoted type; they do not (unsigned-suffixed constants). |
| XBD.stdint.h.003 | FAIL | SYSTEM | Same as stdint.h.002 for the least types. |
| XBD.stdint.h.004 | FAIL | SYSTEM | Same as stdint.h.002 for the fast types. |
| XBD.stdint.h.009 | FAIL | SYSTEM | INT8_C/UINT8_C and 16-bit forms must have the promoted type; they do not. |
| XBD.stdio.h.012 | FAIL | SYSTEM | stdio.h declares getdelim, getline and others (unshaded); not declared with _POSIX_C_SOURCE=202405L. |
| XBD.stdlib.h.003 | FAIL | SYSTEM | MB_CUR_MAX must have type size_t (ISO C); it has another type. |
| XBD.stdlib.h.011 | FAIL | SYSTEM | stdlib.h (XSI) declares char *setstate(char *); declared differently. |
| XBD.sys_mman.h.011 | FAIL | SYSTEM | sys/mman.h (SHM/TYM, 2024) defines O_RDONLY and O_RDWR; not defined. |
| XBD.sys_mman.h.013 | FAIL | SYSTEM | sys/mman.h (SHM, 2024) defines O_CREAT, O_EXCL, O_TRUNC; not defined. |
| XBD.sys_stat.h.010 | FAIL | SYSTEM | st_atim etc. must be struct timespec with tv_sec; the member type has only private-named members. |
| XBD.sys_stat.h.014 | FAIL | SYSTEM | struct stat members st_atim, st_mtim, st_ctim must be struct timespec; they are another type. |
| XBD.sys_stat.h.019 | FAIL | SYSTEM | sys/stat.h defines struct timespec completely; incomplete. |
| XBD.sys_stat.h.020 | FAIL | SYSTEM | sys/stat.h declares fchmodat, fstatat etc. (unshaded); not declared. |
| XBD.sys_stat.h.021 | FAIL | SYSTEM | sys/stat.h (XSI) declares mknodat(); not declared. |
| XBD.sys_types.h.007 | FAIL | SYSTEM | sys/types.h (2024) defines reclen_t; missing. |
| XBD.sys_types.h.011 | FAIL | SYSTEM | Same as sys_types.h.007 (reclen_t). |
| XBD.termios.h.015 | FAIL | SYSTEM | termios.h (2024) defines struct winsize; incomplete. |
| XBD.termios.h.017 | FAIL | SYSTEM | termios.h (2024) declares tcgetwinsize/tcsetwinsize; not declared. |
| XBD.tgmath.h.001 | FAIL | SYSTEM | tgmath.h includes complex.h, so CMPLX must be defined; it is not. |
| XBD.tgmath.h.004 | FAIL | SYSTEM | CMPLX (required by complex.h, hence tgmath.h) is not defined. |
| XBD.unistd.h.001 | FAIL | SYSTEM | unistd.h (2024) defines POSIX_CLOSE_RESTART; missing (documented deviation 1.3). |
| XBD.unistd.h.002 | FAIL | SYSTEM | unistd.h (2024) defines SEEK_HOLE and SEEK_DATA as macros; not defined under the strict macro (deviation 1.12). |
| XBD.unistd.h.004 | FAIL | SYSTEM | unistd.h defines _POSIX_VERSION and _POSIX2_VERSION as 202405L; not that value. |
| XBD.unistd.h.006 | FAIL | SYSTEM | Option symbols that are defined must be -1, 0 or 202405L; e.g. _POSIX_ADVISORY_INFO has another value. |
| XBD.unistd.h.007 | FAIL | SYSTEM | _POSIX_ASYNCHRONOUS_IO, _POSIX_BARRIERS etc. shall always be 202405L; they are not. |
| XBD.unistd.h.008 | FAIL | SYSTEM | _POSIX_CHOWN_RESTRICTED and _POSIX_NO_TRUNC must be defined with a value other than -1; not so. |
| XBD.unistd.h.010 | FAIL | SYSTEM | unistd.h names _SC_DEVICE_CONTROL, _SC_THREAD_ROBUST_PRIO_*; undeclared. |
| XBD.unistd.h.014 | FAIL | SYSTEM | _SC_THREAD_ROBUST_PRIO_INHERIT/PROTECT (added 2008) are undeclared, so the test cannot compile. |
| XBD.unistd.h.019 | FAIL | SYSTEM | _SC_DEVICE_CONTROL, _SC_NSIG (2024) undeclared. |
| XBD.unistd.h.021 | FAIL | SYSTEM | unistd.h defines O_CLOEXEC and O_CLOFORK as in fcntl.h; not defined. |
| XBD.unistd.h.024 | FAIL | SYSTEM | unistd.h (2024) declares _Fork, getentropy, dup3, pipe2, posix_close; not declared. |
| XBD.unistd.h.025 | FAIL | SYSTEM | unistd.h (XSI, 2024) declares getresuid/getresgid/setresuid/setresgid; not declared. |
| XBD.unistd.h.026 | FAIL | SYSTEM | unistd.h declares faccessat, fchownat, linkat etc. (unshaded); not declared. |

Notes.

- Most SYSTEM rows share one cause: with the driver's -D_POSIX_C_SOURCE=202405L (or -D_XOPEN_SOURCE=800) the headers do not show names added in 2008 and 2024 (openat, fstatat, getline, struct timespec in sys/stat.h and poll.h, and so on); the system behaves as an older edition. The tests are right to ask for them.
- Commits: cf707f29 (11.1.035-037, separate write for the byte after the signal character), 4fcb1e65 (netinet_in.h MODULE libs). Both need a re-run on the system; the 11.1 fix cannot be proved here and if 035-037 still lose the byte they become SYSTEM.
- Harness defect, not in this list: xbd 4.13 and 4.2.002 (and any socket user) fail to link with undefined socket functions; their MODULE files have no networking libs either (same fix as netinet_in.h).
- STD-AMBIG: XBD.9.4-ere.003 is already class disputed.
