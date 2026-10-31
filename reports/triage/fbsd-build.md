# FreeBSD triage, group build

851 assertions ended UNINITIATED in the root run. Counts: TEST-BUG 231, SYSTEM 618, STD-AMBIG 0, ENV 0, UNKNOWN 2.

Each name was checked on the page that defines it for option shading; CX is base. Grouped by the first compiler or linker error.

## Error groups

| count | first error | class | reason |
|---|---|---|---|
| 213 | use of undeclared identifier 'INADDR_LOOPBACK' | TEST-BUG | INADDR_LOOPBACK is not defined by POSIX (netinet/in.h names it only as an extension); tests now use getaddrinfo() for the loopback address |
| 181 | no member named 'sa_sigaction' in 'struct sigaction' | SYSTEM | struct sigaction has no sa_sigaction member in a POSIX.1-2024 build (signal.h requires it) |
| 55 | call to undeclared function 'sem_clockwait' | SYSTEM | sem_clockwait is required (unshaded or CX) by the page that defines it and is not provided in a POSIX.1-2024 build |
| 42 | use of undeclared identifier 'MAP_ANONYMOUS' | SYSTEM | MAP_ANONYMOUS is required (unshaded or CX) by the page that defines it and is not provided in a POSIX.1-2024 build |
| 24 | 'libintl.h' file not found | SYSTEM | libintl.h is required (unshaded or CX) by the page that defines it and is not provided in a POSIX.1-2024 build |
| 19 | undefined symbol: thrd_create | SYSTEM | threads.h functions not found when linking with -l pthread, the library c17 names for them |
| 16 | call to undeclared library function 'strlcpy' | SYSTEM | strlcpy is required (unshaded or CX) by the page that defines it and is not provided in a POSIX.1-2024 build |
| 16 | undefined symbol: tss_create | SYSTEM | threads.h functions not found when linking with -l pthread, the library c17 names for them |
| 15 | undefined symbol: cnd_init | SYSTEM | threads.h functions not found when linking with -l pthread, the library c17 names for them |
| 13 | call to undeclared library function 'ccosl' | SYSTEM | ccosl is required (unshaded or CX) by the page that defines it and is not provided in a POSIX.1-2024 build |
| 13 | call to undeclared library function 'ccoshl' | SYSTEM | ccoshl is required (unshaded or CX) by the page that defines it and is not provided in a POSIX.1-2024 build |
| 13 | call to undeclared library function 'csinl' | SYSTEM | csinl is required (unshaded or CX) by the page that defines it and is not provided in a POSIX.1-2024 build |
| 13 | call to undeclared library function 'csinhl' | SYSTEM | csinhl is required (unshaded or CX) by the page that defines it and is not provided in a POSIX.1-2024 build |
| 12 | call to undeclared library function 'ctanl' | SYSTEM | ctanl is required (unshaded or CX) by the page that defines it and is not provided in a POSIX.1-2024 build |
| 12 | call to undeclared library function 'ctanhl' | SYSTEM | ctanhl is required (unshaded or CX) by the page that defines it and is not provided in a POSIX.1-2024 build |
| 10 | use of undeclared identifier '_SC_XOPEN_UNIX' | SYSTEM | _SC_XOPEN_UNIX is required (unshaded or CX) by the page that defines it and is not provided in a POSIX.1-2024 build |
| 10 | variable has incomplete type 'struct winsize' | SYSTEM | struct winsize is required (unshaded or CX) by the page that defines it and is not provided in a POSIX.1-2024 build |
| 8 | use of undeclared identifier 'SIGXFSZ' | TEST-BUG | SIGXFSZ is XSI-shaded in signal.h; its uses are now guarded, and where it is not named the child blocks every signal |
| 7 | call to undeclared function 'pthread_mutex_clocklock' | SYSTEM | pthread_mutex_clocklock is required (unshaded or CX) by the page that defines it and is not provided in a POSIX.1-2024 build |
| 7 | unknown type name 'Dl_info_t'; did you mean '__ino_t'? | SYSTEM | Dl_info_t is required (unshaded or CX) by the page that defines it and is not provided in a POSIX.1-2024 build |
| 7 | use of undeclared identifier 'memmem'; did you mean 'memset'? | SYSTEM | memmem is required (unshaded or CX) by the page that defines it and is not provided in a POSIX.1-2024 build |
| 7 | undefined symbol: mtx_lock | SYSTEM | threads.h functions not found when linking with -l pthread, the library c17 names for them |
| 7 | offsetof of incomplete type 'struct posix_dent' | SYSTEM | struct posix_dent is required (unshaded or CX) by the page that defines it and is not provided in a POSIX.1-2024 build |
| 6 | call to undeclared function 'accept4' | SYSTEM | accept4 is required (unshaded or CX) by the page that defines it and is not provided in a POSIX.1-2024 build |
| 5 | undefined symbol: thrd_sleep | SYSTEM | threads.h functions not found when linking with -l pthread, the library c17 names for them |
| 5 | call to undeclared function 'pthread_cond_clockwait' | SYSTEM | pthread_cond_clockwait is required (unshaded or CX) by the page that defines it and is not provided in a POSIX.1-2024 build |
| 5 | use of undeclared identifier 'EAI_OVERFLOW' | SYSTEM | EAI_OVERFLOW is required (unshaded or CX) by the page that defines it and is not provided in a POSIX.1-2024 build |
| 5 | call to undeclared function 'getlocalename_l' | SYSTEM | getlocalename_l is required (unshaded or CX) by the page that defines it and is not provided in a POSIX.1-2024 build |
| 5 | call to undeclared function 'pthread_rwlock_clockrdlock' | SYSTEM | pthread_rwlock_clockrdlock is required (unshaded or CX) by the page that defines it and is not provided in a POSIX.1-2024 build |
| 5 | call to undeclared function 'wcslcpy' | SYSTEM | wcslcpy is required (unshaded or CX) by the page that defines it and is not provided in a POSIX.1-2024 build |
| 4 | undefined symbol: mtx_init | SYSTEM | threads.h functions not found when linking with -l pthread, the library c17 names for them |
| 4 | call to undeclared function 'dup3' | SYSTEM | dup3 is required (unshaded or CX) by the page that defines it and is not provided in a POSIX.1-2024 build |
| 4 | call to undeclared function 'reallocarray' | SYSTEM | reallocarray is required (unshaded or CX) by the page that defines it and is not provided in a POSIX.1-2024 build |
| 4 | use of undeclared identifier 'SOCK_CLOEXEC' | SYSTEM | SOCK_CLOEXEC is required (unshaded or CX) by the page that defines it and is not provided in a POSIX.1-2024 build |
| 4 | call to undeclared function 'mkostemp' | SYSTEM | mkostemp is required (unshaded or CX) by the page that defines it and is not provided in a POSIX.1-2024 build |
| 4 | undefined symbol: mtx_destroy | SYSTEM | threads.h functions not found when linking with -l pthread, the library c17 names for them |
| 4 | call to undeclared function 'pipe2' | SYSTEM | pipe2 is required (unshaded or CX) by the page that defines it and is not provided in a POSIX.1-2024 build |
| 4 | call to undeclared function 'pthread_rwlock_clockwrlock' | SYSTEM | pthread_rwlock_clockwrlock is required (unshaded or CX) by the page that defines it and is not provided in a POSIX.1-2024 build |
| 4 | undefined symbol: thrd_current | SYSTEM | threads.h functions not found when linking with -l pthread, the library c17 names for them |
| 3 | use of undeclared identifier 'OFILL' | SYSTEM | OFILL is required (unshaded or CX) by the page that defines it and is not provided in a POSIX.1-2024 build |
| 3 | undefined symbol: __signbit | TEST-BUG | signbit() is a math.h interface and the module did not link with -l m (c17 libraries) |
| 3 | use of undeclared identifier 'ABALTMON_1' | SYSTEM | ABALTMON_1 is required (unshaded or CX) by the page that defines it and is not provided in a POSIX.1-2024 build |
| 3 | (none) | UNKNOWN | the test ended without writing a result and left no output; needs a rerun with tracing |
| 3 | undefined symbol: call_once | SYSTEM | threads.h functions not found when linking with -l pthread, the library c17 names for them |
| 3 | call to undeclared function 'getentropy' | SYSTEM | getentropy is required (unshaded or CX) by the page that defines it and is not provided in a POSIX.1-2024 build |
| 3 | undefined symbol: mtx_timedlock | SYSTEM | threads.h functions not found when linking with -l pthread, the library c17 names for them |
| 3 | call to undeclared function 'pthread_setschedprio' | SYSTEM | the system claims TPS but does not declare pthread_setschedprio() |
| 3 | incomplete definition of type 'const struct winsize' | SYSTEM | const struct winsize is required (unshaded or CX) by the page that defines it and is not provided in a POSIX.1-2024 build |
| 3 | call to undeclared function 'wcslcat' | SYSTEM | wcslcat is required (unshaded or CX) by the page that defines it and is not provided in a POSIX.1-2024 build |
| 2 | undefined symbol: __signbitf | TEST-BUG | signbit() is a math.h interface and the module did not link with -l m (c17 libraries) |
| 2 | (no build log: exited 2 without reporting a result) | UNKNOWN | the test ended without writing a result and left no output; needs a rerun with tracing |
| 2 | call to undeclared function 'asprintf' | SYSTEM | asprintf is required (unshaded or CX) by the page that defines it and is not provided in a POSIX.1-2024 build |
| 2 | call to undeclared function 'secure_getenv' | SYSTEM | secure_getenv is required (unshaded or CX) by the page that defines it and is not provided in a POSIX.1-2024 build |
| 2 | call to undeclared function 'qsort_r' | SYSTEM | qsort_r is required (unshaded or CX) by the page that defines it and is not provided in a POSIX.1-2024 build |
| 2 | use of undeclared identifier 'SO_PROTOCOL' | SYSTEM | SO_PROTOCOL is required (unshaded or CX) by the page that defines it and is not provided in a POSIX.1-2024 build |
| 2 | use of undeclared identifier 'SOCK_NONBLOCK' | SYSTEM | SOCK_NONBLOCK is required (unshaded or CX) by the page that defines it and is not provided in a POSIX.1-2024 build |
| 2 | use of undeclared identifier 'SOCK_CLOFORK' | SYSTEM | SOCK_CLOFORK is required (unshaded or CX) by the page that defines it and is not provided in a POSIX.1-2024 build |
| 1 | call to undeclared function 'posix_close' | SYSTEM | posix_close is required (unshaded or CX) by the page that defines it and is not provided in a POSIX.1-2024 build |
| 1 | use of undeclared identifier 'S_IFDIR' | TEST-BUG | the S_IF* type constants are XSI-shaded in sys/stat.h; the test now takes the type bits from a directory's st_mode |
| 1 | use of undeclared identifier 'FNM_CASEFOLD' | SYSTEM | FNM_CASEFOLD is required (unshaded or CX) by the page that defines it and is not provided in a POSIX.1-2024 build |
| 1 | use of undeclared identifier 'FNM_IGNORECASE' | SYSTEM | FNM_IGNORECASE is required (unshaded or CX) by the page that defines it and is not provided in a POSIX.1-2024 build |
| 1 | call to undeclared function 'htons' | TEST-BUG | htons() used without including arpa/inet.h |
| 1 | undefined symbol: mtx_trylock | SYSTEM | threads.h functions not found when linking with -l pthread, the library c17 names for them |
| 1 | use of undeclared identifier 'PTHREAD_NULL' | SYSTEM | PTHREAD_NULL is required (unshaded or CX) by the page that defines it and is not provided in a POSIX.1-2024 build |
| 1 | no member named 'd_ino' in 'struct dirent' | SYSTEM | d_ino is required (unshaded or CX) by the page that defines it and is not provided in a POSIX.1-2024 build |
| 1 | use of undeclared identifier 'REG_MINIMAL' | SYSTEM | REG_MINIMAL is required (unshaded or CX) by the page that defines it and is not provided in a POSIX.1-2024 build |
| 1 | use of undeclared identifier 'SO_DOMAIN' | SYSTEM | SO_DOMAIN is required (unshaded or CX) by the page that defines it and is not provided in a POSIX.1-2024 build |
| 1 | undefined symbol: thrd_exit | SYSTEM | threads.h functions not found when linking with -l pthread, the library c17 names for them |
| 1 | call to undeclared function 'vasprintf' | SYSTEM | vasprintf is required (unshaded or CX) by the page that defines it and is not provided in a POSIX.1-2024 build |
| 1 | undefined symbol: wcscasecmp_l | SYSTEM | wcscasecmp_l() (CX, base) is declared but not found when linking |

## Per assertion

