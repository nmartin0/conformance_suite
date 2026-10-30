# Third-system triage of build failures, group build1

Source: the full run on the third system (BSD-derived, as root); every assertion in this list ended UNINITIATED because the test did not build. 306 assertions, all XSH. The standard is the oracle: every header, macro and function below was checked in its page for option shading (only CX, which is base, and the options named in a row are shaded). Errors are counted by the first compiler error of each build.

Result: TEST-BUG 125, SYSTEM 181, STD-AMBIG 0, ENV 0, UNKNOWN 0.

## Error groups

| count | first error | class | reason |
|---|---|---|---|
| 25 | call to undeclared function 'CMPLX' | SYSTEM | CMPLX/CMPLXF/CMPLXL are required macros of <complex.h>; they build the inputs. A fallback in mt.h through the array layout would be possible (see below) |
| 4 | call to undeclared function 'dup3' | SYSTEM | dup3() is required (new in Issue 8) |
| 68 | 'monetary.h' file not found | mixed | 58 TEST-BUG (lc.h included it for all locale tests, only strfmon() uses it), 10 SYSTEM (strfmon: <monetary.h> is required) |
| 2 | variable has incomplete type 'struct sigevent' | SYSTEM | struct sigevent (timers, required) is only forward-declared |
| 2 | call to undeclared function 'fexecve' | SYSTEM | fexecve() is required |
| 7 | call to undeclared function 'at_quick_exit' | SYSTEM | at_quick_exit()/quick_exit() are required (<stdlib.h>) |
| 8 | 'aio.h' file not found | SYSTEM | <aio.h> is required; Issue 8 has no option shading on it |
| 5 | use of undeclared identifier 'RLIMIT_AS' | mixed | free.002 TEST-BUG (the cap is best effort; guarded); strdup.002/.006 and getrlimit.001/.017 SYSTEM (required constant, the means of the assertion) |
| 2 | use of undeclared identifier 'AI_V4MAPPED' | SYSTEM | AI_V4MAPPED/AI_ALL are defined unshaded in <netdb.h>; IP6 is claimed here |
| 1 | use of undeclared identifier 'NI_NUMERICSCOPE' | SYSTEM | NI_NUMERICSCOPE is defined unshaded in <netdb.h> |
| 28 | 'iconv.h' file not found | mixed | 12 TEST-BUG (gt.h/td.h included it for all tests; only gettext.014/.015 and textdomain.008/.009 use iconv: 4 gettext + 8 textdomain), 16 SYSTEM (14 iconv tests + textdomain.008/.009; required header absent) |
| 1 | use of undeclared identifier 'SIGRTMAX' | mixed | kill.006 TEST-BUG (only a loop bound; guarded); sigtimedwait/sigqueue/sigwait SYSTEM |
| 4 | call to undeclared function 'pipe2' | SYSTEM | pipe2() is required (new in Issue 8) |
| 34 | call to undeclared function 'sched_getparam' | TEST-BUG | dead "sched" mode of the spawned image used sched_getparam()/sched_getscheduler() (PS option, not claimed here); mode removed |
| 2 | call to undeclared function 'psiginfo' | SYSTEM | psiginfo() is CX (base) |
| 19 | call to undeclared function 'pthread_mutexattr_setrobust' | mixed | 17 TEST-BUG (make_mutex() had an unguarded TSH call setpshared and an unused robust path; lock.012 still ends in its robust subject), 2 SYSTEM (robust mutex is the subject: cond_wait.007, mutex_consistent.001) |
| 2 | call to undeclared function 'pthread_mutexattr_getrobust' | SYSTEM | robust mutexes are required in Issue 8 (no shading) and are the subject |
| 1 | use of undeclared identifier 'PTHREAD_MUTEX_ROBUST' | ? | ? |
| 1 | use of undeclared identifier 'PTHREAD_MUTEX_STALLED' | SYSTEM | robust mutex constants are required; the assertion is about them |
| 35 | call to undeclared function 'sigtimedwait' | SYSTEM | sigtimedwait() is CX (base); the assertions name it or need it for the timer/real-time subject (sigpending.002 was a TEST-BUG fixed in the tree by another agent) |
| 2 | call to undeclared function 'qsort_r' | SYSTEM | qsort_r() is CX (base) |
| 14 | use of undeclared identifier 'SIGRTMIN' | SYSTEM | SIGRTMIN is required in <signal.h> (RTS is base in Issue 8); the assertions are about it (sigismember.001 was a TEST-BUG fixed in the tree by another agent) |
| 3 | call to undeclared function 'sigqueue' | SYSTEM | sigqueue() is CX (base) |
| 1 | call to undeclared function 'sigwaitinfo' | SYSTEM | sigwaitinfo() is CX (base) |
| 3 | variable has incomplete type 'struct winsize' | SYSTEM | struct winsize is required in <termios.h> |
| 3 | incomplete definition of type 'const struct winsize' | ? | ? |
| 2 | call to undeclared function 'wcpcpy' | SYSTEM | wcpcpy() is CX (base) |
| 2 | call to undeclared function 'wcpncpy' | SYSTEM | wcpncpy() is CX (base) |
| 25 | 'wordexp.h' file not found | SYSTEM | <wordexp.h> is required, unshaded |

No group is a warning-as-error, a link error, or a construct outside C17: the build line only turns implicit function declarations into errors, and every failure is a header, macro, type or function the system does not provide under the feature macros the driver defines. Only the helper-induced groups (locale, gettext, textdomain, spawn, mutex) were test bugs: the failing dependency was not needed by the assertion.

## Per assertion

