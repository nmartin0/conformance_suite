# FreeBSD triage, group xcu-b

Counts: TEST-BUG 10, SYSTEM 111, STD-AMBIG 2, ENV 0, UNKNOWN 0.

Per-test output files under /tmp/fbout-r/out were empty for every row here, so the verdicts rest on the TSV note, the test and the text.

| ID | result | class | reason |
|---|---|---|---|
| XCU.1.4-utility-defaults.011 | FAIL | SYSTEM | XCU 1.4 (seekable input, ends before EOF): the offset must be left just past the last byte processed. After sed -n 1q, cat read nothing |
| XCU.1.4-utility-defaults.012 | FAIL | SYSTEM | sh STDIN: a shell reading commands from a seekable stdin must not read ahead of a command that reads stdin. The script given on stdin produced no output |
| XCU.1.4-utility-defaults.013 | FAIL | SYSTEM | XCU 1.4 offset rule. After awk exited at NR==2, cat read nothing |
| XCU.1.4-utility-defaults.014 | FAIL | SYSTEM | XCU 1.4 ASYNCHRONOUS EVENTS Default: a signal ignored on entry must stay ignored. One of sleep, cat or sort (sort is likely, the note gives only a pid) died from TERM/HUP |
| XCU.1.4-utility-defaults.015 | FAIL | SYSTEM | Same section: with the default action inherited, the result must be as if the default action was taken. sort f2 exited 255 after SIGTERM instead of dying from the signal |
| XCU.2.5-parameters.017 | FAIL | SYSTEM | 2.5.3 PWD: a value that is the wrong directory, contains dot, or is unset must be replaced by the pwd -P path. Line a was right, a later case was wrong |
| XCU.2.6-word-expansions.009 | FAIL | SYSTEM | 2.6.1: tilde results are treated as quoted, and ~ with HOME set to null gives one empty field. The first line was right, the split/empty-HOME cases were wrong |
| XCU.2.6-word-expansions.029 | FAIL | STD-AMBIG | 2.6.3 requires the arithmetic attempt first and a command-substitution fallback, but the same paragraph tells applications to put a space in "$( (" so they do not rely on this. Behaviour for the unspaced form is not clearly guaranteed. Not disputed now; should be |
| XCU.2.8-exit-status-and-errors.003 | FAIL | SYSTEM | 2.8.1 table plus unset EXIT STATUS (>0 when a name cannot be unset): a non-interactive shell must exit on unset of a readonly variable. It went on and printed notreached |
| XCU.2.8-exit-status-and-errors.008 | FAIL | SYSTEM | 2.8.1: a redirection error on a function call must not make the shell exit. Nothing after it ran |
| XCU.at.001 | FAIL | SYSTEM | at STDERR: "job %s at %s" is required. The system wrote "Job 1 will be executed using /bin/sh" |
| XCU.at.004 | FAIL | SYSTEM | Same STDERR format. at -f wrote no job line |
| XCU.at.008 | FAIL | SYSTEM | at EXIT STATUS: 0 only if every -r job was removed. -r of an unknown ID exited 0 |
| XCU.awk.019 | FAIL | SYSTEM | awk FILENAME: must be '-' when reading standard input. It was not |
| XCU.awk.025 | FAIL | SYSTEM | awk grammar: concatenation cannot take a unary-plus right operand, so /y/ + 0 is addition. Got "10 00" |
| XCU.awk.032 | FAIL | SYSTEM | awk Variables: a field past NF is uninitialized, and comparing it with a number is numeric. ($2 == 0) gave 0 |
| XCU.awk.033 | FAIL | SYSTEM | awk ARGV processing: with the only operand nulled, stdin is read with FILENAME set to '-'. FILENAME was empty (":in") |
| XCU.batch.001 | FAIL | SYSTEM | batch STDERR: "job %s at %s" is required. Got "Job 6 will be executed..." |
| XCU.break.006 | FAIL | SYSTEM | break EXIT STATUS gives >0 for an n that is not >=1. break 0 returned 0 (the application-usage sentence aside, the status for this case is stated) |
| XCU.cd.004 | FAIL | SYSTEM | cd STDOUT/OPERANDS: cd - must write the new directory. The written output did not match |
| XCU.cd.009 | FAIL | SYSTEM | cd STDOUT: the directory must be written when a non-empty CDPATH entry is used. It was not |
| XCU.cd.010 | FAIL | SYSTEM | Same: nothing was written when the first (non-empty) CDPATH entry was used |
| XCU.cmp.003 | FAIL | TEST-BUG | XBD 5: a blank in "%d %o %o" is one or more blanks, an unwidthed %d may be blank-padded and %o zero-padded. "     2 142 130" conforms; fixed in 0a20f07f |
| XCU.cmp.006 | FAIL | TEST-BUG | Same padding issue on the -l line, fixed in 0a20f07f |
| XCU.continue.005 | FAIL | SYSTEM | continue EXIT STATUS gives >0 for an invalid n. continue 0 returned 0 |
| XCU.cp.009 | FAIL | SYSTEM | cp EXIT STATUS: 0 when the only skipped file got a negative -i reply. Exited 1 |
| XCU.crontab.002 | FAIL | SYSTEM | crontab DESCRIPTION/SYNOPSIS: with no file operand, entries are read from stdin. It refused with "file name must be specified" |
| XCU.dd.005 | FAIL | SYSTEM | dd bs=: sets both sizes and supersedes ibs=/obs=. dd ibs=7 obs=9 bs=100 exited 1 |
| XCU.dd.015 | FAIL | SYSTEM | dd STDERR: a short read is a partial input block, so 0+1 records in. That line was missing with conv=sync |
| XCU.dd.016 | FAIL | SYSTEM | dd STDERR: the "N truncated record(s)" line is required. It was missing (the word differs) |
| XCU.dd.020 | FAIL | SYSTEM | dd OPERANDS: iflags=fullblock is required. dd rejected it with exit 1 |
| XCU.df.001 | FAIL | SYSTEM | df STDOUT: the -P header must name the column "Available". The system wrote "Avail" |
| XCU.df.002 | FAIL | SYSTEM | Same for -k -P |
| XCU.diff.003 | FAIL | TEST-BUG | diff -c header "*** %s %s": the blank is one or more blanks (a tab is allowed), but the test required exactly one space. Fixed in 0a20f07f (range lines are relaxed the same way) |
| XCU.file.006 | FAIL | SYSTEM | file -i (unshaded): a regular file must be identified as "regular file". Got a MIME type |
| XCU.file.008 | FAIL | SYSTEM | file DESCRIPTION: links are resolved by default. ld was reported as a symbolic link, not a directory |
| XCU.file.009 | FAIL | SYSTEM | file -M must apply the tests in the named file. The -M run printed nothing (-m worked) |
| XCU.find.011 | FAIL | SYSTEM | find -atime: whole days with the remainder discarded, so 0 is age under a day. A seconds-old file was not found (days rounded up) |
| XCU.find.032 | FAIL | SYSTEM | find -newer with -H must use the referenced file's time. mid was not newer than link |
| XCU.getconf.005 | FAIL | SYSTEM | getconf OPERANDS: POSIX2_C_BIND and the like must be supported as aliases. Exited 64 |
| XCU.getconf.007 | FAIL | SYSTEM | getconf: limits.h Minimum Values names must work, so _POSIX_PIPE_BUF is 512. Got "undefined" |
| XCU.getopts.016 | FAIL | SYSTEM | getopts STDERR: the diagnostic must name the invoking program ($0). "Illegal option -Q" does not |
| XCU.gettext.001 | FAIL | SYSTEM | gettext is a required, unshaded utility (2024). Not installed (status 127) |
| XCU.gettext.002 | FAIL | SYSTEM | Same: gettext absent |
| XCU.gettext.003 | FAIL | SYSTEM | Same: gettext absent |
| XCU.head.007 | FAIL | SYSTEM | 1.4 offset rule and head INPUT FILES: after head -n 1, cat must read the rest. It did not |
| XCU.jobs.001 | INSPECT | SYSTEM | jobs STDOUT needs the command field. The line was "[1] + Running" with no command (the disputed padding is not the cause) |
| XCU.jobs.003 | FAIL | SYSTEM | jobs -l: the command field was empty |
| XCU.jobs.004 | INSPECT | SYSTEM | Done line with no command. The test's demand for + on a finished job was not stated in the text; relaxed in d25d2bf5, but the result still fails on the command |
| XCU.jobs.005 | INSPECT | SYSTEM | Done(3) line with no command; same relaxation as jobs.004 in d25d2bf5 |
| XCU.jobs.006 | INSPECT | SYSTEM | jobs STDOUT: a job stopped by SIGTSTP must show Stopped, plus the command. Got "[1] + Running" with no command |
| XCU.jobs.007 | FAIL | SYSTEM | jobs lines have no command text, so the suspended job's line cannot be found. The command field is required (STDOUT) |
| XCU.jobs.008 | FAIL | SYSTEM | XBD 3.182 %?string matches against the command. The shell keeps no command text, so it exited 2 |
| XCU.join.007 | FAIL | SYSTEM | join -e: replaces empty output fields. With -t :, the empty third field of "a:1:" was not replaced |
| XCU.locale.002 | FAIL | SYSTEM | locale STDOUT: a variable set in the environment is written without double quotes. Got LC_NUMERIC="C" |
| XCU.locale.006 | FAIL | SYSTEM | locale -c keyword must write the category name, then the value. Got a description table |
| XCU.ls.017 | FAIL | SYSTEM | ls STDOUT: with -s, each directory listing starts with a total line. It was missing |
| XCU.ls.044 | FAIL | SYSTEM | Same: ls -i -s -1 has no total line |
| XCU.ls.046 | FAIL | TEST-BUG | Without -d, -F or -l it is not stated whether an operand symlink is followed, yet the test required the link's own size. Fixed in fde7f0cf with -d |
| XCU.ls.048 | FAIL | SYSTEM | ls SYNOPSIS [-A\|-a] is an exclusive set and the last one wins, so -a -A must drop . and .. It did not |
| XCU.ls.050 | FAIL | SYSTEM | ls -1: does not disable the long format. ls -l -1 lost it |
| XCU.m4.008 | FAIL | SYSTEM | m4 -U define must undefine the built-in, so define(a,b) is output as text. It was still a macro |
| XCU.m4.010 | FAIL | SYSTEM | m4 sysval: gives the utility's exit value. Got 768 (the raw wait status) |
| XCU.m4.016 | FAIL | SYSTEM | m4 mkstemp: on failure m4 must continue with empty text. Processing stopped after "a[" |
| XCU.msgfmt.001 | FAIL | SYSTEM | msgfmt is a required, unshaded utility (2024). Absent (127) |
| XCU.msgfmt.002 | FAIL | SYSTEM | Same: msgfmt absent |
| XCU.msgfmt.003 | FAIL | SYSTEM | Same: msgfmt absent |
| XCU.msgfmt.004 | FAIL | SYSTEM | Same: msgfmt absent (127) |
| XCU.msgfmt.005 | FAIL | SYSTEM | Same: msgfmt absent, so every run "diagnosed" |
| XCU.msgfmt.006 | FAIL | SYSTEM | Same: msgfmt absent |
| XCU.ngettext.001 | FAIL | SYSTEM | ngettext is a required, unshaded utility (2024). Absent (127) |
| XCU.ngettext.002 | FAIL | SYSTEM | Same: ngettext absent |
| XCU.ngettext.003 | FAIL | SYSTEM | Same: ngettext absent |
| XCU.od.003 | FAIL | SYSTEM | od -t a uses only the low seven bits, so \311 is I. It was not |
| XCU.od.005 | FAIL | SYSTEM | od -j/-N take a 0x hexadecimal prefix. -j 0x4 -N 0x2 gave the wrong bytes |
| XCU.paste.005 | FAIL | SYSTEM | paste -s: an empty file gives a line holding only a newline. It did not |
| XCU.patch.004 | FAIL | SYSTEM | patch -e must apply an ed-style script. It exited 0 but left the file unchanged |
| XCU.ps.002 | FAIL | SYSTEM | ps -o heading table: tty's heading is TT. Got TTY |
| XCU.ps.011 | FAIL | SYSTEM | ps time format is [dd-]hh:mm:ss. Got 0:00.13 |
| XCU.read.009 | FAIL | SYSTEM | read -d (2024, unshaded) is required. "Illegal option -d" |
| XCU.read.010 | FAIL | SYSTEM | Same: read -d '' rejected |
| XCU.readlink.004 | FAIL | SYSTEM | readlink DESCRIPTION: a non-link operand needs a diagnostic and a non-zero status. No diagnostic was written |
| XCU.realpath.002 | FAIL | SYSTEM | realpath -e is required (2024). Exited 1 for an existing path |
| XCU.realpath.004 | FAIL | SYSTEM | realpath -E: a missing last component is allowed. Exited 1 |
| XCU.realpath.006 | FAIL | SYSTEM | -E/-e: the last one wins. -e -E exited 1 |
| XCU.sed.002 | FAIL | SYSTEM | sed -e: a newline is put between -e pieces, so -e '1a\' -e added is valid. It exited 1 |
| XCU.sed.012 | FAIL | SYSTEM | sed l: a backslash is written as \\. It was written as a single backslash |
| XCU.sh-errors.002 | FAIL | SYSTEM | 2.8.1 and unset EXIT STATUS: unset of a readonly variable must make the shell exit. Got status 0 |
| XCU.sh-jobs.002 | FAIL | SYSTEM | 2.9.3.1: an interactive shell writes "[%d] %d" when a background job starts. There was no such line, though the pty session worked |
| XCU.sh-jobs.008 | FAIL | SYSTEM | 2.11 with set -b: the completion message is written as soon as the job ends. It came only after the foreground command |
| XCU.sh-simple-commands.005 | FAIL | SYSTEM | 2.9.1.2 and the 2.8.1 table: an assignment to a readonly variable before a command is an assignment error, so the shell exits. "PCR=2 true" went on with status 0 |
| XCU.sh.011 | FAIL | SYSTEM | sh follows the Utility Syntax Guidelines, so -- ends the options after -c. "--" was taken as the command string |
| XCU.sh.017 | FAIL | SYSTEM | sh STDIN: no read-ahead past a command that reads stdin. The shell consumed A and B itself |
| XCU.sh.036 | FAIL | TEST-BUG | sh vi U: it is not stated whether the text typed into a new line counts as an undoable change. The test now uses a line recalled with k (6d2cb452) |
| XCU.sh.043 | FAIL | SYSTEM | sh vi #: the commented line must be entered into the history. fc -l did not list it |
| XCU.sh.044 | FAIL | SYSTEM | sh vi =: must display the pathname expansions. They were not shown |
| XCU.sh.045 | FAIL | SYSTEM | sh vi *: must replace the bigword with all its expansions. It did not |
| XCU.sh.046 | FAIL | SYSTEM | sh vi backslash: must complete to the unique match. It did not |
| XCU.sh.047 | FAIL | SYSTEM | sh vi @letter: must insert the value of the alias _letter. It did not |
| XCU.sh.050 | FAIL | SYSTEM | sh vi Y and count p: Y yanks to end of line and 3p puts three copies. abbbb was not produced |
| XCU.sh.057 | FAIL | TEST-BUG | The typed input ended after :wq with no exit, and the pty helper never sends EOF, so the session hangs on every shell (confirmed locally). Fixed in 6d2cb452 |
| XCU.sh.058 | FAIL | SYSTEM | sh Command History List: the history is kept in the HISTFILE file. A later session's fc -l lacked the earlier command |
| XCU.sh.059 | FAIL | SYSTEM | Same: k could not recall an earlier session's command |
| XCU.sh.063 | FAIL | TEST-BUG | MAILPATH: the %message is stated only for a change of modification time, but the test created the file. It now pre-creates it with an old time (6d2cb452) |
| XCU.sh.064 | FAIL | TEST-BUG | Same creation-versus-modification issue, same fix |
| XCU.sh.067 | FAIL | SYSTEM | sh ASYNCHRONOUS EVENTS: an interactive shell with -m ignores TSTP/TTIN/TTOU. After kill -TSTP $$ the session never finished (input ends with exit, so a stopped shell is the plausible cause; pty.out would confirm) |
| XCU.shift.003 | FAIL | SYSTEM | shift EXIT STATUS: if the shell goes on, the status is non-zero and a warning is written. No warning was written |
| XCU.strings.003 | FAIL | TEST-BUG | "%d %s": the blank is one or more blanks and an unwidthed %d may be blank-padded. Fixed in 0a20f07f; leading blanks before %x are still rejected, since only zeros are allowed for %o |
| XCU.stty.005 | FAIL | TEST-BUG | tcsetattr() may succeed while ignoring a change, and the pty kept cread. The test now checks -a only when stty -g shows a change (63b6e0fe) |
| XCU.test.009 | FAIL | SYSTEM | test -nt: true when pathname2 cannot be resolved. new -nt none gave 1 |
| XCU.trap.010 | FAIL | SYSTEM | trap -p condition (2024) is required. The shell rejected trap -p (status 2) |
| XCU.trap.011 | INSPECT | SYSTEM | trap -p was rejected outright (status 2). That is not the disputed default-state output, which was never reached |
| XCU.trap.012 | FAIL | SYSTEM | trap -p with no operands is required. Rejected |
| XCU.trap.017 | FAIL | SYSTEM | trap listing: must include signals ignored on entry. USR1 was missing |
| XCU.tsort.004 | FAIL | SYSTEM | tsort -w (2024) is required. With no cycle it did not give the order and status 0 |
| XCU.ulimit.006 | FAIL | SYSTEM | ulimit: a soft limit can only be set up to the hard limit, and setrlimit must fail with EINVAL. The request exited 0 (the value was clamped) |
| XCU.unalias.005 | FAIL | SYSTEM | 1.4 CONSEQUENCES OF ERRORS: an operand that cannot be acted on needs a diagnostic. unalias of an unknown name wrote none |
| XCU.uudecode.001 | FAIL | SYSTEM | uudecode: the mode comes from the data and umask must not affect it. Got rw------- under umask 077 |
| XCU.uudecode.003 | FAIL | SYSTEM | uudecode: a /dev/stdout name in the data must go to stdout. Output was wrong |
| XCU.uudecode.011 | FAIL | SYSTEM | uudecode: a - name in the data must go to stdout. Output was wrong |
| XCU.xargs.004 | FAIL | SYSTEM | xargs DESCRIPTION: without -r and with no input, the utility runs exactly once. It did not run |
| XCU.xargs.006 | FAIL | SYSTEM | xargs -0: each null byte is a delimiter, so adjacent nulls give an empty argument. It was dropped |
| XCU.xargs.009 | FAIL | STD-AMBIG | xargs -x: "terminate if a constructed command line will not fit". With -n 2 -s 12 the line actually built holds one argument and fits; the test reads it as the -n-sized line (the older wording and the diff example). Not disputed now; should be |
