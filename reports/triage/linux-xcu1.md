# Triage of the first Linux run: group xcu1

Run: native Linux, root, sh = dash, awk = mawk, coreutils/diffutils/make/ed/m4 from the GNU set; the system claims POSIX2_C_DEV, POSIX2_SW_DEV and _XOPEN_UNIX (see the run's claims.txt). Classes as in the triage brief. Lines the test fixes below re-ran PASS (`--only`, `--no-selftest`). Lint is clean apart from 'declares no MUTANT'. Nothing committed.

| ID | code | class | reason |
|---|---|---|---|
| XCU.1.4-utility-defaults.012 | FAIL | SYSTEM | sh reads ahead on a seekable standard input, so read and the commands it starts lose their input (sh STDIN: no read-ahead past the command being started) |
| XCU.1.4-utility-defaults.013 | FAIL | SYSTEM | awk reads ahead and does not leave the offset just past the last record when it exits early (XCU 1.4 INPUT FILES) |
| XCU.2.2-quoting.012 | FAIL | SYSTEM | sh has no dollar-single-quotes (XCU 2.2.4); the test passes under a shell that has them |
| XCU.2.2-quoting.013 | FAIL | SYSTEM | sh has no dollar-single-quotes (XCU 2.2.4); the test passes under a shell that has them |
| XCU.2.2-quoting.014 | FAIL | SYSTEM | sh has no dollar-single-quotes (XCU 2.2.4); the test passes under a shell that has them |
| XCU.2.2-quoting.015 | FAIL | SYSTEM | sh has no dollar-single-quotes (XCU 2.2.4); the test passes under a shell that has them |
| XCU.2.2-quoting.016 | FAIL | SYSTEM | sh has no dollar-single-quotes (XCU 2.2.4); the test passes under a shell that has them |
| XCU.2.2-quoting.017 | FAIL | SYSTEM | sh has no dollar-single-quotes (XCU 2.2.4); the test passes under a shell that has them |
| XCU.2.4-reserved-words.007 | FAIL | SYSTEM | there is no time utility on the system (command -v time finds nothing), so time -p gives 127 (XCU time) |
| XCU.2.5-parameters.017 | FAIL | SYSTEM | sh keeps an inherited PWD that has a dot component instead of setting pwd -P (XCU 2.5.3 PWD) |
| XCU.2.6-word-expansions.009 | FAIL | SYSTEM | sh gives zero fields, not one empty field, for ~ with HOME set to null (XCU 2.6.1) |
| XCU.2.6-word-expansions.029 | FAIL | SYSTEM | sh reports a syntax error for $((echo a); (echo b)) instead of treating it as a command substitution (XCU 2.6.3) |
| XCU.2.6-word-expansions.043 | FAIL | TEST-BUG | the test echoed an unquoted $# while IFS=2, so the count was field-split away; "$#" is now quoted (043-split-cmdsub-arith.sh), re-run PASS |
| XCU.admin.001 | UNRESOLVED | SYSTEM | admin/get/prs (SCCS, XSI) are absent although the system claims XSI; setup cannot proceed (UNRESOLVED is the right verdict) |
| XCU.admin.002 | UNRESOLVED | SYSTEM | admin/get/prs (SCCS, XSI) are absent although the system claims XSI; setup cannot proceed (UNRESOLVED is the right verdict) |
| XCU.admin.003 | UNRESOLVED | SYSTEM | admin/get/prs (SCCS, XSI) are absent although the system claims XSI; setup cannot proceed (UNRESOLVED is the right verdict) |
| XCU.admin.004 | UNRESOLVED | SYSTEM | admin/get/prs (SCCS, XSI) are absent although the system claims XSI; setup cannot proceed (UNRESOLVED is the right verdict) |
| XCU.admin.005 | UNRESOLVED | SYSTEM | admin/get/prs (SCCS, XSI) are absent although the system claims XSI; setup cannot proceed (UNRESOLVED is the right verdict) |
| XCU.admin.006 | UNRESOLVED | SYSTEM | admin/get/prs (SCCS, XSI) are absent although the system claims XSI; setup cannot proceed (UNRESOLVED is the right verdict) |
| XCU.admin.007 | UNRESOLVED | SYSTEM | admin/get/prs (SCCS, XSI) are absent although the system claims XSI; setup cannot proceed (UNRESOLVED is the right verdict) |
| XCU.admin.008 | UNRESOLVED | SYSTEM | admin/get/prs (SCCS, XSI) are absent although the system claims XSI; setup cannot proceed (UNRESOLVED is the right verdict) |
| XCU.ar.002 | FAIL | SYSTEM | ar -t prints the last component (f2), not the operand as given (ar STDOUT) |
| XCU.ar.007 | FAIL | SYSTEM | -u compares against a member time that was not stored (archive members are written with time 0), so an older file replaces it (ar -u) |
| XCU.ar.008 | FAIL | SYSTEM | the archive records mode 644 and owner 0/0 instead of the mode and ids of the file when it was added (ar STDOUT <member mode>) |
| XCU.at.001 | UNRESOLVED | SYSTEM | at is not installed (status 127); the test reports UNRESOLVED, which stays acceptable |
| XCU.at.002 | UNRESOLVED | SYSTEM | at is not installed (status 127); the test reports UNRESOLVED, which stays acceptable |
| XCU.at.003 | UNRESOLVED | SYSTEM | at is not installed (status 127); the test reports UNRESOLVED, which stays acceptable |
| XCU.at.004 | UNRESOLVED | SYSTEM | at is not installed (status 127); the test reports UNRESOLVED, which stays acceptable |
| XCU.at.005 | UNRESOLVED | SYSTEM | at is not installed (status 127); the test reports UNRESOLVED, which stays acceptable |
| XCU.at.007 | UNRESOLVED | SYSTEM | at is not installed (status 127); the test reports UNRESOLVED, which stays acceptable |
| XCU.awk.005 | FAIL | SYSTEM | awk (mawk) does not make newline a field separator when RS is null and FS is another character (awk RS) |
| XCU.awk.013 | FAIL | SYSTEM | srand(1) twice does not repeat the rand() sequence (awk srand) |
| XCU.awk.032 | FAIL | SYSTEM | a nonexistent field compares as a string with 0, not as the uninitialized value (awk Variables and Special Variables) |
| XCU.awk.036 | FAIL | SYSTEM | the system's libc does not select a locale named by an absolute pathname (XBD 7.1) and its localedef drops the portable symbolic names of the charmap, so the made locale has no effect; the first row (category set to the locale) already gives the POSIX-locale result (XBD 7.1, localedef NAME) |
| XCU.awk.037 | FAIL | SYSTEM | the system's libc does not select a locale named by an absolute pathname (XBD 7.1) and its localedef drops the portable symbolic names of the charmap, so the made locale has no effect; the first row (category set to the locale) already gives the POSIX-locale result (XBD 7.1, localedef NAME) |
| XCU.awk.038 | FAIL | SYSTEM | the system's libc does not select a locale named by an absolute pathname (XBD 7.1) and its localedef drops the portable symbolic names of the charmap, so the made locale has no effect; the first row (category set to the locale) already gives the POSIX-locale result (XBD 7.1, localedef NAME) |
| XCU.awk.039 | FAIL | SYSTEM | the system's libc does not select a locale named by an absolute pathname (XBD 7.1) and its localedef drops the portable symbolic names of the charmap, so the made locale has no effect; the first row (category set to the locale) already gives the POSIX-locale result (XBD 7.1, localedef NAME) |
| XCU.batch.001 | UNRESOLVED | SYSTEM | batch is not installed (status 127); UNRESOLVED stays acceptable |
| XCU.bc.005 | FAIL | STD-AMBIG | quit in a loop or function: stop when read or when executed is a matter of reading (new Q-202); the two quit checks now accept both outcomes, the test passes, class disputed |
| XCU.bc.006 | FAIL | STD-AMBIG | quit in a loop or function: stop when read or when executed is a matter of reading (new Q-202); the two quit checks now accept both outcomes, the test passes, class disputed |
| XCU.c17.001 | FAIL | SYSTEM | c17 is not installed (status 127) although the system claims the C-Language Development option (c17) |
| XCU.c17.002 | FAIL | SYSTEM | c17 is not installed (status 127) although the system claims the C-Language Development option (c17) |
| XCU.c17.003 | FAIL | SYSTEM | c17 is not installed (status 127) although the system claims the C-Language Development option (c17) |
| XCU.c17.004 | FAIL | SYSTEM | c17 is not installed (status 127) although the system claims the C-Language Development option (c17) |
| XCU.c17.005 | FAIL | SYSTEM | c17 is not installed (status 127) although the system claims the C-Language Development option (c17) |
| XCU.c17.006 | UNRESOLVED | SYSTEM | c17 is not installed (setup c17 failed); UNRESOLVED is the right verdict |
| XCU.c17.007 | FAIL | SYSTEM | c17 is not installed (status 127) although the system claims the C-Language Development option (c17) |
| XCU.c17.008 | FAIL | SYSTEM | c17 is not installed (status 127) although the system claims the C-Language Development option (c17) |
| XCU.c17.009 | UNRESOLVED | SYSTEM | c17 is not installed (setup c17 failed); UNRESOLVED is the right verdict |
| XCU.c17.010 | FAIL | SYSTEM | c17 is not installed (status 127) although the system claims the C-Language Development option (c17) |
| XCU.c17.011 | FAIL | SYSTEM | c17 is not installed (status 127) although the system claims the C-Language Development option (c17) |
| XCU.c17.012 | FAIL | SYSTEM | c17 is not installed (status 127) although the system claims the C-Language Development option (c17) |
| XCU.c17.013 | FAIL | SYSTEM | c17 is not installed (status 127) although the system claims the C-Language Development option (c17) |
| XCU.c17.014 | FAIL | SYSTEM | c17 is not installed (status 127) although the system claims the C-Language Development option (c17) |
| XCU.c17.015 | FAIL | SYSTEM | c17 is not installed (status 127) although the system claims the C-Language Development option (c17) |
| XCU.c17.016 | UNRESOLVED | SYSTEM | c17 is not installed (setup c17 failed); UNRESOLVED is the right verdict |
| XCU.c17.017 | FAIL | SYSTEM | c17 is not installed (status 127) although the system claims the C-Language Development option (c17) |
| XCU.c17.018 | FAIL | SYSTEM | c17 is not installed (status 127) although the system claims the C-Language Development option (c17) |
| XCU.c17.019 | FAIL | SYSTEM | c17 is not installed (status 127) although the system claims the C-Language Development option (c17) |
| XCU.cal.001 | FAIL | SYSTEM | cal is not installed (XSI claimed) |
| XCU.cal.002 | FAIL | SYSTEM | cal is not installed (XSI claimed) |
| XCU.cal.003 | FAIL | SYSTEM | cal is not installed (XSI claimed) |
| XCU.cal.004 | FAIL | SYSTEM | cal is not installed (XSI claimed) |
| XCU.cal.005 | FAIL | SYSTEM | cal is not installed (XSI claimed) |
| XCU.cal.007 | FAIL | SYSTEM | cal is not installed (XSI claimed) |
| XCU.cd.012 | FAIL | SYSTEM | cd '' exits 0; an empty directory operand must give a diagnostic and nonzero status (cd OPERANDS) |
| XCU.cd.013 | FAIL | SYSTEM | cd nosuch/.. exits 0; step 8 of cd makes a dot-dot after a non-directory an error |
| XCU.cflow.001 | FAIL | SYSTEM | cflow is not installed (XSI claimed) |
| XCU.cflow.002 | FAIL | SYSTEM | cflow is not installed (XSI claimed) |
| XCU.cflow.003 | FAIL | SYSTEM | cflow is not installed (XSI claimed) |
| XCU.cflow.004 | FAIL | SYSTEM | cflow is not installed (XSI claimed) |
| XCU.cflow.005 | FAIL | SYSTEM | cflow is not installed (XSI claimed) |
| XCU.cflow.006 | FAIL | SYSTEM | cflow is not installed (XSI claimed) |
| XCU.cflow.008 | UNRESOLVED | ENV | c17 is missing, so the test cannot build its input; UNRESOLVED is the right verdict |
| XCU.comm.006 | FAIL | SYSTEM | the system's libc does not select a locale named by an absolute pathname (XBD 7.1) and its localedef drops the portable symbolic names of the charmap, so the made locale has no effect; the first row (category set to the locale) already gives the POSIX-locale result (XBD 7.1, localedef NAME) |
| XCU.command.004 | FAIL | SYSTEM | command -v ./bin/pccmd writes the name as given, not an absolute pathname (command -v) |
| XCU.compress.001 | FAIL | SYSTEM | compress is not installed (XSI claimed) |
| XCU.compress.002 | FAIL | SYSTEM | compress is not installed (XSI claimed) |
| XCU.compress.003 | FAIL | SYSTEM | compress is not installed (XSI claimed) |
| XCU.compress.004 | FAIL | SYSTEM | compress is not installed (XSI claimed) |
| XCU.compress.005 | FAIL | SYSTEM | compress is not installed (XSI claimed) |
| XCU.compress.006 | FAIL | SYSTEM | compress is not installed (XSI claimed) |
| XCU.compress.007 | FAIL | SYSTEM | compress is not installed (XSI claimed) |
| XCU.compress.009 | FAIL | SYSTEM | compress is not installed (XSI claimed) |
| XCU.compress.010 | UNRESOLVED | SYSTEM | compress is not installed; setup compress -c failed, UNRESOLVED is the right verdict |
| XCU.compress.011 | FAIL | SYSTEM | compress is not installed (XSI claimed) |
| XCU.compress.012 | FAIL | SYSTEM | compress is not installed (XSI claimed) |
| XCU.compress.013 | FAIL | SYSTEM | compress is not installed (XSI claimed) |
| XCU.cp.009 | FAIL | SYSTEM | cp -i/mv -i exit 1 after a negative reply; the status excludes files whose confirmation was refused (cp/mv EXIT STATUS) |
| XCU.cp.029 | FAIL | SYSTEM | the system's libc does not select a locale named by an absolute pathname (XBD 7.1) and its localedef drops the portable symbolic names of the charmap, so the made locale has no effect; the first row (category set to the locale) already gives the POSIX-locale result (XBD 7.1, localedef NAME) |
| XCU.cp.030 | FAIL | SYSTEM | the system's libc does not select a locale named by an absolute pathname (XBD 7.1) and its localedef drops the portable symbolic names of the charmap, so the made locale has no effect; the first row (category set to the locale) already gives the POSIX-locale result (XBD 7.1, localedef NAME) |
| XCU.cp.031 | FAIL | SYSTEM | the system's libc does not select a locale named by an absolute pathname (XBD 7.1) and its localedef drops the portable symbolic names of the charmap, so the made locale has no effect; the first row (category set to the locale) already gives the POSIX-locale result (XBD 7.1, localedef NAME) |
| XCU.crontab.001 | UNRESOLVED | SYSTEM | crontab is not installed (status 127); UNRESOLVED stays acceptable |
| XCU.crontab.002 | UNRESOLVED | SYSTEM | crontab is not installed (status 127); UNRESOLVED stays acceptable |
| XCU.crontab.003 | UNRESOLVED | SYSTEM | crontab is not installed (status 127); UNRESOLVED stays acceptable |
| XCU.crontab.004 | UNRESOLVED | SYSTEM | crontab is not installed (status 127); UNRESOLVED stays acceptable |
| XCU.crontab.005 | UNRESOLVED | SYSTEM | crontab is not installed (status 127); UNRESOLVED stays acceptable |
| XCU.csplit.009 | FAIL | SYSTEM | the system's libc does not select a locale named by an absolute pathname (XBD 7.1) and its localedef drops the portable symbolic names of the charmap, so the made locale has no effect; the first row (category set to the locale) already gives the POSIX-locale result (XBD 7.1, localedef NAME) |
| XCU.csplit.010 | FAIL | SYSTEM | the system's libc does not select a locale named by an absolute pathname (XBD 7.1) and its localedef drops the portable symbolic names of the charmap, so the made locale has no effect; the first row (category set to the locale) already gives the POSIX-locale result (XBD 7.1, localedef NAME) |
| XCU.ctags.001 | FAIL | SYSTEM | ctags is not installed (CD or SD claimed) |
| XCU.ctags.002 | FAIL | SYSTEM | ctags is not installed (CD or SD claimed) |
| XCU.ctags.003 | FAIL | SYSTEM | ctags is not installed (CD or SD claimed) |
| XCU.ctags.004 | FAIL | SYSTEM | ctags is not installed (CD or SD claimed) |
| XCU.ctags.005 | FAIL | SYSTEM | ctags is not installed (CD or SD claimed) |
| XCU.ctags.006 | FAIL | SYSTEM | ctags is not installed (CD or SD claimed) |
| XCU.cut.002 | FAIL | TEST-BUG | expected output for the second line was wrong (cut -c 1,4-5,8 of ijklmnop is ilmp); 002-chars-list.sh fixed, re-run PASS |
| XCU.cut.005 | FAIL | TEST-BUG | expected output for the second line was wrong (cut -b 3-9 of wxyz is yz); 005-beyond-line-end.sh fixed, re-run PASS |
| XCU.cut.007 | FAIL | TEST-BUG | line w,x:y,z has a colon, so -d : -f 2,4 gives y,z, not the whole line; 007-field-delimiter.sh fixed, re-run PASS |
| XCU.cut.013 | FAIL | SYSTEM | the system's libc does not select a locale named by an absolute pathname (XBD 7.1) and its localedef drops the portable symbolic names of the charmap, so the made locale has no effect; the first row (category set to the locale) already gives the POSIX-locale result (XBD 7.1, localedef NAME) |
| XCU.cxref.001 | FAIL | SYSTEM | cxref is not installed (XSI claimed) |
| XCU.cxref.002 | FAIL | SYSTEM | cxref is not installed (XSI claimed) |
| XCU.cxref.003 | FAIL | SYSTEM | cxref is not installed (XSI claimed) |
| XCU.cxref.004 | FAIL | SYSTEM | cxref is not installed (XSI claimed) |
| XCU.cxref.005 | FAIL | SYSTEM | cxref is not installed (XSI claimed) |
| XCU.cxref.006 | FAIL | SYSTEM | cxref is not installed (XSI claimed) |
| XCU.cxref.007 | FAIL | SYSTEM | cxref is not installed (XSI claimed) |
| XCU.date.010 | FAIL | SYSTEM | the system's libc does not select a locale named by an absolute pathname (XBD 7.1) and its localedef drops the portable symbolic names of the charmap, so the made locale has no effect; the first row (category set to the locale) already gives the POSIX-locale result (XBD 7.1, localedef NAME) |
| XCU.dd.020 | FAIL | SYSTEM | dd has no iflags operand (only the singular iflag), so iflags=fullblock fails (dd OPERANDS) |
| XCU.delta.001 | UNRESOLVED | SYSTEM | delta/admin (SCCS, XSI) are absent although the system claims XSI; UNRESOLVED is the right verdict |
| XCU.delta.002 | UNRESOLVED | SYSTEM | delta/admin (SCCS, XSI) are absent although the system claims XSI; UNRESOLVED is the right verdict |
| XCU.delta.003 | UNRESOLVED | SYSTEM | delta/admin (SCCS, XSI) are absent although the system claims XSI; UNRESOLVED is the right verdict |
| XCU.delta.004 | UNRESOLVED | SYSTEM | delta/admin (SCCS, XSI) are absent although the system claims XSI; UNRESOLVED is the right verdict |
| XCU.delta.005 | UNRESOLVED | SYSTEM | delta/admin (SCCS, XSI) are absent although the system claims XSI; UNRESOLVED is the right verdict |
| XCU.delta.006 | UNRESOLVED | SYSTEM | delta/admin (SCCS, XSI) are absent although the system claims XSI; UNRESOLVED is the right verdict |
| XCU.df.001 | FAIL | SYSTEM | df -P writes the 1024-blocks header without -k; 512-blocks is required (df STDOUT) |
| XCU.df.005 | FAIL | SYSTEM | df -P reports 1024-byte units without -k (df: 512-byte units unless -k) |
| XCU.df.008 | FAIL | SYSTEM | df -t is not supported (XSI option) |
| XCU.diff.003 | FAIL | SYSTEM | the -c header puts a tab, not a space, between the file name and the date (diff -c format) |
| XCU.diff.011 | FAIL | TEST-BUG | set -- $line expanded the glob *** against the directory; set -f is now used around it (011-timezone-context-header.sh), re-run PASS |
| XCU.dot.006 | FAIL | SYSTEM | an interactive sh exits when . does not find its file; it must continue (XCU 2.8.1) |
| XCU.du.005 | FAIL | SYSTEM | du -s counts 1024-byte units without -k (du: 512-byte units unless -k) |
| XCU.echo.005 | FAIL | SYSTEM | the shell's echo treats -n as an option; XSI echo has no options (echo OPERANDS) |
| XCU.echo.006 | FAIL | SYSTEM | the echo utility run through exec does not interpret the XSI escape sequences (echo OPERANDS) |
| XCU.echo.007 | FAIL | SYSTEM | the echo utility run through exec does not interpret the XSI escape sequences (echo OPERANDS) |
| XCU.echo.008 | FAIL | SYSTEM | the echo utility run through exec does not interpret the XSI escape sequences (echo OPERANDS) |
| XCU.ed.013 | FAIL | TEST-BUG | ed may stop reading a script at a command error (CONSEQUENCES OF ERRORS), so the commands after the error are now optional in the expected output; re-run PASS |
| XCU.ed.017 | FAIL | TEST-BUG | ed may stop reading a script at a command error (CONSEQUENCES OF ERRORS), so the commands after the error are now optional in the expected output; re-run PASS |
| XCU.ed.019 | FAIL | TEST-BUG | the expected file lacked the final newline (printf ... | tr); fixed, re-run PASS |
| XCU.ed.021 | FAIL | TEST-BUG | end of input acts as q, which warns (?) on a changed buffer; the scripts now end with Q; re-run PASS |
| XCU.ed.024 | FAIL | TEST-BUG | p sets the current line to the last line printed, so .= must come before ,p, and the script now ends with Q; fixed, re-run PASS |
| XCU.ed.025 | FAIL | TEST-BUG | end of input acts as q, which warns (?) on a changed buffer; the scripts now end with Q; re-run PASS |
| XCU.exec.005 | FAIL | SYSTEM | exec does not accept -- before the utility (XBD 12.2 for special built-ins) |
| XCU.expr.005 | FAIL | TEST-BUG | 1 + + 2 is not necessarily invalid (the second + can be a string operand); that line removed from 005-invalid.sh, re-run PASS |
| XCU.expr.010 | FAIL | SYSTEM | the system's libc does not select a locale named by an absolute pathname (XBD 7.1) and its localedef drops the portable symbolic names of the charmap, so the made locale has no effect; the first row (category set to the locale) already gives the POSIX-locale result (XBD 7.1, localedef NAME) |
| XCU.expr.011 | FAIL | SYSTEM | the system's libc does not select a locale named by an absolute pathname (XBD 7.1) and its localedef drops the portable symbolic names of the charmap, so the made locale has no effect; the first row (category set to the locale) already gives the POSIX-locale result (XBD 7.1, localedef NAME) |
| XCU.expr.012 | FAIL | SYSTEM | the system's libc does not select a locale named by an absolute pathname (XBD 7.1) and its localedef drops the portable symbolic names of the charmap, so the made locale has no effect; the first row (category set to the locale) already gives the POSIX-locale result (XBD 7.1, localedef NAME) |
| XCU.file.006 | FAIL | SYSTEM | file -i writes a MIME type instead of identifying a regular file as such (file -i) |
| XCU.file.008 | FAIL | SYSTEM | file does not resolve a symbolic link by default (file DESCRIPTION) |
| XCU.file.009 | FAIL | SYSTEM | file has no -M option (file OPTIONS) |
| XCU.find.034 | FAIL | SYSTEM | the system's libc does not select a locale named by an absolute pathname (XBD 7.1) and its localedef drops the portable symbolic names of the charmap, so the made locale has no effect; the first row (category set to the locale) already gives the POSIX-locale result (XBD 7.1, localedef NAME) |
| XCU.find.035 | FAIL | SYSTEM | the system's libc does not select a locale named by an absolute pathname (XBD 7.1) and its localedef drops the portable symbolic names of the charmap, so the made locale has no effect; the first row (category set to the locale) already gives the POSIX-locale result (XBD 7.1, localedef NAME) |
| XCU.find.036 | FAIL | SYSTEM | the system's libc does not select a locale named by an absolute pathname (XBD 7.1) and its localedef drops the portable symbolic names of the charmap, so the made locale has no effect; the first row (category set to the locale) already gives the POSIX-locale result (XBD 7.1, localedef NAME) |
| XCU.gencat.001 | UNRESOLVED | ENV | c17 is missing, so the test cannot build the program that reads the catalog; UNRESOLVED is the right verdict |
| XCU.gencat.002 | UNRESOLVED | ENV | c17 is missing, so the test cannot build the program that reads the catalog; UNRESOLVED is the right verdict |
| XCU.gencat.003 | UNRESOLVED | ENV | c17 is missing, so the test cannot build the program that reads the catalog; UNRESOLVED is the right verdict |
| XCU.gencat.004 | UNRESOLVED | ENV | c17 is missing, so the test cannot build the program that reads the catalog; UNRESOLVED is the right verdict |
| XCU.gencat.005 | UNRESOLVED | ENV | c17 is missing, so the test cannot build the program that reads the catalog; UNRESOLVED is the right verdict |
| XCU.get.001 | UNRESOLVED | SYSTEM | admin/get (SCCS, XSI) are absent although the system claims XSI; UNRESOLVED is the right verdict |
| XCU.get.002 | UNRESOLVED | SYSTEM | admin/get (SCCS, XSI) are absent although the system claims XSI; UNRESOLVED is the right verdict |
| XCU.get.003 | UNRESOLVED | SYSTEM | admin/get (SCCS, XSI) are absent although the system claims XSI; UNRESOLVED is the right verdict |
| XCU.get.004 | UNRESOLVED | SYSTEM | admin/get (SCCS, XSI) are absent although the system claims XSI; UNRESOLVED is the right verdict |
| XCU.get.005 | UNRESOLVED | SYSTEM | admin/get (SCCS, XSI) are absent although the system claims XSI; UNRESOLVED is the right verdict |
| XCU.get.006 | UNRESOLVED | SYSTEM | admin/get (SCCS, XSI) are absent although the system claims XSI; UNRESOLVED is the right verdict |
| XCU.get.007 | UNRESOLVED | SYSTEM | admin/get (SCCS, XSI) are absent although the system claims XSI; UNRESOLVED is the right verdict |
| XCU.get.008 | UNRESOLVED | SYSTEM | admin/get (SCCS, XSI) are absent although the system claims XSI; UNRESOLVED is the right verdict |
| XCU.get.009 | UNRESOLVED | SYSTEM | admin/get (SCCS, XSI) are absent although the system claims XSI; UNRESOLVED is the right verdict |
| XCU.getconf.005 | FAIL | SYSTEM | getconf does not know the underscore-prefixed forms _POSIX2_C_BIND ... (getconf OPERANDS) |
| XCU.getconf.006 | FAIL | SYSTEM | getconf has no NPROCESSORS_CONF/NPROCESSORS_ONLN (only the underscore forms) |
| XCU.getconf.007 | FAIL | SYSTEM | getconf _POSIX_ARG_MAX writes the actual limit, not the minimum value 4096 (limits.h minimum values) |
| XCU.getopts.011 | FAIL | SYSTEM | OPTARG is set to empty, not unset, after a flag without argument (getopts DESCRIPTION) |
| XCU.getopts.016 | FAIL | SYSTEM | the diagnostic does not name the invoking program (getopts STDERR) |
| XCU.gettext.001 | FAIL | SYSTEM | gettext is not installed (status 127) |
| XCU.gettext.002 | FAIL | SYSTEM | gettext is not installed (status 127) |
| XCU.gettext.003 | FAIL | SYSTEM | gettext is not installed (status 127) |
| XCU.grep.009 | FAIL | SYSTEM | the system's libc does not select a locale named by an absolute pathname (XBD 7.1) and its localedef drops the portable symbolic names of the charmap, so the made locale has no effect; the first row (category set to the locale) already gives the POSIX-locale result (XBD 7.1, localedef NAME) |
| XCU.grep.010 | FAIL | SYSTEM | the system's libc does not select a locale named by an absolute pathname (XBD 7.1) and its localedef drops the portable symbolic names of the charmap, so the made locale has no effect; the first row (category set to the locale) already gives the POSIX-locale result (XBD 7.1, localedef NAME) |
| XCU.ipcs.001 | FAIL | SYSTEM | ipcs does not write the 'IPC status from ... as of ...' line (ipcs STDOUT) |
| XCU.ipcs.005 | FAIL | SYSTEM | ipcs has no -o option (ipcs OPTIONS) |
| XCU.join.011 | FAIL | SYSTEM | the system's libc does not select a locale named by an absolute pathname (XBD 7.1) and its localedef drops the portable symbolic names of the charmap, so the made locale has no effect; the first row (category set to the locale) already gives the POSIX-locale result (XBD 7.1, localedef NAME) |
| XCU.lex.001 | UNRESOLVED | ENV | c17 (and lex) are missing, so setup stops; UNRESOLVED is the right verdict |
| XCU.lex.002 | UNRESOLVED | ENV | c17 (and lex) are missing, so setup stops; UNRESOLVED is the right verdict |
| XCU.lex.003 | UNRESOLVED | ENV | c17 (and lex) are missing, so setup stops; UNRESOLVED is the right verdict |
| XCU.lex.004 | UNRESOLVED | ENV | c17 (and lex) are missing, so setup stops; UNRESOLVED is the right verdict |
| XCU.lex.005 | UNRESOLVED | ENV | c17 (and lex) are missing, so setup stops; UNRESOLVED is the right verdict |
| XCU.lex.006 | UNRESOLVED | ENV | c17 (and lex) are missing, so setup stops; UNRESOLVED is the right verdict |
| XCU.lex.007 | UNRESOLVED | ENV | c17 (and lex) are missing, so setup stops; UNRESOLVED is the right verdict |
| XCU.lex.008 | UNRESOLVED | ENV | c17 (and lex) are missing, so setup stops; UNRESOLVED is the right verdict |
| XCU.lex.009 | UNRESOLVED | ENV | c17 (and lex) are missing, so setup stops; UNRESOLVED is the right verdict |
| XCU.localedef.003 | FAIL | STD-AMBIG | localedef leaves an empty directory at NAME after the error; whether that is 'permanent output' is not settled (new Q-203); the test now accepts an empty directory, class disputed, re-run PASS |
| XCU.localedef.004 | FAIL | STD-AMBIG | localedef leaves an empty directory at NAME after the error; whether that is 'permanent output' is not settled (new Q-203); the test now accepts an empty directory, class disputed, re-run PASS |
| XCU.ls.018 | FAIL | SYSTEM | ls -l total is in 1024-byte units without -k (ls STDOUT: 512-byte units) |
| XCU.ls.039 | FAIL | SYSTEM | ls -f -t sorts by time; -r, -S and -t are to be ignored with -f (ls -f) |
| XCU.ls.043 | FAIL | TEST-BUG | the page's second form (name -> contents plus the target's indicator, no @) was refused; the test and the .asr now accept it, re-run PASS |
| XCU.ls.044 | FAIL | TEST-BUG | ls -s writes the total line also with -1; the test expected only the file line; fixed, re-run PASS |
| XCU.ls.053 | FAIL | SYSTEM | the system's libc does not select a locale named by an absolute pathname (XBD 7.1) and its localedef drops the portable symbolic names of the charmap, so the made locale has no effect; the first row (category set to the locale) already gives the POSIX-locale result (XBD 7.1, localedef NAME) |
| XCU.ls.054 | FAIL | SYSTEM | the system's libc does not select a locale named by an absolute pathname (XBD 7.1) and its localedef drops the portable symbolic names of the charmap, so the made locale has no effect; the first row (category set to the locale) already gives the POSIX-locale result (XBD 7.1, localedef NAME) |
| XCU.m4.001 | FAIL | TEST-BUG | the macro body used $0 unquoted, which is rescanned and recurses forever in any m4; it is now quoted (`$0'), re-run PASS |
| XCU.m4.007 | FAIL | SYSTEM | m4wrap texts are processed last-in first-out (BA), not in the order of the m4wrap calls (m4 m4wrap) |
| XCU.m4.016 | FAIL | SYSTEM | m4 exits 0 although mkstemp could not create its file (m4 mkstemp) |
| XCU.make.008 | FAIL | SYSTEM | make -q exits 0 after running a + command on an out-of-date target; 1 is required (make -q) |
| XCU.make.011 | FAIL | SYSTEM | make has no ':::=' macro assignment (make macros, Issue 8) |
| XCU.make.020 | FAIL | SYSTEM | make has no .WAIT, so y may run before x (make -j, .WAIT) |
| XCU.make.027 | FAIL | SYSTEM | make removes the target on SIGTERM although '.PRECIOUS:' without prerequisites makes all targets precious (make .PRECIOUS) |
| XCU.msgfmt.001 | FAIL | SYSTEM | msgfmt is not installed (status 127) |
| XCU.msgfmt.002 | FAIL | SYSTEM | msgfmt is not installed (status 127) |
| XCU.msgfmt.003 | FAIL | SYSTEM | msgfmt is not installed (status 127) |
| XCU.msgfmt.004 | FAIL | SYSTEM | msgfmt is not installed (status 127) |
| XCU.msgfmt.005 | FAIL | SYSTEM | msgfmt is not installed (status 127), so the check that a consistent entry passes fails (its earlier 'must be diagnosed' checks pass vacuously on 127) |
| XCU.msgfmt.006 | FAIL | SYSTEM | msgfmt is not installed (status 127) |
| XCU.mv.005 | FAIL | SYSTEM | cp -i/mv -i exit 1 after a negative reply; the status excludes files whose confirmation was refused (cp/mv EXIT STATUS) |
| XCU.mv.015 | FAIL | SYSTEM | the system's libc does not select a locale named by an absolute pathname (XBD 7.1) and its localedef drops the portable symbolic names of the charmap, so the made locale has no effect; the first row (category set to the locale) already gives the POSIX-locale result (XBD 7.1, localedef NAME) |
| XCU.mv.016 | FAIL | SYSTEM | the system's libc does not select a locale named by an absolute pathname (XBD 7.1) and its localedef drops the portable symbolic names of the charmap, so the made locale has no effect; the first row (category set to the locale) already gives the POSIX-locale result (XBD 7.1, localedef NAME) |
| XCU.mv.017 | FAIL | SYSTEM | the system's libc does not select a locale named by an absolute pathname (XBD 7.1) and its localedef drops the portable symbolic names of the charmap, so the made locale has no effect; the first row (category set to the locale) already gives the POSIX-locale result (XBD 7.1, localedef NAME) |
| XCU.newgrp.003 | FAIL | SYSTEM | newgrp rejects a numeric group ID (newgrp OPERANDS) |

## Counts

- SYSTEM: 180
- TEST-BUG: 15
- ENV: 15
- STD-AMBIG: 4
- total: 214

## Patterns

- Absent utilities (about 100 lines): c17, cal, compress, ctags, cflow, cxref, admin/get/delta/prs, at, batch, crontab, gettext, msgfmt, lex, time. The system claims POSIX2_C_DEV, POSIX2_SW_DEV and _XOPEN_UNIX, or the utility is mandatory, so these are counted as SYSTEM; if a minimal image should rather count as the environment they move to ENV in one block. Tests that only need c17 for setup (gencat, lex, cflow.008) stay UNRESOLVED and are counted ENV. Tests that expect a nonzero status for bad input (for example msgfmt.001 to 004, 'must be diagnosed') pass vacuously on status 127; not changed here.
- Locale matrix tests (26 lines): the libc does not select a locale named by an absolute pathname and its localedef cannot digest the portable-name charmap; tests are right.
- GNU and dash defaults that differ from the standard: 1024-byte units (df, du, ls), echo, cut/ls -f options, ar deterministic members, mawk (RS null, srand, nonexistent fields, read-ahead), dash (no $'..', cd '', exec --, getopts, dot in an interactive shell, PWD with dot components).
- Test bugs found: wrong expected data (cut x3, ed.019, ed.024), glob expansion of an unquoted line (diff.011), unquoted $# under IFS=digit (2.6.043), scripts that leave a changed ed buffer without Q (end of input acts as q), expecting ed to go on after a command error (it may stop), $0 unquoted in m4 (infinite rescan), ls -s total line, and the second ls -lF symlink form.
- New standard issues: Q-202 (bc quit when read or executed), Q-203 (localedef: empty directory at NAME after an error).
