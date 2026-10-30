# Second-system triage, group fail-xsh1

Source list: `scratchpad/triage/ob/list-fail-xsh1.tsv` (51 non-PASS lines, second BSD-derived system, run as root). Standard pages from the personal copy; only short phrases quoted.

| ID | this system | Linux | class | reason |
| --- | --- | --- | --- | --- |
| XSH.getaddrinfo.028 | FAIL | PASS | SYSTEM | AI_NUMERICSERV with a non-numeric service shall give EAI_NONAME (getaddrinfo DESCRIPTION); the system returns the service-error code. |
| XSH.getcwd.008 | INSPECT | PASS | STD-AMBIG | Already disputed (Q-66): only the RATIONALE says ENAMETOOLONG is no longer allowed for a long name; no change. |
| XSH.getdelim.008 | UNRESOLVED | PASS | SYSTEM | RLIMIT_AS is a required <sys/resource.h> constant (XBD sys/resource.h) and is missing, so the test cannot run and UNRESOLVED is right. |
| XSH.gethostname.002 | FAIL | PASS | TEST-BUG | A truncated name may be null-terminated within namelen (DESCRIPTION: "unspecified whether ... null-terminated"), so the last byte may be a null byte; the test demanded all namelen bytes be name bytes. |
| XSH.getnameinfo.006 | FAIL | PASS | SYSTEM | nodelen zero means the node name "shall not be returned" and the call succeeds; the system gives EAI_OVERFLOW (getnameinfo DESCRIPTION). |
| XSH.getnameinfo.009 | FAIL | PASS | SYSTEM | An unrecognized family shall fail with EAI_FAMILY; the system returns EAI_SYSTEM (getnameinfo ERRORS). |
| XSH.getnameinfo.010 | FAIL | PASS | SYSTEM | An address length invalid for the family shall fail with EAI_FAMILY; the system accepts a length holding only the family (getnameinfo ERRORS). |
| XSH.getsockopt.017 | FAIL | NOTINUSE | SYSTEM | IPV6_V6ONLY default is 0 (XSH 2.10.20); the system defaults to 1. |
| XSH.glob.029 | FAIL | PASS | SYSTEM | GLOB_ERR shall make glob() return GLOB_ABORTED on a failed directory open; the errfunc cases (.027, .028) pass, only the GLOB_ERR path is ignored (glob DESCRIPTION/ERRORS). |
| XSH.gmtime.005 | FAIL | PASS | SYSTEM | gmtime() shall fail with EOVERFLOW when the result cannot be represented; the system returns null but leaves errno 0 (gmtime ERRORS). |
| XSH.gmtime.006 | FAIL | PASS | SYSTEM | Same as gmtime.005 for gmtime_r(). |
| XSH.ldexp.002 | FAIL | PASS | SYSTEM | math_errhandling includes MATH_ERREXCEPT yet the ldexpl overflow raises no overflow exception (ldexp ERRORS, XBD 4.23); double and float pass. |
| XSH.lgamma.005 | FAIL | PASS | SYSTEM | Pole error at 0: divide-by-zero exception required where MATH_ERREXCEPT is claimed; not raised (lgamma ERRORS). |
| XSH.link.021 | UNRESOLVED | UNRESOLVED | SYSTEM | O_SEARCH is required in <fcntl.h> (XBD fcntl.h) and is missing; the test cannot run, UNRESOLVED is right. |
| XSH.localtime.010 | FAIL | PASS | SYSTEM | localtime() shall fail with EOVERFLOW when the result cannot be represented; errno left 0 (localtime ERRORS). |
| XSH.localtime.011 | FAIL | PASS | SYSTEM | Same as localtime.010 for localtime_r(). |
| XSH.log.005 | FAIL | PASS | SYSTEM | Pole error: divide-by-zero exception required under MATH_ERREXCEPT; logf(0) raises none (log ERRORS). |
| XSH.log.006 | FAIL | PASS | SYSTEM | Domain error: invalid exception required under MATH_ERREXCEPT; logl(-1) raises none (log ERRORS). |
| XSH.log10.005 | FAIL | PASS | SYSTEM | Same as log.005 for log10f(0). |
| XSH.log1p.004 | FAIL | PASS | SYSTEM | Pole error at -1: divide-by-zero exception required; log1pl(-1) raises none (log1p ERRORS). |
| XSH.log1p.005 | FAIL | PASS | SYSTEM | Domain error: invalid exception required; log1pl(-2) raises none (log1p ERRORS). |
| XSH.log2.005 | FAIL | PASS | SYSTEM | Same as log.005 for log2(0). |
| XSH.log2.006 | FAIL | PASS | SYSTEM | Same as log.006 for log2l(-1). |
| XSH.mblen.004 | FAIL | PASS | SYSTEM | mblen() is equivalent to a state reset followed by mbtowc(0,s,n) (DESCRIPTION), so an earlier incomplete character must not affect the next call; the system keeps the pending bytes and the later complete character gives -1. |
| XSH.mkdir.018 | UNRESOLVED | UNRESOLVED | SYSTEM | Same as link.021 (O_SEARCH missing). |
| XSH.mkfifo.016 | UNRESOLVED | UNRESOLVED | SYSTEM | Same as link.021 (O_SEARCH missing). |
| XSH.mktime.009 | FAIL | PASS | SYSTEM | mktime() shall fail with EOVERFLOW when the result cannot be represented; errno left 0 (mktime ERRORS). |
| XSH.mmap.014 | FAIL | PASS | TEST-BUG | The test read the file with read() after munmap() without msync(); the text promises no read()/mapping consistency without msync() (msync RATIONALE: systems "that do not support read/write consistency"). |
| XSH.munmap.003 | FAIL | PASS | SYSTEM | munmap() of a range with no mappings "has no effect" and EINVAL is only for addresses outside the address space or len 0; the system returns EINVAL for a hole between mappings (munmap DESCRIPTION/ERRORS). |
| XSH.nextafter.008 | FAIL | PASS | SYSTEM | nexttoward(1, 1+LDBL_EPSILON) must be the next double above 1 (toward y); the system returns the neighbour below (nextafter DESCRIPTION). |
| XSH.opendir.012 | UNRESOLVED | UNRESOLVED | SYSTEM | Same as link.021 (O_SEARCH missing). |
| XSH.perror.007 | FAIL | PASS | SYSTEM | On error perror() shall set the error indicator of stderr and errno; the indicator stays clear after a failed write (perror DESCRIPTION). |
| XSH.popen.015 | FAIL | PASS | SYSTEM | popen() shall run sh -c -- command (Issue 8, defect 1440); a command starting with - was parsed as shell options. |
| XSH.posix_spawn.014 | FAIL | PASS | SYSTEM | posix_spawnp() shall fail rather than fall back to running a non-executable-format file with sh; the commands ran (posix_spawn DESCRIPTION). |
| XSH.posix_spawn.045 | UNRESOLVED | PASS | TEST-BUG | The test copies its own executable and searches it for a tag array, but nothing referenced the array (the function that used it is unused), so the compiler dropped it; copy_self_tagged() now takes the pattern from the array itself, which keeps it. |
| XSH.posix_spawn.046 | UNRESOLVED | PASS | TEST-BUG | The test copies its own executable and searches it for a tag array, but nothing referenced the array (the function that used it is unused), so the compiler dropped it; copy_self_tagged() now takes the pattern from the array itself, which keeps it. |
| XSH.posix_spawn.047 | UNRESOLVED | PASS | TEST-BUG | The test copies its own executable and searches it for a tag array, but nothing referenced the array (the function that used it is unused), so the compiler dropped it; copy_self_tagged() now takes the pattern from the array itself, which keeps it. |
| XSH.posix_spawn.048 | UNRESOLVED | PASS | TEST-BUG | The test copies its own executable and searches it for a tag array, but nothing referenced the array (the function that used it is unused), so the compiler dropped it; copy_self_tagged() now takes the pattern from the array itself, which keeps it. |
| XSH.posix_spawn.049 | UNRESOLVED | PASS | TEST-BUG | The test copies its own executable and searches it for a tag array, but nothing referenced the array (the function that used it is unused), so the compiler dropped it; copy_self_tagged() now takes the pattern from the array itself, which keeps it. |
| XSH.posix_spawn.050 | UNRESOLVED | PASS | TEST-BUG | The test copies its own executable and searches it for a tag array, but nothing referenced the array (the function that used it is unused), so the compiler dropped it; copy_self_tagged() now takes the pattern from the array itself, which keeps it. |
| XSH.posix_spawn.060 | UNRESOLVED | PASS | TEST-BUG | The test copies its own executable and searches it for a tag array, but nothing referenced the array (the function that used it is unused), so the compiler dropped it; copy_self_tagged() now takes the pattern from the array itself, which keeps it. |
| XSH.posix_spawn.061 | UNRESOLVED | PASS | TEST-BUG | The test copies its own executable and searches it for a tag array, but nothing referenced the array (the function that used it is unused), so the compiler dropped it; copy_self_tagged() now takes the pattern from the array itself, which keeps it. |
| XSH.psiginfo.005 | FAIL | PASS | SYSTEM | psignal() on a write error shall set the error indicator of stderr and errno; the indicator is not set (psiginfo DESCRIPTION). |
| XSH.pthread_cancel.005 | FAIL | FAIL | SYSTEM | The cancelability state shall be PTHREAD_CANCEL_DISABLE when cleanup handlers run (XSH 2.9.5.3); the system shows ENABLE (same on Linux, already in known/). |
| XSH.pthread_join.006 | UNRESOLVED | PASS | TEST-BUG | Driver timeout after 60 s: no bounded wait, so an uncancelled thread (or any stall) gave no verdict; system behaviour not seen. Added a 20 s guard: FAIL if the thread was not canceled after the request, UNRESOLVED naming the step otherwise. |
| XSH.pthread_once.003 | UNRESOLVED | PASS | TEST-BUG | Same as pthread_join.006 (unbounded wait after pthread_cancel of a thread blocked in read() inside the init_routine); same guard. |
| XSH.pthread_spin_init.001 | FAIL | PASS | SYSTEM | pthread_spin_trylock() shall fail with EBUSY when a thread holds the lock; the system gives EDEADLK for the holder (the "undefined if the caller holds it" sentence belongs to pthread_spin_lock, and EDEADLK is listed only for it; pthread_spin_lock ERRORS). |
| XSH.pthread_spin_lock.002 | FAIL | PASS | SYSTEM | Same as pthread_spin_init.001. |
| XSH.rand.002 | FAIL | PASS | SYSTEM | After srand() with the same seed the sequence "shall be repeated"; the system gives a different sequence (rand DESCRIPTION). |
| XSH.rand.003 | FAIL | PASS | SYSTEM | Without srand() the sequence shall be that of srand(1); the system differs (rand DESCRIPTION). |
| XSH.readlink.018 | UNRESOLVED | UNRESOLVED | SYSTEM | Same as link.021 (O_SEARCH missing). |

