# Triage of the first Linux run: group xbd

Run: native Linux, glibc 2.39, gcc 13.3, run as root; test builds use `-D_POSIX_C_SOURCE=202405L` (BASE) or `-D_XOPEN_SOURCE=800` (XSI) as the driver does. Standard: POSIX.1-2024 (susv5-html), consulted for every verdict; no text copied. Utilities on the machine: dash as sh, mawk as awk, GNU find 4.9.0, no c17.

Every row: ID | run result | class | reason (standard section; for TEST-BUG what changed and the re-run). Re-runs used `./posixcheck.sh --no-selftest --only ID`.

| ID | code | class | reason |
|---|---|---|---|
| XBD.11.1-interface-characteristics.004 | FAIL | TEST-BUG | Expected value was read from g_sess after end_session() had reset it to -1, so the process-group id could never match (XBD 11.1.3/11.1.4 controlling terminal and its foreground group). Fix: g_leader keeps the leader pid; the check uses it. Re-run: PASS. |
| XBD.11.1-interface-characteristics.005 | FAIL | TEST-BUG | Expected value was read from g_sess after end_session() had reset it to -1, so the process-group id could never match (XBD 11.1.3/11.1.4 controlling terminal and its foreground group). Fix: g_leader keeps the leader pid; the check uses it. Re-run: PASS. |
| XBD.11.1-interface-characteristics.006 | FAIL | TEST-BUG | Expected value was read from g_sess after end_session() had reset it to -1, so the process-group id could never match (XBD 11.1.3/11.1.4 controlling terminal and its foreground group). Fix: g_leader keeps the leader pid; the check uses it. Re-run: PASS. |
| XBD.11.1-interface-characteristics.007 | FAIL | TEST-BUG | Expected value was read from g_sess after end_session() had reset it to -1, so the process-group id could never match (XBD 11.1.3/11.1.4 controlling terminal and its foreground group). Fix: g_leader keeps the leader pid; the check uses it. Re-run: PASS. |
| XBD.11.1-interface-characteristics.010 | UNRESOLVED | TEST-BUG | Race in building the orphaned group: the grandchild read its parent pid with getppid() after the middle process might already have ended, so it waited for a change that had happened (XBD 11.1.4 orphaned groups). Fix: the middle process passes its own pid before it forks (same change in 008, 009, 012-014, 016, which share the code). Re-run x3: PASS. |
| XBD.11.1-interface-characteristics.015 | UNRESOLVED | TEST-BUG | Race in building the orphaned group: the grandchild read its parent pid with getppid() after the middle process might already have ended, so it waited for a change that had happened (XBD 11.1.4 orphaned groups). Fix: the middle process passes its own pid before it forks (same change in 008, 009, 012-014, 016, which share the code). Re-run x3: PASS. |
| XBD.11.1-interface-characteristics.031 | FAIL | TEST-BUG | Case B (MIN 3, TIME 0, XBD 11.1.7): the signal that ends the wait may end the read either with -1/EINTR or, after the two queued bytes were taken, with the count (read(): "interrupted by a signal after it has successfully read some data"); the test demanded -1/EINTR. Fix: both endings accepted, but not before the alarm (>= 0.8 s), then MIN satisfied is checked. Re-run: PASS. |
| XBD.11.1-interface-characteristics.035 | FAIL | TEST-BUG | Counting the SIGINT per receiver relied on a variable ("who") that each child sets after fork; the handler can run in a child before that store, so the child was counted as the leader (XBD 11.1.9). Fix: the handler tells leader, foreground child and other-group child apart by getpid()/getpgrp(). Re-run x3: PASS. |
| XBD.11.1-interface-characteristics.036 | FAIL | TEST-BUG | Counting the SIGQUIT per receiver relied on a variable ("who") that each child sets after fork; the handler can run in a child before that store, so the child was counted as the leader (XBD 11.1.9). Fix: the handler tells leader, foreground child and other-group child apart by getpid()/getpgrp(). Re-run x3: PASS. |
| XBD.11.1-interface-characteristics.037 | FAIL | TEST-BUG | Counting the SIGTSTP per receiver relied on a variable ("who") that each child sets after fork; the handler can run in a child before that store, so the child was counted as the leader (XBD 11.1.9). Fix: the handler tells leader, foreground child and other-group child apart by getpid()/getpgrp(). Re-run x3: PASS. |
| XBD.11.1-interface-characteristics.039 | FAIL | SYSTEM | With IEXTEN set, ERASE after a backslash still erased (read gave 61 62 0a: the backslash was removed, the ERASE byte is missing); XBD 11.1.9 (XSI) says an escaped ERASE, KILL or EOF has no special function. |
| XBD.11.1-interface-characteristics.045 | FAIL | SYSTEM | Input queued by the manager before the last close of the subsidiary side was still readable after the reopen (3 bytes); XBD 11.1.11 says the last close causes any input to be discarded. |
| XBD.11.2-terminal-parameters.007 | UNRESOLVED | TEST-BUG | The test did a blocking write on the subsidiary side while output was suspended by STOP and only then sent START; a write may block while output is suspended (XBD 11.1.8, 11.2.2 IXON), so it deadlocked until the 60 s timeout. Fix: that write is made by a child, the parent sends START/restart byte, and the child must complete afterwards. Re-run x2: PASS. |
| XBD.11.2-terminal-parameters.009 | UNRESOLVED | TEST-BUG | The test did a blocking write on the subsidiary side while output was suspended by STOP and only then sent START; a write may block while output is suspended (XBD 11.1.8, 11.2.2 IXON), so it deadlocked until the 60 s timeout. Fix: that write is made by a child, the parent sends START/restart byte, and the child must complete afterwards. Re-run x2: PASS. |
| XBD.11.2-terminal-parameters.016 | FAIL | SYSTEM | NL1 with OFILL gave no fill characters (got 61 0a, expected two NUL after the newline); XBD 11.2.3: with OFILL, two fill characters are transmitted for the newline delay. tcsetattr() kept the NL1 setting, so the test did not skip it. |
| XBD.11.2-terminal-parameters.025 | UNRESOLVED | TEST-BUG | The test did a blocking write on the subsidiary side while output was suspended by STOP and only then sent START; a write may block while output is suspended (XBD 11.1.8, 11.2.2 IXON), so it deadlocked until the 60 s timeout. Fix: that write is made by a child, the parent sends START/restart byte, and the child must complete afterwards. Re-run x2: PASS. |
| XBD.2-conformance.001 | FAIL | ENV | Mandatory utilities not installed in this container image: at batch crontab gettext lp msgfmt ngettext pax time uudecode uuencode write (none on the getconf PATH /bin:/usr/bin); the test is right (XCU pages are unshaded). A real distribution install would have them. |
| XBD.2-conformance.002 | FAIL | ENV | c17 and lex are not installed although getconf POSIX2_C_DEV is 200809 (XBD 2.1.6.2); the machine has cc and yacc only. |
| XBD.2-conformance.003 | FAIL | SYSTEM | getconf POSIX2_UPE writes "undefined" while the system claims XSI (_XOPEN_UNIX); XBD 2.1.4.2: an XSI system shall support the User Portability Utilities option. |
| XBD.2-conformance.005 | FAIL | SYSTEM | _XOPEN_REALTIME is 1 but _POSIX_MEMLOCK (and the others of the group) are 200809L; XBD 2.1.5.2 requires 202405L. |
| XBD.2-conformance.006 | FAIL | SYSTEM | _XOPEN_REALTIME_THREADS is claimed but _POSIX_THREAD_PRIO_INHERIT is 200809L; XBD 2.1.5.2 requires 202405L. |
| XBD.2-conformance.008 | FAIL | ENV | No c17 utility on the machine, so the external-name program could not be built; the test now reports UNRESOLVED ("the c17 utility was not found") instead of FAIL, the absence being XBD.2-conformance.002's finding. Re-run: UNRESOLVED. (TEST-BUG fixed in the sense of the verdict; the machine lacks the utility.) |
| XBD.4.1-case-insensitive-comparisons.002 | FAIL | SYSTEM | find -iname '[[:lower:]]ulu' did not match Zulu (GNU findutils 4.9.0 matches only zulu); XBD 4.1: a string that matches case-sensitively still matches when the characters are replaced by their case counterparts. Reproduced by hand. |
| XBD.4.11-file-system-cache.002 | FAIL | SYSTEM | A pread of 256 KiB concurrent with pwrite of the same bytes returned a mixture (ext4); XSH 2.9.7 lists pread, pwrite, read, write as atomic with respect to each other on files. |
| XBD.5-file-format-notation.009 | FAIL | SYSTEM | The awk on the machine is mawk 1.3.4: printf "%.s" wrote the whole string; XBD 5 precision: "a null digit string is treated as zero", so zero bytes. |
| XBD.5-file-format-notation.010 | FAIL | SYSTEM | mawk printf "%F" wrote nothing; XBD 5 lists f,F, and awk printf uses that notation (no exception for F). |
| XBD.5-file-format-notation.013 | FAIL | SYSTEM | mawk printf "%a" wrote nothing; XBD 5 defines a,A and awk printf uses that notation. |
| XBD.6.2-character-encoding.001 | FAIL | SYSTEM | In the POSIX locale mbrtowc() rejects byte 128 (size_t -1); XBD 6.2: the POSIX locale contains 256 single-byte characters. |
| XBD.6.4-charmap-description.001 | UNRESOLVED | SYSTEM | Blocked by the pathname-locale failure (XBD.7.1-locale-general.001): the locale made by localedef cannot be selected through LC_ALL, so the test correctly gives UNRESOLVED (XBD 7, 8.2 require the pathname form). |
| XBD.6.4-charmap-description.002 | UNRESOLVED | SYSTEM | Blocked by the pathname-locale failure (XBD.7.1-locale-general.001): the locale made by localedef cannot be selected through LC_ALL, so the test correctly gives UNRESOLVED (XBD 7, 8.2 require the pathname form). |
| XBD.6.4-charmap-description.003 | UNRESOLVED | SYSTEM | Blocked by the pathname-locale failure (XBD.7.1-locale-general.001): the locale made by localedef cannot be selected through LC_ALL, so the test correctly gives UNRESOLVED (XBD 7, 8.2 require the pathname form). |
| XBD.6.4-charmap-description.004 | UNRESOLVED | SYSTEM | Blocked by the pathname-locale failure (XBD.7.1-locale-general.001): the locale made by localedef cannot be selected through LC_ALL, so the test correctly gives UNRESOLVED (XBD 7, 8.2 require the pathname form). |
| XBD.6.4-charmap-description.005 | UNRESOLVED | SYSTEM | Blocked by the pathname-locale failure (XBD.7.1-locale-general.001): the locale made by localedef cannot be selected through LC_ALL, so the test correctly gives UNRESOLVED (XBD 7, 8.2 require the pathname form). |
| XBD.6.4-charmap-description.006 | UNRESOLVED | SYSTEM | Blocked by the pathname-locale failure (XBD.7.1-locale-general.001): the locale made by localedef cannot be selected through LC_ALL, so the test correctly gives UNRESOLVED (XBD 7, 8.2 require the pathname form). |
| XBD.6.4-charmap-description.007 | UNRESOLVED | SYSTEM | Blocked by the pathname-locale failure (XBD.7.1-locale-general.001): the locale made by localedef cannot be selected through LC_ALL, so the test correctly gives UNRESOLVED (XBD 7, 8.2 require the pathname form). |
| XBD.6.4-charmap-description.008 | UNRESOLVED | SYSTEM | Blocked by the pathname-locale failure (XBD.7.1-locale-general.001): the locale made by localedef cannot be selected through LC_ALL, so the test correctly gives UNRESOLVED (XBD 7, 8.2 require the pathname form). |
| XBD.7.1-locale-general.001 | FAIL | SYSTEM | setlocale(LC_ALL,"") fails with LC_ALL an absolute pathname of a locale made by localedef (checked by hand with a copy of an installed locale directory too); XBD 7 / 8.2: a value beginning with a slash is the pathname of the locale definition. |
| XBD.7.1-locale-general.002 | FAIL | SYSTEM | Same: newlocale(LC_ALL_MASK,"") fails (ENOENT) for a pathname locale; XBD 7, 8.2. |
| XBD.7.2-posix-locale.002 | UNRESOLVED | SYSTEM | Blocked by the pathname-locale failure (XBD.7.1-locale-general.001): the locale made by localedef cannot be selected through LC_ALL, so the test correctly gives UNRESOLVED (XBD 7, 8.2 require the pathname form). |
| XBD.7.3.1-lc-ctype.002 | FAIL | SYSTEM | Same cause as XBD.6.2-character-encoding.001: btowc(128) is WEOF in the POSIX locale (XBD 6.2 says all 256 bytes are characters of it). |
| XBD.7.3.1-lc-ctype.003 | FAIL | SYSTEM | Same cause as XBD.6.2-character-encoding.001: btowc(128) is WEOF in the POSIX locale (XBD 6.2 says all 256 bytes are characters of it). |
| XBD.7.3.1-lc-ctype.004 | FAIL | SYSTEM | Same cause as XBD.6.2-character-encoding.001: btowc(128) is WEOF in the POSIX locale (XBD 6.2 says all 256 bytes are characters of it). |
| XBD.7.3.1-lc-ctype.006 | UNRESOLVED | SYSTEM | Blocked by the pathname-locale failure (XBD.7.1-locale-general.001): the locale made by localedef cannot be selected through LC_ALL, so the test correctly gives UNRESOLVED (XBD 7, 8.2 require the pathname form). |
| XBD.7.3.1-lc-ctype.007 | UNRESOLVED | SYSTEM | Blocked by the pathname-locale failure (XBD.7.1-locale-general.001): the locale made by localedef cannot be selected through LC_ALL, so the test correctly gives UNRESOLVED (XBD 7, 8.2 require the pathname form). |
| XBD.7.3.1-lc-ctype.008 | UNRESOLVED | SYSTEM | Blocked by the pathname-locale failure (XBD.7.1-locale-general.001): the locale made by localedef cannot be selected through LC_ALL, so the test correctly gives UNRESOLVED (XBD 7, 8.2 require the pathname form). |
| XBD.7.3.1-lc-ctype.009 | UNRESOLVED | SYSTEM | Blocked by the pathname-locale failure (XBD.7.1-locale-general.001): the locale made by localedef cannot be selected through LC_ALL, so the test correctly gives UNRESOLVED (XBD 7, 8.2 require the pathname form). |
| XBD.7.3.1-lc-ctype.010 | UNRESOLVED | SYSTEM | Blocked by the pathname-locale failure (XBD.7.1-locale-general.001): the locale made by localedef cannot be selected through LC_ALL, so the test correctly gives UNRESOLVED (XBD 7, 8.2 require the pathname form). |
| XBD.7.3.2-lc-collate.001 | FAIL | SYSTEM | Same cause as XBD.6.2-character-encoding.001: btowc(128) is WEOF in the POSIX locale (XBD 6.2 says all 256 bytes are characters of it). |
| XBD.7.3.2-lc-collate.002 | FAIL | SYSTEM | Same cause as XBD.6.2-character-encoding.001: btowc(128) is WEOF in the POSIX locale (XBD 6.2 says all 256 bytes are characters of it). |
| XBD.7.3.2-lc-collate.004 | UNRESOLVED | SYSTEM | Blocked by the pathname-locale failure (XBD.7.1-locale-general.001): the locale made by localedef cannot be selected through LC_ALL, so the test correctly gives UNRESOLVED (XBD 7, 8.2 require the pathname form). |
| XBD.7.3.2-lc-collate.005 | UNRESOLVED | SYSTEM | Blocked by the pathname-locale failure (XBD.7.1-locale-general.001): the locale made by localedef cannot be selected through LC_ALL, so the test correctly gives UNRESOLVED (XBD 7, 8.2 require the pathname form). |
| XBD.7.3.2-lc-collate.006 | UNRESOLVED | SYSTEM | Blocked by the pathname-locale failure (XBD.7.1-locale-general.001): the locale made by localedef cannot be selected through LC_ALL, so the test correctly gives UNRESOLVED (XBD 7, 8.2 require the pathname form). |
| XBD.7.3.2-lc-collate.007 | UNRESOLVED | SYSTEM | Blocked by the pathname-locale failure (XBD.7.1-locale-general.001): the locale made by localedef cannot be selected through LC_ALL, so the test correctly gives UNRESOLVED (XBD 7, 8.2 require the pathname form). |
| XBD.7.3.2-lc-collate.008 | UNRESOLVED | SYSTEM | Blocked by the pathname-locale failure (XBD.7.1-locale-general.001): the locale made by localedef cannot be selected through LC_ALL, so the test correctly gives UNRESOLVED (XBD 7, 8.2 require the pathname form). |
| XBD.7.3.2-lc-collate.009 | UNRESOLVED | SYSTEM | Blocked by the pathname-locale failure (XBD.7.1-locale-general.001): the locale made by localedef cannot be selected through LC_ALL, so the test correctly gives UNRESOLVED (XBD 7, 8.2 require the pathname form). |
| XBD.7.3.2-lc-collate.010 | UNRESOLVED | SYSTEM | localedef (status 139, SIGSEGV after "syntax error") refuses a valid LC_COLLATE with an initial ellipsis (XBD 7.3.2: an initial ellipsis is interpreted as if the preceding line were NUL); the locale also could not be selected by pathname. Test UNRESOLVED is the right verdict. |
| XBD.7.3.2-lc-collate.011 | UNRESOLVED | SYSTEM | Blocked by the pathname-locale failure (XBD.7.1-locale-general.001): the locale made by localedef cannot be selected through LC_ALL, so the test correctly gives UNRESOLVED (XBD 7, 8.2 require the pathname form). |
| XBD.7.3.2-lc-collate.013 | UNRESOLVED | SYSTEM | Blocked by the pathname-locale failure (XBD.7.1-locale-general.001): the locale made by localedef cannot be selected through LC_ALL, so the test correctly gives UNRESOLVED (XBD 7, 8.2 require the pathname form). |
| XBD.7.3.3-lc-monetary.001 | UNRESOLVED | SYSTEM | Blocked by the pathname-locale failure (XBD.7.1-locale-general.001): the locale made by localedef cannot be selected through LC_ALL, so the test correctly gives UNRESOLVED (XBD 7, 8.2 require the pathname form). |
| XBD.7.3.3-lc-monetary.002 | UNRESOLVED | SYSTEM | Blocked by the pathname-locale failure (XBD.7.1-locale-general.001): the locale made by localedef cannot be selected through LC_ALL, so the test correctly gives UNRESOLVED (XBD 7, 8.2 require the pathname form). |
| XBD.7.3.3-lc-monetary.003 | UNRESOLVED | SYSTEM | Blocked by the pathname-locale failure (XBD.7.1-locale-general.001): the locale made by localedef cannot be selected through LC_ALL, so the test correctly gives UNRESOLVED (XBD 7, 8.2 require the pathname form). |
| XBD.7.3.3-lc-monetary.004 | UNRESOLVED | SYSTEM | Blocked by the pathname-locale failure (XBD.7.1-locale-general.001): the locale made by localedef cannot be selected through LC_ALL, so the test correctly gives UNRESOLVED (XBD 7, 8.2 require the pathname form). |
| XBD.7.3.3-lc-monetary.005 | UNRESOLVED | SYSTEM | Blocked by the pathname-locale failure (XBD.7.1-locale-general.001): the locale made by localedef cannot be selected through LC_ALL, so the test correctly gives UNRESOLVED (XBD 7, 8.2 require the pathname form). |
| XBD.7.3.4-lc-numeric.001 | UNRESOLVED | SYSTEM | Blocked by the pathname-locale failure (XBD.7.1-locale-general.001): the locale made by localedef cannot be selected through LC_ALL, so the test correctly gives UNRESOLVED (XBD 7, 8.2 require the pathname form). |
| XBD.7.3.4-lc-numeric.002 | UNRESOLVED | SYSTEM | Blocked by the pathname-locale failure (XBD.7.1-locale-general.001): the locale made by localedef cannot be selected through LC_ALL, so the test correctly gives UNRESOLVED (XBD 7, 8.2 require the pathname form). |
| XBD.7.3.4-lc-numeric.003 | UNRESOLVED | SYSTEM | Blocked by the pathname-locale failure (XBD.7.1-locale-general.001): the locale made by localedef cannot be selected through LC_ALL, so the test correctly gives UNRESOLVED (XBD 7, 8.2 require the pathname form). |
| XBD.7.3.4-lc-numeric.004 | UNRESOLVED | SYSTEM | Blocked by the pathname-locale failure (XBD.7.1-locale-general.001): the locale made by localedef cannot be selected through LC_ALL, so the test correctly gives UNRESOLVED (XBD 7, 8.2 require the pathname form). |
| XBD.7.3.5-lc-time.001 | UNRESOLVED | SYSTEM | Blocked by the pathname-locale failure (XBD.7.1-locale-general.001): the locale made by localedef cannot be selected through LC_ALL, so the test correctly gives UNRESOLVED (XBD 7, 8.2 require the pathname form). |
| XBD.7.3.5-lc-time.002 | UNRESOLVED | SYSTEM | Blocked by the pathname-locale failure (XBD.7.1-locale-general.001): the locale made by localedef cannot be selected through LC_ALL, so the test correctly gives UNRESOLVED (XBD 7, 8.2 require the pathname form). |
| XBD.7.3.5-lc-time.003 | UNRESOLVED | SYSTEM | Blocked by the pathname-locale failure (XBD.7.1-locale-general.001): the locale made by localedef cannot be selected through LC_ALL, so the test correctly gives UNRESOLVED (XBD 7, 8.2 require the pathname form). |
| XBD.7.3.5-lc-time.004 | UNRESOLVED | SYSTEM | Blocked by the pathname-locale failure (XBD.7.1-locale-general.001): the locale made by localedef cannot be selected through LC_ALL, so the test correctly gives UNRESOLVED (XBD 7, 8.2 require the pathname form). |
| XBD.7.3.5-lc-time.005 | FAIL | SYSTEM | date with LC_ALL set to the pathname of a localedef-made locale writes the POSIX-locale names (Wednesday|October|...): the pathname locale is not honoured (see 7.1-locale-general.001); XBD 7.3.5, 8.2. |
| XBD.7.3.5-lc-time.006 | UNRESOLVED | SYSTEM | Blocked by the pathname-locale failure (XBD.7.1-locale-general.001): the locale made by localedef cannot be selected through LC_ALL, so the test correctly gives UNRESOLVED (XBD 7, 8.2 require the pathname form). |
| XBD.7.3.5-lc-time.008 | UNRESOLVED | SYSTEM | Blocked by the pathname-locale failure (XBD.7.1-locale-general.001): the locale made by localedef cannot be selected through LC_ALL, so the test correctly gives UNRESOLVED (XBD 7, 8.2 require the pathname form). |
| XBD.7.3.5-lc-time.009 | UNRESOLVED | SYSTEM | Blocked by the pathname-locale failure (XBD.7.1-locale-general.001): the locale made by localedef cannot be selected through LC_ALL, so the test correctly gives UNRESOLVED (XBD 7, 8.2 require the pathname form). |
| XBD.7.3.6-lc-messages.001 | UNRESOLVED | SYSTEM | Blocked by the pathname-locale failure (XBD.7.1-locale-general.001): the locale made by localedef cannot be selected through LC_ALL, so the test correctly gives UNRESOLVED (XBD 7, 8.2 require the pathname form). |
| XBD.7.4-locale-grammar.001 | UNRESOLVED | SYSTEM | Blocked by the pathname-locale failure (XBD.7.1-locale-general.001): the locale made by localedef cannot be selected through LC_ALL, so the test correctly gives UNRESOLVED (XBD 7, 8.2 require the pathname form). |
| XBD.7.4-locale-grammar.002 | UNRESOLVED | SYSTEM | Blocked by the pathname-locale failure (XBD.7.1-locale-general.001): the locale made by localedef cannot be selected through LC_ALL, so the test correctly gives UNRESOLVED (XBD 7, 8.2 require the pathname form). |
| XBD.7.4-locale-grammar.003 | UNRESOLVED | SYSTEM | Blocked by the pathname-locale failure (XBD.7.1-locale-general.001): the locale made by localedef cannot be selected through LC_ALL, so the test correctly gives UNRESOLVED (XBD 7, 8.2 require the pathname form). |
| XBD.7.4-locale-grammar.004 | UNRESOLVED | SYSTEM | Blocked by the pathname-locale failure (XBD.7.1-locale-general.001): the locale made by localedef cannot be selected through LC_ALL, so the test correctly gives UNRESOLVED (XBD 7, 8.2 require the pathname form). |
| XBD.7.4-locale-grammar.005 | UNRESOLVED | SYSTEM | Blocked by the pathname-locale failure (XBD.7.1-locale-general.001): the locale made by localedef cannot be selected through LC_ALL, so the test correctly gives UNRESOLVED (XBD 7, 8.2 require the pathname form). |
| XBD.7.4-locale-grammar.006 | UNRESOLVED | SYSTEM | Blocked by the pathname-locale failure (XBD.7.1-locale-general.001): the locale made by localedef cannot be selected through LC_ALL, so the test correctly gives UNRESOLVED (XBD 7, 8.2 require the pathname form). |
| XBD.8.1-environment-variable-definition.004 | FAIL | STD-AMBIG | readonly LINENO is accepted and the shell (dash) then no longer updates LINENO; the three alternatives of XBD 8 (reject / shell still modifies / built-in that would modify fails) are all open to the reading that the third holds vacuously for LINENO, which no built-in modifies. Marked disputed, Q-204 added; now INSPECT. |
| XBD.8.2-internationalization-variables.001 | UNRESOLVED | SYSTEM | Blocked by the pathname-locale failure (XBD.7.1-locale-general.001): the locale made by localedef cannot be selected through LC_ALL, so the test correctly gives UNRESOLVED (XBD 7, 8.2 require the pathname form). |
| XBD.8.2-internationalization-variables.002 | UNRESOLVED | SYSTEM | Blocked by the pathname-locale failure (XBD.7.1-locale-general.001): the locale made by localedef cannot be selected through LC_ALL, so the test correctly gives UNRESOLVED (XBD 7, 8.2 require the pathname form). |
| XBD.8.2-internationalization-variables.003 | UNRESOLVED | SYSTEM | Blocked by the pathname-locale failure (XBD.7.1-locale-general.001): the locale made by localedef cannot be selected through LC_ALL, so the test correctly gives UNRESOLVED (XBD 7, 8.2 require the pathname form). |
| XBD.8.2-internationalization-variables.006 | FAIL | SYSTEM | Same cause: date ignores a pathname locale named by LC_ALL/LC_TIME/LANG (XBD 8.2). |
| XBD.9.3-bre.006 | UNRESOLVED | ENV | regcomp() ran out of memory (REG_ESPACE) for a count of 32767 under the 2 GiB test memory cap (without the cap the process is killed by SIGKILL, i.e. the OOM killer); REG_ESPACE is an allowed answer, so UNRESOLVED is right: resource limit of the machine. |
| XBD.9.4-ere.007 | UNRESOLVED | ENV | regcomp() ran out of memory (REG_ESPACE) for a count of 32767 under the 2 GiB test memory cap (without the cap the process is killed by SIGKILL, i.e. the OOM killer); REG_ESPACE is an allowed answer, so UNRESOLVED is right: resource limit of the machine. |
| XBD.aio.h.008 | FAIL | SYSTEM | lio_listio() is declared with "struct aiocb *const list[restrict]", without restrict on the element pointer; the page has "struct aiocb *restrict const list[restrict]" (the function types are not compatible). |
| XBD.complex.h.006 | FAIL | SYSTEM | The compiler defines __STDC_IEC_559_COMPLEX__ but no imaginary/_Imaginary_I; XBD <complex.h> [MXC] and XBD 1.8.1: the option is mandated for implementations that define that macro. |
| XBD.dirent.h.001 | UNRESOLVED | SYSTEM | DT_* constants are not visible with _POSIX_C_SOURCE=202405L (XBD <dirent.h>). glibc 2.39 header (Issue 7 level). |
| XBD.dirent.h.002 | UNRESOLVED | SYSTEM | Blocked by the missing DT_* constants and posix_getdents (XBD <dirent.h>). glibc 2.39 header (Issue 7 level). |
| XBD.dirent.h.003 | FAIL | SYSTEM | <dirent.h> does not define ino_t (XBD <dirent.h>: defines ino_t, size_t, ssize_t). glibc 2.39 header (Issue 7 level). |
| XBD.dirent.h.004 | FAIL | SYSTEM | <dirent.h> does not define reclen_t (new in Issue 8). glibc 2.39 header (Issue 7 level). |
| XBD.dirent.h.005 | FAIL | SYSTEM | <dirent.h> does not define ino_t, needed for d_ino. glibc 2.39 header (Issue 7 level). |
| XBD.dirent.h.006 | FAIL | SYSTEM | No struct posix_dent (new in Issue 8). glibc 2.39 header (Issue 7 level). |
| XBD.dirent.h.008 | FAIL | SYSTEM | posix_getdents() is not declared (new in Issue 8). glibc 2.39 header (Issue 7 level). |
| XBD.dlfcn.h.002 | FAIL | SYSTEM | Dl_info_t is not defined (new in Issue 8). glibc 2.39 header (Issue 7 level). |
| XBD.dlfcn.h.004 | FAIL | SYSTEM | dladdr() is not declared (new in Issue 8). glibc 2.39 header (Issue 7 level). |
| XBD.fcntl.h.002 | FAIL | SYSTEM | FD_CLOFORK not defined (new in Issue 8). glibc 2.39 header (Issue 7 level). |
| XBD.fcntl.h.003 | UNRESOLVED | SYSTEM | Blocked by XBD.fcntl.h.002. glibc 2.39 header (Issue 7 level). |
| XBD.fcntl.h.005 | FAIL | SYSTEM | O_CLOFORK not defined. glibc 2.39 header (Issue 7 level). |
| XBD.fcntl.h.006 | FAIL | SYSTEM | F_OFD_GETLK/SETLK/SETLKW are not visible under the strict build. glibc 2.39 header (Issue 7 level). |
| XBD.fcntl.h.007 | FAIL | SYSTEM | O_TTY_INIT not defined. glibc 2.39 header (Issue 7 level). |
| XBD.fcntl.h.008 | FAIL | SYSTEM | F_GETOWN_EX/F_SETOWN_EX are not visible under the strict build. glibc 2.39 header (Issue 7 level). |
| XBD.fcntl.h.011 | UNRESOLVED | SYSTEM | Blocked by the missing O_TTY_INIT (and O_CLOFORK). glibc 2.39 header (Issue 7 level). |
| XBD.fnmatch.h.002 | UNRESOLVED | SYSTEM | FNM_IGNORECASE (and FNM_CASEFOLD in the strict build) not defined; XBD <fnmatch.h>. glibc 2.39 header (Issue 7 level). |
| XBD.ftw.h.002 | UNRESOLVED | SYSTEM | FTW_PHYS, FTW_MOUNT, FTW_DEPTH, FTW_CHDIR are enumeration constants only, FTW_XDEV is missing; XBD 3 "Symbolic Constant" requires macros. glibc 2.39 header (Issue 7 level). |
| XBD.inttypes.h.006 | FAIL | SYSTEM | <inttypes.h> does not define wchar_t (page: as described in <stddef.h>). glibc 2.39 header (Issue 7 level). |
| XBD.inttypes.h.007 | FAIL | SYSTEM | Same: wchar_t is not defined by <inttypes.h>. glibc 2.39 header (Issue 7 level). |
| XBD.langinfo.h.003 | FAIL | SYSTEM | ALTMON_* items not defined (new in Issue 8). glibc 2.39 header (Issue 7 level). |
| XBD.langinfo.h.004 | FAIL | SYSTEM | ALTMON_* items not defined. glibc 2.39 header (Issue 7 level). |
| XBD.limits.h.004 | FAIL | SYSTEM | _XOPEN_IOV_MAX expands to an undeclared name instead of 16; _XOPEN_NAME_MAX/_XOPEN_PATH_MAX are missing (XSI values 16, 255, 1024). glibc 2.39 header (Issue 7 level). |
| XBD.limits.h.007 | FAIL | SYSTEM | LONG_BIT/WORD_BIT are not defined with _POSIX_C_SOURCE=202405L (they are base, [CX]). glibc 2.39 header (Issue 7 level). |
| XBD.limits.h.008 | FAIL | SYSTEM | Same: LONG_BIT/WORD_BIT not defined in the base build. glibc 2.39 header (Issue 7 level). |
| XBD.limits.h.011 | FAIL | SYSTEM | HOST_NAME_MAX is 64, below its minimum acceptable value {_POSIX_HOST_NAME_MAX} (255). glibc 2.39 header (Issue 7 level). |
| XBD.limits.h.014 | FAIL | SYSTEM | GETENTROPY_MAX not defined (new in Issue 8). glibc 2.39 header (Issue 7 level). |
| XBD.locale.h.006 | FAIL | SYSTEM | getlocalename_l() not declared (new in Issue 8). glibc 2.39 header (Issue 7 level). |
| XBD.math.h.014 | FAIL | SYSTEM | M_EGAMMA not defined (new in Issue 8). glibc 2.39 header (Issue 7 level). |
| XBD.math.h.015 | FAIL | SYSTEM | M_El and the other long double constants not defined. glibc 2.39 header (Issue 7 level). |
| XBD.netdb.h.002 | FAIL | SYSTEM | NI_NUMERICSCOPE not defined under the strict build. glibc 2.39 header (Issue 7 level). |
| XBD.netinet_in.h.002 | FAIL | STD-AMBIG | IPPROTO_ICMP and SOL_SOCKET are both 1; that the levels differ is only inferred (Q-169, already listed). Marked disputed; now INSPECT. |
| XBD.netinet_in.h.003 | UNRESOLVED | STD-AMBIG | Blocked by XBD.netinet_in.h.002 (now INSPECT); same Q-169 inference. |
| XBD.netinet_in.h.004 | UNRESOLVED | STD-AMBIG | Blocked by XBD.netinet_in.h.002 (now INSPECT); same Q-169 inference. |
| XBD.poll.h.007 | FAIL | SYSTEM | <poll.h> does not define sigset_t (page: as described in <signal.h>). glibc 2.39 header (Issue 7 level). |
| XBD.poll.h.008 | FAIL | SYSTEM | <poll.h> does not define struct timespec (page: as described in <time.h>). glibc 2.39 header (Issue 7 level). |
| XBD.poll.h.009 | FAIL | SYSTEM | ppoll() not declared under the strict build. glibc 2.39 header (Issue 7 level). |
| XBD.pthread.h.004 | UNRESOLVED | SYSTEM | PTHREAD_MUTEX_NORMAL/ERRORCHECK/RECURSIVE are enumeration constants only, not macros (XBD 3 "Symbolic Constant"). glibc 2.39 header (Issue 7 level). |
| XBD.pthread.h.005 | UNRESOLVED | SYSTEM | PTHREAD_MUTEX_ROBUST/STALLED are enumeration constants only. glibc 2.39 header (Issue 7 level). |
| XBD.pthread.h.009 | UNRESOLVED | SYSTEM | PTHREAD_PRIO_* are enumeration constants only. glibc 2.39 header (Issue 7 level). |
| XBD.pthread.h.012 | UNRESOLVED | SYSTEM | PTHREAD_NULL not defined (new in Issue 8). glibc 2.39 header (Issue 7 level). |
| XBD.pthread.h.015 | FAIL | SYSTEM | pthread_cond_clockwait() and the other clock functions are not declared. glibc 2.39 header (Issue 7 level). |
| XBD.regex.h.002 | FAIL | SYSTEM | REG_MINIMAL not defined (new in Issue 8). glibc 2.39 header (Issue 7 level). |
| XBD.regex.h.005 | FAIL | SYSTEM | regoff_t is int, which cannot hold PTRDIFF_MAX; the page requires a type that can. glibc 2.39 header (Issue 7 level). |
| XBD.semaphore.h.004 | FAIL | SYSTEM | <semaphore.h> does not define O_CREAT/O_EXCL. glibc 2.39 header (Issue 7 level). |
| XBD.semaphore.h.005 | FAIL | SYSTEM | sem_clockwait() not declared. glibc 2.39 header (Issue 7 level). |
| XBD.signal.h.006 | FAIL | SYSTEM | SIGRTMIN..SIGRTMAX has 31 signals while {RTSIG_MAX} is 32; the page requires the range to include at least {RTSIG_MAX} signals. glibc 2.39 header (Issue 7 level). |
| XBD.signal.h.007 | FAIL | SYSTEM | _SC_NSIG not defined. glibc 2.39 header (Issue 7 level). |
| XBD.signal.h.008 | FAIL | SYSTEM | SIG2STR_MAX not defined (new in Issue 8). glibc 2.39 header (Issue 7 level). |
| XBD.signal.h.028 | FAIL | SYSTEM | sig2str() not declared. glibc 2.39 header (Issue 7 level). |
| XBD.spawn.h.001 | UNRESOLVED | SYSTEM | POSIX_SPAWN_SETSID is not visible in the strict build. glibc 2.39 header (Issue 7 level). |
| XBD.spawn.h.007 | FAIL | SYSTEM | posix_spawn_file_actions_addchdir() and addfchdir() not declared. glibc 2.39 header (Issue 7 level). |
| XBD.stdlib.h.007 | FAIL | SYSTEM | mkostemp(), qsort_r(), secure_getenv(), reallocarray() not declared under the strict build. glibc 2.39 header (Issue 7 level). |
| XBD.stdlib.h.008 | FAIL | SYSTEM | ptsname_r() not declared. glibc 2.39 header (Issue 7 level). |
| XBD.stdlib.h.009 | FAIL | SYSTEM | realpath() ([CX], base) not declared under _POSIX_C_SOURCE=202405L. glibc 2.39 header (Issue 7 level). |
| XBD.sys_mman.h.011 | FAIL | SYSTEM | <sys/mman.h> does not define O_RDONLY/O_RDWR (required with SHM/TYM; the system claims shared memory objects). glibc 2.39 header (Issue 7 level). |
| XBD.sys_mman.h.013 | FAIL | SYSTEM | <sys/mman.h> does not define O_CREAT/O_EXCL/O_TRUNC. glibc 2.39 header (Issue 7 level). |
| XBD.sys_sem.h.003 | FAIL | SYSTEM | sem_nsems is unsigned long; the page has unsigned short. glibc 2.39 header (Issue 7 level). |
| XBD.sys_shm.h.003 | UNRESOLVED | SYSTEM | SHM_FAILED not defined. glibc 2.39 header (Issue 7 level). |
| XBD.sys_socket.h.006 | FAIL | SYSTEM | SOCK_CLOFORK not defined. glibc 2.39 header (Issue 7 level). |
| XBD.sys_socket.h.007 | FAIL | SYSTEM | SO_DOMAIN and SO_PROTOCOL not visible under the strict build. glibc 2.39 header (Issue 7 level). |
| XBD.sys_socket.h.008 | FAIL | SYSTEM | MSG_CMSG_CLOFORK not defined. glibc 2.39 header (Issue 7 level). |
| XBD.sys_socket.h.016 | FAIL | SYSTEM | CMSG_NXTHDR(mhdr, NULL) crashes (SIGSEGV); XBD <sys/socket.h>: a null second argument makes it equivalent to CMSG_FIRSTHDR(mhdr). glibc 2.39 header (Issue 7 level). |
| XBD.sys_socket.h.018 | FAIL | SYSTEM | msg_iovlen is size_t; the page has int. glibc 2.39 header (Issue 7 level). |
| XBD.sys_socket.h.020 | FAIL | SYSTEM | cmsg_len is size_t; the page has socklen_t. glibc 2.39 header (Issue 7 level). |
| XBD.sys_socket.h.024 | FAIL | SYSTEM | accept4() not declared. glibc 2.39 header (Issue 7 level). |
| XBD.sys_stat.h.004 | UNRESOLVED | SYSTEM | S_IFBLK, S_IFCHR, ... are not visible with _POSIX_C_SOURCE=202405L (base in Issue 8). glibc 2.39 header (Issue 7 level). |
| XBD.sys_stat.h.005 | UNRESOLVED | SYSTEM | Blocked by the missing S_IF* constants. glibc 2.39 header (Issue 7 level). |
| XBD.sys_types.h.007 | FAIL | SYSTEM | reclen_t not defined. glibc 2.39 header (Issue 7 level). |
| XBD.sys_types.h.008 | FAIL | SYSTEM | suseconds_t not defined. glibc 2.39 header (Issue 7 level). |
| XBD.sys_types.h.009 | FAIL | SYSTEM | suseconds_t (and reclen_t) not defined. glibc 2.39 header (Issue 7 level). |
| XBD.sys_types.h.011 | FAIL | SYSTEM | reclen_t not defined. glibc 2.39 header (Issue 7 level). |
| XBD.sys_wait.h.006 | UNRESOLVED | SYSTEM | WCOREDUMP not defined under the strict build. glibc 2.39 header (Issue 7 level). |
| XBD.termios.h.015 | FAIL | SYSTEM | struct winsize not defined in <termios.h>. glibc 2.39 header (Issue 7 level). |
| XBD.termios.h.017 | FAIL | SYSTEM | tcgetwinsize()/tcsetwinsize() not declared. glibc 2.39 header (Issue 7 level). |
| XBD.unistd.h.001 | FAIL | SYSTEM | POSIX_CLOSE_RESTART not defined. glibc 2.39 header (Issue 7 level). |
| XBD.unistd.h.002 | FAIL | SYSTEM | SEEK_HOLE and SEEK_DATA are not macros in the strict build. glibc 2.39 header (Issue 7 level). |
| XBD.unistd.h.004 | FAIL | SYSTEM | _POSIX_VERSION is 200809L, not 202405L. glibc 2.39 header (Issue 7 level). |
| XBD.unistd.h.005 | FAIL | SYSTEM | _XOPEN_VERSION is 700, not 800. glibc 2.39 header (Issue 7 level). |
| XBD.unistd.h.006 | FAIL | SYSTEM | _POSIX_ADVISORY_INFO is 200809L (Issue 8: 202405L, 0 or -1). glibc 2.39 header (Issue 7 level). |
| XBD.unistd.h.007 | FAIL | SYSTEM | _POSIX_ASYNCHRONOUS_IO is 200809L, not 202405L. glibc 2.39 header (Issue 7 level). |
| XBD.unistd.h.010 | FAIL | SYSTEM | _SC_DEVICE_CONTROL not defined. glibc 2.39 header (Issue 7 level). |
| XBD.unistd.h.011 | FAIL | SYSTEM | _POSIX_THREAD_ROBUST_PRIO_INHERIT is 200809L but sysconf(_SC_THREAD_ROBUST_PRIO_INHERIT) is -1. glibc 2.39 header (Issue 7 level). |
| XBD.unistd.h.014 | FAIL | SYSTEM | _SC_XOPEN_UUCP not defined. glibc 2.39 header (Issue 7 level). |
| XBD.unistd.h.015 | FAIL | SYSTEM | _PC_TIMESTAMP_RESOLUTION not defined. glibc 2.39 header (Issue 7 level). |
| XBD.unistd.h.016 | FAIL | SYSTEM | _CS_POSIX_V7_THREADS_CFLAGS (listed on the page) not defined. glibc 2.39 header (Issue 7 level). |
| XBD.unistd.h.019 | FAIL | SYSTEM | _SC_DEVICE_CONTROL not defined. glibc 2.39 header (Issue 7 level). |
| XBD.unistd.h.021 | FAIL | SYSTEM | <unistd.h> does not define O_CLOEXEC. glibc 2.39 header (Issue 7 level). |
| XBD.unistd.h.024 | FAIL | SYSTEM | dup3() not declared. glibc 2.39 header (Issue 7 level). |
| XBD.unistd.h.025 | FAIL | SYSTEM | getresgid()/getresuid() not declared. glibc 2.39 header (Issue 7 level). |
| XBD.utmpx.h.002 | FAIL | SYSTEM | ut_tv is a private structure, not struct timeval. glibc 2.39 header (Issue 7 level). |
| XBD.wchar.h.008 | FAIL | SYSTEM | wcslcat()/wcslcpy() not declared. glibc 2.39 header (Issue 7 level). |
| XBD.wordexp.h.001 | UNRESOLVED | SYSTEM | WRDE_* flags are enumeration constants only (XBD 3 "Symbolic Constant" requires macros). glibc 2.39 header (Issue 7 level). |
| XBD.wordexp.h.002 | UNRESOLVED | SYSTEM | WRDE_* errors are enumeration constants only. glibc 2.39 header (Issue 7 level). |

