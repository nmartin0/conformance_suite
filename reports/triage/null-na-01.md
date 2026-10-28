# Null-system triage, group na-01

Control: the null system in /tmp/null1 (default mode). Rows are in the order of the list.

| ID | kind | decision | reason | what changed | re-verified |
| --- | --- | --- | --- | --- | --- |
| XSH.fscanf.031 | a | LEGIT | sscanf is really called, but the header maps it to another symbol that the control's stubs do not cover, so the stub is never reached (confirmed with nm on the built test); the test compares exact values and fails when the real function misbehaves | none (a test cannot name the alternate symbol; see patterns) | n/a |
| XSH.fscanf.032 | a | LEGIT | same (sscanf) | none | n/a |
| XSH.fscanf.033 | a | LEGIT | same (sscanf) | none | n/a |
| XSH.fscanf.034 | a | LEGIT | same (sscanf) | none | n/a |
| XSH.fscanf.035 | a | LEGIT | same (sscanf) | none | n/a |
| XSH.fscanf.036 | a | LEGIT | same (sscanf) | none | n/a |
| XSH.fscanf.039 | a | LEGIT | same (sscanf) | none | n/a |
| XSH.fscanf.040 | a | LEGIT | same (sscanf) | none | n/a |
| XSH.fscanf.041 | a | LEGIT | same (sscanf; also checks EOF, which the stub's -1 would equal, but the call is not reached) | none | n/a |
| XSH.fscanf.044 | a | LEGIT | same (sscanf) | none | n/a |
| XSH.fscanf.046 | a | LEGIT | same (fscanf); the test requires EBADF, which the stub's ENOSYS cannot match, so a reached stub would FAIL | none | n/a |
| XSH.fscanf.047 | a | LEGIT | same (fscanf); requires EAGAIN | none | n/a |
| XSH.fscanf.048 | a | LEGIT | same (fscanf); requires a count of 1 and stored 12 | none | n/a |
| XSH.fwprintf.014 | a | WEAK | stub returns -1 and writes nothing, which is all the test asked; the standard says swprintf() shall fail with EOVERFLOW when n or more characters were requested | errno must be EOVERFLOW (test and .asr says/anchor); control now FAILs. Real machine now FAILs (errno 0 for n of 0), the deviation already recorded in docs/linux-deviations.md 6.4 and seen in XSH.fwprintf.013 | yes |
| XSH.getnetent.005 | a | LEGIT | not-found is a null pointer (no errno stated), which the stub also returns; no way within the standard to tell "nothing found" from "failed" | none | yes (control still PASS, by design) |
| XSH.getopt.009 | a | LEGIT | the subject is data (optind/optarg/opterr/optopt), not a call; the test reads optind at start and takes the variables' addresses; no stub can change them | none | n/a |
| XSH.getprotoent.005 | a | LEGIT | same: unknown name/number gives a null pointer only | none | yes |
| XSH.getservent.005 | a | LEGIT | same | none | yes |
| XSH.ilogb.008 | a | LEGIT | FP_ILOGB0/FP_ILOGBNAN are macros; compared with the permitted values | none | n/a |
| XSH.in6addr_any.001 | a | LEGIT | data object checked byte by byte (and IN6_IS_ADDR_UNSPECIFIED), no call | none | n/a |
| XSH.isalnum.001 | a | VACUOUS | shared helper ct.h decided class membership with strchr(), which is stubbed in the test phase: no character counted as member, so zero checks ran | membership by a plain loop (ct.h in the module's t/); after the fix the function is called and the control still PASSes because -1 is a non-zero value (the standard asks only "non-zero"; the non-member test 002 is what tells the stub apart) | yes |
| XSH.isalpha.001 | a | VACUOUS | same | same | yes |
| XSH.isdigit.001 | a | VACUOUS | same | same | yes |
| XSH.isfinite.001 | a | LEGIT | type-generic macro evaluated by the compiler, no function to stub; the test checks exact values for all three types | none | n/a |
| XSH.isfinite.002 | a | LEGIT | same | none | n/a |
| XSH.isfinite.003 | a | LEGIT | same (float and double run; the long double part is a deliberate no-op) | none | n/a |
| XSH.isgraph.001 | a | VACUOUS | same | same | yes |
| XSH.isgreater.001 | a | LEGIT | comparison macro, compiler-evaluated; compares with the 13 specified results for each type | none | n/a |
| XSH.isgreater.002 | a | LEGIT | same | none | n/a |
| XSH.isgreater.003 | a | LEGIT | same | none | n/a |
| XSH.isgreater.004 | a | LEGIT | same | none | n/a |
| XSH.isgreater.005 | a | LEGIT | same | none | n/a |
| XSH.isgreater.006 | a | LEGIT | same | none | n/a |
| XSH.isgreater.008 | a | LEGIT | same; single evaluation of the argument is checked through the counters | none | n/a |
| XSH.isgreater.009 | a | LEGIT | same; compares with the operators on the same operands | none | n/a |
| XSH.isinf.001 | a | LEGIT | classification macro, compiler-evaluated | none | n/a |
| XSH.isinf.002 | a | LEGIT | same | none | n/a |
| XSH.isinf.003 | a | LEGIT | same | none | n/a |
| XSH.islower.001 | a | VACUOUS | same | same | yes |
| XSH.isnan.001 | a | LEGIT | same | none | n/a |
| XSH.isnan.002 | a | LEGIT | same | none | n/a |
| XSH.isnormal.001 | a | LEGIT | same | none | n/a |
| XSH.isnormal.002 | a | LEGIT | same | none | n/a |
| XSH.isnormal.003 | a | LEGIT | same | none | n/a |
| XSH.ispunct.001 | a | VACUOUS | same | same | yes |
| XSH.isunordered.001 | a | LEGIT | same | none | n/a |
| XSH.isunordered.002 | a | LEGIT | same | none | n/a |
| XSH.isupper.001 | a | VACUOUS | same | same | yes |
| XSH.iswblank.004 | b | LEGIT | requirement is "iswspace() is non-zero" for the blanks; the stub's -1 is non-zero; the class tests (.001-.003) tell it apart | none | yes |
| XSH.iswxdigit.004 | b | LEGIT | requirement is that the hex digits are also alnum/digit/alpha (all "non-zero"); -1 is non-zero | none | yes |
| XSH.isxdigit.001 | a | VACUOUS | same | same | yes |
| XSH.kill_dependency.001 | a | LEGIT | macro, compiler-evaluated; values compared | none | n/a |
| XSH.openlog.001 | b | LEGIT | setlogmask(0) is called; the standard asks only that the mask contain all eight severities and the stub's -1 has every bit set, so it cannot be told from a conforming all-ones mask (setting and reading masks is covered by other assertions) | none | n/a |
| XSH.openlog.004 | a | LEGIT | LOG_MASK/LOG_UPTO are macros, no call | none | n/a |
| XSH.openlog.005 | a | LEGIT | same | none | n/a |
| XSH.pthread_condattr_getclock.003 | b | WEAK | test accepted any non-zero return; the stub's -1 qualified, but the standard says an error number is returned (positive) | `r > 0` (disputed class kept; control now INSPECT = FAIL of a disputed assertion) | yes (real: PASS) |
| XSH.pthread_condattr_getclock.004 | b | WEAK | same | `r > 0` | yes (real: PASS) |
| XSH.random.006 | b | LEGIT | initstate() with size < 8 "shall return NULL", no errno defined, so the stub's null return is conformant; the companion assertion (initstate success returns non-null / previous state) is what tells it apart | none | yes |
| XSH.recvmsg.026 | a | LEGIT | CMSG_LEN/CMSG_SPACE/CMSG_DATA are macros, compared with sizes | none | n/a |
| XSH.recvmsg.027 | a | LEGIT | CMSG_FIRSTHDR is a macro | none | n/a |
| XSH.recvmsg.029 | a | LEGIT | integer-constant-expression property, decided at translation time | none | n/a |
| XSH.scalbn.013 | b | LEGIT | "a NaN shall be returned" for a NaN argument: the stub returns NaN too; no errno is defined for this case, so nothing else is required (no-error behaviour is scalbn.009/.018) | none | yes |
| XSH.sched_get_priority_max.004 | a | LEGIT | pairwise-distinct constants, no call | none | n/a |
| XSH.select.024 | a | LEGIT | FD_ZERO is a macro (memset in the log is the test's own fill); the control cannot replace it | none | yes |
| XSH.select.025 | a | LEGIT | FD_SET/FD_ISSET/FD_CLR are macros, evaluated by the compiler | none | yes |
| XSH.select.026 | a | LEGIT | same | none | yes |
| XSH.select.027 | a | LEGIT | same | none | yes |
| XSH.signal.001 | a | LEGIT | signal() is really called but the header maps it to another symbol the stubs do not cover (nm), as with the scanf family; exact return values are compared | none | n/a |
| XSH.signal.005 | a | LEGIT | same; requires SIG_ERR and EINVAL, which the stub could not give | none | n/a |
| XSH.signal.006 | a | LEGIT | same; requires success and errno unchanged | none | n/a |
| XSH.signbit.001 | a | LEGIT | same | none | n/a |
| XSH.signbit.002 | a | LEGIT | same | none | n/a |
| XSH.signbit.003 | a | LEGIT | same | none | n/a |
| XSH.signbit.004 | a | LEGIT | same | none | n/a |
| XSH.stdin.007 | a | VACUOUS | fwide() was evaluated in the declarations, before pc_test(), i.e. outside the test phase | the three fwide() calls moved after pc_test() (the only earlier operation is fflush(NULL), not a byte or wide I/O function) | yes |
| XSH.strcoll.005 | a | VACUOUS | `(void)strcoll(...)` / `(void)strcoll_l(...)`: unused pure-call result dropped by the compiler, subject never called | results stored in a volatile sink | yes |
| XSH.strerror.009 | a | LEGIT | strerror_r() is really called but mapped to another symbol (nm); requires 0 or EINVAL, which the stub's -1 is not | none | n/a |
| XSH.va_start.002 | a | LEGIT | language-level macros, no library call | none | n/a |
| XSH.va_start.003 | a | LEGIT | same | none | n/a |
| XSH.va_start.004 | a | LEGIT | same | none | n/a |
| XSH.va_start.006 | a | LEGIT | same | none | n/a |
| XSH.va_start.007 | a | LEGIT | same | none | n/a |
| XSH.vfscanf.003 | a | LEGIT | same (vsscanf) | none | n/a |
| XSH.vfscanf.004 | a | LEGIT | same (vsscanf) | none | n/a |
| XSH.vfscanf.005 | a | LEGIT | same (vfscanf); requires EBADF | none | n/a |
| XSH.wcschr.004 | a | VACUOUS | call `(void)wcschr(...)` has its unused result dropped by the compiler even without optimisation (pure function), so the subject was never called | result stored in a volatile sink so the call stays | yes |
| XSH.wcscmp.004 | a | VACUOUS | same: unused pure-call result dropped | same fix | yes |
| XSH.wcscspn.003 | a | VACUOUS | same | same fix | yes |
| XSH.wcslen.004 | a | VACUOUS | same (wcslen, wcsnlen) | same fix | yes |
| XSH.wcsncmp.005 | a | VACUOUS | same | same fix | yes |
| XSH.wcspbrk.003 | a | VACUOUS | same | same fix | yes |
| XSH.wcsrchr.004 | a | VACUOUS | same | same fix | yes |
| XSH.wcsspn.003 | a | VACUOUS | same | same fix | yes |
| XSH.wcsstr.004 | a | VACUOUS | same | same fix | yes |
| XSH.wmemchr.006 | a | VACUOUS | same | same fix | yes |
| XSH.wmemcmp.005 | a | VACUOUS | same | same fix | yes |
| XSH.wordexp.025 | a | LEGIT | constants and member types, no call | none | n/a |

Counts: LEGIT 73, VACUOUS 21, WEAK 3

Patterns:

- A pure-function call whose result is cast to void is dropped by the compiler even without optimisation, so the subject is never called (11 wcs/wmem tests, strcoll). The result is now kept in a volatile object.
- A shared helper that uses another library function in the test phase (strchr in the ctype membership helper) is itself stubbed and made the loop empty; membership is now a plain loop. After the fix the members-only tests still PASS on the control because the stub returns -1, a non-zero value; the non-member tests tell it apart.
- Calls the control cannot see: functions the header maps to another symbol (scanf family, signal, strerror_r) and compiler-evaluated macros (classification, comparison, FD_*, CMSG_*, LOG_*, va_*). The tests are sound; the log is blind there. A fix belongs in the control (stubs for the alternate symbols), not in the tests.
- An fwide() call evaluated before pc_test() (stdin.007) ran outside the test phase.
- Weak result checks: any non-zero return where the standard says an error number (pthread_condattr_setclock), and a bare negative return where it says EOVERFLOW (swprintf, n of 0); the latter now fails on the glibc machine (errno 0), the deviation already recorded in docs/linux-deviations.md 6.4 and seen in XSH.fwprintf.013. known/*.known was not touched.
