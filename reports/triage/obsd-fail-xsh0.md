# Second-system triage, group fail-xsh0

Codes: this = second system (root run), Linux = latest Linux run. Per-test output files of the second system were empty, so verdicts rest on the run-log notes, the build logs, the tests and the standard.

| ID | this | Linux | class | reason |
|---|---|---|---|---|
| XSH.2.1-use-of-functions.004 | FAIL | PASS | SYSTEM | rand() after the same srand() seed gives different values; ISO C (via XSH 2.1/rand) makes the sequence repeatable. |
| XSH.2.10-sockets.016 | UNRESOLVED | NOTINUSE | SYSTEM | setsockopt(IPV6_V6ONLY, 0) fails EINVAL although XSH 2.10.20 gives default 0 and AF_INET6 sockets may carry IPv4; test left UNRESOLVED (conservative). |
| XSH.2.10-sockets.017 | UNRESOLVED | NOTINUSE | SYSTEM | same cause as 016. |
| XSH.2.10-sockets.019 | UNRESOLVED | NOTINUSE | SYSTEM | same cause as 016. |
| XSH.2.11-data-types.004 | INSPECT | INSPECT | STD-AMBIG | unsigned id_t versus signed pid_t range; already disputed, Q-200. |
| XSH.2.4.2-realtime-signals.001 | UNRESOLVED | PASS | SYSTEM | blocked: sigtimedwait() is not declared (build failed); it is a base (CX) function, XSH sigtimedwait. |
| XSH.2.4.2-realtime-signals.004 | UNRESOLVED | PASS | SYSTEM | blocked: sigqueue() and SIGRTMIN are not declared (build failed); base (CX), signal.h requires SIGRTMIN. |
| XSH.2.4.2-realtime-signals.005 | UNRESOLVED | PASS | SYSTEM | same as 004. |
| XSH.2.4.3-signal-actions.010 | UNRESOLVED | PASS | SYSTEM | same as 004. |
| XSH.2.4.3-signal-actions.013 | FAIL | PASS | UNKNOWN | a tick from the main thread was seen after the stop was reported; test reads right (2.4.3 stop acts on the whole process) but a late tick from a thread that was asleep when the stop came cannot be excluded; settle it by logging tick times against the moment waitpid reports the stop. |
| XSH.2.4.3-signal-actions.019 | FAIL | PASS | SYSTEM | SA_SIGINFO handler for SIGCHLD gets si_code 0, not CLD_EXITED (sigaction, signal.h siginfo_t). |
| XSH.2.9-threads.001 | UNRESOLVED | PASS | ENV | 200 descriptors held at once exceed a low soft descriptor limit (open() or dup() failed; EMFILE is allowed). Test fixed. |
| XSH.2.9-threads.025 | FAIL | PASS | SYSTEM | gmtime_r() is on neither cancellation-point list, and 2.9.5.2 forbids adding points; it acted on a pending cancel. |
| XSH.2.9-threads.026 | FAIL | FAIL | SYSTEM | 2.9.5.3: pthread_exit() sets DISABLE first; the cleanup handler saw another state. |
| XSH._Exit.014 | FAIL | FAIL | SYSTEM | the SIGCHLD siginfo_t carries no status/code (si_status 0, not 0x1234); _Exit DESCRIPTION. |
| XSH.access.018 | UNRESOLVED | UNRESOLVED | SYSTEM | O_SEARCH is a required fcntl.h macro and is not defined; UNRESOLVED is right. |
| XSH.alarm.001 | UNRESOLVED | PASS | SYSTEM | blocked by sigtimedwait() absent (build failed). |
| XSH.alarm.002 | UNRESOLVED | PASS | SYSTEM | same. |
| XSH.alarm.003 | UNRESOLVED | PASS | SYSTEM | same. |
| XSH.alarm.004 | UNRESOLVED | PASS | SYSTEM | same. |
| XSH.atan2.004 | INSPECT | PASS | STD-AMBIG | float result differs from the rounded pi; disputed already, Q-101. (The message prints only 6 digits.) |
| XSH.catgets.004 | FAIL | PASS | SYSTEM | gencat: "number, one blank, nothing" must store an empty string (gencat EXTENDED DESCRIPTION); catgets returns the default. |
| XSH.chmod.019 | UNRESOLVED | UNRESOLVED | SYSTEM | O_SEARCH not defined. |
| XSH.chown.021 | UNRESOLVED | UNRESOLVED | SYSTEM | O_SEARCH not defined. |
| XSH.clock_getcpuclockid.003 | UNRESOLVED | PASS | ENV | permission to read another process's CPU-time clock is implementation-defined (EPERM allowed); UNRESOLVED is the designed outcome. |
| XSH.cosh.005 | FAIL | PASS | SYSTEM | math_errhandling includes MATH_ERREXCEPT but no overflow exception is raised (cosh RETURN VALUE/ERRORS). |
| XSH.exec.026 | FAIL | PASS | SYSTEM | times() values are reset by exec (3 vs 30); exec lists tms_* as inherited. |
| XSH.exec.027 | FAIL | PASS | SYSTEM | CLOCK_PROCESS_CPUTIME_ID is reinitialized by exec (27 vs 301 ms); exec says not reinitialized (CPT). |
| XSH.exit.011 | FAIL | PASS | SYSTEM | SIGCHLD siginfo has si_code 0, not CLD_EXITED. |
| XSH.exit.012 | FAIL | FAIL | SYSTEM | SIGCHLD siginfo has no status (0, not 0x1234). |
| XSH.exp.005 | FAIL | PASS | SYSTEM | overflow exception not raised for exp(big) long double. |
| XSH.exp2.005 | FAIL | PASS | SYSTEM | same, exp2 double. |
| XSH.expm1.005 | FAIL | PASS | SYSTEM | same, expm1 double. |
| XSH.fchmod.007 | UNRESOLVED | UNRESOLVED | SYSTEM | O_SEARCH not defined. |
| XSH.fchown.005 | FAIL | PASS | SYSTEM | fchown() with the file's own IDs does not mark st_ctim (chown DESCRIPTION: marked unless both IDs are -1). |
| XSH.fmemopen.013 | FAIL | PASS | SYSTEM | fseek(-1, SEEK_END) in "a" mode fails EOVERFLOW; the end position is the first null byte (fmemopen DESCRIPTION). |
| XSH.fnmatch.021 | FAIL | UNINITIATED | SYSTEM | with FNM_CASEFOLD "[[:lower:]]" must match "A" (XBD 4.1: counterpart characters also match); plain ranges work, classes do not. |
| XSH.fnmatch.022 | FAIL | UNINITIATED | SYSTEM | same, FNM_IGNORECASE. |
| XSH.fprintf.024 | FAIL | PASS | SYSTEM | %n makes the process abort (signal 6); %n is required. |
| XSH.fscanf.019 | FAIL | PASS | SYSTEM | %% does not skip leading white space; fscanf says white space is skipped for every conversion except [, c, C, n. |
| XSH.fscanf.028 | FAIL | PASS | SYSTEM | %a does not accept strtod's hexadecimal form. |
| XSH.fscanf.029 | FAIL | PASS | SYSTEM | %A likewise. |
| XSH.fscanf.030 | FAIL | PASS | SYSTEM | "inf" printed by snprintf is not read back by %f (fscanf: shall recognize). |
| XSH.fscanf.043 | FAIL | FAIL | SYSTEM | EOF returned although the suppressed %*d completed (RETURN VALUE, TC1 0186). |
| XSH.fstat.006 | FAIL | PASS | SYSTEM | shm_open(0640) under umask 022 (set by the runner) gives 0600; shm_open sets mode minus umask. |
| XSH.fstatat.013 | UNRESOLVED | UNRESOLVED | SYSTEM | O_SEARCH not defined. |
| XSH.ftruncate.007 | FAIL | PASS | SYSTEM | whole page discarded by ftruncate() still readable (no SIGBUS); ftruncate DESCRIPTION. |
| XSH.futimens.012 | FAIL | PASS | SYSTEM | utimensat with both UTIME_OMIT on another user's file fails; ownership/permission check is forbidden (futimens DESCRIPTION/ERRORS EPERM wording). |
| XSH.futimens.021 | FAIL | PASS | SYSTEM | futimens with both UTIME_OMIT on a read-only descriptor of another user's file fails; same passage. |
| XSH.futimens.025 | UNRESOLVED | UNRESOLVED | SYSTEM | O_SEARCH not defined. |
| XSH.fwprintf.006 | FAIL | PASS | SYSTEM | %n aborts the process, as fprintf.024. |

## TEST-BUG changes
None of the failures was a TEST-BUG. One ENV fix: modules/xsh/2.9-threads/t/001-alloc-concurrent.c now raises a low soft RLIMIT_NOFILE to 512 (or the hard limit) in the setup phase and names the errno when open() or dup() fails. Re-run on this machine: PASS. Lint problem count unchanged by this edit (other agents' edits are in the tree).

## Counts
SYSTEM 46, ENV 2, STD-AMBIG 2 (both already disputed: Q-200, Q-101), UNKNOWN 1, TEST-BUG 0 (51 rows).

## Patterns
- Missing or partial base interfaces: sigqueue/sigtimedwait/SIGRTMIN (blocking 11 dependent lines), O_SEARCH (6), FNM_CASEFOLD classes.
- SIGCHLD siginfo is not filled (019, exit.011/012, _Exit.014).
- stdio: %n aborts; scanf lacks %%-whitespace, hex floats, inf/nan.
- exec resets process times and CPU-time clock.
- Math overflow exceptions not raised (4 functions).
- UTIME_OMIT pair still checked for ownership.
