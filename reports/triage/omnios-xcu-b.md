# illumos triage, group xcu-b

Counts: TEST-BUG 18, SYSTEM 113, STD-AMBIG 2, ENV 0, UNKNOWN 11.

| ID | result | class | reason |
|---|---|---|---|
| XCU.ls.018 | FAIL | SYSTEM | ls -l total with -k must be in 1024-byte units (ls OPTIONS -k, STDOUT); the total was the same with and without -k |
| XCU.ls.029 | FAIL | SYSTEM | -H makes ls -l describe the file a symbolic link operand names (ls OPTIONS -H); it described the link |
| XCU.ls.031 | FAIL | SYSTEM | -F must not follow a symbolic link operand to a directory (ls OPTIONS -F); the output was not the link name with @ |
| XCU.ls.041 | FAIL | SYSTEM | a set-group-ID file without group execute shows S (ls STDOUT, mode characters); it showed L |
| XCU.ls.042 | FAIL | TEST-BUG | /dev/null is a symbolic link there and ls -l does not follow operand links, so the mode began with l; the test now uses -lH |
| XCU.ls.043 | FAIL | SYSTEM | with -p the target indicator may only be / (ls STDOUT, file type indicator); an executable target got * |
| XCU.ls.046 | FAIL | SYSTEM | -H evaluates a link operand as its target for -S (ls OPTIONS -S, -H); the sort used the size of the link |
| XCU.ls.048 | FAIL | SYSTEM | of -A and -a the last decides (ls OPTIONS, mutually exclusive sets); -a -A still listed dot and dot-dot |
| XCU.m4.006 | FAIL | TEST-BUG | case of digits above 9 in the eval radix result is unspecified; the output had FF where the test wanted ff; the output is now folded |
| XCU.m4.011 | FAIL | SYSTEM | the mkstemp macro is required (m4 EXTENDED DESCRIPTION); m4 left mkstemp(...) as plain text |
| XCU.m4.016 | FAIL | SYSTEM | the mkstemp macro is required (m4 EXTENDED DESCRIPTION); m4 left mkstemp(...) as plain text |
| XCU.mailx.005 | FAIL | SYSTEM | -H writes the whole header summary (mailx OPTIONS); no line for carol was written (run output not available, but the test matches the text) |
| XCU.mailx.006 | FAIL | SYSTEM | next / empty line writes the current message if it has not been written since start (mailx next); message 2 was written first |
| XCU.mailx.015 | FAIL | SYSTEM | a single quote inside double quotes stays literal (mailx Commands, quoting); it did not |
| XCU.mailx.020 | FAIL | SYSTEM | the :r message list selects read messages (mailx msglist); an unread message was shown |
| XCU.mailx.022 | FAIL | SYSTEM | next / empty line writes the current message if it has not been written since start (mailx next); message 2 was written first |
| XCU.mailx.024 | FAIL | SYSTEM | the default command-mode prompt "? " is written (mailx internal variable prompt); none was written with input from a pipe |
| XCU.mailx.026 | FAIL | SYSTEM | ! and pipe call SHELL with -c, -- and the command (mailx ! and pipe); the log shows another argument list |
| XCU.mailx.033 | FAIL | SYSTEM | a null LISTER means ls (mailx ENVIRONMENT VARIABLES, folders); nothing was listed |
| XCU.mailx.035 | FAIL | SYSTEM | with crt below the message length a message goes through PAGER when output is a terminal (mailx PAGER, crt); the pager never ran |
| XCU.mailx.041 | FAIL | SYSTEM | -f -- file must work (XBD 12.2 guidelines; mailx -f takes the file as operand); status was non-zero |
| XCU.mailx.045 | INSPECT | STD-AMBIG | already disputed (Q-216 in the assertion note), so FAIL shows as INSPECT; nothing to change |
| XCU.make.011 | FAIL | SYSTEM | the ::=, :::=, != and ?= macro assignments are required (make Macros); make rejected them |
| XCU.make.014 | FAIL | SYSTEM | an up-to-date target must not be rebuilt through a suffix rule (make Inference Rules); it was rebuilt |
| XCU.make.019 | FAIL | SYSTEM | CURDIR must be provided by make (make Macros); it was empty |
| XCU.make.022 | FAIL | SYSTEM | $^ and $+ are required internal macros (make Internal Macros); $+ was empty |
| XCU.make.024 | FAIL | SYSTEM | .PHONY targets always run and are not touched by -t (make .PHONY); they were treated as files |
| XCU.make.025 | FAIL | SYSTEM | -include of a missing file is ignored and an include line may name several files (make Include Lines); make failed |
| XCU.make.027 | FAIL | SYSTEM | phony targets are not removed on SIGTERM (make ASYNCHRONOUS EVENTS, .PHONY); the target was removed |
| XCU.make.036 | FAIL | SYSTEM | the CURDIR environment variable must not change the CURDIR macro (make Macros); it did |
| XCU.make.037 | FAIL | SYSTEM | SHELL on the command line must not change the SHELL environment variable (make Macros); it did |
| XCU.make.040 | FAIL | SYSTEM | lib.a(m.o) is built by a .x.a rule with the stated macros (make Libraries, Internal Macros); make had no rule for it |
| XCU.make.041 | FAIL | SYSTEM | .NOTPARALLEL makes make update one target at a time (make .NOTPARALLEL); targets overlapped |
| XCU.make.043 | FAIL | SYSTEM | options may be attached to their option-arguments (XBD 12.2, make -f); make -fmakefile failed |
| XCU.make.044 | FAIL | SYSTEM | macro operands in the ::= form are processed in order (make Macros); the ::= operand was empty |
| XCU.make.045 | FAIL | SYSTEM | -include of a missing file is ignored and an include line may name several files (make Include Lines); make failed |
| XCU.make.050 | FAIL | SYSTEM | the ::=, :::=, != and ?= macro assignments are required (make Macros); make rejected them |
| XCU.make.051 | FAIL | SYSTEM | .PHONY targets always run and are not touched by -t (make .PHONY); they were treated as files |
| XCU.msgfmt.002 | FAIL | SYSTEM | -S is a required option (msgfmt OPTIONS); it was refused |
| XCU.msgfmt.003 | FAIL | SYSTEM | -S is a required option (msgfmt OPTIONS); it was refused |
| XCU.msgfmt.005 | FAIL | SYSTEM | -c with -v diagnoses only abnormal entries and exits 0 otherwise (msgfmt -c); a consistent entry was diagnosed |
| XCU.msgfmt.006 | FAIL | UNKNOWN | either a fuzzy entry was kept without -f (msgfmt -f) or the unspecified .mo format differs between runs of identical input; the run output is not available; settle by running msgfmt twice on one input and comparing |
| XCU.mv.010 | FAIL | SYSTEM | mv of a non-directory to a name ending in a slash is an error (mv DESCRIPTION); it exited 0 |
| XCU.nl.002 | FAIL | SYSTEM | an unnumbered line has no line number and no separator (nl STDOUT); padding was written |
| XCU.od.004 | FAIL | SYSTEM | \v is among the escapes of -t c (od); the system wrote another form |
| XCU.od.005 | FAIL | SYSTEM | -j beyond the end of the input is an error with non-zero status (od -j); status was 0 |
| XCU.od.008 | FAIL | SYSTEM | an unreadable file operand gives non-zero status (od EXIT STATUS); status was 0 |
| XCU.patch.005 | FAIL | SYSTEM | -b -o with an existing outfile makes outfile.orig (patch -b); it was not made |
| XCU.pathchk.002 | FAIL | SYSTEM | a component of exactly NAME_MAX bytes is valid (pathchk DESCRIPTION); it was refused |
| XCU.pathchk.004 | FAIL | SYSTEM | -p allows components of _POSIX_NAME_MAX (14) bytes (pathchk -p); 14 bytes were refused |
| XCU.pathchk.007 | FAIL | SYSTEM | -P diagnoses a component starting with a hyphen (pathchk -P); it did not |
| XCU.pax.011 | FAIL | SYSTEM | -v in list mode writes an ls -l listing with the usual fields (pax STDOUT); fewer than nine fields |
| XCU.pax.023 | FAIL | SYSTEM | -l with -L links to the file a symbolic link names (pax -l); the link was not made to the target |
| XCU.pax.025 | FAIL | SYSTEM | the s and S flags of -s are required (pax -s); -s with S was refused |
| XCU.pax.027 | FAIL | SYSTEM | -p a must not restore the archived access time (pax -p); it did |
| XCU.pax.035 | FAIL | SYSTEM | records from -o keyword:=value go at the beginning of the x header (pax -o); another record (mtime) came first. The test is right |
| XCU.pax.037 | FAIL | SYSTEM | -o gname=v writes a global record that beats the ustar field (pax -o, extended header precedence); the ustar name was shown |
| XCU.pax.040 | FAIL | SYSTEM | %D without a device is a blank (pax list mode format, item 10); it wrote a major,minor pair |
| XCU.pax.041 | FAIL | SYSTEM | -u ignores an older source without an error (pax -u); the status was non-zero |
| XCU.pax.043 | FAIL | TEST-BUG | without linkdata the page leaves open whether hard link data is stored and does not require a typeflag 1 member; the default-case checks are relaxed, the linkdata checks stay |
| XCU.pax.044 | FAIL | SYSTEM | pax format needs a path record for a 304-character name (pax Extended Header); no path record found |
| XCU.pr.001 | FAIL | SYSTEM | page header is "date file Page n" with single blanks, 66-line page (pr STDOUT); the layout differed (a latent zone mismatch between touch and pr was also fixed) |
| XCU.pr.003 | FAIL | SYSTEM | -h replaces the file field in the header with single blanks (pr STDOUT); the header differed (the same zone fix was made) |
| XCU.pr.013 | FAIL | SYSTEM | the header is "%s %s Page %d" (pr STDOUT); two blanks stood between year and file |
| XCU.printf.005 | FAIL | SYSTEM | an operand with a leading 0 is octal (printf OPERANDS); %d 010 did not write 8 |
| XCU.printf.014 | FAIL | SYSTEM | an unconvertible operand gives a diagnostic and non-zero status (printf); the status was 0 |
| XCU.ps.010 | FAIL | TEST-BUG | a shell may run sleep itself, making the background child a copy of the shell with the shell as command name; the test now starts sleep through env |
| XCU.ps.011 | FAIL | SYSTEM | time is [dd-]hh:mm:ss (ps time format); 00:01 was written |
| XCU.pwd.003 | FAIL | SYSTEM | pwd -L writes PWD when it is valid even through a symbolic link (pwd -L); the exec of pwd -L wrote the physical path |
| XCU.read.010 | FAIL | SYSTEM | read -d with an empty delimiter takes NUL as the delimiter (read -d); it exited 1 |
| XCU.read.013 | FAIL | SYSTEM | read assigning a read-only variable is an error with status above 1 (read EXIT STATUS); status was 1 |
| XCU.readlink.004 | FAIL | SYSTEM | readlink writes a diagnostic for a non-link (readlink DESCRIPTION); none was written |
| XCU.readonly.004 | FAIL | SYSTEM | read assigning a read-only variable is an error with status above 1 (read EXIT STATUS); status was 1 |
| XCU.realpath.001 | FAIL | SYSTEM | realpath is a required utility; status 127: it is absent |
| XCU.realpath.002 | FAIL | SYSTEM | realpath is a required utility; status 127: it is absent |
| XCU.realpath.004 | FAIL | SYSTEM | realpath is a required utility; status 127: it is absent |
| XCU.realpath.006 | FAIL | SYSTEM | realpath is a required utility; status 127: it is absent |
| XCU.sed.002 | FAIL | SYSTEM | pieces of -e are joined by a newline, so 1a\ then text works (sed OPTIONS -e); sed failed |
| XCU.sed.006 | FAIL | SYSTEM | the i flag of s is required (sed s flags); it did not match case-insensitively |
| XCU.sed.010 | FAIL | UNKNOWN | the note holds only the first line of the multi-line result, so the failing part of the a/i/c/r/w/= checks cannot be seen; needs the full "got" string of the run |
| XCU.sed.012 | FAIL | SYSTEM | l writes a backslash as two backslashes (sed l); it wrote one |
| XCU.set.005 | FAIL | SYSTEM | with set -a a value assigned by ${x:=v} gets the export attribute (set -a); it was not exported |
| XCU.sh-errors.002 | FAIL | SYSTEM | unset of a read-only variable is an error of a special built-in that ends a non-interactive shell (2.8.1, unset); status 0 |
| XCU.sh-errors.003 | FAIL | SYSTEM | the same errors do not end the shell through command (2.8.1 note 1); the shell exited |
| XCU.sh-errors.010 | FAIL | SYSTEM | an interactive shell goes on after a syntax error (2.8.1); it did not |
| XCU.sh-jobs.002 | FAIL | SYSTEM | an interactive shell writes "[n] pid" for a background job (2.9.3.1); the line was not found although the pid was read |
| XCU.sh-signals.003 | FAIL | SYSTEM | wait returns above 128 when a trapped signal arrives (2.12 wait); it returned 1 |
| XCU.sh-simple-commands.005 | FAIL | SYSTEM | an assignment to a read-only variable before a command is an error that ends a non-interactive shell (2.9.1.2); it went on |
| XCU.sh.036 | FAIL | UNKNOWN | pseudo-terminal tests of vi editing mode, the history file and MAILPATH; each matches the text, but the result notes hold only the first line of the run, so a shell defect cannot be told from a terminal problem; needs the pty.out of a rerun |
| XCU.sh.043 | FAIL | UNKNOWN | pseudo-terminal tests of vi editing mode, the history file and MAILPATH; each matches the text, but the result notes hold only the first line of the run, so a shell defect cannot be told from a terminal problem; needs the pty.out of a rerun |
| XCU.sh.047 | FAIL | UNKNOWN | pseudo-terminal tests of vi editing mode, the history file and MAILPATH; each matches the text, but the result notes hold only the first line of the run, so a shell defect cannot be told from a terminal problem; needs the pty.out of a rerun |
| XCU.sh.052 | FAIL | UNKNOWN | pseudo-terminal tests of vi editing mode, the history file and MAILPATH; each matches the text, but the result notes hold only the first line of the run, so a shell defect cannot be told from a terminal problem; needs the pty.out of a rerun |
| XCU.sh.058 | FAIL | UNKNOWN | pseudo-terminal tests of vi editing mode, the history file and MAILPATH; each matches the text, but the result notes hold only the first line of the run, so a shell defect cannot be told from a terminal problem; needs the pty.out of a rerun |
| XCU.sh.059 | FAIL | UNKNOWN | pseudo-terminal tests of vi editing mode, the history file and MAILPATH; each matches the text, but the result notes hold only the first line of the run, so a shell defect cannot be told from a terminal problem; needs the pty.out of a rerun |
| XCU.sh.063 | FAIL | UNKNOWN | pseudo-terminal tests of vi editing mode, the history file and MAILPATH; each matches the text, but the result notes hold only the first line of the run, so a shell defect cannot be told from a terminal problem; needs the pty.out of a rerun |
| XCU.shift.004 | FAIL | SYSTEM | shift with a non-numeric operand is an error (shift); it returned 0 |
| XCU.sort.010 | FAIL | SYSTEM | -C is required (sort OPTIONS); status 2 for disorder instead of 1 |
| XCU.strings.003 | FAIL | SYSTEM | -t o writes the offset as "%o %s" (strings STDOUT); the output had other padding |
| XCU.stty.004 | FAIL | TEST-BUG | the -g string need only be applied again and the report holds items of its own (stty -g); the whole -a report was compared; the test now compares the settings it changed and the control characters, and the characters the page allows |
| XCU.stty.008 | FAIL | SYSTEM | nl disables icrnl (stty combination modes); -icrnl was not shown |
| XCU.stty.009 | FAIL | SYSTEM | size writes rows and columns (stty size); it wrote nothing |
| XCU.stty.011 | FAIL | SYSTEM | -a shows the delay styles that were set (stty -a); cr0 was not shown |
| XCU.stty.012 | FAIL | SYSTEM | raw disables erase, kill and the other characters and -a shows <undef> (stty raw, STDOUT); they were left enabled or shown differently |
| XCU.tabs.004 | FAIL | SYSTEM | the lists 1,10,20,30 and "1 10 +10 +10" are identical (tabs OPERANDS); the outputs differ |
| XCU.test.005 | FAIL | SYSTEM | < and > string comparisons are required (test); status 2 |
| XCU.test.008 | FAIL | SYSTEM | -x is false for a file with no execute bit even for a privileged user (XBD 4.7, test -x); the utility said true |
| XCU.timeout.003 | FAIL | TEST-BUG | the signal may go to the group of timeout itself, so with -s KILL timeout can be killed too (timeout DESCRIPTION); the test now accepts a death by the signal as well as 124 |
| XCU.times.001 | FAIL | SYSTEM | times writes two lines of NmN.NNs NmN.NNs (times STDOUT); it wrote labelled lines |
| XCU.tput.004 | FAIL | SYSTEM | several operands give their outputs in order (tput); the output differed |
| XCU.trap.010 | FAIL | SYSTEM | trap -p lists conditions in trap -- form (trap); no line for USR1 |
| XCU.trap.011 | INSPECT | STD-AMBIG | already disputed (note in the assertion): the page does not say what -p writes for a condition in the default state; FAIL shows as INSPECT; nothing to change |
| XCU.trap.012 | FAIL | SYSTEM | trap -p lists conditions in trap -- form (trap); no line for USR1 |
| XCU.trap.015 | FAIL | TEST-BUG | the subshell need not be a process of its own, and then the signal went to the main shell; the test is now UNRESOLVED in that case and fails only when the trap ran in a subshell process |
| XCU.trap.017 | FAIL | SYSTEM | a signal ignored on entry appears in the trap listing (trap); it did not |
| XCU.tsort.004 | FAIL | SYSTEM | -w is required (tsort); refused |
| XCU.ulimit.004 | FAIL | TEST-BUG | a soft file size limit of 0 stopped the shell from writing a temporary file for the command substitution (SIGXFSZ); the tests now use 100 |
| XCU.ulimit.005 | FAIL | TEST-BUG | a soft file size limit of 0 stopped the shell from writing a temporary file for the command substitution (SIGXFSZ); the tests now use 100 |
| XCU.ulimit.006 | FAIL | TEST-BUG | a soft file size limit of 0 stopped the shell from writing a temporary file for the command substitution (SIGXFSZ); the tests now use 100 |
| XCU.unalias.005 | FAIL | SYSTEM | an error condition gets a diagnostic (1.4); none was written for a name that is not an alias |
| XCU.unalias.006 | FAIL | SYSTEM | unalias removes the alias in the environment where it runs (unalias DESCRIPTION); it did not in the subshell |
| XCU.unlink.001 | FAIL | SYSTEM | unlink is a required utility (XSI claimed); status 127: it is absent |
| XCU.unlink.003 | FAIL | SYSTEM | unlink is a required utility (XSI claimed); status 127: it is absent |
| XCU.uudecode.011 | FAIL | SYSTEM | -o - sends the output to standard output (uudecode -o); it did not |
| XCU.vi.009 | FAIL | TEST-BUG | the scripts include a failing command (a write to a read-only file, failed searches, an ex line past the last line), so a non-zero status is allowed (vi EXIT STATUS); the status check is removed |
| XCU.vi.016 | FAIL | SYSTEM | ^F/^B move the cursor by the stated rule (vi Page Forward/Backwards); ^B put the cursor on another line |
| XCU.vi.018 | FAIL | SYSTEM | ^E/^Y leave the cursor unless it leaves the display (vi Scroll by Line); the cursor went elsewhere |
| XCU.vi.029 | FAIL | TEST-BUG | the scripts include a failing command (a write to a read-only file, failed searches, an ex line past the last line), so a non-zero status is allowed (vi EXIT STATUS); the status check is removed |
| XCU.vi.036 | FAIL | UNKNOWN | no step is plainly wrong in the test; vi.036 exits 1 without an evident failing command, vi.047 loses one of two inserted lines; needs the screen output of the run |
| XCU.vi.047 | FAIL | UNKNOWN | no step is plainly wrong in the test; vi.036 exits 1 without an evident failing command, vi.047 loses one of two inserted lines; needs the screen output of the run |
| XCU.vi.056 | FAIL | TEST-BUG | the scripts include a failing command (a write to a read-only file, failed searches, an ex line past the last line), so a non-zero status is allowed (vi EXIT STATUS); the status check is removed |
| XCU.vi.062 | FAIL | SYSTEM | buffer and count may come in either order (vi, buffer); one order was not accepted |
| XCU.what.001 | FAIL | SYSTEM | what is required with XSI; status 127: it is absent |
| XCU.what.002 | FAIL | SYSTEM | what is required with XSI; status 127: it is absent |
| XCU.what.003 | FAIL | SYSTEM | what is required with XSI; status 127: it is absent |
| XCU.what.004 | FAIL | SYSTEM | what is required with XSI; status 127: it is absent |
| XCU.xargs.004 | FAIL | SYSTEM | without -r the utility runs once for empty input (xargs); it did not run |
| XCU.xargs.006 | FAIL | SYSTEM | with -0 adjacent nulls give an empty argument (xargs -0); it was dropped |
| XCU.xgettext.001 | FAIL | TEST-BUG | the directive and its value are separated by one or more blanks (msgfmt EXTENDED DESCRIPTION), the tests looked for exactly one; probable cause (run output not available); the tests now accept several blanks |
| XCU.xgettext.003 | FAIL | TEST-BUG | the directive and its value are separated by one or more blanks (msgfmt EXTENDED DESCRIPTION), the tests looked for exactly one; probable cause (run output not available); the tests now accept several blanks |
| XCU.xgettext.004 | FAIL | SYSTEM | -K is required (xgettext OPTIONS); refused |
| XCU.xgettext.005 | FAIL | TEST-BUG | the directive and its value are separated by one or more blanks (msgfmt EXTENDED DESCRIPTION), the tests looked for exactly one; probable cause (run output not available); the tests now accept several blanks |
| XCU.xgettext.006 | FAIL | TEST-BUG | the directive and its value are separated by one or more blanks (msgfmt EXTENDED DESCRIPTION), the tests looked for exactly one; probable cause (run output not available); the tests now accept several blanks |
| XCU.xgettext.007 | FAIL | SYSTEM | -x is required (xgettext OPTIONS); refused |
| XCU.xgettext.008 | FAIL | TEST-BUG | the directive and its value are separated by one or more blanks (msgfmt EXTENDED DESCRIPTION), the tests looked for exactly one; probable cause (run output not available); the tests now accept several blanks |
