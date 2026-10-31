# FreeBSD triage, group xcu-a

Counts: TEST-BUG 11, SYSTEM 74, STD-AMBIG 3, ENV 0, UNKNOWN 2.

The per-test output files for this group were all empty, so verdicts rest on the note in the list, the test and the page. Fixed tests were run on the Linux container where the utility exists (ex, vi). mailx and pax are not installed there, so their fixes were only syntax-checked with sh -n.

| ID | result | class | reason |
|---|---|---|---|
| XCU.ed.013 | FAIL | SYSTEM | ed.html EXTENDED DESCRIPTION: a q on a changed buffer must write "?\n" to standard output. Only "24" came out, so the warning was not on stdout. |
| XCU.ed.015 | FAIL | SYSTEM | List Command: l must escape \t, \\ and \$, give other control bytes as 3-digit octal and end each line with $. The test checks exactly that, and the output differed from line 2 onward (most likely the $ was not escaped). |
| XCU.ed.017 | FAIL | SYSTEM | EXTENDED DESCRIPTION: an invalid command (xyz) must write "?\n" to standard output. Nothing came out on stdout. |
| XCU.ed.018 | FAIL | STD-AMBIG | The page requires "?" on stdout only for an invalid command and for the q/e warning. An out-of-range final address is just "an error", and CONSEQUENCES OF ERRORS points to the XCU 1.4 default (a diagnostic on stderr). Whether 3-5p counts as an invalid command is open. Not disputed now. |
| XCU.ed.019 | FAIL | SYSTEM | Addresses in ed (Defect 1582): ",," means 1,$,$, so ,,p prints only the last line. The system printed all five lines. |
| XCU.ed.024 | FAIL | STD-AMBIG | The m and t results were right. Only the "?" for a destination inside the moved range was missing from stdout, and that is the same invalid-command-or-other-error question as ed.018. Not disputed. |
| XCU.ed.025 | FAIL | STD-AMBIG | A lone % with no earlier s is "an error". Whether its "?" must go to stdout is the same question as ed.018. Not disputed. |
| XCU.ex.008 | FAIL | SYSTEM | Command Descriptions, flags: after "3p #" the current line is not written again when it was the last line written. The system printed line 3 twice. |
| XCU.ex.014 | FAIL | TEST-BUG | sort -r of "two" and "three" leaves their order unchanged, so the expected file was wrong. The second check also used the modified file. Fixed: filter with sort and rewrite the file before the w ! check. |
| XCU.ex.019 | FAIL | SYSTEM | STDOUT/STDERR: a failed search with nowrapscan is an error, so its message belongs on stderr (and -s suppresses informational messages). The system wrote "Reached end-of-file..." to stdout. |
| XCU.ex.020 | FAIL | SYSTEM | Put: the current line becomes the last line entered. After 2,3ya and 1pu that is line 3; the system left it at 2. |
| XCU.ex.022 | FAIL | SYSTEM | Addressing in ex: ?two?;??= is valid (a null ?? reuses the last RE) and should print 2. The system exited with status 1. |
| XCU.ex.026 | FAIL | SYSTEM | Command Line Parsing step 12: a double-quote ends the command and the rest is discarded, so "2d "tail" must work. The system exited with status 1. |
| XCU.ex.028 | FAIL | TEST-BUG | "2d p" deletes into buffer p (the delete synopsis and rationale say so), so no output is correct. Fixed: use 2dp and 2dl. |
| XCU.ex.030 | FAIL | SYSTEM | Join: one address and a count joins addr through addr+count, so "1j 2" joins lines 1-3. The system joined only 2 lines. |
| XCU.ex.033 | FAIL | SYSTEM | Regular Expressions in ex: a delimiter inside a bracket expression does not end the RE ("g%[%]%p" is the page's own example). The system exited with status 1. |
| XCU.ex.035 | FAIL | SYSTEM | Replacement Strings: \n for a group that did not match is replaced by an empty string. The system produced "a[1]". |
| XCU.ex.036 | FAIL | SYSTEM | Substitute: when both pattern and replacement are empty or omitted ("s//"), the last s is repeated. The system replaced with nothing. |
| XCU.ex.039 | FAIL | SYSTEM | z with type +: start = line + ((1-1)*count)+1, so 5z+3 writes lines 6-8. The system wrote 5-7. |
| XCU.ex.040 | FAIL | SYSTEM | Command Line Parsing: a backslash-escaped blank stays in the argument, so "w c\ d" writes the file "c d". The system exited with status 1. |
| XCU.ex.042 | FAIL | SYSTEM | Read: a carriage-return/newline pair in r ! output counts as one newline. The file kept the CRs (the note's two strings differ only by invisible CRs). |
| XCU.ex.043 | FAIL | SYSTEM | shell option: when SHELL is null, sh is the default. With SHELL empty, the ! command did not run. |
| XCU.ex.044 | FAIL | SYSTEM | Filter (!): carriage-return/newline pairs in the output count as one newline. The CRs were kept. |
| XCU.ex.045 | FAIL | SYSTEM | autoprint is on by default and -s does not turn it off, so 2d writes the new current line. Nothing was written. |
| XCU.ex.046 | FAIL | SYSTEM | list option: lines written in ex mode use the l format. After set list, 2p gave "b" with no "$". |
| XCU.ex.047 | FAIL | SYSTEM | edcompatible: the g suffix is remembered and repeating it toggles it, so the second s/a/b/g replaces once. The system replaced both. |
| XCU.ex.048 | FAIL | TEST-BUG | The Edit command is not affected by autowrite (only next, rewind, tag, suspend and ! are), so e on a modified buffer is an error and status 1 is correct. Fixed: use n / n! and adjust the says line. |
| XCU.ex.049 | FAIL | TEST-BUG | set splits its arguments at blanks, so "set tags=tagsnone tagsfile" sets tags to one file and then names an unknown option. A blank inside a value must be backslash-escaped. Fixed. |
| XCU.ex.050 | FAIL | SYSTEM | The * buffer command is the same as @. "2*a" exited with status 1. |
| XCU.ex.053 | FAIL | SYSTEM | Print with the l flag: non-printables outside the escape table are written as 3-digit octal. The system wrote ^A and ^[. |
| XCU.ex.054 | FAIL | TEST-BUG | The test counted every ":" on the terminal, including any inside messages or control sequences, and the page fixes neither. Fixed: count only prompts that come right before an echoed command. A rerun will show whether noprompt really fails. |
| XCU.ex.056 | FAIL | SYSTEM | autoindent: the first line typed into c takes the indentation of the line it replaces. "3c" gave "Z" instead of a tab then Z. |
| XCU.ex.060 | FAIL | SYSTEM | -c: "ex -s -c .= f" with q! from stdin is all valid and should print 5 and exit 0. The system exited with status 1. |
| XCU.ex.062 | FAIL | SYSTEM | autoprint (on by default) applies to a shift, so 2> writes the shifted line. Nothing was written. |
| XCU.ex.064 | FAIL | TEST-BUG | The test compared the output of two runs and assumed both start-up outputs are the same, which the page does not promise; it also used grep -o, which is not POSIX. Fixed: "f pcnew.txt" must show the new name beyond its echo. Rerun to decide. |
| XCU.mailx.004 | FAIL | SYSTEM | Save Messages: save with no file argument saves to the mbox (MBOX). The mbox file was not written. |
| XCU.mailx.005 | FAIL | TEST-BUG | The initial summary and -H write "a page" of header lines, and with no terminal the page size is not fixed (screen variable). Fixed: set screen in the commands and in a start-up file for -H. A rerun may still show a real -H fault. |
| XCU.mailx.006 | FAIL | SYSTEM | next: if the current message has not been written, next prints it. The first next printed message 2. |
| XCU.mailx.010 | FAIL | SYSTEM | Pipe Message: pipe with no arguments sends the current message to the cmd command. Nothing ran; the system appears to have no pipe command (see mailx.030). |
| XCU.mailx.012 | FAIL | SYSTEM | Delete Aliases: unalias deletes the named alias. g1 was still listed. |
| XCU.mailx.015 | FAIL | SYSTEM | Commands in mailx, quoting: a single quote inside double quotes is literal. echo "it's" did not give it's. |
| XCU.mailx.022 | FAIL | SYSTEM | An empty command means next, and next prints the unshown current message 1. It printed message 2. |
| XCU.mailx.023 | FAIL | SYSTEM | set: "set noname" is the same as "unset name". pcv_b stayed defined. |
| XCU.mailx.024 | FAIL | SYSTEM | prompt: the default prompt "? " is written to stdout in command mode; only asksub is limited to a terminal. No prompt was written. |
| XCU.mailx.026 | FAIL | SYSTEM | Invoke a Shell: ! runs SHELL with exactly the three arguments -c, -- and the command. The log shows 2 arguments. |
| XCU.mailx.027 | FAIL | SYSTEM | Pipe Message/page: pipe 1 2 cmd sends both messages to cmd. The output lacked message 1; there is no working pipe command. |
| XCU.mailx.030 | FAIL | SYSTEM | Commands in mailx: pipe/\| is a required command, so list must name it. list does not. |
| XCU.mailx.032 | FAIL | SYSTEM | visual edits the messages with the program named by VISUAL. VISUAL was never run (edit worked). |
| XCU.mailx.033 | FAIL | SYSTEM | LISTER: when it is null, folders uses ls. With LISTER empty the system failed ("Fatal error in process"). |
| XCU.mailx.035 | FAIL | SYSTEM | PAGER/crt: when stdout is a terminal and crt is below the message length, output goes through PAGER. The pager never ran. |
| XCU.mailx.036 | FAIL | SYSTEM | Save (capital S): the file name is the author's name with network addressing removed ("alice"). No file named alice held message 1. |
| XCU.mailx.038 | FAIL | SYSTEM | Invoke a Shell: "!" is replaced only when bang is set. Without bang, "!echo hi!" did not print hi!. |
| XCU.mailx.039 | FAIL | SYSTEM | allnet: network names with the same login name are treated as identical, msglists included. alice@other... did not match. |
| XCU.mailx.041 | FAIL | SYSTEM | OPTIONS (Utility Syntax Guidelines): "-f -- file" must work, since -f takes no option-argument. Nonzero status. |
| XCU.mailx.043 | FAIL | SYSTEM | keep: an emptied mailbox is truncated to zero length instead of being removed. It was removed. |
| XCU.mailx.044 | FAIL | SYSTEM | showto: for mail sent by the invoking user, the summary shows the To field. It showed the author. |
| XCU.mailx.047 | FAIL | SYSTEM | Start-up files: an error must write a diagnostic, and per STDERR diagnostics go to stderr (start-up comes before command mode). stderr stayed empty. |
| XCU.mailx.048 | FAIL | SYSTEM | Internal Variables: header, save and asksub are set by default, and set lists all defined variables. set showed only noprompt (which suggests "set noprompt" defines a variable named noprompt, as in mailx.023). |
| XCU.more.001 | FAIL | SYSTEM | DESCRIPTION: when stdout is not a terminal, the file is copied unchanged except for -s. -c -e -i -u -n 3 changed the copy (most likely -n 3 was not taken as a line count). |
| XCU.more.003 | FAIL | SYSTEM | -s squeezes runs of empty lines even when stdout is not a terminal (DESCRIPTION exception). They were not squeezed. |
| XCU.more.004 | FAIL | SYSTEM | MORE: the options in it apply as if typed before the command-line options, so MORE=-s must squeeze. It did not. |
| XCU.more.005 | FAIL | SYSTEM | EXIT STATUS: an unreadable file operand is an error and gives >0. The exit status was 0. |
| XCU.pax.006 | FAIL | SYSTEM | ustar mode field: it holds 12 bits (the table lists only the permission and set-id bits). It was written as 0100640, file type bits included. |
| XCU.pax.008 | FAIL | SYSTEM | -x: the pax format is required. "pax -w -x pax" failed. |
| XCU.pax.011 | FAIL | SYSTEM | STDOUT: list mode with -v writes one ls -l line per member. 3 members gave 4 lines on stdout. |
| XCU.pax.015 | FAIL | SYSTEM | -o listopt=format applies to every format, not only pax. It was rejected with a nonzero status. |
| XCU.pax.018 | FAIL | SYSTEM | The default archives the link itself (type l), and -v lists one line per member. The listing had a second line, the same extra stdout line as pax.011. |
| XCU.pax.021 | FAIL | SYSTEM | -x pax with -b is required. The ustar and cpio cases passed; the -x pax case failed. |
| XCU.pax.022 | FAIL | SYSTEM | -i: end-of-file when reading an answer must end pax at once with a nonzero status. The exit status was 0. |
| XCU.pax.025 | FAIL | SYSTEM | -s: the trailing s and S flags are part of the syntax. ",tgt,new,S" was rejected. |
| XCU.pax.026 | FAIL | SYSTEM | -p: with neither e nor o, pax must not set S_ISUID or S_ISGID. "-p p" restored -rwsr-sr-x. |
| XCU.pax.029 | FAIL | SYSTEM | DESCRIPTION: when both -H and -L are given, the last one decides. "-L -H" behaved like -L. |
| XCU.pax.030 | FAIL | TEST-BUG | STDERR says only that "the pathnames it processes" are written; in copy mode that can be the source (f1) or the destination (dest/f1). The system wrote dest/f1. Fixed: accept either. |
| XCU.pax.035 | FAIL | SYSTEM | -x pax and -o comment:= are required. "-x pax -o comment:=hello" failed. |
| XCU.pax.036 | FAIL | SYSTEM | -x pax is required. "pax -w -x pax -o comment=..." failed. |
| XCU.pax.038 | FAIL | SYSTEM | -x pax is required. Writing it failed (ustar and cpio passed). |
| XCU.pax.040 | FAIL | SYSTEM | List Mode Format Specifications: %M and listopt are required. The output was empty. |
| XCU.pax.041 | FAIL | SYSTEM | -u: an older file is ignored, which is not an error, so the exit status should be 0. "pax -w -a -u" with an older file returned nonzero. |
| XCU.pax.043 | FAIL | SYSTEM | -x pax (and -o linkdata) are required. "pax -w -x pax" failed. |
| XCU.pax.044 | FAIL | SYSTEM | -x pax is required. Writing the long names with it failed. |
| XCU.vi.008 | FAIL | UNKNOWN | "vi -c 1d f" and then ":wq" should exit. vi hit the 15 s timeout, and nothing in the page explains a wait. Settling it needs the pty.out of the FreeBSD run, which the test does not keep. |
| XCU.vi.016 | FAIL | SYSTEM | Page Backwards: when old and new screens overlap, ^B puts the cursor at the current line minus the new lines shown (7-3=4). The system put it on the last line shown (8, then 9 and 12 later). |
| XCU.vi.017 | FAIL | SYSTEM | scroll: the option is the initial ^D/^U count, and EXINIT set it to 3. The first ^D moved 12 lines; later counts were right. |
| XCU.vi.018 | FAIL | SYSTEM | Scroll Backward by Line: when the current line leaves the display, the cursor goes to the first line displayed. 4^Y put it on the last line (5) instead of 1. |
| XCU.vi.024 | FAIL | TEST-BUG | Move Down: newline (^J) moves down one line and keeps the column. The test assumed each typed line starts at column 1. The system's file is exactly what the page gives. Fixed: begin those commands with 0. |
| XCU.vi.038 | FAIL | TEST-BUG | Same ^J cause: the newline after dddddd moves to a5, so the puts follow a5. The system's file matches the page. Fixed: k before the puts. |
| XCU.vi.040 | FAIL | TEST-BUG | Same ^J cause as vi.038. The system's a4/a5/a1/a6 is correct for the typed keys. Fixed: k before the puts. |
| XCU.vi.055 | FAIL | SYSTEM | DESCRIPTION, count: a count given to u is ignored. "2u" did not restore "abcdef" (the count was applied, or the command failed). |
| XCU.vi.060 | FAIL | UNKNOWN | ESC ending a colon line (which must run the command), ^H on an empty colon line and the final :wq should all finish. vi hit the timeout. Settling it needs the FreeBSD pty.out. |
| XCU.vi.064 | FAIL | SYSTEM | When a command from a map expansion or a buffer fails, the rest of that expansion or buffer is discarded. A dd after the failing fz still ran (line 1 deleted). |