## Counts

| class | lines |
|---|---|
| TEST-BUG | 13 |
| SYSTEM | 163 |
| STD-AMBIG | 4 |
| ENV | 5 |
| UNKNOWN | 0 |
| total | 185 |

## Changes made (all re-run, no commit)

- `modules/xbd/11.1-interface-characteristics/t/004..007`: leader pid kept in `g_leader` (the checks read `g_sess` after `end_session()` had set it to -1).
- `.../t/008, 009, 010, 012, 013, 014, 015, 016` (shared orphan-group code; only 010 and 015 failed in the run): the middle process hands its own pid down instead of the grandchild calling `getppid()`, which raced with the middle process ending.
- `.../t/031-case-b.c` and the `says`/`note` of `.031`: a read interrupted by the alarm may return the two taken bytes or -1/EINTR (XSH read: interrupted after data was read returns the count); it must not end before the alarm.
- `.../t/035, 036, 037`: handlers identify leader / foreground child / other-group child by pid and process group, not by a variable set after fork.
- `modules/xbd/11.2-terminal-parameters/t/007, 009, 025`: the write made while output is suspended is done by a child (a write may block until START); `reap_wr()` checks it completed after the restart.
- `modules/xbd/2-conformance/t/008-external-names.sh`: UNRESOLVED when c17 is not found (002 carries that finding).
- `8.1-environment-variable-definition.004` and `netinet_in.h.002`: `class disputed` with notes; Q-204 added to `docs/standard-issues.md` (netinet_in.h.002 rests on the existing Q-169). Lint: only the usual "declares no MUTANT" lines.