| ID | class | reason |
|---|---|---|
| XSH.csqrt.006 | SYSTEM | CMPLX/CMPLXF/CMPLXL (required macros of <complex.h>) are not defined; used to build the inputs |
| XSH.ctan.001 | SYSTEM | CMPLX/CMPLXF/CMPLXL (required macros of <complex.h>) are not defined; used to build the inputs |
| XSH.ctan.002 | SYSTEM | CMPLX/CMPLXF/CMPLXL (required macros of <complex.h>) are not defined; used to build the inputs |
| XSH.ctan.003 | SYSTEM | CMPLX/CMPLXF/CMPLXL (required macros of <complex.h>) are not defined; used to build the inputs |
| XSH.ctan.004 | SYSTEM | CMPLX/CMPLXF/CMPLXL (required macros of <complex.h>) are not defined; used to build the inputs |
| XSH.ctan.005 | SYSTEM | CMPLX/CMPLXF/CMPLXL (required macros of <complex.h>) are not defined; used to build the inputs |
| XSH.ctan.006 | SYSTEM | CMPLX/CMPLXF/CMPLXL (required macros of <complex.h>) are not defined; used to build the inputs |
| XSH.ctan.007 | SYSTEM | CMPLX/CMPLXF/CMPLXL (required macros of <complex.h>) are not defined; used to build the inputs |
| XSH.ctan.008 | SYSTEM | CMPLX/CMPLXF/CMPLXL (required macros of <complex.h>) are not defined; used to build the inputs |
| XSH.ctan.009 | SYSTEM | CMPLX/CMPLXF/CMPLXL (required macros of <complex.h>) are not defined; used to build the inputs |
| XSH.ctan.010 | SYSTEM | CMPLX/CMPLXF/CMPLXL (required macros of <complex.h>) are not defined; used to build the inputs |
| XSH.ctan.011 | SYSTEM | CMPLX/CMPLXF/CMPLXL (required macros of <complex.h>) are not defined; used to build the inputs |
| XSH.ctan.012 | SYSTEM | CMPLX/CMPLXF/CMPLXL (required macros of <complex.h>) are not defined; used to build the inputs |
| XSH.ctanh.001 | SYSTEM | CMPLX/CMPLXF/CMPLXL (required macros of <complex.h>) are not defined; used to build the inputs |
| XSH.ctanh.002 | SYSTEM | CMPLX/CMPLXF/CMPLXL (required macros of <complex.h>) are not defined; used to build the inputs |
| XSH.ctanh.003 | SYSTEM | CMPLX/CMPLXF/CMPLXL (required macros of <complex.h>) are not defined; used to build the inputs |
| XSH.ctanh.004 | SYSTEM | CMPLX/CMPLXF/CMPLXL (required macros of <complex.h>) are not defined; used to build the inputs |
| XSH.ctanh.005 | SYSTEM | CMPLX/CMPLXF/CMPLXL (required macros of <complex.h>) are not defined; used to build the inputs |
| XSH.ctanh.006 | SYSTEM | CMPLX/CMPLXF/CMPLXL (required macros of <complex.h>) are not defined; used to build the inputs |
| XSH.ctanh.007 | SYSTEM | CMPLX/CMPLXF/CMPLXL (required macros of <complex.h>) are not defined; used to build the inputs |
| XSH.ctanh.008 | SYSTEM | CMPLX/CMPLXF/CMPLXL (required macros of <complex.h>) are not defined; used to build the inputs |
| XSH.ctanh.009 | SYSTEM | CMPLX/CMPLXF/CMPLXL (required macros of <complex.h>) are not defined; used to build the inputs |
| XSH.ctanh.010 | SYSTEM | CMPLX/CMPLXF/CMPLXL (required macros of <complex.h>) are not defined; used to build the inputs |
| XSH.ctanh.011 | SYSTEM | CMPLX/CMPLXF/CMPLXL (required macros of <complex.h>) are not defined; used to build the inputs |
| XSH.ctanh.012 | SYSTEM | CMPLX/CMPLXF/CMPLXL (required macros of <complex.h>) are not defined; used to build the inputs |
| XSH.dup.012 | SYSTEM | dup3() (required, new in Issue 8) is not declared |
| XSH.dup.013 | SYSTEM | dup3() (required, new in Issue 8) is not declared |
| XSH.dup.014 | SYSTEM | dup3() (required, new in Issue 8) is not declared |
| XSH.dup.015 | SYSTEM | dup3() (required, new in Issue 8) is not declared |
| XSH.duplocale.001 | TEST-BUG | lc.h included <monetary.h> for every locale test, but only strfmon() tests use it; the include is removed from this module (header absent on this system) |
| XSH.duplocale.002 | TEST-BUG | lc.h included <monetary.h> for every locale test, but only strfmon() tests use it; the include is removed from this module (header absent on this system) |
| XSH.duplocale.003 | TEST-BUG | lc.h included <monetary.h> for every locale test, but only strfmon() tests use it; the include is removed from this module (header absent on this system) |
| XSH.duplocale.004 | TEST-BUG | lc.h included <monetary.h> for every locale test, but only strfmon() tests use it; the include is removed from this module (header absent on this system) |
| XSH.duplocale.005 | TEST-BUG | lc.h included <monetary.h> for every locale test, but only strfmon() tests use it; the include is removed from this module (header absent on this system) |
| XSH.exec.029 | SYSTEM | timers (timer_create, struct sigevent; required, base in Issue 8) are absent from <signal.h>/<time.h> |
| XSH.exec.048 | SYSTEM | fexecve() (required) is not declared |
| XSH.exec.049 | SYSTEM | fexecve() (required) is not declared |
| XSH.exit.006 | SYSTEM | at_quick_exit()/quick_exit() (required in <stdlib.h>) are not declared |
| XSH.fork.012 | SYSTEM | timers (timer_create, struct sigevent, sigtimedwait; required) are absent; the assertion is about timers being not inherited |
| XSH.fork.023 | SYSTEM | <aio.h> (required, no option shading in Issue 8) is absent |
| XSH.free.002 | TEST-BUG | RLIMIT_AS cap is stated to be best effort but made the file unbuildable; now guarded |
| XSH.freelocale.001 | TEST-BUG | lc.h included <monetary.h> for every locale test, but only strfmon() tests use it; the include is removed from this module (header absent on this system) |
| XSH.freelocale.002 | TEST-BUG | lc.h included <monetary.h> for every locale test, but only strfmon() tests use it; the include is removed from this module (header absent on this system) |
| XSH.getaddrinfo.011 | SYSTEM | AI_V4MAPPED/AI_ALL (<netdb.h>, unshaded; IP6 is claimed here) are not defined |
| XSH.getaddrinfo.022 | SYSTEM | AI_V4MAPPED/AI_ALL (<netdb.h>, unshaded; IP6 is claimed here) are not defined |
| XSH.getnameinfo.012 | SYSTEM | NI_NUMERICSCOPE (<netdb.h>, unshaded) is not defined |
| XSH.getrlimit.001 | SYSTEM | RLIMIT_AS (required in <sys/resource.h>) is not defined; it is the means of the assertion |
| XSH.getrlimit.017 | SYSTEM | RLIMIT_AS (required in <sys/resource.h>) is not defined; it is the means of the assertion |
| XSH.gettext.001 | TEST-BUG | gt.h included <iconv.h> for every test, but only gettext.014/.015 use iconv; the include moved to those two (header absent here) |
| XSH.gettext.002 | TEST-BUG | gt.h included <iconv.h> for every test, but only gettext.014/.015 use iconv; the include moved to those two (header absent here) |
| XSH.gettext.003 | TEST-BUG | gt.h included <iconv.h> for every test, but only gettext.014/.015 use iconv; the include moved to those two (header absent here) |
| XSH.gettext.006 | TEST-BUG | gt.h included <iconv.h> for every test, but only gettext.014/.015 use iconv; the include moved to those two (header absent here) |
| XSH.iconv.001 | SYSTEM | <iconv.h> (required, unshaded) is absent |
| XSH.iconv.002 | SYSTEM | <iconv.h> (required, unshaded) is absent |
| XSH.iconv.003 | SYSTEM | <iconv.h> (required, unshaded) is absent |
| XSH.iconv.004 | SYSTEM | <iconv.h> (required, unshaded) is absent |
| XSH.iconv.005 | SYSTEM | <iconv.h> (required, unshaded) is absent |
| XSH.iconv.009 | SYSTEM | <iconv.h> (required, unshaded) is absent |
| XSH.iconv.010 | SYSTEM | <iconv.h> (required, unshaded) is absent |
| XSH.iconv.011 | SYSTEM | <iconv.h> (required, unshaded) is absent |
| XSH.iconv.012 | SYSTEM | <iconv.h> (required, unshaded) is absent |
| XSH.iconv_close.001 | SYSTEM | <iconv.h> (required, unshaded) is absent |
| XSH.iconv_close.002 | SYSTEM | <iconv.h> (required, unshaded) is absent |
| XSH.iconv_open.001 | SYSTEM | <iconv.h> (required, unshaded) is absent |
| XSH.iconv_open.002 | SYSTEM | <iconv.h> (required, unshaded) is absent |
| XSH.iconv_open.003 | SYSTEM | <iconv.h> (required, unshaded) is absent |
| XSH.kill.006 | TEST-BUG | SIGRTMAX used only as a loop bound for scanning pending signals; now guarded |
| XSH.lio_listio.001 | SYSTEM | <aio.h> (required, no option shading in Issue 8) is absent |
| XSH.lio_listio.002 | SYSTEM | <aio.h> (required, no option shading in Issue 8) is absent |
| XSH.lio_listio.003 | SYSTEM | <aio.h> (required, no option shading in Issue 8) is absent |
| XSH.lio_listio.004 | SYSTEM | <aio.h> (required, no option shading in Issue 8) is absent |
| XSH.lio_listio.005 | SYSTEM | <aio.h> (required, no option shading in Issue 8) is absent |
| XSH.lio_listio.006 | SYSTEM | <aio.h> (required, no option shading in Issue 8) is absent |
| XSH.lio_listio.007 | SYSTEM | <aio.h> (required, no option shading in Issue 8) is absent |
| XSH.localeconv.001 | TEST-BUG | lc.h included <monetary.h> for every locale test, but only strfmon() tests use it; the include is removed from this module (header absent on this system) |
| XSH.localeconv.002 | TEST-BUG | lc.h included <monetary.h> for every locale test, but only strfmon() tests use it; the include is removed from this module (header absent on this system) |
| XSH.localeconv.003 | TEST-BUG | lc.h included <monetary.h> for every locale test, but only strfmon() tests use it; the include is removed from this module (header absent on this system) |
| XSH.localeconv.004 | TEST-BUG | lc.h included <monetary.h> for every locale test, but only strfmon() tests use it; the include is removed from this module (header absent on this system) |
| XSH.localeconv.005 | TEST-BUG | lc.h included <monetary.h> for every locale test, but only strfmon() tests use it; the include is removed from this module (header absent on this system) |
| XSH.localeconv.006 | TEST-BUG | lc.h included <monetary.h> for every locale test, but only strfmon() tests use it; the include is removed from this module (header absent on this system) |
| XSH.newlocale.001 | TEST-BUG | lc.h included <monetary.h> for every locale test, but only strfmon() tests use it; the include is removed from this module (header absent on this system) |
| XSH.newlocale.002 | TEST-BUG | lc.h included <monetary.h> for every locale test, but only strfmon() tests use it; the include is removed from this module (header absent on this system) |
| XSH.newlocale.003 | TEST-BUG | lc.h included <monetary.h> for every locale test, but only strfmon() tests use it; the include is removed from this module (header absent on this system) |
| XSH.newlocale.004 | TEST-BUG | lc.h included <monetary.h> for every locale test, but only strfmon() tests use it; the include is removed from this module (header absent on this system) |
| XSH.newlocale.005 | TEST-BUG | lc.h included <monetary.h> for every locale test, but only strfmon() tests use it; the include is removed from this module (header absent on this system) |
| XSH.newlocale.006 | TEST-BUG | lc.h included <monetary.h> for every locale test, but only strfmon() tests use it; the include is removed from this module (header absent on this system) |
| XSH.newlocale.007 | TEST-BUG | lc.h included <monetary.h> for every locale test, but only strfmon() tests use it; the include is removed from this module (header absent on this system) |
| XSH.newlocale.008 | TEST-BUG | lc.h included <monetary.h> for every locale test, but only strfmon() tests use it; the include is removed from this module (header absent on this system) |
| XSH.nl_langinfo.001 | TEST-BUG | lc.h included <monetary.h> for every locale test, but only strfmon() tests use it; the include is removed from this module (header absent on this system) |
| XSH.nl_langinfo.002 | TEST-BUG | lc.h included <monetary.h> for every locale test, but only strfmon() tests use it; the include is removed from this module (header absent on this system) |
| XSH.nl_langinfo.003 | TEST-BUG | lc.h included <monetary.h> for every locale test, but only strfmon() tests use it; the include is removed from this module (header absent on this system) |
| XSH.nl_langinfo.004 | TEST-BUG | lc.h included <monetary.h> for every locale test, but only strfmon() tests use it; the include is removed from this module (header absent on this system) |
| XSH.nl_langinfo.005 | TEST-BUG | lc.h included <monetary.h> for every locale test, but only strfmon() tests use it; the include is removed from this module (header absent on this system) |
| XSH.nl_langinfo.008 | TEST-BUG | lc.h included <monetary.h> for every locale test, but only strfmon() tests use it; the include is removed from this module (header absent on this system) |
| XSH.nl_langinfo.009 | TEST-BUG | lc.h included <monetary.h> for every locale test, but only strfmon() tests use it; the include is removed from this module (header absent on this system) |
| XSH.nl_langinfo.010 | TEST-BUG | lc.h included <monetary.h> for every locale test, but only strfmon() tests use it; the include is removed from this module (header absent on this system) |
| XSH.nl_langinfo.011 | TEST-BUG | lc.h included <monetary.h> for every locale test, but only strfmon() tests use it; the include is removed from this module (header absent on this system) |
| XSH.pipe.007 | SYSTEM | pipe2() (required, new in Issue 8) is not declared |
| XSH.pipe.008 | SYSTEM | pipe2() (required, new in Issue 8) is not declared |
| XSH.pipe.009 | SYSTEM | pipe2() (required, new in Issue 8) is not declared |
| XSH.pipe.010 | SYSTEM | pipe2() (required, new in Issue 8) is not declared |
| XSH.posix_spawn_file_actions_addchdir.002 | TEST-BUG | the child image of the test had an unused mode "sched" calling sched_getparam()/sched_getscheduler() (PS option, not claimed, not declared); the dead mode is removed |
| XSH.posix_spawn_file_actions_addchdir.003 | TEST-BUG | the child image of the test had an unused mode "sched" calling sched_getparam()/sched_getscheduler() (PS option, not claimed, not declared); the dead mode is removed |
| XSH.posix_spawn_file_actions_addchdir.004 | TEST-BUG | the child image of the test had an unused mode "sched" calling sched_getparam()/sched_getscheduler() (PS option, not claimed, not declared); the dead mode is removed |
| XSH.posix_spawn_file_actions_addchdir.005 | TEST-BUG | the child image of the test had an unused mode "sched" calling sched_getparam()/sched_getscheduler() (PS option, not claimed, not declared); the dead mode is removed |
| XSH.posix_spawn_file_actions_addchdir.006 | TEST-BUG | the child image of the test had an unused mode "sched" calling sched_getparam()/sched_getscheduler() (PS option, not claimed, not declared); the dead mode is removed |
| XSH.posix_spawn_file_actions_addchdir.008 | TEST-BUG | the child image of the test had an unused mode "sched" calling sched_getparam()/sched_getscheduler() (PS option, not claimed, not declared); the dead mode is removed |
| XSH.posix_spawn_file_actions_addchdir.010 | TEST-BUG | the child image of the test had an unused mode "sched" calling sched_getparam()/sched_getscheduler() (PS option, not claimed, not declared); the dead mode is removed |
| XSH.posix_spawn_file_actions_addclose.002 | TEST-BUG | the child image of the test had an unused mode "sched" calling sched_getparam()/sched_getscheduler() (PS option, not claimed, not declared); the dead mode is removed |
| XSH.posix_spawn_file_actions_addclose.003 | TEST-BUG | the child image of the test had an unused mode "sched" calling sched_getparam()/sched_getscheduler() (PS option, not claimed, not declared); the dead mode is removed |
| XSH.posix_spawn_file_actions_addclose.005 | TEST-BUG | the child image of the test had an unused mode "sched" calling sched_getparam()/sched_getscheduler() (PS option, not claimed, not declared); the dead mode is removed |
| XSH.posix_spawn_file_actions_addclose.006 | TEST-BUG | the child image of the test had an unused mode "sched" calling sched_getparam()/sched_getscheduler() (PS option, not claimed, not declared); the dead mode is removed |
| XSH.posix_spawn_file_actions_addclose.008 | TEST-BUG | the child image of the test had an unused mode "sched" calling sched_getparam()/sched_getscheduler() (PS option, not claimed, not declared); the dead mode is removed |
| XSH.posix_spawn_file_actions_addclose.009 | TEST-BUG | the child image of the test had an unused mode "sched" calling sched_getparam()/sched_getscheduler() (PS option, not claimed, not declared); the dead mode is removed |
| XSH.posix_spawn_file_actions_addclose.010 | TEST-BUG | the child image of the test had an unused mode "sched" calling sched_getparam()/sched_getscheduler() (PS option, not claimed, not declared); the dead mode is removed |
| XSH.posix_spawn_file_actions_addclose.011 | TEST-BUG | the child image of the test had an unused mode "sched" calling sched_getparam()/sched_getscheduler() (PS option, not claimed, not declared); the dead mode is removed |
| XSH.posix_spawn_file_actions_addclose.014 | TEST-BUG | the child image of the test had an unused mode "sched" calling sched_getparam()/sched_getscheduler() (PS option, not claimed, not declared); the dead mode is removed |
| XSH.posix_spawn_file_actions_addclose.015 | TEST-BUG | the child image of the test had an unused mode "sched" calling sched_getparam()/sched_getscheduler() (PS option, not claimed, not declared); the dead mode is removed |
| XSH.posix_spawn_file_actions_addclose.016 | TEST-BUG | the child image of the test had an unused mode "sched" calling sched_getparam()/sched_getscheduler() (PS option, not claimed, not declared); the dead mode is removed |
| XSH.posix_spawn_file_actions_addclose.017 | TEST-BUG | the child image of the test had an unused mode "sched" calling sched_getparam()/sched_getscheduler() (PS option, not claimed, not declared); the dead mode is removed |
| XSH.posix_spawn_file_actions_adddup2.002 | TEST-BUG | the child image of the test had an unused mode "sched" calling sched_getparam()/sched_getscheduler() (PS option, not claimed, not declared); the dead mode is removed |
| XSH.posix_spawn_file_actions_adddup2.003 | TEST-BUG | the child image of the test had an unused mode "sched" calling sched_getparam()/sched_getscheduler() (PS option, not claimed, not declared); the dead mode is removed |
| XSH.posix_spawn_file_actions_adddup2.004 | TEST-BUG | the child image of the test had an unused mode "sched" calling sched_getparam()/sched_getscheduler() (PS option, not claimed, not declared); the dead mode is removed |
| XSH.posix_spawn_file_actions_adddup2.005 | TEST-BUG | the child image of the test had an unused mode "sched" calling sched_getparam()/sched_getscheduler() (PS option, not claimed, not declared); the dead mode is removed |
| XSH.posix_spawn_file_actions_adddup2.008 | TEST-BUG | the child image of the test had an unused mode "sched" calling sched_getparam()/sched_getscheduler() (PS option, not claimed, not declared); the dead mode is removed |
| XSH.posix_spawn_file_actions_init.003 | TEST-BUG | the child image of the test had an unused mode "sched" calling sched_getparam()/sched_getscheduler() (PS option, not claimed, not declared); the dead mode is removed |
| XSH.posix_spawn_file_actions_init.004 | TEST-BUG | the child image of the test had an unused mode "sched" calling sched_getparam()/sched_getscheduler() (PS option, not claimed, not declared); the dead mode is removed |
| XSH.posix_spawn_file_actions_init.005 | TEST-BUG | the child image of the test had an unused mode "sched" calling sched_getparam()/sched_getscheduler() (PS option, not claimed, not declared); the dead mode is removed |
| XSH.posix_spawnattr_getflags.006 | TEST-BUG | the child image of the test had an unused mode "sched" calling sched_getparam()/sched_getscheduler() (PS option, not claimed, not declared); the dead mode is removed |
| XSH.posix_spawnattr_getpgroup.003 | TEST-BUG | the child image of the test had an unused mode "sched" calling sched_getparam()/sched_getscheduler() (PS option, not claimed, not declared); the dead mode is removed |
| XSH.posix_spawnattr_getpgroup.004 | TEST-BUG | the child image of the test had an unused mode "sched" calling sched_getparam()/sched_getscheduler() (PS option, not claimed, not declared); the dead mode is removed |
| XSH.posix_spawnattr_getsigdefault.004 | TEST-BUG | the child image of the test had an unused mode "sched" calling sched_getparam()/sched_getscheduler() (PS option, not claimed, not declared); the dead mode is removed |
| XSH.posix_spawnattr_getsigmask.004 | TEST-BUG | the child image of the test had an unused mode "sched" calling sched_getparam()/sched_getscheduler() (PS option, not claimed, not declared); the dead mode is removed |
| XSH.posix_spawnattr_init.005 | TEST-BUG | the child image of the test had an unused mode "sched" calling sched_getparam()/sched_getscheduler() (PS option, not claimed, not declared); the dead mode is removed |
| XSH.posix_spawnattr_init.006 | TEST-BUG | the child image of the test had an unused mode "sched" calling sched_getparam()/sched_getscheduler() (PS option, not claimed, not declared); the dead mode is removed |
| XSH.psiginfo.006 | SYSTEM | psiginfo() (CX, required) is not declared |
| XSH.pthread_cond_wait.007 | SYSTEM | robust mutexes (pthread_mutexattr_setrobust/getrobust, pthread_mutex_consistent, PTHREAD_MUTEX_ROBUST/STALLED; required) are absent; the assertion is about them |
| XSH.pthread_mutex_consistent.001 | SYSTEM | robust mutexes (pthread_mutexattr_setrobust/getrobust, pthread_mutex_consistent, PTHREAD_MUTEX_ROBUST/STALLED; required) are absent; the assertion is about them |
| XSH.pthread_mutex_init.001 | TEST-BUG | helper make_mutex() called pthread_mutexattr_setpshared() (TSH option, not claimed) and setrobust() unguarded though this test never uses either; now guarded |
| XSH.pthread_mutex_init.002 | TEST-BUG | helper make_mutex() called pthread_mutexattr_setpshared() (TSH option, not claimed) and setrobust() unguarded though this test never uses either; now guarded |
| XSH.pthread_mutex_init.003 | TEST-BUG | helper make_mutex() called pthread_mutexattr_setpshared() (TSH option, not claimed) and setrobust() unguarded though this test never uses either; now guarded |
| XSH.pthread_mutex_init.004 | TEST-BUG | helper make_mutex() called pthread_mutexattr_setpshared() (TSH option, not claimed) and setrobust() unguarded though this test never uses either; now guarded |
| XSH.pthread_mutex_lock.001 | TEST-BUG | helper make_mutex() called pthread_mutexattr_setpshared() (TSH option, not claimed) and setrobust() unguarded though this test never uses either; now guarded |
| XSH.pthread_mutex_lock.002 | TEST-BUG | helper make_mutex() called pthread_mutexattr_setpshared() (TSH option, not claimed) and setrobust() unguarded though this test never uses either; now guarded |
| XSH.pthread_mutex_lock.003 | TEST-BUG | helper make_mutex() called pthread_mutexattr_setpshared() (TSH option, not claimed) and setrobust() unguarded though this test never uses either; now guarded |
| XSH.pthread_mutex_lock.004 | TEST-BUG | helper make_mutex() called pthread_mutexattr_setpshared() (TSH option, not claimed) and setrobust() unguarded though this test never uses either; now guarded |
| XSH.pthread_mutex_lock.005 | TEST-BUG | helper make_mutex() called pthread_mutexattr_setpshared() (TSH option, not claimed) and setrobust() unguarded though this test never uses either; now guarded |
| XSH.pthread_mutex_lock.006 | TEST-BUG | helper make_mutex() called pthread_mutexattr_setpshared() (TSH option, not claimed) and setrobust() unguarded though this test never uses either; now guarded |
| XSH.pthread_mutex_lock.007 | TEST-BUG | helper make_mutex() called pthread_mutexattr_setpshared() (TSH option, not claimed) and setrobust() unguarded though this test never uses either; now guarded |
| XSH.pthread_mutex_lock.008 | TEST-BUG | helper make_mutex() called pthread_mutexattr_setpshared() (TSH option, not claimed) and setrobust() unguarded though this test never uses either; now guarded |
| XSH.pthread_mutex_lock.009 | TEST-BUG | helper make_mutex() called pthread_mutexattr_setpshared() (TSH option, not claimed) and setrobust() unguarded though this test never uses either; now guarded |
| XSH.pthread_mutex_lock.010 | TEST-BUG | helper make_mutex() called pthread_mutexattr_setpshared() (TSH option, not claimed) and setrobust() unguarded though this test never uses either; now guarded |
| XSH.pthread_mutex_lock.011 | TEST-BUG | helper make_mutex() called pthread_mutexattr_setpshared() (TSH option, not claimed) and setrobust() unguarded though this test never uses either; now guarded |
| XSH.pthread_mutex_lock.012 | TEST-BUG | helper make_mutex() called pthread_mutexattr_setpshared() (TSH option) unguarded; now guarded; the test is about robust mutexes (setrobust/PTHREAD_MUTEX_ROBUST required, absent here), so it ends UNRESOLVED here (SYSTEM) |
| XSH.pthread_mutex_lock.015 | TEST-BUG | helper make_mutex() called pthread_mutexattr_setpshared() (TSH option, not claimed) and setrobust() unguarded though this test never uses either; now guarded |
| XSH.pthread_mutexattr_getrobust.001 | SYSTEM | robust mutexes (pthread_mutexattr_setrobust/getrobust, pthread_mutex_consistent, PTHREAD_MUTEX_ROBUST/STALLED; required) are absent; the assertion is about them |
| XSH.pthread_mutexattr_getrobust.002 | SYSTEM | robust mutexes (pthread_mutexattr_setrobust/getrobust, pthread_mutex_consistent, PTHREAD_MUTEX_ROBUST/STALLED; required) are absent; the assertion is about them |
| XSH.pthread_mutexattr_getrobust.003 | SYSTEM | robust mutexes (pthread_mutexattr_setrobust/getrobust, pthread_mutex_consistent, PTHREAD_MUTEX_ROBUST/STALLED; required) are absent; the assertion is about them |
| XSH.pthread_mutexattr_init.002 | SYSTEM | robust mutexes (pthread_mutexattr_setrobust/getrobust, pthread_mutex_consistent, PTHREAD_MUTEX_ROBUST/STALLED; required) are absent; the assertion is about them |
| XSH.pthread_sigmask.011 | SYSTEM | the assertion says the signals are accepted by sigtimedwait() (required), which is not declared |
| XSH.qsort.005 | SYSTEM | qsort_r() (CX, required) is not declared |
| XSH.qsort.006 | SYSTEM | qsort_r() (CX, required) is not declared |
| XSH.quick_exit.001 | SYSTEM | at_quick_exit()/quick_exit() (required in <stdlib.h>) are not declared |
| XSH.quick_exit.002 | SYSTEM | at_quick_exit()/quick_exit() (required in <stdlib.h>) are not declared |
| XSH.quick_exit.003 | SYSTEM | at_quick_exit()/quick_exit() (required in <stdlib.h>) are not declared |
| XSH.quick_exit.004 | SYSTEM | at_quick_exit()/quick_exit() (required in <stdlib.h>) are not declared |
| XSH.quick_exit.005 | SYSTEM | at_quick_exit()/quick_exit() (required in <stdlib.h>) are not declared |
| XSH.quick_exit.006 | SYSTEM | at_quick_exit()/quick_exit() (required in <stdlib.h>) are not declared |
| XSH.setlocale.001 | TEST-BUG | lc.h included <monetary.h> for every locale test, but only strfmon() tests use it; the include is removed from this module (header absent on this system) |
| XSH.setlocale.002 | TEST-BUG | lc.h included <monetary.h> for every locale test, but only strfmon() tests use it; the include is removed from this module (header absent on this system) |
| XSH.setlocale.003 | TEST-BUG | lc.h included <monetary.h> for every locale test, but only strfmon() tests use it; the include is removed from this module (header absent on this system) |
| XSH.setlocale.004 | TEST-BUG | lc.h included <monetary.h> for every locale test, but only strfmon() tests use it; the include is removed from this module (header absent on this system) |
| XSH.setlocale.005 | TEST-BUG | lc.h included <monetary.h> for every locale test, but only strfmon() tests use it; the include is removed from this module (header absent on this system) |
| XSH.setlocale.006 | TEST-BUG | lc.h included <monetary.h> for every locale test, but only strfmon() tests use it; the include is removed from this module (header absent on this system) |
| XSH.setlocale.007 | TEST-BUG | lc.h included <monetary.h> for every locale test, but only strfmon() tests use it; the include is removed from this module (header absent on this system) |
| XSH.setlocale.008 | TEST-BUG | lc.h included <monetary.h> for every locale test, but only strfmon() tests use it; the include is removed from this module (header absent on this system) |
| XSH.setlocale.009 | TEST-BUG | lc.h included <monetary.h> for every locale test, but only strfmon() tests use it; the include is removed from this module (header absent on this system) |
| XSH.sigaction.005 | SYSTEM | the assertion names SIGRTMIN / sigqueue() (required), which are absent |
| XSH.sigaction.006 | SYSTEM | the assertion names SIGRTMIN / sigqueue() (required), which are absent |
| XSH.sigaction.007 | SYSTEM | the assertion accepts SIGCHLD with sigtimedwait() (required), which is not declared |
| XSH.sigaction.010 | SYSTEM | the assertion names SIGRTMIN / sigqueue() (required), which are absent |
| XSH.sigaction.011 | SYSTEM | the assertion accepts SIGCHLD with sigtimedwait() (required), which is not declared |
| XSH.sigaction.020 | SYSTEM | the assertion names SIGRTMIN / sigqueue() (required), which are absent |
| XSH.sigismember.001 | TEST-BUG | incidental use of SIGRTMIN / sigtimedwait() (cleanup) in a test about other things; fixed in the tree by another triage agent while this group was being worked (guarded / replaced) |
| XSH.sigpending.002 | TEST-BUG | incidental use of SIGRTMIN / sigtimedwait() (cleanup) in a test about other things; fixed in the tree by another triage agent while this group was being worked (guarded / replaced) |
| XSH.sigqueue.001 | SYSTEM | sigqueue()/sigtimedwait()/sigwaitinfo()/SIGRTMIN/SIGRTMAX (required, base in Issue 8) are absent; the assertion is about them |
| XSH.sigqueue.002 | SYSTEM | sigqueue()/sigtimedwait()/sigwaitinfo()/SIGRTMIN/SIGRTMAX (required, base in Issue 8) are absent; the assertion is about them |
| XSH.sigqueue.003 | SYSTEM | sigqueue()/sigtimedwait()/sigwaitinfo()/SIGRTMIN/SIGRTMAX (required, base in Issue 8) are absent; the assertion is about them |
| XSH.sigqueue.004 | SYSTEM | sigqueue()/sigtimedwait()/sigwaitinfo()/SIGRTMIN/SIGRTMAX (required, base in Issue 8) are absent; the assertion is about them |
| XSH.sigqueue.005 | SYSTEM | sigqueue()/sigtimedwait()/sigwaitinfo()/SIGRTMIN/SIGRTMAX (required, base in Issue 8) are absent; the assertion is about them |
| XSH.sigqueue.006 | SYSTEM | sigqueue()/sigtimedwait()/sigwaitinfo()/SIGRTMIN/SIGRTMAX (required, base in Issue 8) are absent; the assertion is about them |
| XSH.sigqueue.007 | SYSTEM | sigqueue()/sigtimedwait()/sigwaitinfo()/SIGRTMIN/SIGRTMAX (required, base in Issue 8) are absent; the assertion is about them |
| XSH.sigqueue.008 | SYSTEM | sigqueue()/sigtimedwait()/sigwaitinfo()/SIGRTMIN/SIGRTMAX (required, base in Issue 8) are absent; the assertion is about them |
| XSH.sigqueue.009 | SYSTEM | sigqueue()/sigtimedwait()/sigwaitinfo()/SIGRTMIN/SIGRTMAX (required, base in Issue 8) are absent; the assertion is about them |
| XSH.sigtimedwait.002 | SYSTEM | sigqueue()/sigtimedwait()/sigwaitinfo()/SIGRTMIN/SIGRTMAX (required, base in Issue 8) are absent; the assertion is about them |
| XSH.sigtimedwait.003 | SYSTEM | sigqueue()/sigtimedwait()/sigwaitinfo()/SIGRTMIN/SIGRTMAX (required, base in Issue 8) are absent; the assertion is about them |
| XSH.sigtimedwait.004 | SYSTEM | sigqueue()/sigtimedwait()/sigwaitinfo()/SIGRTMIN/SIGRTMAX (required, base in Issue 8) are absent; the assertion is about them |
| XSH.sigtimedwait.005 | SYSTEM | sigqueue()/sigtimedwait()/sigwaitinfo()/SIGRTMIN/SIGRTMAX (required, base in Issue 8) are absent; the assertion is about them |
| XSH.sigtimedwait.006 | SYSTEM | sigqueue()/sigtimedwait()/sigwaitinfo()/SIGRTMIN/SIGRTMAX (required, base in Issue 8) are absent; the assertion is about them |
| XSH.sigtimedwait.007 | SYSTEM | sigqueue()/sigtimedwait()/sigwaitinfo()/SIGRTMIN/SIGRTMAX (required, base in Issue 8) are absent; the assertion is about them |
| XSH.sigtimedwait.008 | SYSTEM | sigqueue()/sigtimedwait()/sigwaitinfo()/SIGRTMIN/SIGRTMAX (required, base in Issue 8) are absent; the assertion is about them |
| XSH.sigwait.004 | SYSTEM | sigqueue()/sigtimedwait()/sigwaitinfo()/SIGRTMIN/SIGRTMAX (required, base in Issue 8) are absent; the assertion is about them |
| XSH.strcoll.001 | TEST-BUG | lc.h included <monetary.h> for every locale test, but only strfmon() tests use it; the include is removed from this module (header absent on this system) |
| XSH.strcoll.002 | TEST-BUG | lc.h included <monetary.h> for every locale test, but only strfmon() tests use it; the include is removed from this module (header absent on this system) |
| XSH.strcoll.003 | TEST-BUG | lc.h included <monetary.h> for every locale test, but only strfmon() tests use it; the include is removed from this module (header absent on this system) |
| XSH.strcoll.004 | TEST-BUG | lc.h included <monetary.h> for every locale test, but only strfmon() tests use it; the include is removed from this module (header absent on this system) |
| XSH.strcoll.005 | TEST-BUG | lc.h included <monetary.h> for every locale test, but only strfmon() tests use it; the include is removed from this module (header absent on this system) |
| XSH.strcoll.006 | TEST-BUG | lc.h included <monetary.h> for every locale test, but only strfmon() tests use it; the include is removed from this module (header absent on this system) |
| XSH.strcoll.007 | TEST-BUG | lc.h included <monetary.h> for every locale test, but only strfmon() tests use it; the include is removed from this module (header absent on this system) |
| XSH.strdup.002 | SYSTEM | RLIMIT_AS (required in <sys/resource.h>) is not defined; it is the means of the assertion |
| XSH.strdup.006 | SYSTEM | RLIMIT_AS (required in <sys/resource.h>) is not defined; it is the means of the assertion |
| XSH.strfmon.001 | SYSTEM | <monetary.h> (strfmon, required, unshaded) is absent |
| XSH.strfmon.002 | SYSTEM | <monetary.h> (strfmon, required, unshaded) is absent |
| XSH.strfmon.003 | SYSTEM | <monetary.h> (strfmon, required, unshaded) is absent |
| XSH.strfmon.004 | SYSTEM | <monetary.h> (strfmon, required, unshaded) is absent |
| XSH.strfmon.007 | SYSTEM | <monetary.h> (strfmon, required, unshaded) is absent |
| XSH.strfmon.008 | SYSTEM | <monetary.h> (strfmon, required, unshaded) is absent |
| XSH.strfmon.009 | SYSTEM | <monetary.h> (strfmon, required, unshaded) is absent |
| XSH.strfmon.010 | SYSTEM | <monetary.h> (strfmon, required, unshaded) is absent |
| XSH.strfmon.011 | SYSTEM | <monetary.h> (strfmon, required, unshaded) is absent |
| XSH.strfmon.012 | SYSTEM | <monetary.h> (strfmon, required, unshaded) is absent |
| XSH.strsignal.003 | SYSTEM | the assertion names psiginfo() (CX, required), which is not declared |
| XSH.strxfrm.001 | TEST-BUG | lc.h included <monetary.h> for every locale test, but only strfmon() tests use it; the include is removed from this module (header absent on this system) |
| XSH.strxfrm.002 | TEST-BUG | lc.h included <monetary.h> for every locale test, but only strfmon() tests use it; the include is removed from this module (header absent on this system) |
| XSH.strxfrm.003 | TEST-BUG | lc.h included <monetary.h> for every locale test, but only strfmon() tests use it; the include is removed from this module (header absent on this system) |
| XSH.strxfrm.004 | TEST-BUG | lc.h included <monetary.h> for every locale test, but only strfmon() tests use it; the include is removed from this module (header absent on this system) |
| XSH.strxfrm.005 | TEST-BUG | lc.h included <monetary.h> for every locale test, but only strfmon() tests use it; the include is removed from this module (header absent on this system) |
| XSH.strxfrm.006 | TEST-BUG | lc.h included <monetary.h> for every locale test, but only strfmon() tests use it; the include is removed from this module (header absent on this system) |
| XSH.strxfrm.007 | TEST-BUG | lc.h included <monetary.h> for every locale test, but only strfmon() tests use it; the include is removed from this module (header absent on this system) |
| XSH.tcgetwinsize.001 | SYSTEM | tcgetwinsize()/tcsetwinsize() and struct winsize (required in <termios.h>) are absent |
| XSH.tcgetwinsize.002 | SYSTEM | tcgetwinsize()/tcsetwinsize() and struct winsize (required in <termios.h>) are absent |
| XSH.tcgetwinsize.003 | SYSTEM | tcgetwinsize()/tcsetwinsize() and struct winsize (required in <termios.h>) are absent |
| XSH.tcgetwinsize.005 | SYSTEM | tcgetwinsize()/tcsetwinsize() and struct winsize (required in <termios.h>) are absent |
| XSH.tcsetwinsize.001 | SYSTEM | tcgetwinsize()/tcsetwinsize() and struct winsize (required in <termios.h>) are absent |
| XSH.tcsetwinsize.004 | SYSTEM | tcgetwinsize()/tcsetwinsize() and struct winsize (required in <termios.h>) are absent |
| XSH.textdomain.001 | TEST-BUG | td.h included <iconv.h> for every test, but only textdomain.008/.009 use it; now opt-in (TD_ICONV) |
| XSH.textdomain.002 | TEST-BUG | td.h included <iconv.h> for every test, but only textdomain.008/.009 use it; now opt-in (TD_ICONV) |
| XSH.textdomain.003 | TEST-BUG | td.h included <iconv.h> for every test, but only textdomain.008/.009 use it; now opt-in (TD_ICONV) |
| XSH.textdomain.004 | TEST-BUG | td.h included <iconv.h> for every test, but only textdomain.008/.009 use it; now opt-in (TD_ICONV) |
| XSH.textdomain.005 | TEST-BUG | td.h included <iconv.h> for every test, but only textdomain.008/.009 use it; now opt-in (TD_ICONV) |
| XSH.textdomain.006 | TEST-BUG | td.h included <iconv.h> for every test, but only textdomain.008/.009 use it; now opt-in (TD_ICONV) |
| XSH.textdomain.007 | TEST-BUG | td.h included <iconv.h> for every test, but only textdomain.008/.009 use it; now opt-in (TD_ICONV) |
| XSH.textdomain.008 | SYSTEM | the test binds a code set and picks one with iconv_open(); <iconv.h> (required) is absent |
| XSH.textdomain.009 | SYSTEM | the test binds a code set and picks one with iconv_open(); <iconv.h> (required) is absent |
| XSH.textdomain.010 | TEST-BUG | td.h included <iconv.h> for every test, but only textdomain.008/.009 use it; now opt-in (TD_ICONV) |
| XSH.timer_create.001 | SYSTEM | timers (timer_create, struct sigevent, SIGEV_*; required, base in Issue 8) and sigtimedwait()/SIGRTMIN are absent |
| XSH.timer_create.002 | SYSTEM | timers (timer_create, struct sigevent, SIGEV_*; required, base in Issue 8) and sigtimedwait()/SIGRTMIN are absent |
| XSH.timer_create.003 | SYSTEM | timers (timer_create, struct sigevent, SIGEV_*; required, base in Issue 8) and sigtimedwait()/SIGRTMIN are absent |
| XSH.timer_create.004 | SYSTEM | timers (timer_create, struct sigevent, SIGEV_*; required, base in Issue 8) and sigtimedwait()/SIGRTMIN are absent |
| XSH.timer_create.005 | SYSTEM | timers (timer_create, struct sigevent, SIGEV_*; required, base in Issue 8) and sigtimedwait()/SIGRTMIN are absent |
| XSH.timer_create.006 | SYSTEM | timers (timer_create, struct sigevent, SIGEV_*; required, base in Issue 8) and sigtimedwait()/SIGRTMIN are absent |
| XSH.timer_create.007 | SYSTEM | timers (timer_create, struct sigevent, SIGEV_*; required, base in Issue 8) and sigtimedwait()/SIGRTMIN are absent |
| XSH.timer_create.008 | SYSTEM | timers (timer_create, struct sigevent, SIGEV_*; required, base in Issue 8) and sigtimedwait()/SIGRTMIN are absent |
| XSH.timer_create.009 | SYSTEM | timers (timer_create, struct sigevent, SIGEV_*; required, base in Issue 8) and sigtimedwait()/SIGRTMIN are absent |
| XSH.timer_create.010 | SYSTEM | timers (timer_create, struct sigevent, SIGEV_*; required, base in Issue 8) and sigtimedwait()/SIGRTMIN are absent |
| XSH.timer_create.011 | SYSTEM | timers (timer_create, struct sigevent, SIGEV_*; required, base in Issue 8) and sigtimedwait()/SIGRTMIN are absent |
| XSH.timer_delete.001 | SYSTEM | timers (timer_create, struct sigevent, SIGEV_*; required, base in Issue 8) and sigtimedwait()/SIGRTMIN are absent |
| XSH.timer_delete.002 | SYSTEM | timers (timer_create, struct sigevent, SIGEV_*; required, base in Issue 8) and sigtimedwait()/SIGRTMIN are absent |
| XSH.timer_delete.003 | SYSTEM | timers (timer_create, struct sigevent, SIGEV_*; required, base in Issue 8) and sigtimedwait()/SIGRTMIN are absent |
| XSH.timer_getoverrun.001 | SYSTEM | timers (timer_create, struct sigevent, SIGEV_*; required, base in Issue 8) and sigtimedwait()/SIGRTMIN are absent |
| XSH.timer_getoverrun.002 | SYSTEM | timers (timer_create, struct sigevent, SIGEV_*; required, base in Issue 8) and sigtimedwait()/SIGRTMIN are absent |
| XSH.timer_getoverrun.003 | SYSTEM | timers (timer_create, struct sigevent, SIGEV_*; required, base in Issue 8) and sigtimedwait()/SIGRTMIN are absent |
| XSH.timer_getoverrun.004 | SYSTEM | timers (timer_create, struct sigevent, SIGEV_*; required, base in Issue 8) and sigtimedwait()/SIGRTMIN are absent |
| XSH.timer_getoverrun.005 | SYSTEM | timers (timer_create, struct sigevent, SIGEV_*; required, base in Issue 8) and sigtimedwait()/SIGRTMIN are absent |
| XSH.timer_getoverrun.006 | SYSTEM | timers (timer_create, struct sigevent, SIGEV_*; required, base in Issue 8) and sigtimedwait()/SIGRTMIN are absent |
| XSH.timer_getoverrun.007 | SYSTEM | timers (timer_create, struct sigevent, SIGEV_*; required, base in Issue 8) and sigtimedwait()/SIGRTMIN are absent |
| XSH.timer_getoverrun.008 | SYSTEM | timers (timer_create, struct sigevent, SIGEV_*; required, base in Issue 8) and sigtimedwait()/SIGRTMIN are absent |
| XSH.timer_getoverrun.009 | SYSTEM | timers (timer_create, struct sigevent, SIGEV_*; required, base in Issue 8) and sigtimedwait()/SIGRTMIN are absent |
| XSH.timer_getoverrun.010 | SYSTEM | timers (timer_create, struct sigevent, SIGEV_*; required, base in Issue 8) and sigtimedwait()/SIGRTMIN are absent |
| XSH.timer_getoverrun.011 | SYSTEM | timers (timer_create, struct sigevent, SIGEV_*; required, base in Issue 8) and sigtimedwait()/SIGRTMIN are absent |
| XSH.timer_getoverrun.012 | SYSTEM | timers (timer_create, struct sigevent, SIGEV_*; required, base in Issue 8) and sigtimedwait()/SIGRTMIN are absent |
| XSH.timer_getoverrun.013 | SYSTEM | timers (timer_create, struct sigevent, SIGEV_*; required, base in Issue 8) and sigtimedwait()/SIGRTMIN are absent |
| XSH.uselocale.001 | TEST-BUG | lc.h included <monetary.h> for every locale test, but only strfmon() tests use it; the include is removed from this module (header absent on this system) |
| XSH.uselocale.002 | TEST-BUG | lc.h included <monetary.h> for every locale test, but only strfmon() tests use it; the include is removed from this module (header absent on this system) |
| XSH.uselocale.003 | TEST-BUG | lc.h included <monetary.h> for every locale test, but only strfmon() tests use it; the include is removed from this module (header absent on this system) |
| XSH.uselocale.004 | TEST-BUG | lc.h included <monetary.h> for every locale test, but only strfmon() tests use it; the include is removed from this module (header absent on this system) |
| XSH.uselocale.005 | TEST-BUG | lc.h included <monetary.h> for every locale test, but only strfmon() tests use it; the include is removed from this module (header absent on this system) |
| XSH.wcscpy.002 | SYSTEM | wcpcpy()/wcpncpy() (CX, required) are not declared |
| XSH.wcscpy.003 | SYSTEM | wcpcpy()/wcpncpy() (CX, required) are not declared |
| XSH.wcsncpy.004 | SYSTEM | wcpcpy()/wcpncpy() (CX, required) are not declared |
| XSH.wcsncpy.005 | SYSTEM | wcpcpy()/wcpncpy() (CX, required) are not declared |
| XSH.wordexp.001 | SYSTEM | <wordexp.h> (required, unshaded) is absent |
| XSH.wordexp.002 | SYSTEM | <wordexp.h> (required, unshaded) is absent |
| XSH.wordexp.003 | SYSTEM | <wordexp.h> (required, unshaded) is absent |
| XSH.wordexp.004 | SYSTEM | <wordexp.h> (required, unshaded) is absent |
| XSH.wordexp.005 | SYSTEM | <wordexp.h> (required, unshaded) is absent |
| XSH.wordexp.006 | SYSTEM | <wordexp.h> (required, unshaded) is absent |
| XSH.wordexp.007 | SYSTEM | <wordexp.h> (required, unshaded) is absent |
| XSH.wordexp.008 | SYSTEM | <wordexp.h> (required, unshaded) is absent |
| XSH.wordexp.009 | SYSTEM | <wordexp.h> (required, unshaded) is absent |
| XSH.wordexp.010 | SYSTEM | <wordexp.h> (required, unshaded) is absent |
| XSH.wordexp.011 | SYSTEM | <wordexp.h> (required, unshaded) is absent |
| XSH.wordexp.012 | SYSTEM | <wordexp.h> (required, unshaded) is absent |
| XSH.wordexp.013 | SYSTEM | <wordexp.h> (required, unshaded) is absent |
| XSH.wordexp.014 | SYSTEM | <wordexp.h> (required, unshaded) is absent |
| XSH.wordexp.015 | SYSTEM | <wordexp.h> (required, unshaded) is absent |
| XSH.wordexp.016 | SYSTEM | <wordexp.h> (required, unshaded) is absent |
| XSH.wordexp.017 | SYSTEM | <wordexp.h> (required, unshaded) is absent |
| XSH.wordexp.018 | SYSTEM | <wordexp.h> (required, unshaded) is absent |
| XSH.wordexp.019 | SYSTEM | <wordexp.h> (required, unshaded) is absent |
| XSH.wordexp.020 | SYSTEM | <wordexp.h> (required, unshaded) is absent |
| XSH.wordexp.021 | SYSTEM | <wordexp.h> (required, unshaded) is absent |
| XSH.wordexp.022 | SYSTEM | <wordexp.h> (required, unshaded) is absent |
| XSH.wordexp.023 | SYSTEM | <wordexp.h> (required, unshaded) is absent |
| XSH.wordexp.024 | SYSTEM | <wordexp.h> (required, unshaded) is absent |
| XSH.wordexp.025 | SYSTEM | <wordexp.h> (required, unshaded) is absent |

