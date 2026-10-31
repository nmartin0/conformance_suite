# illumos triage, group xcu-a

Counts: TEST-BUG 3, SYSTEM 144, STD-AMBIG 0, ENV 0, UNKNOWN 3.

| ID | result | class | reason |
|---|---|---|---|
| XCU.1.2-utility-limits.005 | FAIL | TEST-BUG | bc STDOUT: a value below one may be written with or without a leading zero (bc STDOUT); test compared exactly. Fixed (strip a leading 0 before the radix point). Commit 16beac94 |
| XCU.1.2-utility-limits.006 | FAIL | SYSTEM | Utility Limits: pathchk (no -p) must accept 16 components of 14 bytes when getconf NAME_MAX . is at least 14 (which the test checks first); it rejected a 14-byte component as too long |
| XCU.1.4-utility-defaults.014 | FAIL | SYSTEM | 1.4 ASYNCHRONOUS EVENTS Default: an ignored SIGTERM/SIGHUP stays ignored through exec; the first process (exec sleep under trap '') was already gone when the signal was sent again, so the ignore was lost (by the shell exec or by sleep) |
| XCU.1.4-utility-defaults.015 | FAIL | SYSTEM | 1.4 Default: a utility started with default SIGTERM is terminated by it; sort on a FIFO ended with status 142 (ALRM) instead of a TERM status |
| XCU.1.4-utility-defaults.020 | FAIL | SYSTEM | od EXIT STATUS: an unreadable operand is an error and the exit status is >0; od of a missing file exited 0 |
| XCU.2.5-parameters.014 | FAIL | SYSTEM | 2.5.3 IFS: unset IFS must leave it unset (and split as space/tab/newline); count 4 was right but the second line differed, so IFS was reported set after unset |
| XCU.2.5-parameters.017 | FAIL | SYSTEM | 2.5.3 PWD: an inherited PWD that is not the current directory (PWD=/) must be replaced by pwd -P; the first case (PWD=/) kept the inherited value |
| XCU.2.5-parameters.020 | FAIL | SYSTEM | 2.5.3 PS1: an interactive shell (sh -i) writes the expanded prompt to standard error; nothing was written when input was not a terminal |
| XCU.2.5-parameters.021 | FAIL | SYSTEM | 2.5.3 PS1 (! history number): no prompt was written to standard error by sh -i, so no numbers appeared |
| XCU.2.5-parameters.022 | FAIL | SYSTEM | 2.5.3 PS2: sh -i must write the expanded PS2 to standard error for a continuation line; nothing was written |
| XCU.2.5-parameters.023 | FAIL | SYSTEM | 2.5.3 PS2 default is '> '; sh -i wrote no continuation prompt |
| XCU.2.6-word-expansions.009 | FAIL | SYSTEM | 2.6.1 tilde: the replacement is treated as quoted (no splitting/globbing) and an empty HOME gives one empty field; the first line (pcg*) was right, a later line (split HOME or empty HOME field count) differed |
| XCU.2.6-word-expansions.035 | FAIL | SYSTEM | 2.6.4 arithmetic: a variable holding an integer constant (with optional sign, octal/hex forms) gives the same value in $((x)) and $(($x)); first case 5 was right, a later form differed |
| XCU.2.7-redirection.006 | FAIL | SYSTEM | 2.7.7 <>: the file is opened read-write on stdin; 'cat <>f' of a file holding abc wrote nothing (first output line was 'created') |
| XCU.2.8-exit-status-and-errors.002 | FAIL | SYSTEM | 2.8.1: eval of a string that cannot be parsed is a shell syntax error (eval EXIT STATUS: non-interactive shell aborts); in a subshell the subshell must end with nonzero status; 'notreached' was written |
| XCU.2.8-exit-status-and-errors.003 | FAIL | SYSTEM | 2.8.1: an error in a special built-in (unset of a readonly variable) makes a non-interactive shell exit; it went on and wrote notreached |
| XCU.ar.002 | FAIL | SYSTEM | ar STDOUT/-t: with file operands the name written is the operand as given (sub/f2); ar wrote the member name f2 |
| XCU.ar.008 | FAIL | SYSTEM | ar STDOUT -tv: format is '%s %u/%u %u ...' with no padding, so uid/gid is one field 0/0; ar padded it, giving 9 fields |
| XCU.at.002 | FAIL | SYSTEM | at STDOUT -l: each job is '%s\t%s' (id, date); at -l wrote an extra 'user = root' field and a second tab |
| XCU.at.003 | FAIL | SYSTEM | at -l job IDs: listing line starts with the extra 'user = root' field so the job ID is not the first tab field (same cause as at.002) |
| XCU.at.004 | FAIL | SYSTEM | at -l: same extra 'user = root' field before the ID, so the job is not found in the listing |
| XCU.at.007 | FAIL | SYSTEM | at -l: same extra 'user = root' field before the ID (queue check could not find the job) |
| XCU.at.008 | FAIL | SYSTEM | at EXIT STATUS: 0 only when all named jobs were removed; at -r of a job that does not exist exited 0 |
| XCU.awk.005 | FAIL | SYSTEM | awk RS: with RS null, newline is a field separator in addition to FS; with FS=: the lines a:b/c:d gave other than 4 fields |
| XCU.awk.008 | FAIL | SYSTEM | awk expressions: length(array) and 'in' on a created element; length of an array (Issue 8, 'if arg is an array') is not supported |
| XCU.awk.009 | FAIL | SYSTEM | awk arrays: length(a) of an array must be the number of elements; wrong |
| XCU.awk.010 | FAIL | SYSTEM | awk exit: an exit with no expression in END keeps the status of the earlier exit 6; status was changed |
| XCU.awk.012 | FAIL | SYSTEM | awk length(array) returns the number of elements; wrong |
| XCU.awk.013 | FAIL | SYSTEM | awk rand/srand: the rand-distinctness check counts keys with length(array); length(array) unsupported (or rand constant); required behaviour not met |
| XCU.awk.014 | FAIL | SYSTEM | awk getline: plain getline returns 1 then 0 at end of input; it did not |
| XCU.awk.015 | FAIL | SYSTEM | awk grammar: 'return' without an expression is allowed and the call yields uninitialized; wrong |
| XCU.awk.017 | FAIL | SYSTEM | awk comparison: a string constant is compared as a string ("10.0"==10 is 0); it was compared as a number |
| XCU.awk.018 | FAIL | SYSTEM | awk CONSEQUENCES OF ERRORS: an inaccessible file operand writes a diagnostic and terminates without further action (no END); awk went on |
| XCU.awk.019 | FAIL | SYSTEM | awk nextfile (Issue 8) must abandon the current file; it did not |
| XCU.awk.027 | FAIL | SYSTEM | awk numeric strings: .5 vs 0.5, 5. vs 5, +.5e1 vs 5 from fields compare numerically; they compared as strings |
| XCU.awk.034 | FAIL | SYSTEM | awk OPTIONS/12.1: a mandatory option-argument may be attached (-vx=1) and implementations must accept it; -vx=1 gave x empty |
| XCU.basename.003 | FAIL | SYSTEM | basename step 3: a string of only slashes (other than exactly //) becomes one slash; '///' wrote '//' |
| XCU.bc.003 | FAIL | TEST-BUG | bc: obase is limited to BC_BASE_MAX (minimum 99), so base 125 need not work; test now runs that case only when getconf BC_BASE_MAX >= 125. Commit 16beac94 |
| XCU.bg.002 | FAIL | UNKNOWN | bg: no bgrc= line reached stdout at all (shell died or stalled after bg?); the stderr and exact stdout of the interactive script would settle it |
| XCU.bg.003 | FAIL | SYSTEM | bg DESCRIPTION: bg of a job already running in the background has no effect and exits successfully; it exited 1 |
| XCU.c17.001 | FAIL | SYSTEM | c17 DESCRIPTION: the utility is required where the C-Language Development option is claimed (the test ran, so the option was reported); c17 is absent from the system (status 127 or failures follow). If CD was forced by a profile rather than reported by the system this is ENV |
| XCU.c17.002 | FAIL | SYSTEM | c17 DESCRIPTION: the utility is required where the C-Language Development option is claimed (the test ran, so the option was reported); c17 is absent from the system (status 127 or failures follow). If CD was forced by a profile rather than reported by the system this is ENV |
| XCU.c17.003 | FAIL | SYSTEM | c17 DESCRIPTION: the utility is required where the C-Language Development option is claimed (the test ran, so the option was reported); c17 is absent from the system (status 127 or failures follow). If CD was forced by a profile rather than reported by the system this is ENV |
| XCU.c17.004 | FAIL | SYSTEM | c17 DESCRIPTION: the utility is required where the C-Language Development option is claimed (the test ran, so the option was reported); c17 is absent from the system (status 127 or failures follow). If CD was forced by a profile rather than reported by the system this is ENV |
| XCU.c17.005 | FAIL | SYSTEM | c17 DESCRIPTION: the utility is required where the C-Language Development option is claimed (the test ran, so the option was reported); c17 is absent from the system (status 127 or failures follow). If CD was forced by a profile rather than reported by the system this is ENV |
| XCU.c17.007 | FAIL | SYSTEM | c17 DESCRIPTION: the utility is required where the C-Language Development option is claimed (the test ran, so the option was reported); c17 is absent from the system (status 127 or failures follow). If CD was forced by a profile rather than reported by the system this is ENV |
| XCU.c17.008 | FAIL | SYSTEM | c17 DESCRIPTION: the utility is required where the C-Language Development option is claimed (the test ran, so the option was reported); c17 is absent from the system (status 127 or failures follow). If CD was forced by a profile rather than reported by the system this is ENV |
| XCU.c17.010 | FAIL | SYSTEM | c17 DESCRIPTION: the utility is required where the C-Language Development option is claimed (the test ran, so the option was reported); c17 is absent from the system (status 127 or failures follow). If CD was forced by a profile rather than reported by the system this is ENV |
| XCU.c17.011 | FAIL | SYSTEM | c17 DESCRIPTION: the utility is required where the C-Language Development option is claimed (the test ran, so the option was reported); c17 is absent from the system (status 127 or failures follow). If CD was forced by a profile rather than reported by the system this is ENV |
| XCU.c17.012 | FAIL | SYSTEM | c17 DESCRIPTION: the utility is required where the C-Language Development option is claimed (the test ran, so the option was reported); c17 is absent from the system (status 127 or failures follow). If CD was forced by a profile rather than reported by the system this is ENV |
| XCU.c17.013 | FAIL | SYSTEM | c17 DESCRIPTION: the utility is required where the C-Language Development option is claimed (the test ran, so the option was reported); c17 is absent from the system (status 127 or failures follow). If CD was forced by a profile rather than reported by the system this is ENV |
| XCU.c17.014 | FAIL | SYSTEM | c17 DESCRIPTION: the utility is required where the C-Language Development option is claimed (the test ran, so the option was reported); c17 is absent from the system (status 127 or failures follow). If CD was forced by a profile rather than reported by the system this is ENV |
| XCU.c17.015 | FAIL | SYSTEM | c17 DESCRIPTION: the utility is required where the C-Language Development option is claimed (the test ran, so the option was reported); c17 is absent from the system (status 127 or failures follow). If CD was forced by a profile rather than reported by the system this is ENV |
| XCU.c17.017 | FAIL | SYSTEM | c17 DESCRIPTION: the utility is required where the C-Language Development option is claimed (the test ran, so the option was reported); c17 is absent from the system (status 127 or failures follow). If CD was forced by a profile rather than reported by the system this is ENV |
| XCU.c17.018 | FAIL | SYSTEM | c17 DESCRIPTION: the utility is required where the C-Language Development option is claimed (the test ran, so the option was reported); c17 is absent from the system (status 127 or failures follow). If CD was forced by a profile rather than reported by the system this is ENV |
| XCU.c17.019 | FAIL | SYSTEM | c17 DESCRIPTION: the utility is required where the C-Language Development option is claimed (the test ran, so the option was reported); c17 is absent from the system (status 127 or failures follow). If CD was forced by a profile rather than reported by the system this is ENV |
| XCU.cd.010 | FAIL | SYSTEM | cd step 3/5: an operand whose first component is . or .. is not searched through CDPATH; cd ./dup used CDPATH |
| XCU.cflow.001 | FAIL | SYSTEM | cflow is required under the XSI option; the system does not provide a working cflow (nonzero status on a plain C file) |
| XCU.cflow.002 | FAIL | SYSTEM | cflow is required under the XSI option; the system does not provide a working cflow (nonzero status on a plain C file) |
| XCU.cflow.003 | FAIL | SYSTEM | cflow is required under the XSI option; the system does not provide a working cflow (nonzero status on a plain C file) |
| XCU.cflow.004 | FAIL | SYSTEM | cflow is required under the XSI option; the system does not provide a working cflow (nonzero status on a plain C file) |
| XCU.cflow.005 | FAIL | SYSTEM | cflow is required under the XSI option; the system does not provide a working cflow (nonzero status on a plain C file) |
| XCU.cflow.006 | FAIL | SYSTEM | cflow is required under the XSI option; the system does not provide a working cflow (nonzero status on a plain C file) |
| XCU.command.002 | FAIL | SYSTEM | command DESCRIPTION: special properties of a special built-in do not occur under command, so a preceding assignment must not persist; it stayed in the shell |
| XCU.command.006 | FAIL | SYSTEM | command -v: an alias is written as a command line representing its definition (name and value); only the quoted value was written |
| XCU.command.012 | FAIL | SYSTEM | command DESCRIPTION: the operand is not subject to alias substitution; the alias was applied |
| XCU.compress.003 | FAIL | SYSTEM | compress OPTIONS (Issue 8): -g and -m gzip/deflate are required options; the system's compress rejects them (exit 1) |
| XCU.compress.004 | FAIL | SYSTEM | compress EXIT STATUS: status 2 when a file would grow and -f is absent; with -g the system exited 1 (option not accepted) |
| XCU.compress.005 | FAIL | SYSTEM | compress OPTIONS (Issue 8): -g and -m gzip/deflate are required options; the system's compress rejects them (exit 1) |
| XCU.compress.007 | FAIL | SYSTEM | compress OPTIONS (Issue 8): -g and -m gzip/deflate are required options; the system's compress rejects them (exit 1) |
| XCU.compress.008 | FAIL | SYSTEM | compress DESCRIPTION: an unsupported -m algorithm leaves the file and exits >2; the system exited 1 |
| XCU.ctags.005 | FAIL | SYSTEM | ctags STDOUT -x: format is '%s %d %s %s' with single spaces; ctags padded the columns |
| XCU.cxref.001 | FAIL | SYSTEM | cxref is required under the XSI option; the system does not provide a working cxref (nonzero status on a plain C file) |
| XCU.cxref.002 | FAIL | SYSTEM | cxref is required under the XSI option; the system does not provide a working cxref (nonzero status on a plain C file) |
| XCU.cxref.003 | FAIL | SYSTEM | cxref is required under the XSI option; the system does not provide a working cxref (nonzero status on a plain C file) |
| XCU.cxref.004 | FAIL | SYSTEM | cxref is required under the XSI option; the system does not provide a working cxref (nonzero status on a plain C file) |
| XCU.cxref.005 | FAIL | SYSTEM | cxref is required under the XSI option; the system does not provide a working cxref (nonzero status on a plain C file) |
| XCU.cxref.006 | FAIL | SYSTEM | cxref is required under the XSI option; the system does not provide a working cxref (nonzero status on a plain C file) |
| XCU.cxref.007 | FAIL | SYSTEM | cxref is required under the XSI option; the system does not provide a working cxref (nonzero status on a plain C file) |
| XCU.dd.016 | FAIL | SYSTEM | dd STDERR: with conv=block a truncated record is reported as 'N truncated record(s)' on stderr; no such line was written |
| XCU.dd.020 | FAIL | SYSTEM | dd OPERANDS (Issue 8): iflags=fullblock is a required operand; dd exited 2 |
| XCU.diff.003 | FAIL | TEST-BUG | diff -c: the text does not say whether the header is written for identical files, only that the exit status is 0; test now checks only that. Commit 16beac94 |
| XCU.diff.004 | FAIL | SYSTEM | diff -u STDOUT: header is '--- %s\t%s%s %s' with date +%Y-%m-%d %H:%M:%S, optional fraction and numeric zone; the system wrote another timestamp form |
| XCU.dot.007 | FAIL | SYSTEM | dot EXIT STATUS / 2.8.1: an unparsable file read by dot makes a non-interactive shell abort with nonzero status; status was 0 |
| XCU.du.009 | FAIL | SYSTEM | du CONSEQUENCES/EXIT STATUS: a nonexistent operand gives a diagnostic and status >0; du exited 0 |
| XCU.echo.007 | FAIL | SYSTEM | echo (XSI) \c: suppresses the newline and ignores all following characters in the arguments, later arguments included; 'ab\c cd' wrote something else |
| XCU.ed.013 | FAIL | SYSTEM | ed q: warns with ? when the buffer was changed by a real append; ed -s quit silently after a/x/. then q |
| XCU.ed.015 | FAIL | SYSTEM | ed l: a dollar sign inside the text is written \$, control bytes as \ooo; output differed from that (first line a\tb$ was fine) |
| XCU.ed.019 | FAIL | SYSTEM | ed addresses: ',2p' (omitted first address means 1) is valid; ed answered ? at that command and stopped |
| XCU.eval.007 | FAIL | SYSTEM | eval EXIT STATUS: a constructed command that cannot be parsed aborts a non-interactive shell (nonzero, no later commands); shell continued with status 0 |
| XCU.ex.002 | FAIL | SYSTEM | ex STDOUT: lines are written as the line text and a newline; the system writes a stray carriage return before many lines (raw CR bytes in the output) (-s ignores TERM, so no terminal handling is allowed to explain it) |
| XCU.ex.006 | FAIL | SYSTEM | ex STDOUT: lines are written as the line text and a newline; the system writes a stray carriage return before many lines (raw CR bytes in the output) (-s ignores TERM, so no terminal handling is allowed to explain it) |
| XCU.ex.008 | FAIL | SYSTEM | ex STDOUT: lines are written as the line text and a newline; the system writes a stray carriage return before many lines (raw CR bytes in the output) (-s ignores TERM, so no terminal handling is allowed to explain it) |
| XCU.ex.019 | FAIL | SYSTEM | ex STDOUT/STDERR: an error message is a diagnostic and goes to stderr; the failed search message went to stdout under -s |
| XCU.ex.020 | FAIL | SYSTEM | ex STDOUT: lines are written as the line text and a newline; the system writes a stray carriage return before many lines (raw CR bytes in the output) (-s ignores TERM, so no terminal handling is allowed to explain it) |
| XCU.ex.021 | FAIL | SYSTEM | ex insert (1addr) on an empty buffer: line zero is allowed for insert; i on an empty buffer failed (exit 1) |
| XCU.ex.022 | FAIL | SYSTEM | ex addressing: a null ?? search reuses the last expression backwards; it failed (exit 1) |
| XCU.ex.026 | FAIL | SYSTEM | ex command parsing: '//' executes two implied commands, not three; a third line was printed |
| XCU.ex.029 | FAIL | SYSTEM | ex count: count is an extra address equal to the last address plus count-1 and the first address is then dropped; 1,2d x 2 should delete lines 2-3, it deleted 1-2 |
| XCU.ex.030 | FAIL | SYSTEM | ex join: with one address and a count it joins addr..addr+count; '1j 2' joined only two lines |
| XCU.ex.031 | FAIL | SYSTEM | ex move: only a destination inside the range is an error; 2,4m1 (destination before the range) failed |
| XCU.ex.032 | FAIL | SYSTEM | ex buffers: commands that store into a named buffer also store into the unnamed buffer; pu with no name put nothing after 2ya b |
| XCU.ex.033 | FAIL | SYSTEM | ex STDOUT: lines are written as the line text and a newline; the system writes a stray carriage return before many lines (raw CR bytes in the output) (-s ignores TERM, so no terminal handling is allowed to explain it) |
| XCU.ex.036 | FAIL | SYSTEM | ex substitute: with pattern and repl both empty (s//) the last s command is repeated; the system applied an empty replacement |
| XCU.ex.038 | FAIL | SYSTEM | ex undo: undoes the last command that modified the buffer, yank is not one; '2d, ya, u' failed (exit 1) |
| XCU.ex.039 | FAIL | SYSTEM | ex STDOUT: lines are written as the line text and a newline; the system writes a stray carriage return before many lines (raw CR bytes in the output) (-s ignores TERM, so no terminal handling is allowed to explain it) |
| XCU.ex.040 | FAIL | SYSTEM | ex command parsing: a backslash-escaped blank stays in a file argument; 'w c\ d' failed |
| XCU.ex.041 | FAIL | SYSTEM | ex write: with an empty edit buffer an empty file is written; no empty file resulted |
| XCU.ex.042 | FAIL | SYSTEM | ex read: carriage-return newline pairs in the input are single newlines; the CR was kept in the buffer |
| XCU.ex.043 | FAIL | SYSTEM | ex write !command: if the program output does not end in a newline one is added; 'out' then 'x' without the final newline |
| XCU.ex.044 | FAIL | SYSTEM | ex !filter: carriage-return newline pairs in the output are single newlines; CR was kept |
| XCU.ex.045 | FAIL | SYSTEM | ex autoprint (default set; -s suppresses only prompts and informational messages): d must print the new current line; nothing printed |
| XCU.ex.046 | FAIL | SYSTEM | ex set: a paragraphs/sections value of odd length is an error; it was accepted |
| XCU.ex.047 | FAIL | SYSTEM | ex edcompatible: repeating the g suffix toggles it off; the second substitution still replaced all |
| XCU.ex.049 | FAIL | SYSTEM | ex tag/taglength: taglength limits significant characters of the tag name; tag tgxyz with taglength=2 failed |
| XCU.ex.051 | FAIL | SYSTEM | ex source: blank lines in the file are ignored; a blank line caused an implied print (d) |
| XCU.ex.052 | FAIL | SYSTEM | ex edit: if the file cannot be accessed the buffer is empty (new file); e of a nonexistent file exited 1. Weak reading: the text does not say that no error status may result |
| XCU.ex.053 | FAIL | SYSTEM | ex list flag l: non-printable bytes not in the escape table are written as \ooo; control bytes were written in caret form |
| XCU.ex.057 | FAIL | SYSTEM | ex initialization: blank lines in .exrc are ignored; the set sw=2 after a blank line did not take effect (same blank-line defect as ex.051) |
| XCU.ex.060 | FAIL | SYSTEM | ex -c: if more than one -c is accepted they run in order; the second -c was accepted but not run after the first |
| XCU.ex.062 | FAIL | SYSTEM | ex insert at line zero places text at the top; 0i failed (exit 1) |
| XCU.exec.011 | FAIL | SYSTEM | exec: a failed exec does not end an interactive shell; shell exited (no 'after' output) |
| XCU.fc.021 | FAIL | SYSTEM | fc STDERR: only diagnostics go to stderr; fc -s echoed the re-executed command to stderr |
| XCU.fc.023 | FAIL | SYSTEM | fc -e: the default editor is ed, looked up through PATH; the ed found first in PATH was not used |
| XCU.fg.002 | FAIL | SYSTEM | fg STDOUT: the command line of the job is written; jobs here have command text '<command unknown>' so the line was not written |
| XCU.find.028 | FAIL | SYSTEM | find -L/-H: a dangling link is treated as the link itself, no error; find reported 'cannot follow symbolic link' and exited nonzero |
| XCU.find.032 | FAIL | SYSTEM | find -newer: if the named link's target is missing the link's own time is used, no error; find failed 'cannot access' |
| XCU.fold.005 | FAIL | SYSTEM | fold: a carriage return resets the column count and no newline is inserted next to it; fold -w 4 broke after abcd |
| XCU.getconf.007 | FAIL | SYSTEM | limits.h minimum _POSIX_PATH_MAX is 256; getconf wrote 255 |
| XCU.gettext.002 | FAIL | SYSTEM | gettext OPTIONS: -e and -E are both required; -E was rejected (nonzero) |
| XCU.grep.001 | FAIL | SYSTEM | grep DESCRIPTION: a null BRE/pattern matches every line (also from -f or a newline list; -v '' selects none); the system did not treat an empty pattern as matching every line |
| XCU.grep.003 | FAIL | SYSTEM | grep DESCRIPTION: a null BRE/pattern matches every line (also from -f or a newline list; -v '' selects none); the system did not treat an empty pattern as matching every line |
| XCU.grep.004 | FAIL | SYSTEM | grep DESCRIPTION: a null BRE/pattern matches every line (also from -f or a newline list; -v '' selects none); the system did not treat an empty pattern as matching every line |
| XCU.iconv.003 | FAIL | SYSTEM | iconv OPERANDS: a file operand - means standard input at that position; iconv with f1 - f2 failed |
| XCU.jobs.001 | INSPECT | SYSTEM | jobs STDOUT: the line holds the command given to the shell; it wrote '<command unknown>' (the assertion is already disputed for column padding; this mismatch is separate and still a real defect) |
| XCU.jobs.003 | FAIL | SYSTEM | jobs -l STDOUT: command field must be the command given to the shell; got '<command unknown>' |
| XCU.jobs.004 | INSPECT | SYSTEM | jobs STDOUT: Done line must show the command; got '<command unknown>' (already disputed for padding only) |
| XCU.jobs.006 | INSPECT | SYSTEM | jobs STDOUT: Stopped line must show the command; got '<command unknown>' (already disputed for padding only) |
| XCU.jobs.007 | FAIL | SYSTEM | jobs: root cause is the missing command text ('<command unknown>'), so neither job line was matched; the + marking itself could not be judged |
| XCU.jobs.008 | FAIL | SYSTEM | jobs %?string: selects a job whose command contains the string; the command text is unknown to the shell so the job_id failed (exit 1) |
| XCU.kill.005 | FAIL | SYSTEM | kill -l: lists the signal_name values supported; ABRT (signal 6, required) is not listed under that name |
| XCU.ln.008 | FAIL | SYSTEM | ln SYNOPSIS (Issue 8): -L and -P are required options; ln exited 2 for -L |
| XCU.ln.010 | FAIL | SYSTEM | ln SYNOPSIS (Issue 8): -L and -P are required options; ln exited 2 for -L |
| XCU.ln.011 | FAIL | SYSTEM | ln SYNOPSIS (Issue 8): -L and -P are required options; ln exited 2 for -L |
| XCU.locale.002 | FAIL | SYSTEM | locale STDOUT: a LC_ variable set in the environment is written NAME=value unquoted exactly as set; LC_TIME=POSIX was written as LC_TIME="C" |
| XCU.locale.003 | FAIL | SYSTEM | locale STDOUT: a LC_ variable overridden by LC_ALL is written quoted with the implied locale; it was written LC_TIME=C unquoted |
| XCU.locale.005 | FAIL | SYSTEM | locale -m: lists available charmaps, one per line, exit 0; it exited 2 |
| XCU.localedef.001 | FAIL | UNKNOWN | localedef with copy "POSIX" in all six categories should succeed (exit 0/1) when POSIX2_LOCALEDEF is claimed; exit 4 means errors, but the diagnostic ($d/err) is not kept; seeing it would settle whether the source or the system is at fault |
| XCU.localedef.002 | FAIL | UNKNOWN | as localedef.001, same source on stdin; exit 4, diagnostic not kept |
| XCU.ls.005 | FAIL | SYSTEM | ls -m: starts a new line when the next entry does not fit the line; COLUMNS=8 overrides the line width (XBD COLUMNS), 'aaaa, bbbb' does not fit; output was not wrapped (some doubt: the ls page ties COLUMNS to -C explicitly) |