## Patterns

1. Issue 8 lag of glibc 2.39 (about 95 lines): new interfaces, constants and types missing or hidden under the strict feature macros (`*_CLOFORK`, `posix_getdents`, `reclen_t`, `sig2str`, `*clock*` waits, `ALTMON_*`, `DT_*`, `S_IF*` in the base build, `SO_DOMAIN`, ...), option macros still at 200809L, `_XOPEN_VERSION` 700.
2. Constants that exist only as enumerators (`PTHREAD_MUTEX_*`, `PTHREAD_PRIO_*`, `WRDE_*`, `FTW_*`): XBD 3 defines a symbolic constant as an object-like macro.
3. Member/prototype type mismatches: `msg_iovlen`, `cmsg_len`, `sem_nsems`, `ut_tv`, `regoff_t`, `lio_listio` inner `restrict`.
4. Pathname locales are not accepted by setlocale()/newlocale()/date (XBD 7, 8.2); this alone blocks about 70 localedef-based tests (UNRESOLVED) and fails 7.1-001/002, 7.3.5-005, 8.2-006. glibc localedef also crashes on an initial ellipsis in LC_COLLATE.
5. POSIX locale byte 128: mbrtowc()/btowc() reject it (XBD 6.2: 256 characters); behind 6.2-001 and five more.
6. Test bugs were all in the pseudo-terminal tests (shared copied helper code): a stale variable, two races, a blocking write while output is stopped, an over-strict EINTR expectation.
7. Utilities of the machine: mawk (`%F`, `%a`, `%.s`), find -iname with classes; not installed here: at, batch, crontab, gettext family, lp, pax, time, uuencode family, write, c17, lex.