## What was changed

All edits are test sources under modules/xsh; no .asr or .cov changed. Lint: 6373 problems before and after, all of the allowed kind ("declares no MUTANT").

- lc.h (setlocale, duplocale, freelocale, getlocalename_l, localeconv, newlocale, nl_langinfo, strcoll, strxfrm, uselocale): removed `#include <monetary.h>`. Nothing in these modules calls strfmon(); the strfmon module keeps its own copy of lc.h with the include (its copy now differs from the others on purpose). getlocalename_l has no assertion in this list; its copy was changed only to keep the nine copies identical.
- gettext/t/gt.h: removed `#include <iconv.h>`; added it to gettext 014 and 015, the only tests that use iconv_open().
- textdomain/t/td.h: `<iconv.h>` and td_codesets() are now under `#ifdef TD_ICONV`; textdomain 008 and 009 define it.
- 34 posix_spawn_* tests (addchdir, addclose, adddup2, file_actions_init, spawnattr_getflags, getpgroup, getsigdefault, getsigmask, init): removed the "sched" mode of the child image. No test of this list ever asks for it, and it called sched_getparam() and sched_getscheduler(), interfaces of the PS option, which is not claimed here and whose declarations are absent. The two modules that use that mode (getschedparam, getschedpolicy) are untouched.
- pthread_mutex_init 001-004 and pthread_mutex_lock 001-012, 015: make_mutex() called pthread_mutexattr_setpshared() (TSH option, shaded) unguarded and had a robust path none of these tests use. The pshared call now follows the suite's convention (`#if defined(_POSIX_THREAD_PROCESS_SHARED) && ... >= 0`, else pc_unsupported); the unused robust path is dropped (lock.012, which uses it, keeps it). PTHREAD_MUTEX_ROBUST is not tested with #ifdef because a system may define it as an enumerator.
- kill.006: the upper bound of the scan for pending signals used SIGRTMAX; now `#ifdef SIGRTMAX`, else 128.
- free.002: the RLIMIT_AS cap is best effort by the test's own comment; now under `#ifdef RLIMIT_AS`.