| ID | class | reason |
|---|---|---|
| XBD.11.2-terminal-parameters.010 | SYSTEM | OFILL is required (unshaded or CX) by the page that defines it and is not provided in a POSIX.1-2024 build |
| XBD.11.2-terminal-parameters.015 | SYSTEM | OFILL is required (unshaded or CX) by the page that defines it and is not provided in a POSIX.1-2024 build |
| XBD.11.2-terminal-parameters.016 | SYSTEM | OFILL is required (unshaded or CX) by the page that defines it and is not provided in a POSIX.1-2024 build |
| XBD.4.13-host-and-network-byte-orders.001 | TEST-BUG | INADDR_LOOPBACK is not defined by POSIX (netinet/in.h names it only as an extension); tests now use getaddrinfo() for the loopback address |
| XBD.4.3-default-initialization.001 | TEST-BUG | signbit() is a math.h interface and the module did not link with -l m (c17 libraries) |
| XBD.4.3-default-initialization.002 | TEST-BUG | signbit() is a math.h interface and the module did not link with -l m (c17 libraries) |
| XBD.4.3-default-initialization.004 | TEST-BUG | signbit() is a math.h interface and the module did not link with -l m (c17 libraries) |
| XBD.4.3-default-initialization.005 | TEST-BUG | signbit() is a math.h interface and the module did not link with -l m (c17 libraries) |
| XBD.4.3-default-initialization.006 | TEST-BUG | signbit() is a math.h interface and the module did not link with -l m (c17 libraries) |
| XBD.7.3.5-lc-time.007 | SYSTEM | ABALTMON_1 is required (unshaded or CX) by the page that defines it and is not provided in a POSIX.1-2024 build |
| XCU.sh-environment.005 | UNKNOWN | the test ended without writing a result and left no output; needs a rerun with tracing |
| XCU.sh-simple-commands.009 | UNKNOWN | the test ended without writing a result and left no output; needs a rerun with tracing |
| XSH.2.10-sockets.001 | TEST-BUG | INADDR_LOOPBACK is not defined by POSIX (netinet/in.h names it only as an extension); tests now use getaddrinfo() for the loopback address |
| XSH.2.10-sockets.002 | TEST-BUG | INADDR_LOOPBACK is not defined by POSIX (netinet/in.h names it only as an extension); tests now use getaddrinfo() for the loopback address |
| XSH.2.10-sockets.003 | TEST-BUG | INADDR_LOOPBACK is not defined by POSIX (netinet/in.h names it only as an extension); tests now use getaddrinfo() for the loopback address |
| XSH.2.10-sockets.004 | TEST-BUG | INADDR_LOOPBACK is not defined by POSIX (netinet/in.h names it only as an extension); tests now use getaddrinfo() for the loopback address |
| XSH.2.10-sockets.005 | TEST-BUG | INADDR_LOOPBACK is not defined by POSIX (netinet/in.h names it only as an extension); tests now use getaddrinfo() for the loopback address |
| XSH.2.10-sockets.006 | TEST-BUG | INADDR_LOOPBACK is not defined by POSIX (netinet/in.h names it only as an extension); tests now use getaddrinfo() for the loopback address |
| XSH.2.10-sockets.007 | TEST-BUG | INADDR_LOOPBACK is not defined by POSIX (netinet/in.h names it only as an extension); tests now use getaddrinfo() for the loopback address |
| XSH.2.10-sockets.008 | TEST-BUG | INADDR_LOOPBACK is not defined by POSIX (netinet/in.h names it only as an extension); tests now use getaddrinfo() for the loopback address |
| XSH.2.10-sockets.009 | TEST-BUG | INADDR_LOOPBACK is not defined by POSIX (netinet/in.h names it only as an extension); tests now use getaddrinfo() for the loopback address |
| XSH.2.10-sockets.010 | TEST-BUG | INADDR_LOOPBACK is not defined by POSIX (netinet/in.h names it only as an extension); tests now use getaddrinfo() for the loopback address |
| XSH.2.10-sockets.011 | TEST-BUG | INADDR_LOOPBACK is not defined by POSIX (netinet/in.h names it only as an extension); tests now use getaddrinfo() for the loopback address |
| XSH.2.10-sockets.012 | TEST-BUG | INADDR_LOOPBACK is not defined by POSIX (netinet/in.h names it only as an extension); tests now use getaddrinfo() for the loopback address |
| XSH.2.10-sockets.013 | TEST-BUG | INADDR_LOOPBACK is not defined by POSIX (netinet/in.h names it only as an extension); tests now use getaddrinfo() for the loopback address |
| XSH.2.10-sockets.014 | TEST-BUG | INADDR_LOOPBACK is not defined by POSIX (netinet/in.h names it only as an extension); tests now use getaddrinfo() for the loopback address |
| XSH.2.10-sockets.015 | TEST-BUG | INADDR_LOOPBACK is not defined by POSIX (netinet/in.h names it only as an extension); tests now use getaddrinfo() for the loopback address |
| XSH.2.10-sockets.016 | TEST-BUG | INADDR_LOOPBACK is not defined by POSIX (netinet/in.h names it only as an extension); tests now use getaddrinfo() for the loopback address |
| XSH.2.10-sockets.017 | TEST-BUG | INADDR_LOOPBACK is not defined by POSIX (netinet/in.h names it only as an extension); tests now use getaddrinfo() for the loopback address |
| XSH.2.10-sockets.018 | TEST-BUG | INADDR_LOOPBACK is not defined by POSIX (netinet/in.h names it only as an extension); tests now use getaddrinfo() for the loopback address |
| XSH.2.10-sockets.019 | TEST-BUG | INADDR_LOOPBACK is not defined by POSIX (netinet/in.h names it only as an extension); tests now use getaddrinfo() for the loopback address |
| XSH.2.10-sockets.020 | TEST-BUG | INADDR_LOOPBACK is not defined by POSIX (netinet/in.h names it only as an extension); tests now use getaddrinfo() for the loopback address |
| XSH.2.10-sockets.021 | TEST-BUG | INADDR_LOOPBACK is not defined by POSIX (netinet/in.h names it only as an extension); tests now use getaddrinfo() for the loopback address |
| XSH.2.10-sockets.022 | TEST-BUG | INADDR_LOOPBACK is not defined by POSIX (netinet/in.h names it only as an extension); tests now use getaddrinfo() for the loopback address |
| XSH.2.4.1-signal-generation.001 | SYSTEM | struct sigaction has no sa_sigaction member in a POSIX.1-2024 build (signal.h requires it) |
| XSH.2.4.1-signal-generation.002 | SYSTEM | struct sigaction has no sa_sigaction member in a POSIX.1-2024 build (signal.h requires it) |
| XSH.2.4.1-signal-generation.003 | SYSTEM | struct sigaction has no sa_sigaction member in a POSIX.1-2024 build (signal.h requires it) |
| XSH.2.4.1-signal-generation.004 | SYSTEM | struct sigaction has no sa_sigaction member in a POSIX.1-2024 build (signal.h requires it) |
| XSH.2.4.1-signal-generation.005 | SYSTEM | struct sigaction has no sa_sigaction member in a POSIX.1-2024 build (signal.h requires it) |
| XSH.2.4.1-signal-generation.006 | SYSTEM | struct sigaction has no sa_sigaction member in a POSIX.1-2024 build (signal.h requires it) |
| XSH.2.4.1-signal-generation.007 | SYSTEM | struct sigaction has no sa_sigaction member in a POSIX.1-2024 build (signal.h requires it) |
| XSH.2.4.1-signal-generation.008 | SYSTEM | struct sigaction has no sa_sigaction member in a POSIX.1-2024 build (signal.h requires it) |
| XSH.2.4.1-signal-generation.009 | SYSTEM | struct sigaction has no sa_sigaction member in a POSIX.1-2024 build (signal.h requires it) |
| XSH.2.4.1-signal-generation.010 | SYSTEM | struct sigaction has no sa_sigaction member in a POSIX.1-2024 build (signal.h requires it) |
| XSH.sigqueue.001 | SYSTEM | struct sigaction has no sa_sigaction member in a POSIX.1-2024 build (signal.h requires it) |
| XSH.sigqueue.002 | SYSTEM | struct sigaction has no sa_sigaction member in a POSIX.1-2024 build (signal.h requires it) |
| XSH.sigqueue.003 | SYSTEM | struct sigaction has no sa_sigaction member in a POSIX.1-2024 build (signal.h requires it) |
| XSH.sigqueue.004 | SYSTEM | struct sigaction has no sa_sigaction member in a POSIX.1-2024 build (signal.h requires it) |
| XSH.sigqueue.005 | SYSTEM | struct sigaction has no sa_sigaction member in a POSIX.1-2024 build (signal.h requires it) |
| XSH.sigqueue.006 | TEST-BUG | sysconf(_SC_NSIG) gave a number that is a supported signal here, so the signal was really sent and ended the test; setup now checks the number is not a signal (UNRESOLVED if it is; the fault is XSH.sysconf.015) |
| XSH.sigqueue.007 | SYSTEM | struct sigaction has no sa_sigaction member in a POSIX.1-2024 build (signal.h requires it) |
| XSH.sigtimedwait.004 | SYSTEM | struct sigaction has no sa_sigaction member in a POSIX.1-2024 build (signal.h requires it) |
| XSH.sigtimedwait.008 | SYSTEM | struct sigaction has no sa_sigaction member in a POSIX.1-2024 build (signal.h requires it) |
| XSH.2.4.2-realtime-signals.001 | SYSTEM | struct sigaction has no sa_sigaction member in a POSIX.1-2024 build (signal.h requires it) |
| XSH.2.4.2-realtime-signals.002 | SYSTEM | struct sigaction has no sa_sigaction member in a POSIX.1-2024 build (signal.h requires it) |
| XSH.2.4.2-realtime-signals.003 | SYSTEM | struct sigaction has no sa_sigaction member in a POSIX.1-2024 build (signal.h requires it) |
| XSH.2.4.2-realtime-signals.006 | SYSTEM | struct sigaction has no sa_sigaction member in a POSIX.1-2024 build (signal.h requires it) |
| XSH.2.4.3-signal-actions.001 | SYSTEM | struct sigaction has no sa_sigaction member in a POSIX.1-2024 build (signal.h requires it) |
| XSH.2.4.3-signal-actions.003 | SYSTEM | struct sigaction has no sa_sigaction member in a POSIX.1-2024 build (signal.h requires it) |
| XSH.2.4.3-signal-actions.004 | SYSTEM | struct sigaction has no sa_sigaction member in a POSIX.1-2024 build (signal.h requires it) |
| XSH.2.4.3-signal-actions.005 | SYSTEM | struct sigaction has no sa_sigaction member in a POSIX.1-2024 build (signal.h requires it) |
| XSH.2.4.3-signal-actions.006 | SYSTEM | struct sigaction has no sa_sigaction member in a POSIX.1-2024 build (signal.h requires it) |
| XSH.2.4.3-signal-actions.007 | SYSTEM | struct sigaction has no sa_sigaction member in a POSIX.1-2024 build (signal.h requires it) |
| XSH.2.4.3-signal-actions.008 | SYSTEM | struct sigaction has no sa_sigaction member in a POSIX.1-2024 build (signal.h requires it) |
| XSH.2.4.3-signal-actions.009 | SYSTEM | struct sigaction has no sa_sigaction member in a POSIX.1-2024 build (signal.h requires it) |
| XSH.2.4.3-signal-actions.011 | SYSTEM | struct sigaction has no sa_sigaction member in a POSIX.1-2024 build (signal.h requires it) |
| XSH.2.4.3-signal-actions.012 | SYSTEM | struct sigaction has no sa_sigaction member in a POSIX.1-2024 build (signal.h requires it) |
| XSH.2.4.3-signal-actions.013 | SYSTEM | struct sigaction has no sa_sigaction member in a POSIX.1-2024 build (signal.h requires it) |
| XSH.2.4.3-signal-actions.014 | SYSTEM | struct sigaction has no sa_sigaction member in a POSIX.1-2024 build (signal.h requires it) |
| XSH.2.4.3-signal-actions.015 | SYSTEM | struct sigaction has no sa_sigaction member in a POSIX.1-2024 build (signal.h requires it) |
| XSH.2.4.3-signal-actions.016 | SYSTEM | struct sigaction has no sa_sigaction member in a POSIX.1-2024 build (signal.h requires it) |
| XSH.2.4.3-signal-actions.017 | SYSTEM | struct sigaction has no sa_sigaction member in a POSIX.1-2024 build (signal.h requires it) |
| XSH.2.4.3-signal-actions.018 | SYSTEM | struct sigaction has no sa_sigaction member in a POSIX.1-2024 build (signal.h requires it) |
| XSH.2.4.3-signal-actions.019 | SYSTEM | struct sigaction has no sa_sigaction member in a POSIX.1-2024 build (signal.h requires it) |
| XSH.2.4.3-signal-actions.020 | SYSTEM | struct sigaction has no sa_sigaction member in a POSIX.1-2024 build (signal.h requires it) |
| XSH.2.4.3-signal-actions.021 | SYSTEM | struct sigaction has no sa_sigaction member in a POSIX.1-2024 build (signal.h requires it) |
| XSH.2.4.4-signal-effects.001 | SYSTEM | struct sigaction has no sa_sigaction member in a POSIX.1-2024 build (signal.h requires it) |
| XSH.2.4.4-signal-effects.002 | SYSTEM | struct sigaction has no sa_sigaction member in a POSIX.1-2024 build (signal.h requires it) |
| XSH.2.4.4-signal-effects.003 | SYSTEM | struct sigaction has no sa_sigaction member in a POSIX.1-2024 build (signal.h requires it) |
| XSH.2.9-threads.005 | SYSTEM | threads.h functions not found when linking with -l pthread, the library c17 names for them |
| XSH.2.9-threads.006 | SYSTEM | pthread_mutex_clocklock is required (unshaded or CX) by the page that defines it and is not provided in a POSIX.1-2024 build |
| XSH.2.9-threads.007 | SYSTEM | threads.h functions not found when linking with -l pthread, the library c17 names for them |
| XSH.2.9-threads.012 | SYSTEM | posix_close is required (unshaded or CX) by the page that defines it and is not provided in a POSIX.1-2024 build |
| XSH.2.9-threads.013 | SYSTEM | accept4 is required (unshaded or CX) by the page that defines it and is not provided in a POSIX.1-2024 build |
| XSH.2.9-threads.014 | SYSTEM | threads.h functions not found when linking with -l pthread, the library c17 names for them |
| XSH.2.9-threads.015 | SYSTEM | pthread_cond_clockwait is required (unshaded or CX) by the page that defines it and is not provided in a POSIX.1-2024 build |
| XSH.2.9-threads.024 | SYSTEM | threads.h functions not found when linking with -l pthread, the library c17 names for them |
| XSH._Exit.014 | SYSTEM | struct sigaction has no sa_sigaction member in a POSIX.1-2024 build (signal.h requires it) |
| XSH.accept.005 | SYSTEM | accept4 is required (unshaded or CX) by the page that defines it and is not provided in a POSIX.1-2024 build |
| XSH.accept.006 | TEST-BUG | INADDR_LOOPBACK is not defined by POSIX (netinet/in.h names it only as an extension); tests now use getaddrinfo() for the loopback address |
| XSH.accept.007 | TEST-BUG | INADDR_LOOPBACK is not defined by POSIX (netinet/in.h names it only as an extension); tests now use getaddrinfo() for the loopback address |
| XSH.accept.014 | SYSTEM | accept4 is required (unshaded or CX) by the page that defines it and is not provided in a POSIX.1-2024 build |
| XSH.accept.015 | SYSTEM | accept4 is required (unshaded or CX) by the page that defines it and is not provided in a POSIX.1-2024 build |
| XSH.accept.016 | SYSTEM | accept4 is required (unshaded or CX) by the page that defines it and is not provided in a POSIX.1-2024 build |
| XSH.accept.017 | SYSTEM | accept4 is required (unshaded or CX) by the page that defines it and is not provided in a POSIX.1-2024 build |
| XSH.accept.020 | TEST-BUG | INADDR_LOOPBACK is not defined by POSIX (netinet/in.h names it only as an extension); tests now use getaddrinfo() for the loopback address |
| XSH.aio_read.008 | SYSTEM | struct sigaction has no sa_sigaction member in a POSIX.1-2024 build (signal.h requires it) |
| XSH.bind.009 | TEST-BUG | INADDR_LOOPBACK is not defined by POSIX (netinet/in.h names it only as an extension); tests now use getaddrinfo() for the loopback address |
| XSH.bind.010 | TEST-BUG | INADDR_LOOPBACK is not defined by POSIX (netinet/in.h names it only as an extension); tests now use getaddrinfo() for the loopback address |
| XSH.bind.019 | TEST-BUG | INADDR_LOOPBACK is not defined by POSIX (netinet/in.h names it only as an extension); tests now use getaddrinfo() for the loopback address |
| XSH.bind.020 | TEST-BUG | INADDR_LOOPBACK is not defined by POSIX (netinet/in.h names it only as an extension); tests now use getaddrinfo() for the loopback address |
| XSH.bind.021 | TEST-BUG | INADDR_LOOPBACK is not defined by POSIX (netinet/in.h names it only as an extension); tests now use getaddrinfo() for the loopback address |
| XSH.call_once.001 | SYSTEM | threads.h functions not found when linking with -l pthread, the library c17 names for them |
| XSH.call_once.002 | SYSTEM | threads.h functions not found when linking with -l pthread, the library c17 names for them |
| XSH.call_once.003 | SYSTEM | threads.h functions not found when linking with -l pthread, the library c17 names for them |
| XSH.call_once.004 | SYSTEM | threads.h functions not found when linking with -l pthread, the library c17 names for them |
| XSH.call_once.005 | SYSTEM | threads.h functions not found when linking with -l pthread, the library c17 names for them |
| XSH.ccos.001 | SYSTEM | ccosl is required (unshaded or CX) by the page that defines it and is not provided in a POSIX.1-2024 build |
| XSH.ccos.002 | SYSTEM | ccosl is required (unshaded or CX) by the page that defines it and is not provided in a POSIX.1-2024 build |
| XSH.ccos.003 | SYSTEM | ccosl is required (unshaded or CX) by the page that defines it and is not provided in a POSIX.1-2024 build |
| XSH.ccos.004 | SYSTEM | ccosl is required (unshaded or CX) by the page that defines it and is not provided in a POSIX.1-2024 build |
| XSH.ccos.005 | SYSTEM | ccosl is required (unshaded or CX) by the page that defines it and is not provided in a POSIX.1-2024 build |
| XSH.ccos.006 | SYSTEM | ccosl is required (unshaded or CX) by the page that defines it and is not provided in a POSIX.1-2024 build |
| XSH.ccos.007 | SYSTEM | ccosl is required (unshaded or CX) by the page that defines it and is not provided in a POSIX.1-2024 build |
| XSH.ccos.008 | SYSTEM | ccosl is required (unshaded or CX) by the page that defines it and is not provided in a POSIX.1-2024 build |
| XSH.ccos.009 | SYSTEM | ccosl is required (unshaded or CX) by the page that defines it and is not provided in a POSIX.1-2024 build |
| XSH.ccos.010 | SYSTEM | ccosl is required (unshaded or CX) by the page that defines it and is not provided in a POSIX.1-2024 build |
| XSH.ccos.011 | SYSTEM | ccosl is required (unshaded or CX) by the page that defines it and is not provided in a POSIX.1-2024 build |
| XSH.ccos.012 | SYSTEM | ccosl is required (unshaded or CX) by the page that defines it and is not provided in a POSIX.1-2024 build |
| XSH.ccos.013 | SYSTEM | ccosl is required (unshaded or CX) by the page that defines it and is not provided in a POSIX.1-2024 build |
| XSH.ccosh.001 | SYSTEM | ccoshl is required (unshaded or CX) by the page that defines it and is not provided in a POSIX.1-2024 build |
| XSH.ccosh.002 | SYSTEM | ccoshl is required (unshaded or CX) by the page that defines it and is not provided in a POSIX.1-2024 build |
| XSH.ccosh.003 | SYSTEM | ccoshl is required (unshaded or CX) by the page that defines it and is not provided in a POSIX.1-2024 build |
| XSH.ccosh.004 | SYSTEM | ccoshl is required (unshaded or CX) by the page that defines it and is not provided in a POSIX.1-2024 build |
| XSH.ccosh.005 | SYSTEM | ccoshl is required (unshaded or CX) by the page that defines it and is not provided in a POSIX.1-2024 build |
| XSH.ccosh.006 | SYSTEM | ccoshl is required (unshaded or CX) by the page that defines it and is not provided in a POSIX.1-2024 build |
| XSH.ccosh.007 | SYSTEM | ccoshl is required (unshaded or CX) by the page that defines it and is not provided in a POSIX.1-2024 build |
| XSH.ccosh.008 | SYSTEM | ccoshl is required (unshaded or CX) by the page that defines it and is not provided in a POSIX.1-2024 build |
| XSH.ccosh.009 | SYSTEM | ccoshl is required (unshaded or CX) by the page that defines it and is not provided in a POSIX.1-2024 build |
| XSH.ccosh.010 | SYSTEM | ccoshl is required (unshaded or CX) by the page that defines it and is not provided in a POSIX.1-2024 build |
| XSH.ccosh.011 | SYSTEM | ccoshl is required (unshaded or CX) by the page that defines it and is not provided in a POSIX.1-2024 build |
| XSH.ccosh.012 | SYSTEM | ccoshl is required (unshaded or CX) by the page that defines it and is not provided in a POSIX.1-2024 build |
| XSH.ccosh.013 | SYSTEM | ccoshl is required (unshaded or CX) by the page that defines it and is not provided in a POSIX.1-2024 build |
| XSH.chmod.003 | TEST-BUG | the S_IF* type constants are XSI-shaded in sys/stat.h; the test now takes the type bits from a directory's st_mode |
| XSH.cnd_init.001 | SYSTEM | threads.h functions not found when linking with -l pthread, the library c17 names for them |
| XSH.cnd_init.002 | SYSTEM | threads.h functions not found when linking with -l pthread, the library c17 names for them |
| XSH.cnd_init.003 | SYSTEM | threads.h functions not found when linking with -l pthread, the library c17 names for them |
| XSH.cnd_signal.001 | SYSTEM | threads.h functions not found when linking with -l pthread, the library c17 names for them |
| XSH.cnd_signal.002 | SYSTEM | threads.h functions not found when linking with -l pthread, the library c17 names for them |
| XSH.cnd_signal.003 | SYSTEM | threads.h functions not found when linking with -l pthread, the library c17 names for them |
| XSH.cnd_signal.004 | SYSTEM | threads.h functions not found when linking with -l pthread, the library c17 names for them |
| XSH.cnd_wait.001 | SYSTEM | threads.h functions not found when linking with -l pthread, the library c17 names for them |
| XSH.cnd_wait.002 | SYSTEM | threads.h functions not found when linking with -l pthread, the library c17 names for them |
| XSH.cnd_wait.003 | SYSTEM | threads.h functions not found when linking with -l pthread, the library c17 names for them |
| XSH.cnd_wait.004 | SYSTEM | threads.h functions not found when linking with -l pthread, the library c17 names for them |
| XSH.cnd_wait.005 | SYSTEM | threads.h functions not found when linking with -l pthread, the library c17 names for them |
| XSH.cnd_wait.006 | SYSTEM | threads.h functions not found when linking with -l pthread, the library c17 names for them |
| XSH.cnd_wait.007 | SYSTEM | threads.h functions not found when linking with -l pthread, the library c17 names for them |
| XSH.cnd_wait.008 | SYSTEM | threads.h functions not found when linking with -l pthread, the library c17 names for them |
| XSH.connect.002 | TEST-BUG | INADDR_LOOPBACK is not defined by POSIX (netinet/in.h names it only as an extension); tests now use getaddrinfo() for the loopback address |
| XSH.connect.004 | TEST-BUG | INADDR_LOOPBACK is not defined by POSIX (netinet/in.h names it only as an extension); tests now use getaddrinfo() for the loopback address |
| XSH.connect.011 | TEST-BUG | INADDR_LOOPBACK is not defined by POSIX (netinet/in.h names it only as an extension); tests now use getaddrinfo() for the loopback address |
| XSH.connect.012 | TEST-BUG | INADDR_LOOPBACK is not defined by POSIX (netinet/in.h names it only as an extension); tests now use getaddrinfo() for the loopback address |
| XSH.connect.015 | TEST-BUG | INADDR_LOOPBACK is not defined by POSIX (netinet/in.h names it only as an extension); tests now use getaddrinfo() for the loopback address |
| XSH.connect.016 | TEST-BUG | INADDR_LOOPBACK is not defined by POSIX (netinet/in.h names it only as an extension); tests now use getaddrinfo() for the loopback address |
| XSH.connect.017 | TEST-BUG | INADDR_LOOPBACK is not defined by POSIX (netinet/in.h names it only as an extension); tests now use getaddrinfo() for the loopback address |
| XSH.connect.018 | TEST-BUG | INADDR_LOOPBACK is not defined by POSIX (netinet/in.h names it only as an extension); tests now use getaddrinfo() for the loopback address |
| XSH.connect.019 | TEST-BUG | INADDR_LOOPBACK is not defined by POSIX (netinet/in.h names it only as an extension); tests now use getaddrinfo() for the loopback address |
| XSH.connect.020 | TEST-BUG | INADDR_LOOPBACK is not defined by POSIX (netinet/in.h names it only as an extension); tests now use getaddrinfo() for the loopback address |
| XSH.csin.001 | SYSTEM | csinl is required (unshaded or CX) by the page that defines it and is not provided in a POSIX.1-2024 build |
| XSH.csin.002 | SYSTEM | csinl is required (unshaded or CX) by the page that defines it and is not provided in a POSIX.1-2024 build |
| XSH.csin.003 | SYSTEM | csinl is required (unshaded or CX) by the page that defines it and is not provided in a POSIX.1-2024 build |
| XSH.csin.004 | SYSTEM | csinl is required (unshaded or CX) by the page that defines it and is not provided in a POSIX.1-2024 build |
| XSH.csin.005 | SYSTEM | csinl is required (unshaded or CX) by the page that defines it and is not provided in a POSIX.1-2024 build |
| XSH.csin.006 | SYSTEM | csinl is required (unshaded or CX) by the page that defines it and is not provided in a POSIX.1-2024 build |
| XSH.csin.007 | SYSTEM | csinl is required (unshaded or CX) by the page that defines it and is not provided in a POSIX.1-2024 build |
| XSH.csin.008 | SYSTEM | csinl is required (unshaded or CX) by the page that defines it and is not provided in a POSIX.1-2024 build |
| XSH.csin.009 | SYSTEM | csinl is required (unshaded or CX) by the page that defines it and is not provided in a POSIX.1-2024 build |
| XSH.csin.010 | SYSTEM | csinl is required (unshaded or CX) by the page that defines it and is not provided in a POSIX.1-2024 build |
| XSH.csin.011 | SYSTEM | csinl is required (unshaded or CX) by the page that defines it and is not provided in a POSIX.1-2024 build |
| XSH.csin.012 | SYSTEM | csinl is required (unshaded or CX) by the page that defines it and is not provided in a POSIX.1-2024 build |
| XSH.csin.013 | SYSTEM | csinl is required (unshaded or CX) by the page that defines it and is not provided in a POSIX.1-2024 build |
| XSH.csinh.001 | SYSTEM | csinhl is required (unshaded or CX) by the page that defines it and is not provided in a POSIX.1-2024 build |
| XSH.csinh.002 | SYSTEM | csinhl is required (unshaded or CX) by the page that defines it and is not provided in a POSIX.1-2024 build |
| XSH.csinh.003 | SYSTEM | csinhl is required (unshaded or CX) by the page that defines it and is not provided in a POSIX.1-2024 build |
| XSH.csinh.004 | SYSTEM | csinhl is required (unshaded or CX) by the page that defines it and is not provided in a POSIX.1-2024 build |
| XSH.csinh.005 | SYSTEM | csinhl is required (unshaded or CX) by the page that defines it and is not provided in a POSIX.1-2024 build |
| XSH.csinh.006 | SYSTEM | csinhl is required (unshaded or CX) by the page that defines it and is not provided in a POSIX.1-2024 build |
| XSH.csinh.007 | SYSTEM | csinhl is required (unshaded or CX) by the page that defines it and is not provided in a POSIX.1-2024 build |
| XSH.csinh.008 | SYSTEM | csinhl is required (unshaded or CX) by the page that defines it and is not provided in a POSIX.1-2024 build |
| XSH.csinh.009 | SYSTEM | csinhl is required (unshaded or CX) by the page that defines it and is not provided in a POSIX.1-2024 build |
| XSH.csinh.010 | SYSTEM | csinhl is required (unshaded or CX) by the page that defines it and is not provided in a POSIX.1-2024 build |
| XSH.csinh.011 | SYSTEM | csinhl is required (unshaded or CX) by the page that defines it and is not provided in a POSIX.1-2024 build |
| XSH.csinh.012 | SYSTEM | csinhl is required (unshaded or CX) by the page that defines it and is not provided in a POSIX.1-2024 build |
| XSH.csinh.013 | SYSTEM | csinhl is required (unshaded or CX) by the page that defines it and is not provided in a POSIX.1-2024 build |
| XSH.ctan.001 | SYSTEM | ctanl is required (unshaded or CX) by the page that defines it and is not provided in a POSIX.1-2024 build |
| XSH.ctan.002 | SYSTEM | ctanl is required (unshaded or CX) by the page that defines it and is not provided in a POSIX.1-2024 build |
| XSH.ctan.003 | SYSTEM | ctanl is required (unshaded or CX) by the page that defines it and is not provided in a POSIX.1-2024 build |
| XSH.ctan.004 | SYSTEM | ctanl is required (unshaded or CX) by the page that defines it and is not provided in a POSIX.1-2024 build |
| XSH.ctan.005 | SYSTEM | ctanl is required (unshaded or CX) by the page that defines it and is not provided in a POSIX.1-2024 build |
| XSH.ctan.006 | SYSTEM | ctanl is required (unshaded or CX) by the page that defines it and is not provided in a POSIX.1-2024 build |
| XSH.ctan.007 | SYSTEM | ctanl is required (unshaded or CX) by the page that defines it and is not provided in a POSIX.1-2024 build |
| XSH.ctan.008 | SYSTEM | ctanl is required (unshaded or CX) by the page that defines it and is not provided in a POSIX.1-2024 build |
| XSH.ctan.009 | SYSTEM | ctanl is required (unshaded or CX) by the page that defines it and is not provided in a POSIX.1-2024 build |
| XSH.ctan.010 | SYSTEM | ctanl is required (unshaded or CX) by the page that defines it and is not provided in a POSIX.1-2024 build |
| XSH.ctan.011 | SYSTEM | ctanl is required (unshaded or CX) by the page that defines it and is not provided in a POSIX.1-2024 build |
| XSH.ctan.012 | SYSTEM | ctanl is required (unshaded or CX) by the page that defines it and is not provided in a POSIX.1-2024 build |
| XSH.ctanh.001 | SYSTEM | ctanhl is required (unshaded or CX) by the page that defines it and is not provided in a POSIX.1-2024 build |
| XSH.ctanh.002 | SYSTEM | ctanhl is required (unshaded or CX) by the page that defines it and is not provided in a POSIX.1-2024 build |
| XSH.ctanh.003 | SYSTEM | ctanhl is required (unshaded or CX) by the page that defines it and is not provided in a POSIX.1-2024 build |
| XSH.ctanh.004 | SYSTEM | ctanhl is required (unshaded or CX) by the page that defines it and is not provided in a POSIX.1-2024 build |
| XSH.ctanh.005 | SYSTEM | ctanhl is required (unshaded or CX) by the page that defines it and is not provided in a POSIX.1-2024 build |
| XSH.ctanh.006 | SYSTEM | ctanhl is required (unshaded or CX) by the page that defines it and is not provided in a POSIX.1-2024 build |
| XSH.ctanh.007 | SYSTEM | ctanhl is required (unshaded or CX) by the page that defines it and is not provided in a POSIX.1-2024 build |
| XSH.ctanh.008 | SYSTEM | ctanhl is required (unshaded or CX) by the page that defines it and is not provided in a POSIX.1-2024 build |
| XSH.ctanh.009 | SYSTEM | ctanhl is required (unshaded or CX) by the page that defines it and is not provided in a POSIX.1-2024 build |
| XSH.ctanh.010 | SYSTEM | ctanhl is required (unshaded or CX) by the page that defines it and is not provided in a POSIX.1-2024 build |
| XSH.ctanh.011 | SYSTEM | ctanhl is required (unshaded or CX) by the page that defines it and is not provided in a POSIX.1-2024 build |
| XSH.ctanh.012 | SYSTEM | ctanhl is required (unshaded or CX) by the page that defines it and is not provided in a POSIX.1-2024 build |
| XSH.dladdr.001 | SYSTEM | Dl_info_t is required (unshaded or CX) by the page that defines it and is not provided in a POSIX.1-2024 build |
| XSH.dladdr.002 | SYSTEM | Dl_info_t is required (unshaded or CX) by the page that defines it and is not provided in a POSIX.1-2024 build |
| XSH.dladdr.003 | SYSTEM | Dl_info_t is required (unshaded or CX) by the page that defines it and is not provided in a POSIX.1-2024 build |
| XSH.dladdr.004 | SYSTEM | Dl_info_t is required (unshaded or CX) by the page that defines it and is not provided in a POSIX.1-2024 build |
| XSH.dlclose.002 | SYSTEM | Dl_info_t is required (unshaded or CX) by the page that defines it and is not provided in a POSIX.1-2024 build |
| XSH.dlopen.004 | SYSTEM | Dl_info_t is required (unshaded or CX) by the page that defines it and is not provided in a POSIX.1-2024 build |
| XSH.dlsym.003 | SYSTEM | Dl_info_t is required (unshaded or CX) by the page that defines it and is not provided in a POSIX.1-2024 build |
| XSH.dup.012 | SYSTEM | dup3 is required (unshaded or CX) by the page that defines it and is not provided in a POSIX.1-2024 build |
| XSH.dup.013 | SYSTEM | dup3 is required (unshaded or CX) by the page that defines it and is not provided in a POSIX.1-2024 build |
| XSH.dup.014 | SYSTEM | dup3 is required (unshaded or CX) by the page that defines it and is not provided in a POSIX.1-2024 build |
| XSH.dup.015 | SYSTEM | dup3 is required (unshaded or CX) by the page that defines it and is not provided in a POSIX.1-2024 build |
| XSH.exit.011 | SYSTEM | struct sigaction has no sa_sigaction member in a POSIX.1-2024 build (signal.h requires it) |
| XSH.exit.012 | SYSTEM | struct sigaction has no sa_sigaction member in a POSIX.1-2024 build (signal.h requires it) |
| XSH.fnmatch.021 | SYSTEM | FNM_CASEFOLD is required (unshaded or CX) by the page that defines it and is not provided in a POSIX.1-2024 build |
| XSH.fnmatch.022 | SYSTEM | FNM_IGNORECASE is required (unshaded or CX) by the page that defines it and is not provided in a POSIX.1-2024 build |
| XSH.fpathconf.004 | SYSTEM | _SC_XOPEN_UNIX is required (unshaded or CX) by the page that defines it and is not provided in a POSIX.1-2024 build |
| XSH.fprintf.055 | SYSTEM | asprintf is required (unshaded or CX) by the page that defines it and is not provided in a POSIX.1-2024 build |
| XSH.fprintf.056 | SYSTEM | asprintf is required (unshaded or CX) by the page that defines it and is not provided in a POSIX.1-2024 build |
| XSH.free.003 | SYSTEM | reallocarray is required (unshaded or CX) by the page that defines it and is not provided in a POSIX.1-2024 build |
| XSH.fseek.013 | TEST-BUG | SIGXFSZ is XSI-shaded in signal.h; its uses are now guarded, and where it is not named the child blocks every signal |
| XSH.fsetpos.009 | TEST-BUG | SIGXFSZ is XSI-shaded in signal.h; its uses are now guarded, and where it is not named the child blocks every signal |
| XSH.gai_strerror.001 | SYSTEM | EAI_OVERFLOW is required (unshaded or CX) by the page that defines it and is not provided in a POSIX.1-2024 build |
| XSH.gai_strerror.002 | SYSTEM | EAI_OVERFLOW is required (unshaded or CX) by the page that defines it and is not provided in a POSIX.1-2024 build |
| XSH.getaddrinfo.009 | SYSTEM | EAI_OVERFLOW is required (unshaded or CX) by the page that defines it and is not provided in a POSIX.1-2024 build |
| XSH.getaddrinfo.025 | SYSTEM | SOCK_CLOEXEC is required (unshaded or CX) by the page that defines it and is not provided in a POSIX.1-2024 build |
| XSH.getentropy.001 | SYSTEM | getentropy is required (unshaded or CX) by the page that defines it and is not provided in a POSIX.1-2024 build |
| XSH.getentropy.002 | SYSTEM | getentropy is required (unshaded or CX) by the page that defines it and is not provided in a POSIX.1-2024 build |
| XSH.getentropy.003 | SYSTEM | getentropy is required (unshaded or CX) by the page that defines it and is not provided in a POSIX.1-2024 build |
| XSH.getenv.004 | SYSTEM | secure_getenv is required (unshaded or CX) by the page that defines it and is not provided in a POSIX.1-2024 build |
| XSH.getenv.005 | SYSTEM | secure_getenv is required (unshaded or CX) by the page that defines it and is not provided in a POSIX.1-2024 build |
| XSH.getlocalename_l.001 | SYSTEM | getlocalename_l is required (unshaded or CX) by the page that defines it and is not provided in a POSIX.1-2024 build |
| XSH.getlocalename_l.002 | SYSTEM | getlocalename_l is required (unshaded or CX) by the page that defines it and is not provided in a POSIX.1-2024 build |
| XSH.getlocalename_l.003 | SYSTEM | getlocalename_l is required (unshaded or CX) by the page that defines it and is not provided in a POSIX.1-2024 build |
| XSH.getlocalename_l.004 | SYSTEM | getlocalename_l is required (unshaded or CX) by the page that defines it and is not provided in a POSIX.1-2024 build |
| XSH.getlocalename_l.005 | SYSTEM | getlocalename_l is required (unshaded or CX) by the page that defines it and is not provided in a POSIX.1-2024 build |
| XSH.getnameinfo.003 | SYSTEM | EAI_OVERFLOW is required (unshaded or CX) by the page that defines it and is not provided in a POSIX.1-2024 build |
| XSH.getnameinfo.004 | SYSTEM | EAI_OVERFLOW is required (unshaded or CX) by the page that defines it and is not provided in a POSIX.1-2024 build |
| XSH.getpeername.002 | TEST-BUG | INADDR_LOOPBACK is not defined by POSIX (netinet/in.h names it only as an extension); tests now use getaddrinfo() for the loopback address |
| XSH.getpeername.004 | TEST-BUG | INADDR_LOOPBACK is not defined by POSIX (netinet/in.h names it only as an extension); tests now use getaddrinfo() for the loopback address |
| XSH.getpeername.005 | TEST-BUG | INADDR_LOOPBACK is not defined by POSIX (netinet/in.h names it only as an extension); tests now use getaddrinfo() for the loopback address |
| XSH.getservent.005 | TEST-BUG | htons() used without including arpa/inet.h |
| XSH.getsockname.001 | TEST-BUG | INADDR_LOOPBACK is not defined by POSIX (netinet/in.h names it only as an extension); tests now use getaddrinfo() for the loopback address |
| XSH.getsockname.003 | TEST-BUG | INADDR_LOOPBACK is not defined by POSIX (netinet/in.h names it only as an extension); tests now use getaddrinfo() for the loopback address |
| XSH.getsockopt.001 | TEST-BUG | INADDR_LOOPBACK is not defined by POSIX (netinet/in.h names it only as an extension); tests now use getaddrinfo() for the loopback address |
| XSH.getsockopt.002 | TEST-BUG | INADDR_LOOPBACK is not defined by POSIX (netinet/in.h names it only as an extension); tests now use getaddrinfo() for the loopback address |
| XSH.getsockopt.003 | TEST-BUG | INADDR_LOOPBACK is not defined by POSIX (netinet/in.h names it only as an extension); tests now use getaddrinfo() for the loopback address |
| XSH.getsockopt.004 | TEST-BUG | INADDR_LOOPBACK is not defined by POSIX (netinet/in.h names it only as an extension); tests now use getaddrinfo() for the loopback address |
| XSH.getsockopt.005 | TEST-BUG | INADDR_LOOPBACK is not defined by POSIX (netinet/in.h names it only as an extension); tests now use getaddrinfo() for the loopback address |
| XSH.getsockopt.006 | TEST-BUG | INADDR_LOOPBACK is not defined by POSIX (netinet/in.h names it only as an extension); tests now use getaddrinfo() for the loopback address |
| XSH.getsockopt.007 | TEST-BUG | INADDR_LOOPBACK is not defined by POSIX (netinet/in.h names it only as an extension); tests now use getaddrinfo() for the loopback address |
| XSH.getsockopt.008 | TEST-BUG | INADDR_LOOPBACK is not defined by POSIX (netinet/in.h names it only as an extension); tests now use getaddrinfo() for the loopback address |
| XSH.getsockopt.009 | TEST-BUG | INADDR_LOOPBACK is not defined by POSIX (netinet/in.h names it only as an extension); tests now use getaddrinfo() for the loopback address |
| XSH.getsockopt.010 | TEST-BUG | INADDR_LOOPBACK is not defined by POSIX (netinet/in.h names it only as an extension); tests now use getaddrinfo() for the loopback address |
| XSH.getsockopt.011 | TEST-BUG | INADDR_LOOPBACK is not defined by POSIX (netinet/in.h names it only as an extension); tests now use getaddrinfo() for the loopback address |
| XSH.getsockopt.012 | TEST-BUG | INADDR_LOOPBACK is not defined by POSIX (netinet/in.h names it only as an extension); tests now use getaddrinfo() for the loopback address |
| XSH.getsockopt.013 | TEST-BUG | INADDR_LOOPBACK is not defined by POSIX (netinet/in.h names it only as an extension); tests now use getaddrinfo() for the loopback address |
| XSH.getsockopt.014 | TEST-BUG | INADDR_LOOPBACK is not defined by POSIX (netinet/in.h names it only as an extension); tests now use getaddrinfo() for the loopback address |
| XSH.getsockopt.015 | TEST-BUG | INADDR_LOOPBACK is not defined by POSIX (netinet/in.h names it only as an extension); tests now use getaddrinfo() for the loopback address |
| XSH.getsockopt.016 | TEST-BUG | INADDR_LOOPBACK is not defined by POSIX (netinet/in.h names it only as an extension); tests now use getaddrinfo() for the loopback address |
| XSH.getsockopt.017 | TEST-BUG | INADDR_LOOPBACK is not defined by POSIX (netinet/in.h names it only as an extension); tests now use getaddrinfo() for the loopback address |
| XSH.getsockopt.018 | TEST-BUG | INADDR_LOOPBACK is not defined by POSIX (netinet/in.h names it only as an extension); tests now use getaddrinfo() for the loopback address |
| XSH.gettext.001 | SYSTEM | libintl.h is required (unshaded or CX) by the page that defines it and is not provided in a POSIX.1-2024 build |
| XSH.gettext.002 | SYSTEM | libintl.h is required (unshaded or CX) by the page that defines it and is not provided in a POSIX.1-2024 build |
| XSH.gettext.003 | SYSTEM | libintl.h is required (unshaded or CX) by the page that defines it and is not provided in a POSIX.1-2024 build |
| XSH.gettext.004 | SYSTEM | libintl.h is required (unshaded or CX) by the page that defines it and is not provided in a POSIX.1-2024 build |
| XSH.gettext.005 | SYSTEM | libintl.h is required (unshaded or CX) by the page that defines it and is not provided in a POSIX.1-2024 build |
| XSH.gettext.006 | SYSTEM | libintl.h is required (unshaded or CX) by the page that defines it and is not provided in a POSIX.1-2024 build |
| XSH.gettext.007 | SYSTEM | libintl.h is required (unshaded or CX) by the page that defines it and is not provided in a POSIX.1-2024 build |
| XSH.gettext.008 | SYSTEM | libintl.h is required (unshaded or CX) by the page that defines it and is not provided in a POSIX.1-2024 build |
| XSH.gettext.009 | SYSTEM | libintl.h is required (unshaded or CX) by the page that defines it and is not provided in a POSIX.1-2024 build |
| XSH.gettext.010 | SYSTEM | libintl.h is required (unshaded or CX) by the page that defines it and is not provided in a POSIX.1-2024 build |
| XSH.gettext.011 | SYSTEM | libintl.h is required (unshaded or CX) by the page that defines it and is not provided in a POSIX.1-2024 build |
| XSH.gettext.012 | SYSTEM | libintl.h is required (unshaded or CX) by the page that defines it and is not provided in a POSIX.1-2024 build |
| XSH.gettext.014 | SYSTEM | libintl.h is required (unshaded or CX) by the page that defines it and is not provided in a POSIX.1-2024 build |
| XSH.gettext.015 | SYSTEM | libintl.h is required (unshaded or CX) by the page that defines it and is not provided in a POSIX.1-2024 build |
| XSH.kill.007 | TEST-BUG | sysconf(_SC_NSIG) gave a number that is a supported signal here, so the signal was really sent and ended the test; setup now checks the number is not a signal (UNRESOLVED if it is; the fault is XSH.sysconf.015) |
| XSH.lio_listio.003 | SYSTEM | struct sigaction has no sa_sigaction member in a POSIX.1-2024 build (signal.h requires it) |
| XSH.listen.003 | TEST-BUG | INADDR_LOOPBACK is not defined by POSIX (netinet/in.h names it only as an extension); tests now use getaddrinfo() for the loopback address |
| XSH.listen.007 | TEST-BUG | INADDR_LOOPBACK is not defined by POSIX (netinet/in.h names it only as an extension); tests now use getaddrinfo() for the loopback address |
| XSH.memmem.001 | SYSTEM | memmem is required (unshaded or CX) by the page that defines it and is not provided in a POSIX.1-2024 build |
| XSH.memmem.002 | SYSTEM | memmem is required (unshaded or CX) by the page that defines it and is not provided in a POSIX.1-2024 build |
| XSH.memmem.003 | SYSTEM | memmem is required (unshaded or CX) by the page that defines it and is not provided in a POSIX.1-2024 build |
| XSH.memmem.004 | SYSTEM | memmem is required (unshaded or CX) by the page that defines it and is not provided in a POSIX.1-2024 build |
| XSH.memmem.005 | SYSTEM | memmem is required (unshaded or CX) by the page that defines it and is not provided in a POSIX.1-2024 build |
| XSH.memmem.006 | SYSTEM | memmem is required (unshaded or CX) by the page that defines it and is not provided in a POSIX.1-2024 build |
| XSH.memmem.007 | SYSTEM | memmem is required (unshaded or CX) by the page that defines it and is not provided in a POSIX.1-2024 build |
| XSH.mkdtemp.015 | SYSTEM | mkostemp is required (unshaded or CX) by the page that defines it and is not provided in a POSIX.1-2024 build |
| XSH.mkdtemp.016 | SYSTEM | mkostemp is required (unshaded or CX) by the page that defines it and is not provided in a POSIX.1-2024 build |
| XSH.mkdtemp.017 | SYSTEM | mkostemp is required (unshaded or CX) by the page that defines it and is not provided in a POSIX.1-2024 build |
| XSH.mkdtemp.018 | SYSTEM | mkostemp is required (unshaded or CX) by the page that defines it and is not provided in a POSIX.1-2024 build |
| XSH.mlock.003 | SYSTEM | MAP_ANONYMOUS is required (unshaded or CX) by the page that defines it and is not provided in a POSIX.1-2024 build |
| XSH.mmap.001 | SYSTEM | MAP_ANONYMOUS is required (unshaded or CX) by the page that defines it and is not provided in a POSIX.1-2024 build |
| XSH.mmap.002 | SYSTEM | MAP_ANONYMOUS is required (unshaded or CX) by the page that defines it and is not provided in a POSIX.1-2024 build |
| XSH.mmap.003 | SYSTEM | MAP_ANONYMOUS is required (unshaded or CX) by the page that defines it and is not provided in a POSIX.1-2024 build |
| XSH.mmap.004 | SYSTEM | MAP_ANONYMOUS is required (unshaded or CX) by the page that defines it and is not provided in a POSIX.1-2024 build |
| XSH.mmap.005 | SYSTEM | MAP_ANONYMOUS is required (unshaded or CX) by the page that defines it and is not provided in a POSIX.1-2024 build |
| XSH.mmap.006 | SYSTEM | MAP_ANONYMOUS is required (unshaded or CX) by the page that defines it and is not provided in a POSIX.1-2024 build |
| XSH.mmap.007 | SYSTEM | MAP_ANONYMOUS is required (unshaded or CX) by the page that defines it and is not provided in a POSIX.1-2024 build |
| XSH.mmap.008 | SYSTEM | MAP_ANONYMOUS is required (unshaded or CX) by the page that defines it and is not provided in a POSIX.1-2024 build |
| XSH.mmap.009 | SYSTEM | MAP_ANONYMOUS is required (unshaded or CX) by the page that defines it and is not provided in a POSIX.1-2024 build |
| XSH.mmap.010 | SYSTEM | MAP_ANONYMOUS is required (unshaded or CX) by the page that defines it and is not provided in a POSIX.1-2024 build |
| XSH.mmap.011 | SYSTEM | MAP_ANONYMOUS is required (unshaded or CX) by the page that defines it and is not provided in a POSIX.1-2024 build |
| XSH.mmap.012 | SYSTEM | MAP_ANONYMOUS is required (unshaded or CX) by the page that defines it and is not provided in a POSIX.1-2024 build |
| XSH.mmap.013 | SYSTEM | MAP_ANONYMOUS is required (unshaded or CX) by the page that defines it and is not provided in a POSIX.1-2024 build |
| XSH.mmap.014 | SYSTEM | MAP_ANONYMOUS is required (unshaded or CX) by the page that defines it and is not provided in a POSIX.1-2024 build |
| XSH.mmap.015 | SYSTEM | MAP_ANONYMOUS is required (unshaded or CX) by the page that defines it and is not provided in a POSIX.1-2024 build |
| XSH.mmap.016 | SYSTEM | MAP_ANONYMOUS is required (unshaded or CX) by the page that defines it and is not provided in a POSIX.1-2024 build |
| XSH.mmap.017 | SYSTEM | MAP_ANONYMOUS is required (unshaded or CX) by the page that defines it and is not provided in a POSIX.1-2024 build |
| XSH.mmap.018 | SYSTEM | MAP_ANONYMOUS is required (unshaded or CX) by the page that defines it and is not provided in a POSIX.1-2024 build |
| XSH.mmap.019 | SYSTEM | MAP_ANONYMOUS is required (unshaded or CX) by the page that defines it and is not provided in a POSIX.1-2024 build |
| XSH.mmap.020 | SYSTEM | MAP_ANONYMOUS is required (unshaded or CX) by the page that defines it and is not provided in a POSIX.1-2024 build |
| XSH.mmap.021 | SYSTEM | MAP_ANONYMOUS is required (unshaded or CX) by the page that defines it and is not provided in a POSIX.1-2024 build |
| XSH.mprotect.001 | SYSTEM | MAP_ANONYMOUS is required (unshaded or CX) by the page that defines it and is not provided in a POSIX.1-2024 build |
| XSH.mprotect.002 | SYSTEM | MAP_ANONYMOUS is required (unshaded or CX) by the page that defines it and is not provided in a POSIX.1-2024 build |
| XSH.mprotect.003 | SYSTEM | MAP_ANONYMOUS is required (unshaded or CX) by the page that defines it and is not provided in a POSIX.1-2024 build |
| XSH.mprotect.004 | SYSTEM | MAP_ANONYMOUS is required (unshaded or CX) by the page that defines it and is not provided in a POSIX.1-2024 build |
| XSH.mprotect.005 | SYSTEM | MAP_ANONYMOUS is required (unshaded or CX) by the page that defines it and is not provided in a POSIX.1-2024 build |
| XSH.mprotect.006 | SYSTEM | MAP_ANONYMOUS is required (unshaded or CX) by the page that defines it and is not provided in a POSIX.1-2024 build |
| XSH.mprotect.007 | SYSTEM | MAP_ANONYMOUS is required (unshaded or CX) by the page that defines it and is not provided in a POSIX.1-2024 build |
| XSH.mprotect.008 | SYSTEM | MAP_ANONYMOUS is required (unshaded or CX) by the page that defines it and is not provided in a POSIX.1-2024 build |
| XSH.mq_close.001 | SYSTEM | struct sigaction has no sa_sigaction member in a POSIX.1-2024 build (signal.h requires it) |
| XSH.mq_close.002 | SYSTEM | struct sigaction has no sa_sigaction member in a POSIX.1-2024 build (signal.h requires it) |
| XSH.mq_close.003 | SYSTEM | struct sigaction has no sa_sigaction member in a POSIX.1-2024 build (signal.h requires it) |
| XSH.mq_close.004 | SYSTEM | struct sigaction has no sa_sigaction member in a POSIX.1-2024 build (signal.h requires it) |
| XSH.mq_getattr.001 | SYSTEM | struct sigaction has no sa_sigaction member in a POSIX.1-2024 build (signal.h requires it) |
| XSH.mq_getattr.002 | SYSTEM | struct sigaction has no sa_sigaction member in a POSIX.1-2024 build (signal.h requires it) |
| XSH.mq_getattr.003 | SYSTEM | struct sigaction has no sa_sigaction member in a POSIX.1-2024 build (signal.h requires it) |
| XSH.mq_notify.001 | SYSTEM | struct sigaction has no sa_sigaction member in a POSIX.1-2024 build (signal.h requires it) |
| XSH.mq_notify.002 | SYSTEM | struct sigaction has no sa_sigaction member in a POSIX.1-2024 build (signal.h requires it) |
| XSH.mq_notify.003 | SYSTEM | struct sigaction has no sa_sigaction member in a POSIX.1-2024 build (signal.h requires it) |
| XSH.mq_notify.004 | SYSTEM | struct sigaction has no sa_sigaction member in a POSIX.1-2024 build (signal.h requires it) |
| XSH.mq_notify.005 | SYSTEM | struct sigaction has no sa_sigaction member in a POSIX.1-2024 build (signal.h requires it) |
| XSH.mq_notify.006 | SYSTEM | struct sigaction has no sa_sigaction member in a POSIX.1-2024 build (signal.h requires it) |
| XSH.mq_notify.007 | SYSTEM | struct sigaction has no sa_sigaction member in a POSIX.1-2024 build (signal.h requires it) |
| XSH.mq_notify.008 | SYSTEM | struct sigaction has no sa_sigaction member in a POSIX.1-2024 build (signal.h requires it) |
| XSH.mq_notify.009 | SYSTEM | struct sigaction has no sa_sigaction member in a POSIX.1-2024 build (signal.h requires it) |
| XSH.mq_notify.010 | SYSTEM | struct sigaction has no sa_sigaction member in a POSIX.1-2024 build (signal.h requires it) |
| XSH.mq_open.001 | SYSTEM | struct sigaction has no sa_sigaction member in a POSIX.1-2024 build (signal.h requires it) |
| XSH.mq_open.002 | SYSTEM | struct sigaction has no sa_sigaction member in a POSIX.1-2024 build (signal.h requires it) |
| XSH.mq_open.003 | SYSTEM | struct sigaction has no sa_sigaction member in a POSIX.1-2024 build (signal.h requires it) |
| XSH.mq_open.004 | SYSTEM | struct sigaction has no sa_sigaction member in a POSIX.1-2024 build (signal.h requires it) |
| XSH.mq_open.005 | SYSTEM | struct sigaction has no sa_sigaction member in a POSIX.1-2024 build (signal.h requires it) |
| XSH.mq_open.006 | SYSTEM | struct sigaction has no sa_sigaction member in a POSIX.1-2024 build (signal.h requires it) |
| XSH.mq_open.007 | SYSTEM | struct sigaction has no sa_sigaction member in a POSIX.1-2024 build (signal.h requires it) |
| XSH.mq_open.008 | SYSTEM | struct sigaction has no sa_sigaction member in a POSIX.1-2024 build (signal.h requires it) |
| XSH.mq_open.009 | SYSTEM | struct sigaction has no sa_sigaction member in a POSIX.1-2024 build (signal.h requires it) |
| XSH.mq_open.010 | SYSTEM | struct sigaction has no sa_sigaction member in a POSIX.1-2024 build (signal.h requires it) |
| XSH.mq_open.011 | SYSTEM | struct sigaction has no sa_sigaction member in a POSIX.1-2024 build (signal.h requires it) |
| XSH.mq_open.012 | SYSTEM | struct sigaction has no sa_sigaction member in a POSIX.1-2024 build (signal.h requires it) |
| XSH.mq_open.013 | SYSTEM | struct sigaction has no sa_sigaction member in a POSIX.1-2024 build (signal.h requires it) |
| XSH.mq_open.014 | SYSTEM | struct sigaction has no sa_sigaction member in a POSIX.1-2024 build (signal.h requires it) |
| XSH.mq_open.015 | SYSTEM | struct sigaction has no sa_sigaction member in a POSIX.1-2024 build (signal.h requires it) |
| XSH.mq_open.016 | SYSTEM | struct sigaction has no sa_sigaction member in a POSIX.1-2024 build (signal.h requires it) |
| XSH.mq_open.017 | SYSTEM | struct sigaction has no sa_sigaction member in a POSIX.1-2024 build (signal.h requires it) |
| XSH.mq_receive.001 | SYSTEM | struct sigaction has no sa_sigaction member in a POSIX.1-2024 build (signal.h requires it) |
| XSH.mq_receive.002 | SYSTEM | struct sigaction has no sa_sigaction member in a POSIX.1-2024 build (signal.h requires it) |
| XSH.mq_receive.003 | SYSTEM | struct sigaction has no sa_sigaction member in a POSIX.1-2024 build (signal.h requires it) |
| XSH.mq_receive.004 | SYSTEM | struct sigaction has no sa_sigaction member in a POSIX.1-2024 build (signal.h requires it) |
| XSH.mq_receive.005 | SYSTEM | struct sigaction has no sa_sigaction member in a POSIX.1-2024 build (signal.h requires it) |
| XSH.mq_receive.006 | SYSTEM | struct sigaction has no sa_sigaction member in a POSIX.1-2024 build (signal.h requires it) |
| XSH.mq_receive.007 | SYSTEM | struct sigaction has no sa_sigaction member in a POSIX.1-2024 build (signal.h requires it) |
| XSH.mq_receive.008 | SYSTEM | struct sigaction has no sa_sigaction member in a POSIX.1-2024 build (signal.h requires it) |
| XSH.mq_send.001 | SYSTEM | struct sigaction has no sa_sigaction member in a POSIX.1-2024 build (signal.h requires it) |
| XSH.mq_send.002 | SYSTEM | struct sigaction has no sa_sigaction member in a POSIX.1-2024 build (signal.h requires it) |
| XSH.mq_send.003 | SYSTEM | struct sigaction has no sa_sigaction member in a POSIX.1-2024 build (signal.h requires it) |
| XSH.mq_send.004 | SYSTEM | struct sigaction has no sa_sigaction member in a POSIX.1-2024 build (signal.h requires it) |
| XSH.mq_send.005 | SYSTEM | struct sigaction has no sa_sigaction member in a POSIX.1-2024 build (signal.h requires it) |
| XSH.mq_send.006 | SYSTEM | struct sigaction has no sa_sigaction member in a POSIX.1-2024 build (signal.h requires it) |
| XSH.mq_send.007 | SYSTEM | struct sigaction has no sa_sigaction member in a POSIX.1-2024 build (signal.h requires it) |
| XSH.mq_send.008 | SYSTEM | struct sigaction has no sa_sigaction member in a POSIX.1-2024 build (signal.h requires it) |
| XSH.mq_send.009 | SYSTEM | struct sigaction has no sa_sigaction member in a POSIX.1-2024 build (signal.h requires it) |
| XSH.mq_setattr.001 | SYSTEM | struct sigaction has no sa_sigaction member in a POSIX.1-2024 build (signal.h requires it) |
| XSH.mq_setattr.002 | SYSTEM | struct sigaction has no sa_sigaction member in a POSIX.1-2024 build (signal.h requires it) |
| XSH.mq_setattr.003 | SYSTEM | struct sigaction has no sa_sigaction member in a POSIX.1-2024 build (signal.h requires it) |
| XSH.mq_setattr.004 | SYSTEM | struct sigaction has no sa_sigaction member in a POSIX.1-2024 build (signal.h requires it) |
| XSH.mq_setattr.005 | SYSTEM | struct sigaction has no sa_sigaction member in a POSIX.1-2024 build (signal.h requires it) |
| XSH.mq_setattr.006 | SYSTEM | struct sigaction has no sa_sigaction member in a POSIX.1-2024 build (signal.h requires it) |
| XSH.mq_timedreceive.001 | SYSTEM | struct sigaction has no sa_sigaction member in a POSIX.1-2024 build (signal.h requires it) |
| XSH.mq_timedreceive.002 | SYSTEM | struct sigaction has no sa_sigaction member in a POSIX.1-2024 build (signal.h requires it) |
| XSH.mq_timedreceive.003 | SYSTEM | struct sigaction has no sa_sigaction member in a POSIX.1-2024 build (signal.h requires it) |
| XSH.mq_timedreceive.004 | SYSTEM | struct sigaction has no sa_sigaction member in a POSIX.1-2024 build (signal.h requires it) |
| XSH.mq_timedreceive.005 | SYSTEM | struct sigaction has no sa_sigaction member in a POSIX.1-2024 build (signal.h requires it) |
| XSH.mq_timedreceive.006 | SYSTEM | struct sigaction has no sa_sigaction member in a POSIX.1-2024 build (signal.h requires it) |
| XSH.mq_timedreceive.007 | SYSTEM | struct sigaction has no sa_sigaction member in a POSIX.1-2024 build (signal.h requires it) |
| XSH.mq_timedreceive.008 | SYSTEM | struct sigaction has no sa_sigaction member in a POSIX.1-2024 build (signal.h requires it) |
| XSH.mq_timedreceive.009 | SYSTEM | struct sigaction has no sa_sigaction member in a POSIX.1-2024 build (signal.h requires it) |
| XSH.mq_timedsend.001 | SYSTEM | struct sigaction has no sa_sigaction member in a POSIX.1-2024 build (signal.h requires it) |
| XSH.mq_timedsend.002 | SYSTEM | struct sigaction has no sa_sigaction member in a POSIX.1-2024 build (signal.h requires it) |
| XSH.mq_timedsend.003 | SYSTEM | struct sigaction has no sa_sigaction member in a POSIX.1-2024 build (signal.h requires it) |
| XSH.mq_timedsend.004 | SYSTEM | struct sigaction has no sa_sigaction member in a POSIX.1-2024 build (signal.h requires it) |
| XSH.mq_timedsend.005 | SYSTEM | struct sigaction has no sa_sigaction member in a POSIX.1-2024 build (signal.h requires it) |
| XSH.mq_timedsend.006 | SYSTEM | struct sigaction has no sa_sigaction member in a POSIX.1-2024 build (signal.h requires it) |
| XSH.mq_timedsend.007 | SYSTEM | struct sigaction has no sa_sigaction member in a POSIX.1-2024 build (signal.h requires it) |
| XSH.mq_timedsend.008 | SYSTEM | struct sigaction has no sa_sigaction member in a POSIX.1-2024 build (signal.h requires it) |
| XSH.mq_timedsend.009 | SYSTEM | struct sigaction has no sa_sigaction member in a POSIX.1-2024 build (signal.h requires it) |
| XSH.mq_timedsend.010 | SYSTEM | struct sigaction has no sa_sigaction member in a POSIX.1-2024 build (signal.h requires it) |
| XSH.mq_unlink.001 | SYSTEM | struct sigaction has no sa_sigaction member in a POSIX.1-2024 build (signal.h requires it) |
| XSH.mq_unlink.002 | SYSTEM | struct sigaction has no sa_sigaction member in a POSIX.1-2024 build (signal.h requires it) |
| XSH.mq_unlink.003 | SYSTEM | struct sigaction has no sa_sigaction member in a POSIX.1-2024 build (signal.h requires it) |
| XSH.mq_unlink.004 | SYSTEM | struct sigaction has no sa_sigaction member in a POSIX.1-2024 build (signal.h requires it) |
| XSH.mq_unlink.005 | SYSTEM | struct sigaction has no sa_sigaction member in a POSIX.1-2024 build (signal.h requires it) |
| XSH.mtx_init.001 | SYSTEM | threads.h functions not found when linking with -l pthread, the library c17 names for them |
| XSH.mtx_init.002 | SYSTEM | threads.h functions not found when linking with -l pthread, the library c17 names for them |
| XSH.mtx_init.003 | SYSTEM | threads.h functions not found when linking with -l pthread, the library c17 names for them |
| XSH.mtx_init.004 | SYSTEM | threads.h functions not found when linking with -l pthread, the library c17 names for them |
| XSH.mtx_lock.001 | SYSTEM | threads.h functions not found when linking with -l pthread, the library c17 names for them |
| XSH.mtx_lock.002 | SYSTEM | threads.h functions not found when linking with -l pthread, the library c17 names for them |
| XSH.mtx_lock.003 | SYSTEM | threads.h functions not found when linking with -l pthread, the library c17 names for them |
| XSH.mtx_lock.004 | SYSTEM | threads.h functions not found when linking with -l pthread, the library c17 names for them |
| XSH.mtx_lock.005 | SYSTEM | threads.h functions not found when linking with -l pthread, the library c17 names for them |
| XSH.mtx_lock.006 | SYSTEM | threads.h functions not found when linking with -l pthread, the library c17 names for them |
| XSH.mtx_lock.007 | SYSTEM | threads.h functions not found when linking with -l pthread, the library c17 names for them |
| XSH.mtx_lock.008 | SYSTEM | threads.h functions not found when linking with -l pthread, the library c17 names for them |
| XSH.mtx_lock.009 | SYSTEM | threads.h functions not found when linking with -l pthread, the library c17 names for them |
| XSH.mtx_lock.010 | SYSTEM | threads.h functions not found when linking with -l pthread, the library c17 names for them |
| XSH.mtx_lock.011 | SYSTEM | threads.h functions not found when linking with -l pthread, the library c17 names for them |
| XSH.mtx_lock.012 | SYSTEM | threads.h functions not found when linking with -l pthread, the library c17 names for them |
| XSH.munmap.001 | SYSTEM | MAP_ANONYMOUS is required (unshaded or CX) by the page that defines it and is not provided in a POSIX.1-2024 build |
| XSH.munmap.002 | SYSTEM | MAP_ANONYMOUS is required (unshaded or CX) by the page that defines it and is not provided in a POSIX.1-2024 build |
| XSH.munmap.003 | SYSTEM | MAP_ANONYMOUS is required (unshaded or CX) by the page that defines it and is not provided in a POSIX.1-2024 build |
| XSH.munmap.004 | SYSTEM | MAP_ANONYMOUS is required (unshaded or CX) by the page that defines it and is not provided in a POSIX.1-2024 build |
| XSH.munmap.005 | SYSTEM | MAP_ANONYMOUS is required (unshaded or CX) by the page that defines it and is not provided in a POSIX.1-2024 build |
| XSH.munmap.006 | SYSTEM | MAP_ANONYMOUS is required (unshaded or CX) by the page that defines it and is not provided in a POSIX.1-2024 build |
| XSH.nl_langinfo.006 | SYSTEM | ABALTMON_1 is required (unshaded or CX) by the page that defines it and is not provided in a POSIX.1-2024 build |
| XSH.nl_langinfo.007 | SYSTEM | ABALTMON_1 is required (unshaded or CX) by the page that defines it and is not provided in a POSIX.1-2024 build |
| XSH.pipe.007 | SYSTEM | pipe2 is required (unshaded or CX) by the page that defines it and is not provided in a POSIX.1-2024 build |
| XSH.pipe.008 | SYSTEM | pipe2 is required (unshaded or CX) by the page that defines it and is not provided in a POSIX.1-2024 build |
| XSH.pipe.009 | SYSTEM | pipe2 is required (unshaded or CX) by the page that defines it and is not provided in a POSIX.1-2024 build |
| XSH.pipe.010 | SYSTEM | pipe2 is required (unshaded or CX) by the page that defines it and is not provided in a POSIX.1-2024 build |
| XSH.posix_fallocate.010 | TEST-BUG | SIGXFSZ is XSI-shaded in signal.h; its uses are now guarded, and where it is not named the child blocks every signal |
| XSH.posix_getdents.001 | SYSTEM | struct posix_dent is required (unshaded or CX) by the page that defines it and is not provided in a POSIX.1-2024 build |
| XSH.posix_getdents.002 | SYSTEM | struct posix_dent is required (unshaded or CX) by the page that defines it and is not provided in a POSIX.1-2024 build |
| XSH.posix_getdents.003 | SYSTEM | struct posix_dent is required (unshaded or CX) by the page that defines it and is not provided in a POSIX.1-2024 build |
| XSH.posix_getdents.004 | SYSTEM | struct posix_dent is required (unshaded or CX) by the page that defines it and is not provided in a POSIX.1-2024 build |
| XSH.posix_getdents.005 | SYSTEM | struct posix_dent is required (unshaded or CX) by the page that defines it and is not provided in a POSIX.1-2024 build |
| XSH.posix_getdents.006 | SYSTEM | struct posix_dent is required (unshaded or CX) by the page that defines it and is not provided in a POSIX.1-2024 build |
| XSH.posix_getdents.007 | SYSTEM | struct posix_dent is required (unshaded or CX) by the page that defines it and is not provided in a POSIX.1-2024 build |
| XSH.posix_madvise.001 | SYSTEM | MAP_ANONYMOUS is required (unshaded or CX) by the page that defines it and is not provided in a POSIX.1-2024 build |
| XSH.posix_madvise.002 | SYSTEM | MAP_ANONYMOUS is required (unshaded or CX) by the page that defines it and is not provided in a POSIX.1-2024 build |
| XSH.posix_madvise.003 | SYSTEM | MAP_ANONYMOUS is required (unshaded or CX) by the page that defines it and is not provided in a POSIX.1-2024 build |
| XSH.posix_madvise.004 | SYSTEM | MAP_ANONYMOUS is required (unshaded or CX) by the page that defines it and is not provided in a POSIX.1-2024 build |
| XSH.pread.010 | TEST-BUG | SIGXFSZ is XSI-shaded in signal.h; its uses are now guarded, and where it is not named the child blocks every signal |
| XSH.psiginfo.006 | SYSTEM | struct sigaction has no sa_sigaction member in a POSIX.1-2024 build (signal.h requires it) |
| XSH.psiginfo.007 | SYSTEM | struct sigaction has no sa_sigaction member in a POSIX.1-2024 build (signal.h requires it) |
| XSH.pthread_cond_wait.012 | SYSTEM | MAP_ANONYMOUS is required (unshaded or CX) by the page that defines it and is not provided in a POSIX.1-2024 build |
| XSH.pthread_cond_wait.013 | SYSTEM | MAP_ANONYMOUS is required (unshaded or CX) by the page that defines it and is not provided in a POSIX.1-2024 build |
| XSH.pthread_cond_wait.014 | SYSTEM | pthread_cond_clockwait is required (unshaded or CX) by the page that defines it and is not provided in a POSIX.1-2024 build |
| XSH.pthread_cond_wait.015 | SYSTEM | pthread_cond_clockwait is required (unshaded or CX) by the page that defines it and is not provided in a POSIX.1-2024 build |
| XSH.pthread_cond_wait.016 | SYSTEM | pthread_cond_clockwait is required (unshaded or CX) by the page that defines it and is not provided in a POSIX.1-2024 build |
| XSH.pthread_condattr_getclock.007 | SYSTEM | pthread_cond_clockwait is required (unshaded or CX) by the page that defines it and is not provided in a POSIX.1-2024 build |
| XSH.pthread_equal.003 | SYSTEM | PTHREAD_NULL is required (unshaded or CX) by the page that defines it and is not provided in a POSIX.1-2024 build |
| XSH.pthread_mutex_clocklock.001 | SYSTEM | pthread_mutex_clocklock is required (unshaded or CX) by the page that defines it and is not provided in a POSIX.1-2024 build |
| XSH.pthread_mutex_clocklock.002 | SYSTEM | pthread_mutex_clocklock is required (unshaded or CX) by the page that defines it and is not provided in a POSIX.1-2024 build |
| XSH.pthread_mutex_clocklock.003 | SYSTEM | pthread_mutex_clocklock is required (unshaded or CX) by the page that defines it and is not provided in a POSIX.1-2024 build |
| XSH.pthread_mutex_clocklock.004 | SYSTEM | pthread_mutex_clocklock is required (unshaded or CX) by the page that defines it and is not provided in a POSIX.1-2024 build |
| XSH.pthread_mutex_clocklock.005 | SYSTEM | pthread_mutex_clocklock is required (unshaded or CX) by the page that defines it and is not provided in a POSIX.1-2024 build |
| XSH.pthread_mutex_clocklock.006 | SYSTEM | pthread_mutex_clocklock is required (unshaded or CX) by the page that defines it and is not provided in a POSIX.1-2024 build |
| XSH.pthread_rwlock_clockrdlock.001 | SYSTEM | pthread_rwlock_clockrdlock is required (unshaded or CX) by the page that defines it and is not provided in a POSIX.1-2024 build |
| XSH.pthread_rwlock_clockrdlock.002 | SYSTEM | pthread_rwlock_clockrdlock is required (unshaded or CX) by the page that defines it and is not provided in a POSIX.1-2024 build |
| XSH.pthread_rwlock_clockrdlock.003 | SYSTEM | pthread_rwlock_clockrdlock is required (unshaded or CX) by the page that defines it and is not provided in a POSIX.1-2024 build |
| XSH.pthread_rwlock_clockrdlock.004 | SYSTEM | pthread_rwlock_clockrdlock is required (unshaded or CX) by the page that defines it and is not provided in a POSIX.1-2024 build |
| XSH.pthread_rwlock_clockrdlock.005 | SYSTEM | pthread_rwlock_clockrdlock is required (unshaded or CX) by the page that defines it and is not provided in a POSIX.1-2024 build |
| XSH.pthread_rwlock_clockwrlock.001 | SYSTEM | pthread_rwlock_clockwrlock is required (unshaded or CX) by the page that defines it and is not provided in a POSIX.1-2024 build |
| XSH.pthread_rwlock_clockwrlock.002 | SYSTEM | pthread_rwlock_clockwrlock is required (unshaded or CX) by the page that defines it and is not provided in a POSIX.1-2024 build |
| XSH.pthread_rwlock_clockwrlock.003 | SYSTEM | pthread_rwlock_clockwrlock is required (unshaded or CX) by the page that defines it and is not provided in a POSIX.1-2024 build |
| XSH.pthread_rwlock_clockwrlock.004 | SYSTEM | pthread_rwlock_clockwrlock is required (unshaded or CX) by the page that defines it and is not provided in a POSIX.1-2024 build |
| XSH.pthread_setschedprio.001 | SYSTEM | the system claims TPS but does not declare pthread_setschedprio() |
| XSH.pthread_setschedprio.002 | SYSTEM | the system claims TPS but does not declare pthread_setschedprio() |
| XSH.pthread_setschedprio.003 | SYSTEM | the system claims TPS but does not declare pthread_setschedprio() |
| XSH.pwrite.011 | TEST-BUG | SIGXFSZ is XSI-shaded in signal.h; its uses are now guarded, and where it is not named the child blocks every signal |
| XSH.pwrite.012 | TEST-BUG | SIGXFSZ is XSI-shaded in signal.h; its uses are now guarded, and where it is not named the child blocks every signal |
| XSH.pwrite.013 | TEST-BUG | SIGXFSZ is XSI-shaded in signal.h; its uses are now guarded, and where it is not named the child blocks every signal |
| XSH.qsort.005 | SYSTEM | qsort_r is required (unshaded or CX) by the page that defines it and is not provided in a POSIX.1-2024 build |
| XSH.qsort.006 | SYSTEM | qsort_r is required (unshaded or CX) by the page that defines it and is not provided in a POSIX.1-2024 build |
| XSH.raise.003 | TEST-BUG | sysconf(_SC_NSIG) gave a number that is a supported signal here, so the signal was really sent and ended the test; setup now checks the number is not a signal (UNRESOLVED if it is; the fault is XSH.sysconf.015) |
| XSH.read.022 | TEST-BUG | INADDR_LOOPBACK is not defined by POSIX (netinet/in.h names it only as an extension); tests now use getaddrinfo() for the loopback address |
| XSH.readdir.002 | SYSTEM | d_ino is required (unshaded or CX) by the page that defines it and is not provided in a POSIX.1-2024 build |
| XSH.realloc.005 | SYSTEM | reallocarray is required (unshaded or CX) by the page that defines it and is not provided in a POSIX.1-2024 build |
| XSH.realloc.006 | SYSTEM | reallocarray is required (unshaded or CX) by the page that defines it and is not provided in a POSIX.1-2024 build |
| XSH.realloc.007 | SYSTEM | reallocarray is required (unshaded or CX) by the page that defines it and is not provided in a POSIX.1-2024 build |
| XSH.recv.001 | TEST-BUG | INADDR_LOOPBACK is not defined by POSIX (netinet/in.h names it only as an extension); tests now use getaddrinfo() for the loopback address |
| XSH.recv.002 | TEST-BUG | INADDR_LOOPBACK is not defined by POSIX (netinet/in.h names it only as an extension); tests now use getaddrinfo() for the loopback address |
| XSH.recv.003 | TEST-BUG | INADDR_LOOPBACK is not defined by POSIX (netinet/in.h names it only as an extension); tests now use getaddrinfo() for the loopback address |
| XSH.recv.004 | TEST-BUG | INADDR_LOOPBACK is not defined by POSIX (netinet/in.h names it only as an extension); tests now use getaddrinfo() for the loopback address |
| XSH.recv.005 | TEST-BUG | INADDR_LOOPBACK is not defined by POSIX (netinet/in.h names it only as an extension); tests now use getaddrinfo() for the loopback address |
| XSH.recv.006 | TEST-BUG | INADDR_LOOPBACK is not defined by POSIX (netinet/in.h names it only as an extension); tests now use getaddrinfo() for the loopback address |
| XSH.recv.007 | TEST-BUG | INADDR_LOOPBACK is not defined by POSIX (netinet/in.h names it only as an extension); tests now use getaddrinfo() for the loopback address |
| XSH.recv.008 | TEST-BUG | INADDR_LOOPBACK is not defined by POSIX (netinet/in.h names it only as an extension); tests now use getaddrinfo() for the loopback address |
| XSH.recv.009 | TEST-BUG | INADDR_LOOPBACK is not defined by POSIX (netinet/in.h names it only as an extension); tests now use getaddrinfo() for the loopback address |
| XSH.recv.010 | TEST-BUG | INADDR_LOOPBACK is not defined by POSIX (netinet/in.h names it only as an extension); tests now use getaddrinfo() for the loopback address |
| XSH.recv.011 | TEST-BUG | INADDR_LOOPBACK is not defined by POSIX (netinet/in.h names it only as an extension); tests now use getaddrinfo() for the loopback address |
| XSH.recv.012 | TEST-BUG | INADDR_LOOPBACK is not defined by POSIX (netinet/in.h names it only as an extension); tests now use getaddrinfo() for the loopback address |
| XSH.recv.013 | TEST-BUG | INADDR_LOOPBACK is not defined by POSIX (netinet/in.h names it only as an extension); tests now use getaddrinfo() for the loopback address |
| XSH.recv.014 | TEST-BUG | INADDR_LOOPBACK is not defined by POSIX (netinet/in.h names it only as an extension); tests now use getaddrinfo() for the loopback address |
| XSH.recv.015 | TEST-BUG | INADDR_LOOPBACK is not defined by POSIX (netinet/in.h names it only as an extension); tests now use getaddrinfo() for the loopback address |
| XSH.recv.016 | TEST-BUG | INADDR_LOOPBACK is not defined by POSIX (netinet/in.h names it only as an extension); tests now use getaddrinfo() for the loopback address |
| XSH.recv.017 | TEST-BUG | INADDR_LOOPBACK is not defined by POSIX (netinet/in.h names it only as an extension); tests now use getaddrinfo() for the loopback address |
| XSH.recv.018 | TEST-BUG | INADDR_LOOPBACK is not defined by POSIX (netinet/in.h names it only as an extension); tests now use getaddrinfo() for the loopback address |
| XSH.recvfrom.001 | TEST-BUG | INADDR_LOOPBACK is not defined by POSIX (netinet/in.h names it only as an extension); tests now use getaddrinfo() for the loopback address |
| XSH.recvfrom.002 | TEST-BUG | INADDR_LOOPBACK is not defined by POSIX (netinet/in.h names it only as an extension); tests now use getaddrinfo() for the loopback address |
| XSH.recvfrom.003 | TEST-BUG | INADDR_LOOPBACK is not defined by POSIX (netinet/in.h names it only as an extension); tests now use getaddrinfo() for the loopback address |
| XSH.recvfrom.004 | TEST-BUG | INADDR_LOOPBACK is not defined by POSIX (netinet/in.h names it only as an extension); tests now use getaddrinfo() for the loopback address |
| XSH.recvfrom.005 | TEST-BUG | INADDR_LOOPBACK is not defined by POSIX (netinet/in.h names it only as an extension); tests now use getaddrinfo() for the loopback address |
| XSH.recvfrom.006 | TEST-BUG | INADDR_LOOPBACK is not defined by POSIX (netinet/in.h names it only as an extension); tests now use getaddrinfo() for the loopback address |
| XSH.recvfrom.007 | TEST-BUG | INADDR_LOOPBACK is not defined by POSIX (netinet/in.h names it only as an extension); tests now use getaddrinfo() for the loopback address |
| XSH.recvfrom.008 | TEST-BUG | INADDR_LOOPBACK is not defined by POSIX (netinet/in.h names it only as an extension); tests now use getaddrinfo() for the loopback address |
| XSH.recvfrom.009 | TEST-BUG | INADDR_LOOPBACK is not defined by POSIX (netinet/in.h names it only as an extension); tests now use getaddrinfo() for the loopback address |
| XSH.recvfrom.010 | TEST-BUG | INADDR_LOOPBACK is not defined by POSIX (netinet/in.h names it only as an extension); tests now use getaddrinfo() for the loopback address |
| XSH.recvfrom.011 | TEST-BUG | INADDR_LOOPBACK is not defined by POSIX (netinet/in.h names it only as an extension); tests now use getaddrinfo() for the loopback address |
| XSH.recvfrom.012 | TEST-BUG | INADDR_LOOPBACK is not defined by POSIX (netinet/in.h names it only as an extension); tests now use getaddrinfo() for the loopback address |
| XSH.recvfrom.013 | TEST-BUG | INADDR_LOOPBACK is not defined by POSIX (netinet/in.h names it only as an extension); tests now use getaddrinfo() for the loopback address |
| XSH.recvfrom.014 | TEST-BUG | INADDR_LOOPBACK is not defined by POSIX (netinet/in.h names it only as an extension); tests now use getaddrinfo() for the loopback address |
| XSH.recvfrom.015 | TEST-BUG | INADDR_LOOPBACK is not defined by POSIX (netinet/in.h names it only as an extension); tests now use getaddrinfo() for the loopback address |
| XSH.recvfrom.016 | TEST-BUG | INADDR_LOOPBACK is not defined by POSIX (netinet/in.h names it only as an extension); tests now use getaddrinfo() for the loopback address |
| XSH.recvmsg.001 | TEST-BUG | INADDR_LOOPBACK is not defined by POSIX (netinet/in.h names it only as an extension); tests now use getaddrinfo() for the loopback address |
| XSH.recvmsg.002 | TEST-BUG | INADDR_LOOPBACK is not defined by POSIX (netinet/in.h names it only as an extension); tests now use getaddrinfo() for the loopback address |
| XSH.recvmsg.003 | TEST-BUG | INADDR_LOOPBACK is not defined by POSIX (netinet/in.h names it only as an extension); tests now use getaddrinfo() for the loopback address |
| XSH.recvmsg.004 | TEST-BUG | INADDR_LOOPBACK is not defined by POSIX (netinet/in.h names it only as an extension); tests now use getaddrinfo() for the loopback address |
| XSH.recvmsg.005 | TEST-BUG | INADDR_LOOPBACK is not defined by POSIX (netinet/in.h names it only as an extension); tests now use getaddrinfo() for the loopback address |
| XSH.recvmsg.006 | TEST-BUG | INADDR_LOOPBACK is not defined by POSIX (netinet/in.h names it only as an extension); tests now use getaddrinfo() for the loopback address |
| XSH.recvmsg.007 | TEST-BUG | INADDR_LOOPBACK is not defined by POSIX (netinet/in.h names it only as an extension); tests now use getaddrinfo() for the loopback address |
| XSH.recvmsg.008 | TEST-BUG | INADDR_LOOPBACK is not defined by POSIX (netinet/in.h names it only as an extension); tests now use getaddrinfo() for the loopback address |
| XSH.recvmsg.009 | TEST-BUG | INADDR_LOOPBACK is not defined by POSIX (netinet/in.h names it only as an extension); tests now use getaddrinfo() for the loopback address |
| XSH.recvmsg.010 | TEST-BUG | INADDR_LOOPBACK is not defined by POSIX (netinet/in.h names it only as an extension); tests now use getaddrinfo() for the loopback address |
| XSH.recvmsg.011 | TEST-BUG | INADDR_LOOPBACK is not defined by POSIX (netinet/in.h names it only as an extension); tests now use getaddrinfo() for the loopback address |
| XSH.recvmsg.012 | TEST-BUG | INADDR_LOOPBACK is not defined by POSIX (netinet/in.h names it only as an extension); tests now use getaddrinfo() for the loopback address |
| XSH.recvmsg.013 | TEST-BUG | INADDR_LOOPBACK is not defined by POSIX (netinet/in.h names it only as an extension); tests now use getaddrinfo() for the loopback address |
| XSH.recvmsg.014 | TEST-BUG | INADDR_LOOPBACK is not defined by POSIX (netinet/in.h names it only as an extension); tests now use getaddrinfo() for the loopback address |
| XSH.recvmsg.015 | TEST-BUG | INADDR_LOOPBACK is not defined by POSIX (netinet/in.h names it only as an extension); tests now use getaddrinfo() for the loopback address |
| XSH.recvmsg.016 | TEST-BUG | INADDR_LOOPBACK is not defined by POSIX (netinet/in.h names it only as an extension); tests now use getaddrinfo() for the loopback address |
| XSH.recvmsg.017 | TEST-BUG | INADDR_LOOPBACK is not defined by POSIX (netinet/in.h names it only as an extension); tests now use getaddrinfo() for the loopback address |
| XSH.recvmsg.018 | TEST-BUG | INADDR_LOOPBACK is not defined by POSIX (netinet/in.h names it only as an extension); tests now use getaddrinfo() for the loopback address |
| XSH.recvmsg.019 | TEST-BUG | INADDR_LOOPBACK is not defined by POSIX (netinet/in.h names it only as an extension); tests now use getaddrinfo() for the loopback address |
| XSH.recvmsg.020 | TEST-BUG | INADDR_LOOPBACK is not defined by POSIX (netinet/in.h names it only as an extension); tests now use getaddrinfo() for the loopback address |
| XSH.recvmsg.021 | TEST-BUG | INADDR_LOOPBACK is not defined by POSIX (netinet/in.h names it only as an extension); tests now use getaddrinfo() for the loopback address |
| XSH.recvmsg.022 | TEST-BUG | INADDR_LOOPBACK is not defined by POSIX (netinet/in.h names it only as an extension); tests now use getaddrinfo() for the loopback address |
| XSH.recvmsg.023 | TEST-BUG | INADDR_LOOPBACK is not defined by POSIX (netinet/in.h names it only as an extension); tests now use getaddrinfo() for the loopback address |
| XSH.recvmsg.024 | TEST-BUG | INADDR_LOOPBACK is not defined by POSIX (netinet/in.h names it only as an extension); tests now use getaddrinfo() for the loopback address |
| XSH.recvmsg.025 | TEST-BUG | INADDR_LOOPBACK is not defined by POSIX (netinet/in.h names it only as an extension); tests now use getaddrinfo() for the loopback address |
| XSH.recvmsg.026 | TEST-BUG | INADDR_LOOPBACK is not defined by POSIX (netinet/in.h names it only as an extension); tests now use getaddrinfo() for the loopback address |
| XSH.recvmsg.027 | TEST-BUG | INADDR_LOOPBACK is not defined by POSIX (netinet/in.h names it only as an extension); tests now use getaddrinfo() for the loopback address |
| XSH.recvmsg.028 | TEST-BUG | INADDR_LOOPBACK is not defined by POSIX (netinet/in.h names it only as an extension); tests now use getaddrinfo() for the loopback address |
| XSH.recvmsg.029 | TEST-BUG | INADDR_LOOPBACK is not defined by POSIX (netinet/in.h names it only as an extension); tests now use getaddrinfo() for the loopback address |
| XSH.regcomp.034 | SYSTEM | REG_MINIMAL is required (unshaded or CX) by the page that defines it and is not provided in a POSIX.1-2024 build |
| XSH.rewind.008 | TEST-BUG | SIGXFSZ is XSI-shaded in signal.h; its uses are now guarded, and where it is not named the child blocks every signal |
| XSH.sem_clockwait.001 | SYSTEM | sem_clockwait is required (unshaded or CX) by the page that defines it and is not provided in a POSIX.1-2024 build |
| XSH.sem_clockwait.002 | SYSTEM | sem_clockwait is required (unshaded or CX) by the page that defines it and is not provided in a POSIX.1-2024 build |
| XSH.sem_clockwait.003 | SYSTEM | sem_clockwait is required (unshaded or CX) by the page that defines it and is not provided in a POSIX.1-2024 build |
| XSH.sem_clockwait.004 | SYSTEM | sem_clockwait is required (unshaded or CX) by the page that defines it and is not provided in a POSIX.1-2024 build |
| XSH.sem_clockwait.005 | SYSTEM | sem_clockwait is required (unshaded or CX) by the page that defines it and is not provided in a POSIX.1-2024 build |
| XSH.sem_clockwait.006 | SYSTEM | sem_clockwait is required (unshaded or CX) by the page that defines it and is not provided in a POSIX.1-2024 build |
| XSH.sem_clockwait.007 | SYSTEM | sem_clockwait is required (unshaded or CX) by the page that defines it and is not provided in a POSIX.1-2024 build |
| XSH.sem_clockwait.008 | SYSTEM | sem_clockwait is required (unshaded or CX) by the page that defines it and is not provided in a POSIX.1-2024 build |
| XSH.sem_close.001 | SYSTEM | sem_clockwait is required (unshaded or CX) by the page that defines it and is not provided in a POSIX.1-2024 build |
| XSH.sem_close.002 | SYSTEM | sem_clockwait is required (unshaded or CX) by the page that defines it and is not provided in a POSIX.1-2024 build |
| XSH.sem_close.003 | SYSTEM | sem_clockwait is required (unshaded or CX) by the page that defines it and is not provided in a POSIX.1-2024 build |
| XSH.sem_close.004 | SYSTEM | sem_clockwait is required (unshaded or CX) by the page that defines it and is not provided in a POSIX.1-2024 build |
| XSH.sem_close.005 | SYSTEM | sem_clockwait is required (unshaded or CX) by the page that defines it and is not provided in a POSIX.1-2024 build |
| XSH.sem_destroy.001 | SYSTEM | sem_clockwait is required (unshaded or CX) by the page that defines it and is not provided in a POSIX.1-2024 build |
| XSH.sem_destroy.002 | SYSTEM | sem_clockwait is required (unshaded or CX) by the page that defines it and is not provided in a POSIX.1-2024 build |
| XSH.sem_destroy.003 | SYSTEM | sem_clockwait is required (unshaded or CX) by the page that defines it and is not provided in a POSIX.1-2024 build |
| XSH.sem_destroy.004 | SYSTEM | sem_clockwait is required (unshaded or CX) by the page that defines it and is not provided in a POSIX.1-2024 build |
| XSH.sem_getvalue.001 | SYSTEM | sem_clockwait is required (unshaded or CX) by the page that defines it and is not provided in a POSIX.1-2024 build |
| XSH.sem_getvalue.002 | SYSTEM | sem_clockwait is required (unshaded or CX) by the page that defines it and is not provided in a POSIX.1-2024 build |
| XSH.sem_getvalue.003 | SYSTEM | sem_clockwait is required (unshaded or CX) by the page that defines it and is not provided in a POSIX.1-2024 build |
| XSH.sem_getvalue.004 | SYSTEM | sem_clockwait is required (unshaded or CX) by the page that defines it and is not provided in a POSIX.1-2024 build |
| XSH.sem_init.001 | SYSTEM | sem_clockwait is required (unshaded or CX) by the page that defines it and is not provided in a POSIX.1-2024 build |
| XSH.sem_init.002 | SYSTEM | sem_clockwait is required (unshaded or CX) by the page that defines it and is not provided in a POSIX.1-2024 build |
| XSH.sem_init.003 | SYSTEM | sem_clockwait is required (unshaded or CX) by the page that defines it and is not provided in a POSIX.1-2024 build |
| XSH.sem_init.004 | SYSTEM | sem_clockwait is required (unshaded or CX) by the page that defines it and is not provided in a POSIX.1-2024 build |
| XSH.sem_init.005 | SYSTEM | sem_clockwait is required (unshaded or CX) by the page that defines it and is not provided in a POSIX.1-2024 build |
| XSH.sem_init.006 | SYSTEM | sem_clockwait is required (unshaded or CX) by the page that defines it and is not provided in a POSIX.1-2024 build |
| XSH.sem_open.001 | SYSTEM | sem_clockwait is required (unshaded or CX) by the page that defines it and is not provided in a POSIX.1-2024 build |
| XSH.sem_open.002 | SYSTEM | sem_clockwait is required (unshaded or CX) by the page that defines it and is not provided in a POSIX.1-2024 build |
| XSH.sem_open.003 | SYSTEM | sem_clockwait is required (unshaded or CX) by the page that defines it and is not provided in a POSIX.1-2024 build |
| XSH.sem_open.004 | SYSTEM | sem_clockwait is required (unshaded or CX) by the page that defines it and is not provided in a POSIX.1-2024 build |
| XSH.sem_open.005 | SYSTEM | sem_clockwait is required (unshaded or CX) by the page that defines it and is not provided in a POSIX.1-2024 build |
| XSH.sem_open.006 | SYSTEM | sem_clockwait is required (unshaded or CX) by the page that defines it and is not provided in a POSIX.1-2024 build |
| XSH.sem_open.007 | SYSTEM | sem_clockwait is required (unshaded or CX) by the page that defines it and is not provided in a POSIX.1-2024 build |
| XSH.sem_open.008 | SYSTEM | sem_clockwait is required (unshaded or CX) by the page that defines it and is not provided in a POSIX.1-2024 build |
| XSH.sem_open.009 | SYSTEM | sem_clockwait is required (unshaded or CX) by the page that defines it and is not provided in a POSIX.1-2024 build |
| XSH.sem_open.010 | SYSTEM | sem_clockwait is required (unshaded or CX) by the page that defines it and is not provided in a POSIX.1-2024 build |
| XSH.sem_post.001 | SYSTEM | sem_clockwait is required (unshaded or CX) by the page that defines it and is not provided in a POSIX.1-2024 build |
| XSH.sem_post.002 | SYSTEM | sem_clockwait is required (unshaded or CX) by the page that defines it and is not provided in a POSIX.1-2024 build |
| XSH.sem_post.003 | SYSTEM | sem_clockwait is required (unshaded or CX) by the page that defines it and is not provided in a POSIX.1-2024 build |
| XSH.sem_post.004 | SYSTEM | sem_clockwait is required (unshaded or CX) by the page that defines it and is not provided in a POSIX.1-2024 build |
| XSH.sem_post.005 | SYSTEM | sem_clockwait is required (unshaded or CX) by the page that defines it and is not provided in a POSIX.1-2024 build |
| XSH.sem_unlink.001 | SYSTEM | sem_clockwait is required (unshaded or CX) by the page that defines it and is not provided in a POSIX.1-2024 build |
| XSH.sem_unlink.002 | SYSTEM | sem_clockwait is required (unshaded or CX) by the page that defines it and is not provided in a POSIX.1-2024 build |
| XSH.sem_unlink.003 | SYSTEM | sem_clockwait is required (unshaded or CX) by the page that defines it and is not provided in a POSIX.1-2024 build |
| XSH.sem_unlink.004 | SYSTEM | sem_clockwait is required (unshaded or CX) by the page that defines it and is not provided in a POSIX.1-2024 build |
| XSH.sem_unlink.005 | SYSTEM | sem_clockwait is required (unshaded or CX) by the page that defines it and is not provided in a POSIX.1-2024 build |
| XSH.sem_unlink.006 | SYSTEM | sem_clockwait is required (unshaded or CX) by the page that defines it and is not provided in a POSIX.1-2024 build |
| XSH.sem_unlink.007 | SYSTEM | sem_clockwait is required (unshaded or CX) by the page that defines it and is not provided in a POSIX.1-2024 build |
| XSH.sem_wait.001 | SYSTEM | sem_clockwait is required (unshaded or CX) by the page that defines it and is not provided in a POSIX.1-2024 build |
| XSH.sem_wait.002 | SYSTEM | sem_clockwait is required (unshaded or CX) by the page that defines it and is not provided in a POSIX.1-2024 build |
| XSH.sem_wait.003 | SYSTEM | sem_clockwait is required (unshaded or CX) by the page that defines it and is not provided in a POSIX.1-2024 build |
| XSH.sem_wait.004 | SYSTEM | sem_clockwait is required (unshaded or CX) by the page that defines it and is not provided in a POSIX.1-2024 build |
| XSH.sem_wait.005 | SYSTEM | sem_clockwait is required (unshaded or CX) by the page that defines it and is not provided in a POSIX.1-2024 build |
| XSH.sem_wait.006 | SYSTEM | sem_clockwait is required (unshaded or CX) by the page that defines it and is not provided in a POSIX.1-2024 build |
| XSH.send.001 | TEST-BUG | INADDR_LOOPBACK is not defined by POSIX (netinet/in.h names it only as an extension); tests now use getaddrinfo() for the loopback address |
| XSH.send.002 | TEST-BUG | INADDR_LOOPBACK is not defined by POSIX (netinet/in.h names it only as an extension); tests now use getaddrinfo() for the loopback address |
| XSH.send.003 | TEST-BUG | INADDR_LOOPBACK is not defined by POSIX (netinet/in.h names it only as an extension); tests now use getaddrinfo() for the loopback address |
| XSH.send.004 | TEST-BUG | INADDR_LOOPBACK is not defined by POSIX (netinet/in.h names it only as an extension); tests now use getaddrinfo() for the loopback address |
| XSH.send.005 | TEST-BUG | INADDR_LOOPBACK is not defined by POSIX (netinet/in.h names it only as an extension); tests now use getaddrinfo() for the loopback address |
| XSH.send.006 | TEST-BUG | INADDR_LOOPBACK is not defined by POSIX (netinet/in.h names it only as an extension); tests now use getaddrinfo() for the loopback address |
| XSH.send.007 | TEST-BUG | INADDR_LOOPBACK is not defined by POSIX (netinet/in.h names it only as an extension); tests now use getaddrinfo() for the loopback address |
| XSH.send.008 | TEST-BUG | INADDR_LOOPBACK is not defined by POSIX (netinet/in.h names it only as an extension); tests now use getaddrinfo() for the loopback address |
| XSH.send.009 | TEST-BUG | INADDR_LOOPBACK is not defined by POSIX (netinet/in.h names it only as an extension); tests now use getaddrinfo() for the loopback address |
| XSH.send.010 | TEST-BUG | INADDR_LOOPBACK is not defined by POSIX (netinet/in.h names it only as an extension); tests now use getaddrinfo() for the loopback address |
| XSH.send.011 | TEST-BUG | INADDR_LOOPBACK is not defined by POSIX (netinet/in.h names it only as an extension); tests now use getaddrinfo() for the loopback address |
| XSH.send.012 | TEST-BUG | INADDR_LOOPBACK is not defined by POSIX (netinet/in.h names it only as an extension); tests now use getaddrinfo() for the loopback address |
| XSH.send.013 | TEST-BUG | INADDR_LOOPBACK is not defined by POSIX (netinet/in.h names it only as an extension); tests now use getaddrinfo() for the loopback address |
| XSH.sendmsg.001 | TEST-BUG | INADDR_LOOPBACK is not defined by POSIX (netinet/in.h names it only as an extension); tests now use getaddrinfo() for the loopback address |
| XSH.sendmsg.002 | TEST-BUG | INADDR_LOOPBACK is not defined by POSIX (netinet/in.h names it only as an extension); tests now use getaddrinfo() for the loopback address |
| XSH.sendmsg.003 | TEST-BUG | INADDR_LOOPBACK is not defined by POSIX (netinet/in.h names it only as an extension); tests now use getaddrinfo() for the loopback address |
| XSH.sendmsg.004 | TEST-BUG | INADDR_LOOPBACK is not defined by POSIX (netinet/in.h names it only as an extension); tests now use getaddrinfo() for the loopback address |
| XSH.sendmsg.005 | TEST-BUG | INADDR_LOOPBACK is not defined by POSIX (netinet/in.h names it only as an extension); tests now use getaddrinfo() for the loopback address |
| XSH.sendmsg.006 | TEST-BUG | INADDR_LOOPBACK is not defined by POSIX (netinet/in.h names it only as an extension); tests now use getaddrinfo() for the loopback address |
| XSH.sendmsg.007 | TEST-BUG | INADDR_LOOPBACK is not defined by POSIX (netinet/in.h names it only as an extension); tests now use getaddrinfo() for the loopback address |
| XSH.sendmsg.008 | TEST-BUG | INADDR_LOOPBACK is not defined by POSIX (netinet/in.h names it only as an extension); tests now use getaddrinfo() for the loopback address |
| XSH.sendmsg.009 | TEST-BUG | INADDR_LOOPBACK is not defined by POSIX (netinet/in.h names it only as an extension); tests now use getaddrinfo() for the loopback address |
| XSH.sendmsg.010 | TEST-BUG | INADDR_LOOPBACK is not defined by POSIX (netinet/in.h names it only as an extension); tests now use getaddrinfo() for the loopback address |
| XSH.sendmsg.011 | TEST-BUG | INADDR_LOOPBACK is not defined by POSIX (netinet/in.h names it only as an extension); tests now use getaddrinfo() for the loopback address |
| XSH.sendmsg.012 | TEST-BUG | INADDR_LOOPBACK is not defined by POSIX (netinet/in.h names it only as an extension); tests now use getaddrinfo() for the loopback address |
| XSH.sendmsg.013 | TEST-BUG | INADDR_LOOPBACK is not defined by POSIX (netinet/in.h names it only as an extension); tests now use getaddrinfo() for the loopback address |
| XSH.sendmsg.014 | TEST-BUG | INADDR_LOOPBACK is not defined by POSIX (netinet/in.h names it only as an extension); tests now use getaddrinfo() for the loopback address |
| XSH.sendmsg.015 | TEST-BUG | INADDR_LOOPBACK is not defined by POSIX (netinet/in.h names it only as an extension); tests now use getaddrinfo() for the loopback address |
| XSH.sendmsg.016 | TEST-BUG | INADDR_LOOPBACK is not defined by POSIX (netinet/in.h names it only as an extension); tests now use getaddrinfo() for the loopback address |
| XSH.sendmsg.017 | TEST-BUG | INADDR_LOOPBACK is not defined by POSIX (netinet/in.h names it only as an extension); tests now use getaddrinfo() for the loopback address |
| XSH.sendmsg.018 | TEST-BUG | INADDR_LOOPBACK is not defined by POSIX (netinet/in.h names it only as an extension); tests now use getaddrinfo() for the loopback address |
| XSH.sendmsg.019 | TEST-BUG | INADDR_LOOPBACK is not defined by POSIX (netinet/in.h names it only as an extension); tests now use getaddrinfo() for the loopback address |
| XSH.sendmsg.020 | TEST-BUG | INADDR_LOOPBACK is not defined by POSIX (netinet/in.h names it only as an extension); tests now use getaddrinfo() for the loopback address |
| XSH.sendmsg.021 | TEST-BUG | INADDR_LOOPBACK is not defined by POSIX (netinet/in.h names it only as an extension); tests now use getaddrinfo() for the loopback address |
| XSH.sendmsg.022 | TEST-BUG | INADDR_LOOPBACK is not defined by POSIX (netinet/in.h names it only as an extension); tests now use getaddrinfo() for the loopback address |
| XSH.sendmsg.023 | TEST-BUG | INADDR_LOOPBACK is not defined by POSIX (netinet/in.h names it only as an extension); tests now use getaddrinfo() for the loopback address |
| XSH.sendmsg.024 | TEST-BUG | INADDR_LOOPBACK is not defined by POSIX (netinet/in.h names it only as an extension); tests now use getaddrinfo() for the loopback address |
| XSH.sendto.001 | TEST-BUG | INADDR_LOOPBACK is not defined by POSIX (netinet/in.h names it only as an extension); tests now use getaddrinfo() for the loopback address |
| XSH.sendto.002 | TEST-BUG | INADDR_LOOPBACK is not defined by POSIX (netinet/in.h names it only as an extension); tests now use getaddrinfo() for the loopback address |
| XSH.sendto.003 | TEST-BUG | INADDR_LOOPBACK is not defined by POSIX (netinet/in.h names it only as an extension); tests now use getaddrinfo() for the loopback address |
| XSH.sendto.004 | TEST-BUG | INADDR_LOOPBACK is not defined by POSIX (netinet/in.h names it only as an extension); tests now use getaddrinfo() for the loopback address |
| XSH.sendto.005 | TEST-BUG | INADDR_LOOPBACK is not defined by POSIX (netinet/in.h names it only as an extension); tests now use getaddrinfo() for the loopback address |
| XSH.sendto.006 | TEST-BUG | INADDR_LOOPBACK is not defined by POSIX (netinet/in.h names it only as an extension); tests now use getaddrinfo() for the loopback address |
| XSH.sendto.007 | TEST-BUG | INADDR_LOOPBACK is not defined by POSIX (netinet/in.h names it only as an extension); tests now use getaddrinfo() for the loopback address |
| XSH.sendto.008 | TEST-BUG | INADDR_LOOPBACK is not defined by POSIX (netinet/in.h names it only as an extension); tests now use getaddrinfo() for the loopback address |
| XSH.sendto.009 | TEST-BUG | INADDR_LOOPBACK is not defined by POSIX (netinet/in.h names it only as an extension); tests now use getaddrinfo() for the loopback address |
| XSH.sendto.010 | TEST-BUG | INADDR_LOOPBACK is not defined by POSIX (netinet/in.h names it only as an extension); tests now use getaddrinfo() for the loopback address |
| XSH.sendto.011 | TEST-BUG | INADDR_LOOPBACK is not defined by POSIX (netinet/in.h names it only as an extension); tests now use getaddrinfo() for the loopback address |
| XSH.sendto.012 | TEST-BUG | INADDR_LOOPBACK is not defined by POSIX (netinet/in.h names it only as an extension); tests now use getaddrinfo() for the loopback address |
| XSH.sendto.013 | TEST-BUG | INADDR_LOOPBACK is not defined by POSIX (netinet/in.h names it only as an extension); tests now use getaddrinfo() for the loopback address |
| XSH.sendto.014 | TEST-BUG | INADDR_LOOPBACK is not defined by POSIX (netinet/in.h names it only as an extension); tests now use getaddrinfo() for the loopback address |
| XSH.sendto.015 | TEST-BUG | INADDR_LOOPBACK is not defined by POSIX (netinet/in.h names it only as an extension); tests now use getaddrinfo() for the loopback address |
| XSH.sendto.016 | TEST-BUG | INADDR_LOOPBACK is not defined by POSIX (netinet/in.h names it only as an extension); tests now use getaddrinfo() for the loopback address |
| XSH.sendto.017 | TEST-BUG | INADDR_LOOPBACK is not defined by POSIX (netinet/in.h names it only as an extension); tests now use getaddrinfo() for the loopback address |
| XSH.setsockopt.001 | TEST-BUG | INADDR_LOOPBACK is not defined by POSIX (netinet/in.h names it only as an extension); tests now use getaddrinfo() for the loopback address |
| XSH.setsockopt.002 | TEST-BUG | INADDR_LOOPBACK is not defined by POSIX (netinet/in.h names it only as an extension); tests now use getaddrinfo() for the loopback address |
| XSH.setsockopt.003 | TEST-BUG | INADDR_LOOPBACK is not defined by POSIX (netinet/in.h names it only as an extension); tests now use getaddrinfo() for the loopback address |
| XSH.setsockopt.004 | TEST-BUG | INADDR_LOOPBACK is not defined by POSIX (netinet/in.h names it only as an extension); tests now use getaddrinfo() for the loopback address |
| XSH.setsockopt.005 | TEST-BUG | INADDR_LOOPBACK is not defined by POSIX (netinet/in.h names it only as an extension); tests now use getaddrinfo() for the loopback address |
| XSH.setsockopt.006 | TEST-BUG | INADDR_LOOPBACK is not defined by POSIX (netinet/in.h names it only as an extension); tests now use getaddrinfo() for the loopback address |
| XSH.setsockopt.007 | TEST-BUG | INADDR_LOOPBACK is not defined by POSIX (netinet/in.h names it only as an extension); tests now use getaddrinfo() for the loopback address |
| XSH.setsockopt.008 | TEST-BUG | INADDR_LOOPBACK is not defined by POSIX (netinet/in.h names it only as an extension); tests now use getaddrinfo() for the loopback address |
| XSH.setsockopt.009 | TEST-BUG | INADDR_LOOPBACK is not defined by POSIX (netinet/in.h names it only as an extension); tests now use getaddrinfo() for the loopback address |
| XSH.setsockopt.010 | TEST-BUG | INADDR_LOOPBACK is not defined by POSIX (netinet/in.h names it only as an extension); tests now use getaddrinfo() for the loopback address |
| XSH.setsockopt.011 | TEST-BUG | INADDR_LOOPBACK is not defined by POSIX (netinet/in.h names it only as an extension); tests now use getaddrinfo() for the loopback address |
| XSH.setsockopt.012 | TEST-BUG | INADDR_LOOPBACK is not defined by POSIX (netinet/in.h names it only as an extension); tests now use getaddrinfo() for the loopback address |
| XSH.setsockopt.013 | TEST-BUG | INADDR_LOOPBACK is not defined by POSIX (netinet/in.h names it only as an extension); tests now use getaddrinfo() for the loopback address |
| XSH.setsockopt.014 | TEST-BUG | INADDR_LOOPBACK is not defined by POSIX (netinet/in.h names it only as an extension); tests now use getaddrinfo() for the loopback address |
| XSH.setsockopt.015 | TEST-BUG | INADDR_LOOPBACK is not defined by POSIX (netinet/in.h names it only as an extension); tests now use getaddrinfo() for the loopback address |
| XSH.setsockopt.016 | TEST-BUG | INADDR_LOOPBACK is not defined by POSIX (netinet/in.h names it only as an extension); tests now use getaddrinfo() for the loopback address |
| XSH.setsockopt.017 | TEST-BUG | INADDR_LOOPBACK is not defined by POSIX (netinet/in.h names it only as an extension); tests now use getaddrinfo() for the loopback address |
| XSH.setsockopt.018 | TEST-BUG | INADDR_LOOPBACK is not defined by POSIX (netinet/in.h names it only as an extension); tests now use getaddrinfo() for the loopback address |
| XSH.setsockopt.019 | TEST-BUG | INADDR_LOOPBACK is not defined by POSIX (netinet/in.h names it only as an extension); tests now use getaddrinfo() for the loopback address |
| XSH.setsockopt.020 | TEST-BUG | INADDR_LOOPBACK is not defined by POSIX (netinet/in.h names it only as an extension); tests now use getaddrinfo() for the loopback address |
| XSH.setsockopt.021 | TEST-BUG | INADDR_LOOPBACK is not defined by POSIX (netinet/in.h names it only as an extension); tests now use getaddrinfo() for the loopback address |
| XSH.setsockopt.022 | TEST-BUG | INADDR_LOOPBACK is not defined by POSIX (netinet/in.h names it only as an extension); tests now use getaddrinfo() for the loopback address |
| XSH.shutdown.006 | TEST-BUG | INADDR_LOOPBACK is not defined by POSIX (netinet/in.h names it only as an extension); tests now use getaddrinfo() for the loopback address |
| XSH.shutdown.008 | TEST-BUG | INADDR_LOOPBACK is not defined by POSIX (netinet/in.h names it only as an extension); tests now use getaddrinfo() for the loopback address |
| XSH.sigaction.001 | SYSTEM | struct sigaction has no sa_sigaction member in a POSIX.1-2024 build (signal.h requires it) |
| XSH.sigaction.002 | SYSTEM | struct sigaction has no sa_sigaction member in a POSIX.1-2024 build (signal.h requires it) |
| XSH.sigaction.003 | SYSTEM | struct sigaction has no sa_sigaction member in a POSIX.1-2024 build (signal.h requires it) |
| XSH.sigaction.004 | SYSTEM | struct sigaction has no sa_sigaction member in a POSIX.1-2024 build (signal.h requires it) |
| XSH.sigaction.005 | SYSTEM | struct sigaction has no sa_sigaction member in a POSIX.1-2024 build (signal.h requires it) |
| XSH.sigaction.006 | SYSTEM | struct sigaction has no sa_sigaction member in a POSIX.1-2024 build (signal.h requires it) |
| XSH.sigaction.007 | SYSTEM | struct sigaction has no sa_sigaction member in a POSIX.1-2024 build (signal.h requires it) |
| XSH.sigaction.008 | SYSTEM | struct sigaction has no sa_sigaction member in a POSIX.1-2024 build (signal.h requires it) |
| XSH.sigaction.010 | SYSTEM | struct sigaction has no sa_sigaction member in a POSIX.1-2024 build (signal.h requires it) |
| XSH.sigaction.011 | SYSTEM | struct sigaction has no sa_sigaction member in a POSIX.1-2024 build (signal.h requires it) |
| XSH.sigaction.012 | SYSTEM | struct sigaction has no sa_sigaction member in a POSIX.1-2024 build (signal.h requires it) |
| XSH.sigaction.013 | SYSTEM | struct sigaction has no sa_sigaction member in a POSIX.1-2024 build (signal.h requires it) |
| XSH.sigaction.014 | SYSTEM | struct sigaction has no sa_sigaction member in a POSIX.1-2024 build (signal.h requires it) |
| XSH.sigaction.015 | SYSTEM | struct sigaction has no sa_sigaction member in a POSIX.1-2024 build (signal.h requires it) |
| XSH.sigaction.016 | SYSTEM | struct sigaction has no sa_sigaction member in a POSIX.1-2024 build (signal.h requires it) |
| XSH.sigaction.017 | SYSTEM | struct sigaction has no sa_sigaction member in a POSIX.1-2024 build (signal.h requires it) |
| XSH.sigaction.018 | SYSTEM | struct sigaction has no sa_sigaction member in a POSIX.1-2024 build (signal.h requires it) |
| XSH.sigaction.019 | SYSTEM | struct sigaction has no sa_sigaction member in a POSIX.1-2024 build (signal.h requires it) |
| XSH.sigaction.020 | SYSTEM | struct sigaction has no sa_sigaction member in a POSIX.1-2024 build (signal.h requires it) |
| XSH.sigaction.021 | SYSTEM | struct sigaction has no sa_sigaction member in a POSIX.1-2024 build (signal.h requires it) |
| XSH.sigaction.022 | SYSTEM | struct sigaction has no sa_sigaction member in a POSIX.1-2024 build (signal.h requires it) |
| XSH.sigaction.023 | SYSTEM | struct sigaction has no sa_sigaction member in a POSIX.1-2024 build (signal.h requires it) |
| XSH.sockatmark.001 | TEST-BUG | INADDR_LOOPBACK is not defined by POSIX (netinet/in.h names it only as an extension); tests now use getaddrinfo() for the loopback address |
| XSH.sockatmark.002 | TEST-BUG | INADDR_LOOPBACK is not defined by POSIX (netinet/in.h names it only as an extension); tests now use getaddrinfo() for the loopback address |
| XSH.sockatmark.003 | TEST-BUG | INADDR_LOOPBACK is not defined by POSIX (netinet/in.h names it only as an extension); tests now use getaddrinfo() for the loopback address |
| XSH.sockatmark.004 | TEST-BUG | INADDR_LOOPBACK is not defined by POSIX (netinet/in.h names it only as an extension); tests now use getaddrinfo() for the loopback address |
| XSH.sockatmark.005 | TEST-BUG | INADDR_LOOPBACK is not defined by POSIX (netinet/in.h names it only as an extension); tests now use getaddrinfo() for the loopback address |
| XSH.socket.002 | SYSTEM | SO_PROTOCOL is required (unshaded or CX) by the page that defines it and is not provided in a POSIX.1-2024 build |
| XSH.socket.003 | SYSTEM | SO_PROTOCOL is required (unshaded or CX) by the page that defines it and is not provided in a POSIX.1-2024 build |
| XSH.socket.007 | SYSTEM | SOCK_CLOEXEC is required (unshaded or CX) by the page that defines it and is not provided in a POSIX.1-2024 build |
| XSH.socket.008 | SYSTEM | SOCK_NONBLOCK is required (unshaded or CX) by the page that defines it and is not provided in a POSIX.1-2024 build |
| XSH.socket.009 | SYSTEM | SOCK_CLOFORK is required (unshaded or CX) by the page that defines it and is not provided in a POSIX.1-2024 build |
| XSH.socket.010 | SYSTEM | SOCK_CLOEXEC is required (unshaded or CX) by the page that defines it and is not provided in a POSIX.1-2024 build |
| XSH.socket.016 | SYSTEM | SO_DOMAIN is required (unshaded or CX) by the page that defines it and is not provided in a POSIX.1-2024 build |
| XSH.socketpair.006 | SYSTEM | SOCK_CLOEXEC is required (unshaded or CX) by the page that defines it and is not provided in a POSIX.1-2024 build |
| XSH.socketpair.007 | SYSTEM | SOCK_NONBLOCK is required (unshaded or CX) by the page that defines it and is not provided in a POSIX.1-2024 build |
| XSH.socketpair.008 | SYSTEM | SOCK_CLOFORK is required (unshaded or CX) by the page that defines it and is not provided in a POSIX.1-2024 build |
| XSH.strlcpy.001 | SYSTEM | strlcpy is required (unshaded or CX) by the page that defines it and is not provided in a POSIX.1-2024 build |
| XSH.strlcpy.002 | SYSTEM | strlcpy is required (unshaded or CX) by the page that defines it and is not provided in a POSIX.1-2024 build |
| XSH.strlcpy.003 | SYSTEM | strlcpy is required (unshaded or CX) by the page that defines it and is not provided in a POSIX.1-2024 build |
| XSH.strlcpy.004 | SYSTEM | strlcpy is required (unshaded or CX) by the page that defines it and is not provided in a POSIX.1-2024 build |
| XSH.strlcpy.005 | SYSTEM | strlcpy is required (unshaded or CX) by the page that defines it and is not provided in a POSIX.1-2024 build |
| XSH.strlcpy.006 | SYSTEM | strlcpy is required (unshaded or CX) by the page that defines it and is not provided in a POSIX.1-2024 build |
| XSH.strlcpy.007 | SYSTEM | strlcpy is required (unshaded or CX) by the page that defines it and is not provided in a POSIX.1-2024 build |
| XSH.strlcpy.008 | SYSTEM | strlcpy is required (unshaded or CX) by the page that defines it and is not provided in a POSIX.1-2024 build |
| XSH.strlcpy.009 | SYSTEM | strlcpy is required (unshaded or CX) by the page that defines it and is not provided in a POSIX.1-2024 build |
| XSH.strlcpy.010 | SYSTEM | strlcpy is required (unshaded or CX) by the page that defines it and is not provided in a POSIX.1-2024 build |
| XSH.strlcpy.011 | SYSTEM | strlcpy is required (unshaded or CX) by the page that defines it and is not provided in a POSIX.1-2024 build |
| XSH.strlcpy.012 | SYSTEM | strlcpy is required (unshaded or CX) by the page that defines it and is not provided in a POSIX.1-2024 build |
| XSH.strlcpy.013 | SYSTEM | strlcpy is required (unshaded or CX) by the page that defines it and is not provided in a POSIX.1-2024 build |
| XSH.strlcpy.014 | SYSTEM | strlcpy is required (unshaded or CX) by the page that defines it and is not provided in a POSIX.1-2024 build |
| XSH.strlcpy.015 | SYSTEM | strlcpy is required (unshaded or CX) by the page that defines it and is not provided in a POSIX.1-2024 build |
| XSH.strlcpy.016 | SYSTEM | strlcpy is required (unshaded or CX) by the page that defines it and is not provided in a POSIX.1-2024 build |
| XSH.sysconf.001 | SYSTEM | _SC_XOPEN_UNIX is required (unshaded or CX) by the page that defines it and is not provided in a POSIX.1-2024 build |
| XSH.sysconf.002 | SYSTEM | _SC_XOPEN_UNIX is required (unshaded or CX) by the page that defines it and is not provided in a POSIX.1-2024 build |
| XSH.sysconf.003 | SYSTEM | _SC_XOPEN_UNIX is required (unshaded or CX) by the page that defines it and is not provided in a POSIX.1-2024 build |
| XSH.sysconf.004 | SYSTEM | _SC_XOPEN_UNIX is required (unshaded or CX) by the page that defines it and is not provided in a POSIX.1-2024 build |
| XSH.sysconf.005 | SYSTEM | _SC_XOPEN_UNIX is required (unshaded or CX) by the page that defines it and is not provided in a POSIX.1-2024 build |
| XSH.sysconf.006 | SYSTEM | _SC_XOPEN_UNIX is required (unshaded or CX) by the page that defines it and is not provided in a POSIX.1-2024 build |
| XSH.sysconf.007 | SYSTEM | _SC_XOPEN_UNIX is required (unshaded or CX) by the page that defines it and is not provided in a POSIX.1-2024 build |
| XSH.sysconf.008 | SYSTEM | _SC_XOPEN_UNIX is required (unshaded or CX) by the page that defines it and is not provided in a POSIX.1-2024 build |
| XSH.sysconf.009 | SYSTEM | _SC_XOPEN_UNIX is required (unshaded or CX) by the page that defines it and is not provided in a POSIX.1-2024 build |
| XSH.tcgetwinsize.001 | SYSTEM | struct winsize is required (unshaded or CX) by the page that defines it and is not provided in a POSIX.1-2024 build |
| XSH.tcgetwinsize.002 | SYSTEM | const struct winsize is required (unshaded or CX) by the page that defines it and is not provided in a POSIX.1-2024 build |
| XSH.tcgetwinsize.003 | SYSTEM | struct winsize is required (unshaded or CX) by the page that defines it and is not provided in a POSIX.1-2024 build |
| XSH.tcgetwinsize.004 | SYSTEM | struct winsize is required (unshaded or CX) by the page that defines it and is not provided in a POSIX.1-2024 build |
| XSH.tcgetwinsize.005 | SYSTEM | const struct winsize is required (unshaded or CX) by the page that defines it and is not provided in a POSIX.1-2024 build |
| XSH.tcsetwinsize.001 | SYSTEM | const struct winsize is required (unshaded or CX) by the page that defines it and is not provided in a POSIX.1-2024 build |
| XSH.tcsetwinsize.002 | SYSTEM | struct winsize is required (unshaded or CX) by the page that defines it and is not provided in a POSIX.1-2024 build |
| XSH.tcsetwinsize.003 | SYSTEM | struct winsize is required (unshaded or CX) by the page that defines it and is not provided in a POSIX.1-2024 build |
| XSH.tcsetwinsize.004 | SYSTEM | struct winsize is required (unshaded or CX) by the page that defines it and is not provided in a POSIX.1-2024 build |
| XSH.tcsetwinsize.005 | SYSTEM | struct winsize is required (unshaded or CX) by the page that defines it and is not provided in a POSIX.1-2024 build |
| XSH.tcsetwinsize.006 | SYSTEM | struct winsize is required (unshaded or CX) by the page that defines it and is not provided in a POSIX.1-2024 build |
| XSH.tcsetwinsize.007 | SYSTEM | struct winsize is required (unshaded or CX) by the page that defines it and is not provided in a POSIX.1-2024 build |
| XSH.tcsetwinsize.008 | SYSTEM | struct winsize is required (unshaded or CX) by the page that defines it and is not provided in a POSIX.1-2024 build |
| XSH.textdomain.001 | SYSTEM | libintl.h is required (unshaded or CX) by the page that defines it and is not provided in a POSIX.1-2024 build |
| XSH.textdomain.002 | SYSTEM | libintl.h is required (unshaded or CX) by the page that defines it and is not provided in a POSIX.1-2024 build |
| XSH.textdomain.003 | SYSTEM | libintl.h is required (unshaded or CX) by the page that defines it and is not provided in a POSIX.1-2024 build |
| XSH.textdomain.004 | SYSTEM | libintl.h is required (unshaded or CX) by the page that defines it and is not provided in a POSIX.1-2024 build |
| XSH.textdomain.005 | SYSTEM | libintl.h is required (unshaded or CX) by the page that defines it and is not provided in a POSIX.1-2024 build |
| XSH.textdomain.006 | SYSTEM | libintl.h is required (unshaded or CX) by the page that defines it and is not provided in a POSIX.1-2024 build |
| XSH.textdomain.007 | SYSTEM | libintl.h is required (unshaded or CX) by the page that defines it and is not provided in a POSIX.1-2024 build |
| XSH.textdomain.008 | SYSTEM | libintl.h is required (unshaded or CX) by the page that defines it and is not provided in a POSIX.1-2024 build |
| XSH.textdomain.009 | SYSTEM | libintl.h is required (unshaded or CX) by the page that defines it and is not provided in a POSIX.1-2024 build |
| XSH.textdomain.010 | SYSTEM | libintl.h is required (unshaded or CX) by the page that defines it and is not provided in a POSIX.1-2024 build |
| XSH.thrd_create.001 | SYSTEM | threads.h functions not found when linking with -l pthread, the library c17 names for them |
| XSH.thrd_create.002 | SYSTEM | threads.h functions not found when linking with -l pthread, the library c17 names for them |
| XSH.thrd_create.003 | SYSTEM | threads.h functions not found when linking with -l pthread, the library c17 names for them |
| XSH.thrd_create.004 | SYSTEM | threads.h functions not found when linking with -l pthread, the library c17 names for them |
| XSH.thrd_create.005 | SYSTEM | threads.h functions not found when linking with -l pthread, the library c17 names for them |
| XSH.thrd_create.007 | SYSTEM | threads.h functions not found when linking with -l pthread, the library c17 names for them |
| XSH.thrd_current.001 | SYSTEM | threads.h functions not found when linking with -l pthread, the library c17 names for them |
| XSH.thrd_current.002 | SYSTEM | threads.h functions not found when linking with -l pthread, the library c17 names for them |
| XSH.thrd_detach.001 | SYSTEM | threads.h functions not found when linking with -l pthread, the library c17 names for them |
| XSH.thrd_detach.002 | SYSTEM | threads.h functions not found when linking with -l pthread, the library c17 names for them |
| XSH.thrd_equal.001 | SYSTEM | threads.h functions not found when linking with -l pthread, the library c17 names for them |
| XSH.thrd_equal.002 | SYSTEM | threads.h functions not found when linking with -l pthread, the library c17 names for them |
| XSH.thrd_exit.001 | SYSTEM | threads.h functions not found when linking with -l pthread, the library c17 names for them |
| XSH.thrd_exit.002 | SYSTEM | threads.h functions not found when linking with -l pthread, the library c17 names for them |
| XSH.thrd_exit.003 | SYSTEM | threads.h functions not found when linking with -l pthread, the library c17 names for them |
| XSH.thrd_exit.004 | SYSTEM | threads.h functions not found when linking with -l pthread, the library c17 names for them |
| XSH.thrd_exit.005 | SYSTEM | threads.h functions not found when linking with -l pthread, the library c17 names for them |
| XSH.thrd_exit.006 | SYSTEM | threads.h functions not found when linking with -l pthread, the library c17 names for them |
| XSH.thrd_join.001 | SYSTEM | threads.h functions not found when linking with -l pthread, the library c17 names for them |
| XSH.thrd_join.002 | SYSTEM | threads.h functions not found when linking with -l pthread, the library c17 names for them |
| XSH.thrd_join.003 | SYSTEM | threads.h functions not found when linking with -l pthread, the library c17 names for them |
| XSH.thrd_join.004 | SYSTEM | threads.h functions not found when linking with -l pthread, the library c17 names for them |
| XSH.thrd_join.005 | SYSTEM | threads.h functions not found when linking with -l pthread, the library c17 names for them |
| XSH.thrd_sleep.001 | SYSTEM | threads.h functions not found when linking with -l pthread, the library c17 names for them |
| XSH.thrd_sleep.002 | SYSTEM | threads.h functions not found when linking with -l pthread, the library c17 names for them |
| XSH.thrd_sleep.003 | SYSTEM | threads.h functions not found when linking with -l pthread, the library c17 names for them |
| XSH.thrd_sleep.004 | SYSTEM | threads.h functions not found when linking with -l pthread, the library c17 names for them |
| XSH.thrd_yield.001 | SYSTEM | threads.h functions not found when linking with -l pthread, the library c17 names for them |
| XSH.timer_create.001 | SYSTEM | struct sigaction has no sa_sigaction member in a POSIX.1-2024 build (signal.h requires it) |
| XSH.timer_create.002 | SYSTEM | struct sigaction has no sa_sigaction member in a POSIX.1-2024 build (signal.h requires it) |
| XSH.timer_create.003 | SYSTEM | struct sigaction has no sa_sigaction member in a POSIX.1-2024 build (signal.h requires it) |
| XSH.timer_create.004 | SYSTEM | struct sigaction has no sa_sigaction member in a POSIX.1-2024 build (signal.h requires it) |
| XSH.timer_create.005 | SYSTEM | struct sigaction has no sa_sigaction member in a POSIX.1-2024 build (signal.h requires it) |
| XSH.timer_create.006 | SYSTEM | struct sigaction has no sa_sigaction member in a POSIX.1-2024 build (signal.h requires it) |
| XSH.timer_create.007 | SYSTEM | struct sigaction has no sa_sigaction member in a POSIX.1-2024 build (signal.h requires it) |
| XSH.timer_create.008 | SYSTEM | struct sigaction has no sa_sigaction member in a POSIX.1-2024 build (signal.h requires it) |
| XSH.timer_create.009 | SYSTEM | struct sigaction has no sa_sigaction member in a POSIX.1-2024 build (signal.h requires it) |
| XSH.timer_create.010 | SYSTEM | struct sigaction has no sa_sigaction member in a POSIX.1-2024 build (signal.h requires it) |
| XSH.timer_create.011 | SYSTEM | struct sigaction has no sa_sigaction member in a POSIX.1-2024 build (signal.h requires it) |
| XSH.timer_delete.001 | SYSTEM | struct sigaction has no sa_sigaction member in a POSIX.1-2024 build (signal.h requires it) |
| XSH.timer_delete.002 | SYSTEM | struct sigaction has no sa_sigaction member in a POSIX.1-2024 build (signal.h requires it) |
| XSH.timer_delete.003 | SYSTEM | struct sigaction has no sa_sigaction member in a POSIX.1-2024 build (signal.h requires it) |
| XSH.timer_getoverrun.001 | SYSTEM | struct sigaction has no sa_sigaction member in a POSIX.1-2024 build (signal.h requires it) |
| XSH.timer_getoverrun.002 | SYSTEM | struct sigaction has no sa_sigaction member in a POSIX.1-2024 build (signal.h requires it) |
| XSH.timer_getoverrun.003 | SYSTEM | struct sigaction has no sa_sigaction member in a POSIX.1-2024 build (signal.h requires it) |
| XSH.timer_getoverrun.004 | SYSTEM | struct sigaction has no sa_sigaction member in a POSIX.1-2024 build (signal.h requires it) |
| XSH.timer_getoverrun.005 | SYSTEM | struct sigaction has no sa_sigaction member in a POSIX.1-2024 build (signal.h requires it) |
| XSH.timer_getoverrun.006 | SYSTEM | struct sigaction has no sa_sigaction member in a POSIX.1-2024 build (signal.h requires it) |
| XSH.timer_getoverrun.007 | SYSTEM | struct sigaction has no sa_sigaction member in a POSIX.1-2024 build (signal.h requires it) |
| XSH.timer_getoverrun.008 | SYSTEM | struct sigaction has no sa_sigaction member in a POSIX.1-2024 build (signal.h requires it) |
| XSH.timer_getoverrun.009 | SYSTEM | struct sigaction has no sa_sigaction member in a POSIX.1-2024 build (signal.h requires it) |
| XSH.timer_getoverrun.010 | SYSTEM | struct sigaction has no sa_sigaction member in a POSIX.1-2024 build (signal.h requires it) |
| XSH.timer_getoverrun.011 | SYSTEM | struct sigaction has no sa_sigaction member in a POSIX.1-2024 build (signal.h requires it) |
| XSH.timer_getoverrun.012 | SYSTEM | struct sigaction has no sa_sigaction member in a POSIX.1-2024 build (signal.h requires it) |
| XSH.timer_getoverrun.013 | SYSTEM | struct sigaction has no sa_sigaction member in a POSIX.1-2024 build (signal.h requires it) |
| XSH.tss_create.001 | SYSTEM | threads.h functions not found when linking with -l pthread, the library c17 names for them |
| XSH.tss_create.002 | SYSTEM | threads.h functions not found when linking with -l pthread, the library c17 names for them |
| XSH.tss_create.003 | SYSTEM | threads.h functions not found when linking with -l pthread, the library c17 names for them |
| XSH.tss_create.004 | SYSTEM | threads.h functions not found when linking with -l pthread, the library c17 names for them |
| XSH.tss_create.005 | SYSTEM | threads.h functions not found when linking with -l pthread, the library c17 names for them |
| XSH.tss_delete.001 | SYSTEM | threads.h functions not found when linking with -l pthread, the library c17 names for them |
| XSH.tss_delete.002 | SYSTEM | threads.h functions not found when linking with -l pthread, the library c17 names for them |
| XSH.tss_delete.003 | SYSTEM | threads.h functions not found when linking with -l pthread, the library c17 names for them |
| XSH.tss_delete.004 | SYSTEM | threads.h functions not found when linking with -l pthread, the library c17 names for them |
| XSH.tss_get.001 | SYSTEM | threads.h functions not found when linking with -l pthread, the library c17 names for them |
| XSH.tss_get.002 | SYSTEM | threads.h functions not found when linking with -l pthread, the library c17 names for them |
| XSH.tss_get.003 | SYSTEM | threads.h functions not found when linking with -l pthread, the library c17 names for them |
| XSH.tss_get.004 | SYSTEM | threads.h functions not found when linking with -l pthread, the library c17 names for them |
| XSH.tss_get.005 | SYSTEM | threads.h functions not found when linking with -l pthread, the library c17 names for them |
| XSH.vfprintf.008 | SYSTEM | vasprintf is required (unshaded or CX) by the page that defines it and is not provided in a POSIX.1-2024 build |
| XSH.wcscasecmp.004 | SYSTEM | wcscasecmp_l() (CX, base) is declared but not found when linking |
| XSH.wcslcat.001 | SYSTEM | wcslcpy is required (unshaded or CX) by the page that defines it and is not provided in a POSIX.1-2024 build |
| XSH.wcslcat.002 | SYSTEM | wcslcpy is required (unshaded or CX) by the page that defines it and is not provided in a POSIX.1-2024 build |
| XSH.wcslcat.003 | SYSTEM | wcslcpy is required (unshaded or CX) by the page that defines it and is not provided in a POSIX.1-2024 build |
| XSH.wcslcat.004 | SYSTEM | wcslcpy is required (unshaded or CX) by the page that defines it and is not provided in a POSIX.1-2024 build |
| XSH.wcslcat.005 | SYSTEM | wcslcat is required (unshaded or CX) by the page that defines it and is not provided in a POSIX.1-2024 build |
| XSH.wcslcat.006 | SYSTEM | wcslcat is required (unshaded or CX) by the page that defines it and is not provided in a POSIX.1-2024 build |
| XSH.wcslcat.007 | SYSTEM | wcslcat is required (unshaded or CX) by the page that defines it and is not provided in a POSIX.1-2024 build |
| XSH.wcslcat.008 | SYSTEM | wcslcpy is required (unshaded or CX) by the page that defines it and is not provided in a POSIX.1-2024 build |
