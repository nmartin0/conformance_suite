# Second-system triage, group xcu1 (first part of XCU)

Evidence: output of the second-system run (`/tmp/nbsd-res/res-root/out`, mostly empty; the run log gives only the first message line), the Linux run `/tmp/pcl4/root.txt`, the test sources and the standard. Neither system is an oracle.

| ID | second | Linux | class | reason |
|---|---|---|---|---|
| XCU.m4.010 | FAIL | PASS | SYSTEM | sysval gives the wait status (768) instead of the exit value 3; m4 sysval/syscmd text: "exit value of the utility". |
| XCU.m4.011 | FAIL | PASS | SYSTEM | mkstemp is not recognised as a built-in macro (m4 EXTENDED DESCRIPTION, mkstemp). |
| XCU.m4.016 | FAIL | FAIL | SYSTEM | second system lacks the mkstemp macro; Linux m4 writes the diagnostic but exits 0 although the page requires a non-zero final status. |
| XCU.msgfmt.002 | FAIL | FAIL | SYSTEM | -S (in the msgfmt OPTIONS) is rejected; the test follows the page. Linux has no msgfmt. |
| XCU.msgfmt.003 | FAIL | FAIL | SYSTEM | same: -S rejected (msgfmt OPTIONS -S). |
| XCU.msgfmt.005 | FAIL | FAIL | SYSTEM | -c -v (msgfmt OPTIONS -c) not implemented, a consistent entry gives non-zero; Linux has no msgfmt. |
| XCU.ngettext.001 | FAIL | FAIL | SYSTEM | ngettext utility missing on both systems (127); it is a base utility. |
| XCU.ngettext.002 | FAIL | FAIL | SYSTEM | ngettext missing on both. |
| XCU.ngettext.003 | FAIL | FAIL | SYSTEM | ngettext missing on both. |
| XCU.od.003 | FAIL | PASS | SYSTEM | -t a must use only the low seven bits (od EXTENDED DESCRIPTION, type a); byte 0311 must read as I. |
| XCU.od.004 | FAIL | PASS | SYSTEM | -t c must write \v for vertical tab (od type c, XBD 5 escapes); system writes octal. |
| XCU.od.005 | FAIL | PASS | SYSTEM | -j/-N with a leading 0x are hexadecimal (od OPTIONS -j, -N); not honoured. |
| XCU.paste.005 | FAIL | PASS | SYSTEM | paste -s of an empty file must give a lone newline (paste OPTIONS -s). |
| XCU.paste.008 | FAIL | PASS | SYSTEM | with -s an unopenable file must not stop the others, status non-zero (paste CONSEQUENCES OF ERRORS, XCU 1.4). |
| XCU.patch.007 | FAIL | PASS | TEST-BUG | the "no .orig without -b" check is not in the text (only -b is specified). Removed that check and the words in the .asr; Linux re-run PASS. |
| XCU.patch.010 | UNRESOLVED | FAIL | TEST-BUG | -N: whether an ignored hunk is also put in a reject file is not stated ("may" create one, patch EXTENDED DESCRIPTION); removed the .rej check under -N; Linux re-run PASS. The second-system timeout is the prompt of the already-applied case with nobody to answer (ENV, UNRESOLVED already). |
| XCU.pathchk.007 | FAIL | PASS | SYSTEM | -P (pathchk OPTIONS) not supported: status 1 for a good name. |
| XCU.pathchk.009 | FAIL | PASS | SYSTEM | -p -P: -P not supported. |
| XCU.pax.005 | FAIL | FAIL | TEST-BUG | the listing command had a second stdin redirection (</dev/null) after <a, so pax read an empty archive; removed it. Cannot re-run (no pax on this machine); sh -n only. |
| XCU.pax.006 | FAIL | FAIL | SYSTEM | ustar mode field holds 12 bits (ustar Header Block, mode); system stores 0100640 with the file-type bit. |
| XCU.pax.008 | FAIL | FAIL | SYSTEM | -x pax (a required format in pax OPTIONS -x) is refused. |
| XCU.pax.011 | FAIL | UNRESOLVED | SYSTEM | list-mode -v wrote one extra line on standard output (the archive summary); STDOUT of list mode allows only the member lines, and optional messages go to standard error. Evidence: pax.018 output also holds a second line. |
| XCU.pax.015 | FAIL | UNRESOLVED | SYSTEM | -o listopt (pax OPTIONS -o listopt, List Mode Format Specifications) not accepted: non-zero status (probable; the test text is as the page). |
| XCU.pax.018 | FAIL | FAIL | SYSTEM | same extra stdout line in list -v: the first-column check saw "l" followed by another line (pax STDOUT). |
| XCU.pax.021 | FAIL | FAIL | SYSTEM | fails for the pax format (-x pax refused), see pax.008. |
| XCU.pax.022 | FAIL | UNRESOLVED | TEST-BUG | the helper only gets a controlling terminal where opening the terminal makes one (implementation-defined, XBD 11.2.3), so the prompts via /dev/tty failed (exit 1); the test now uses pc_pty_run -c (UNRESOLVED where no controlling terminal). Cannot re-run here (no pax). |
| XCU.pax.025 | FAIL | FAIL | SYSTEM | the S flag of -s must apply the substitution to link contents in write mode (pax OPTIONS -s); not applied (the other -s checks passed). |
| XCU.pax.026 | FAIL | UNRESOLVED | SYSTEM | -p p alone must not set S_ISUID/S_ISGID (pax OPTIONS -p, "if neither e nor o"); the set-id bits were kept. |
| XCU.pax.027 | UNRESOLVED | UNRESOLVED | SYSTEM | setup uses -x pax -o times; -x pax refused (see pax.008). Setup failure gives UNRESOLVED by design. |
| XCU.pax.029 | FAIL | FAIL | SYSTEM | with -L -H the last option must decide (pax OPTIONS -H/-L); -L won. |
| XCU.pax.030 | FAIL | UNRESOLVED | UNKNOWN | pax -rw -v did not give the line f1 on standard error; need the stderr of that call (the failure message now includes it). Possibly the copy-mode name printed differs, which the text does not fix. |
| XCU.pax.031 | FAIL | UNRESOLVED | SYSTEM | extracting a file member over a non-empty directory gave status 0 and no diagnostic; the action cannot be performed (XCU 1.4 CONSEQUENCES OF ERRORS). Caveat: an implementation replacing the directory would also give 0. |
| XCU.pax.035 | FAIL | FAIL | SYSTEM | -x pax refused (see pax.008). |
| XCU.pax.036 | FAIL | FAIL | SYSTEM | -x pax refused. |
| XCU.pax.037 | UNRESOLVED | UNRESOLVED | SYSTEM | setup needs -x pax; refused. |
| XCU.pax.038 | FAIL | FAIL | SYSTEM | -x pax refused (autodetect loop covers it). |
| XCU.pax.039 | UNRESOLVED | UNRESOLVED | SYSTEM | setup needs -x pax; refused. |
| XCU.pax.040 | FAIL | UNRESOLVED | SYSTEM | -o listopt not accepted (see pax.015); empty output. |
| XCU.pax.041 | FAIL | UNRESOLVED | SYSTEM | pax -w -a -u with an older file must succeed and append nothing (pax OPTIONS -u); non-zero status here (probable). |
| XCU.pax.042 | UNRESOLVED | UNRESOLVED | SYSTEM | setup needs -x pax; refused. |
| XCU.pax.043 | FAIL | FAIL | SYSTEM | -x pax refused. |
| XCU.pax.044 | FAIL | FAIL | SYSTEM | -x pax refused. |
| XCU.pax.046 | FAIL | UNRESOLVED | SYSTEM | a 100-character name fills the ustar name field without NUL and must be written (ustar Header Block, name/prefix); refused with non-zero status. |
| XCU.printf.012 | FAIL | FAIL | SYSTEM | %n$ numbered conversions (printf EXTENDED DESCRIPTION, new in 2024) unsupported on both systems. |
| XCU.ps.002 | FAIL | PASS | SYSTEM | default header for tty must be TT (ps Table of default headers); system says TTY. |
| XCU.ps.005 | FAIL | PASS | SYSTEM | -p takes a blank or comma separated list (ps OPTIONS -p); a comma list is refused. |
| XCU.ps.011 | FAIL | PASS | SYSTEM | etime must be [[dd-]hh:]mm:ss with two-digit mm/ss (ps etime); system gives 0:01. |
| XCU.ps.013 | FAIL | PASS | TEST-BUG | depends on the pseudo-terminal helper making the terminal the controlling one, which is implementation-defined; now pc_pty_run -c (UNRESOLVED when not). Linux re-run PASS. |
| XCU.ps.014 | FAIL | PASS | TEST-BUG | same as ps.013. Linux re-run PASS. |
| XCU.pwd.003 | FAIL | FAIL | SYSTEM | pwd -L after cd through a symbolic link must give the logical path (pwd OPTIONS -L); the shell built-in gives the physical one here, and on Linux the external pwd without options does (second-system cut at the first check, Linux at the exec check). |
| XCU.pwd.004 | FAIL | PASS | SYSTEM | pwd -P -L: the last option (-L) must apply (pwd OPTIONS); physical path written. |
| XCU.readlink.004 | FAIL | FAIL | SYSTEM | readlink of a non-link must write a diagnostic (readlink DESCRIPTION); both exit non-zero silently. |
| XCU.readonly.007 | FAIL | PASS | SYSTEM | readonly is a declaration utility in 2024: no field splitting, tilde expanded after = and : (XCU 2.9.1.1); not implemented (output [p][~/x:~/y]). |
| XCU.renice.005 | FAIL | FAIL | SYSTEM | STDOUT is "Not used" for renice; both systems write the old and new priority. |
| XCU.rm.015 | UNRESOLVED | PASS | ENV | "cd: Out of space": the shell cannot hold a working directory path of 5000 bytes (path-length limit) in setup; already UNRESOLVED. |
| XCU.sed.002 | FAIL | PASS | UNKNOWN | only the "-e 1a\ -e added" check can have failed; the standard (sed OPTIONS -e) inserts a newline between -e pieces, so the test looks right, but the actual output is not recorded; the message now includes it. |
| XCU.sed.010 | FAIL | PASS | UNKNOWN | the plain "2a\ AFTER" check failed, which the text clearly allows to work; need the actual output (message now includes it). |
| XCU.sed.012 | FAIL | PASS | UNKNOWN | the l output check failed; the text gives \t, \\ and \001 forms; need the actual output (message now includes it). |
| XCU.sh-errors.002 | FAIL | PASS | SYSTEM | unsetting a read-only variable is a special built-in error: the shell must exit non-zero (XCU 2.8.1); status 0 here. |
| XCU.sh-errors.010 | UNRESOLVED | FAIL | ENV | timed out: the interactive shell started with stdin from a file hung in the guest (likely job-control/terminal interplay); UNRESOLVED already. On Linux the FAIL is the shell discarding the input that followed an error (SYSTEM there). |
| XCU.sh-errors.011 | UNRESOLVED | FAIL | ENV | same hang as sh-errors.010; Linux FAIL is the same discard of later input. |
| XCU.sh-simple-commands.005 | FAIL | PASS | SYSTEM | PCR=2 true with PCR read-only is a variable assignment error: the shell must exit non-zero (XCU 2.8.1, 2.9.1.2); status 0. |
| XCU.sh.008 | FAIL | FAIL | SYSTEM | a missing command_file must give 127 (sh EXIT STATUS); both give 2. |
| XCU.sh.016 | UNRESOLVED | PASS | ENV | timed out: sh -i with a script on stdin hangs in the guest (see sh-errors.010); Linux passes. |
| XCU.sh.017 | FAIL | FAIL | SYSTEM | sh reading commands from a pipe read ahead, so a command reading stdin saw nothing (sh STDIN); both systems. |
| XCU.shift.004 | FAIL | PASS | SYSTEM | shift -1 is an invalid operand: error status required (shift EXIT STATUS); status 0. |
| XCU.sort.005 | FAIL | PASS | SYSTEM | -f ties are broken by comparing whole lines bytewise (sort OPTIONS -k last paragraph): A a B b C; order differs. |
| XCU.sort.015 | FAIL | PASS | SYSTEM | a key with its own modifier (n) does not take the global -r (sort OPTIONS -k); it did. |
| XCU.strings.003 | FAIL | FAIL | SYSTEM | -t d output is "%d %s" with no padding (strings STDOUT); both systems pad the offset. |
| XCU.stty.004 | FAIL | PASS | UNKNOWN | -g round trip: before and after -a differ; do not know how (the message now includes the diff). The text says -g "attempts to apply" the settings; if the difference is in something not settable on a pty this may be over-strict. |
| XCU.stty.008 | FAIL | PASS | SYSTEM | combination mode ek (stty OPERANDS) is refused. |
| XCU.test.009 | FAIL | FAIL | SYSTEM | -nt/-ot with one operand unresolvable must be true (test primaries); false on both. |
| XCU.timeout.003 | FAIL | FAIL | SYSTEM | with -s and the limit reached the status must be 124 (timeout EXIT STATUS); Linux gives 137 for -s KILL, the second system 64 already for -s USR1. |
| XCU.timeout.006 | FAIL | PASS | SYSTEM | a missing utility must give 127 (timeout EXIT STATUS); 69 here. |
| XCU.tput.002 | FAIL | PASS | UNKNOWN | tput init exits 1 while clear works on a known type; the text allows no failure for a type without init. May be due to the terminal having no controlling terminal; stderr of tput needed. |
| XCU.tput.004 | UNRESOLVED | PASS | UNKNOWN | same cause as tput.002 (tput init in setup). |
| XCU.tput.005 | FAIL | PASS | SYSTEM | unknown terminal type must give status 3 (tput EXIT STATUS); gives 1. |
| XCU.tput.006 | FAIL | PASS | SYSTEM | tput without operands is a usage error, status 2 (tput EXIT STATUS); gives 0. |
| XCU.tr.004 | FAIL | PASS | SYSTEM | -C must be like -c in the POSIX locale (tr OPTIONS -C); -c worked, -C result wrong. |
| XCU.trap.017 | FAIL | FAIL | SYSTEM | trap with no operands must list signals ignored on entry to a non-interactive shell (XCU trap DESCRIPTION); omitted on both. |
| XCU.tsort.004 | FAIL | FAIL | SYSTEM | -w (new in 2024) not supported on both. |
| XCU.unalias.005 | FAIL | PASS | SYSTEM | unalias of a name that is not an alias must write a diagnostic (XCU 1.4 CONSEQUENCES OF ERRORS); none here. |
| XCU.unexpand.003 | FAIL | PASS | SYSTEM | with -t blanks inside the line are converted too (unexpand OPTIONS -t); only leading ones were. |
| XCU.uniq.007 | FAIL | PASS | SYSTEM | an input_file of - means standard input (uniq OPERANDS); treated as a file name. |
| XCU.xargs.004 | FAIL | PASS | SYSTEM | without -r and without input the utility runs exactly once (xargs DESCRIPTION); not run. |
| XCU.xargs.006 | FAIL | PASS | SYSTEM | with -0 each of adjacent null bytes is a delimiter, so an empty argument arises (xargs DESCRIPTION); collapsed. |
| XCU.xargs.009 | FAIL | PASS | SYSTEM | with -x a pair of arguments that does not fit in -s must terminate with status 1-125 (xargs OPTIONS -x and the -n 2 -x example); status 0. |