Changed by another triage agent while this group was being worked, found in the tree and counted as TEST-BUG here: sigismember.001 (SIGRTMIN guarded) and sigpending.002 (incidental sigtimedwait() replaced). Both were re-run here.

Not changed on purpose (SYSTEM): everything that is the subject of the assertion. Possible improvement, not done because the other group (build0) classed the same symbol SYSTEM and mt.h is shared by 22 complex modules: a fallback for CMPLX/CMPLXF/CMPLXL in mt.h built through the array layout of a complex type (the header already reads results that way), which would let ctan, ctanh, csqrt and the other complex function tests run when only CMPLX is missing; XBD complex.h and the CMPLX module would still report the missing macro.

## Re-run on this machine (flock, --only ID, one results directory per ID)

127 tests re-run: PASS 119, UNINITIATED 8.

- All pass except eight that are UNINITIATED here for a reason outside this group: posix_spawn_file_actions_addchdir 002, 003, 004, 005, 006, 008, 010 and posix_spawn_file_actions_addclose.016 use posix_spawn_file_actions_addchdir(), which is not declared here (already UNINITIATED on the earlier Linux run; docs/linux-deviations.md, missing-2024). The sched_* errors that stopped them on the third system are gone. On the third system they may still end UNINITIATED for addchdir (SYSTEM).
- A first version of the make_mutex() change guarded the robust path with #ifdef PTHREAD_MUTEX_ROBUST; pthread_mutex_lock.012 then ended UNRESOLVED here (the constant is not a macro in this C library); that guard was removed and all 17 mutex tests pass.
- The rest of this list (SYSTEM) was not rebuilt here: those symbols exist on this Linux build only partly, and the earlier Linux verdicts are in column 4 of the list.