## TEST-BUG changes and re-runs (this Linux machine)

- XSH.gethostname.002: `t/002-truncation.c` and the `says` in `gethostname.asr`: first namelen-1 bytes must be the name, the last byte the next character or a null byte. Re-run: PASS.
- XSH.mmap.014: `t/014-required-prot.c` calls msync(MS_SYNC) on the PROT_WRITE mapping before munmap() and the read(); note added in `mmap.asr`. Re-run: PASS; ci/mutate.sh: 2 mutants caught, variant tolerated.
- XSH.posix_spawn.045-050, 060, 061: in each `t/NNN-*.c`, `copy_self_tagged()` builds its search pattern from the `tag` array (loop copy) instead of two string literals. Re-run of all eight as root: PASS. The other posix_spawn tests never call copy_self() and are unaffected.
- XSH.pthread_join.006, XSH.pthread_once.003: SIGALRM guard (arm()/on_stall(), 20 s per step). Re-run: PASS each; ci/mutate.sh: both mutants caught, both variants tolerated. The next run on the second system will show whether the stall is the cancellation itself (FAIL, then SYSTEM) or an earlier step.

## Counts

- STD-AMBIG: 1
- SYSTEM: 38
- TEST-BUG: 12

lint: 6373 problems before and after, all "declares no MUTANT".

## Recurring patterns

- Required error reporting missing: EOVERFLOW errno from the time functions (5 lines), floating-point exceptions under MATH_ERREXCEPT for float and long double forms (9 lines), EAI_* codes from getnameinfo/getaddrinfo (4 lines).
- Required constants absent (O_SEARCH: 5 lines; RLIMIT_AS: 1 line); the tests correctly end UNRESOLVED.
- Non-conforming behaviour: rand()/srand() not repeatable; mblen() keeps state across calls; perror()/psignal() bypass the stdio error indicator; popen() omits "--".
- Test bugs: code that works only if the compiler keeps an unreferenced array, an expectation on read()/mapping coherence without msync(), an expectation that a truncated host name has no terminating null, and unbounded waits after pthread_cancel.