## Counts

- TEST-BUG: 6
- SYSTEM: 70
- STD-AMBIG: 0
- ENV: 4
- UNKNOWN: 7

## TEST-BUG changes

- XCU.patch.007: removed the check that no .orig exists without -b (and the matching words in patch.asr). Re-run on Linux: PASS.
- XCU.patch.010: removed the check that -N leaves no reject file. Re-run: PASS.
- XCU.pax.005: removed the stray `</dev/null` after `<a` on the list command. No pax here to re-run; `sh -n` clean.
- XCU.pax.022, XCU.pax.042 (pty part): `pc_pty_run -c`, so a terminal that did not become controlling gives UNRESOLVED. No pax here; `sh -n` clean.
- XCU.ps.013, XCU.ps.014: `pc_pty_run -c`. Re-run: PASS.
- Diagnostics only (no logic change): sed.002, sed.010, sed.012 (print the output), stty.004 (prints the diff), pax.030 (prints stderr). Re-run: PASS.
- Lint: 6373 problems before and after.

## Patterns

1. Newer (2024) requirements that both systems lack: printf %n$, tsort -w, readonly as declaration utility, xargs -r/-0 details, timeout, trap listing ignored signals, test -nt/-ot, msgfmt -S/-c, ngettext, mkstemp macro, pathchk -P, unalias diagnostic.
2. The second system's pax refuses `-x pax` (about 15 lines, several only UNRESOLVED in setup), writes a summary line on stdout for list -v, keeps file-type bits in the ustar mode field, and lacks -o listopt.
3. Pseudo-terminal tests assume that opening the slave makes it the controlling terminal; that is implementation-defined. Tests that need one now say -c. tput init and stty -g failures may share this cause (UNKNOWN until stderr is seen).
4. Guest hangs with an interactive shell started with stdin from a file (sh.016, sh-errors.010/011): ENV, UNRESOLVED already.
5. Many failing messages hide the actual output; where the cause was unclear the message now carries it.
