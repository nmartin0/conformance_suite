# Triage of the first native run, group xcu2

Run: native, root, the shell is dash (`sh` on the path), GNU coreutils/util-linux/patch/procps/binutils; list of 215 non-PASS lines (modules ngettext to xgettext). Linux was not used as the oracle: each verdict rests on the page named.

| ID | code | class | reason |
|---|---|---|---|
| XCU.ngettext.001 | FAIL | SYSTEM | ngettext (required utility) is not installed: exit 127 (XCU ngettext) |
| XCU.ngettext.002 | FAIL | SYSTEM | ngettext (required utility) is not installed: exit 127 (XCU ngettext) |
| XCU.ngettext.003 | FAIL | SYSTEM | ngettext (required utility) is not installed: exit 127 (XCU ngettext) |
| XCU.nl.002 | FAIL | SYSTEM | writes a padded blank number field and separator for an unnumbered line instead of the line alone (XCU nl STDOUT: line number and separator are empty) |
| XCU.nl.009 | FAIL | TEST-BUG | pcloc_make trusted a locale that localedef had flagged (status 1: symbols of the charmap such as <c>, <comma> not accepted inside strings) and that is not in effect when named; the utility under test was blamed (FAIL) for a locale never selected. Fixed in core/pcloc.sh (pcloc__verify: the locale utility must show the comma radix / a charmap other than the POSIX one, else UNRESOLVED). Re-run: UNRESOLVED 'locale ... not in effect' (the system's localedef cannot build the test locale from a source that uses charmap symbols; XCU localedef, locale definition source) |
| XCU.nl.010 | FAIL | TEST-BUG | locale helper issue, as XCU.nl.009 (pcloc_make trusted a locale that is not in effect); now UNRESOLVED |
| XCU.nm.001 | UNRESOLVED | ENV | setup needs c17 to build an object file; c17 is not installed (CD/SD claimed through headers, so UNRESOLVED is the right verdict) |
| XCU.nm.002 | UNRESOLVED | ENV | setup needs c17 to build an object file; c17 is not installed (CD/SD claimed through headers, so UNRESOLVED is the right verdict) |
| XCU.nm.003 | UNRESOLVED | ENV | setup needs c17 to build an object file; c17 is not installed (CD/SD claimed through headers, so UNRESOLVED is the right verdict) |
| XCU.nm.004 | UNRESOLVED | ENV | setup needs c17 to build an object file; c17 is not installed (CD/SD claimed through headers, so UNRESOLVED is the right verdict) |
| XCU.nm.005 | UNRESOLVED | ENV | setup needs c17 to build an object file; c17 is not installed (CD/SD claimed through headers, so UNRESOLVED is the right verdict) |
| XCU.nm.006 | UNRESOLVED | ENV | setup needs c17 to build an object file; c17 is not installed (CD/SD claimed through headers, so UNRESOLVED is the right verdict) |
| XCU.nm.007 | UNRESOLVED | ENV | setup needs c17 to build an object file; c17 is not installed (CD/SD claimed through headers, so UNRESOLVED is the right verdict) |
| XCU.nm.008 | UNRESOLVED | ENV | setup needs c17 to build an object file; c17 is not installed (CD/SD claimed through headers, so UNRESOLVED is the right verdict) |
| XCU.nm.009 | UNRESOLVED | ENV | setup needs c17 to build an object file; c17 is not installed (CD/SD claimed through headers, so UNRESOLVED is the right verdict) |
| XCU.od.011 | FAIL | SYSTEM | od -b f1 +2. : the +offset. operand is taken as a file name (XCU od OPERANDS, XSI offset operand) |
| XCU.paste.009 | FAIL | TEST-BUG | locale helper issue, as XCU.nl.009 (pcloc_make trusted a locale that is not in effect); now UNRESOLVED |
| XCU.patch.005 | FAIL | SYSTEM | -b with -o and an existing outfile did not create outfile.orig (XCU patch -b: outfile.orig shall be created) |
| XCU.patch.010 | FAIL | SYSTEM | -N writes the already-applied hunk to the reject file ('saving rejects') although the page says -N ignores it and only the default rejects it (XCU patch -N) |
| XCU.pax.001 | FAIL | SYSTEM | pax (required utility) is not installed: write mode fails with 127 (or setup 'pax -w failed' for read/list tests) |
| XCU.pax.002 | FAIL | SYSTEM | pax (required utility) is not installed: write mode fails with 127 (or setup 'pax -w failed' for read/list tests) |
| XCU.pax.003 | UNRESOLVED | SYSTEM | pax (required utility) is not installed: write mode fails with 127 (or setup 'pax -w failed' for read/list tests) |
| XCU.pax.004 | UNRESOLVED | SYSTEM | pax (required utility) is not installed: write mode fails with 127 (or setup 'pax -w failed' for read/list tests) |
| XCU.pax.005 | FAIL | SYSTEM | pax (required utility) is not installed: write mode fails with 127 (or setup 'pax -w failed' for read/list tests) |
| XCU.pax.006 | FAIL | SYSTEM | pax (required utility) is not installed: write mode fails with 127 (or setup 'pax -w failed' for read/list tests) |
| XCU.pax.007 | FAIL | SYSTEM | pax (required utility) is not installed: write mode fails with 127 (or setup 'pax -w failed' for read/list tests) |
| XCU.pax.008 | FAIL | SYSTEM | pax (required utility) is not installed: write mode fails with 127 (or setup 'pax -w failed' for read/list tests) |
| XCU.pax.009 | FAIL | SYSTEM | pax (required utility) is not installed: write mode fails with 127 (or setup 'pax -w failed' for read/list tests) |
| XCU.pax.010 | FAIL | SYSTEM | pax (required utility) is not installed: write mode fails with 127 (or setup 'pax -w failed' for read/list tests) |
| XCU.pax.011 | UNRESOLVED | SYSTEM | pax (required utility) is not installed: write mode fails with 127 (or setup 'pax -w failed' for read/list tests) |
| XCU.pax.012 | UNRESOLVED | SYSTEM | pax (required utility) is not installed: write mode fails with 127 (or setup 'pax -w failed' for read/list tests) |
| XCU.pax.013 | UNRESOLVED | SYSTEM | pax (required utility) is not installed: write mode fails with 127 (or setup 'pax -w failed' for read/list tests) |
| XCU.pax.014 | UNRESOLVED | SYSTEM | pax (required utility) is not installed: write mode fails with 127 (or setup 'pax -w failed' for read/list tests) |
| XCU.pax.015 | UNRESOLVED | SYSTEM | pax (required utility) is not installed: write mode fails with 127 (or setup 'pax -w failed' for read/list tests) |
| XCU.pax.016 | FAIL | SYSTEM | pax (required utility) is not installed: write mode fails with 127 (or setup 'pax -w failed' for read/list tests) |
| XCU.pax.017 | FAIL | SYSTEM | pax (required utility) is not installed: write mode fails with 127 (or setup 'pax -w failed' for read/list tests) |
| XCU.pax.018 | FAIL | SYSTEM | pax (required utility) is not installed: write mode fails with 127 (or setup 'pax -w failed' for read/list tests) |
| XCU.pax.019 | FAIL | SYSTEM | pax (required utility) is not installed: write mode fails with 127 (or setup 'pax -w failed' for read/list tests) |
| XCU.pax.020 | UNRESOLVED | SYSTEM | pax (required utility) is not installed: write mode fails with 127 (or setup 'pax -w failed' for read/list tests) |
| XCU.pax.021 | FAIL | SYSTEM | pax (required utility) is not installed: write mode fails with 127 (or setup 'pax -w failed' for read/list tests) |
| XCU.pax.022 | UNRESOLVED | SYSTEM | pax (required utility) is not installed: write mode fails with 127 (or setup 'pax -w failed' for read/list tests) |
| XCU.pax.023 | FAIL | SYSTEM | pax (required utility) is not installed: write mode fails with 127 (or setup 'pax -w failed' for read/list tests) |
| XCU.pax.025 | FAIL | SYSTEM | pax (required utility) is not installed: write mode fails with 127 (or setup 'pax -w failed' for read/list tests) |
| XCU.pax.026 | UNRESOLVED | SYSTEM | pax (required utility) is not installed: write mode fails with 127 (or setup 'pax -w failed' for read/list tests) |
| XCU.pax.027 | UNRESOLVED | SYSTEM | pax (required utility) is not installed: write mode fails with 127 (or setup 'pax -w failed' for read/list tests) |
| XCU.pax.028 | FAIL | SYSTEM | pax (required utility) is not installed: write mode fails with 127 (or setup 'pax -w failed' for read/list tests) |
| XCU.pax.029 | FAIL | SYSTEM | pax (required utility) is not installed: write mode fails with 127 (or setup 'pax -w failed' for read/list tests) |
| XCU.pax.030 | UNRESOLVED | SYSTEM | pax (required utility) is not installed: write mode fails with 127 (or setup 'pax -w failed' for read/list tests) |
| XCU.pax.031 | UNRESOLVED | SYSTEM | pax (required utility) is not installed: write mode fails with 127 (or setup 'pax -w failed' for read/list tests) |
| XCU.pax.032 | FAIL | SYSTEM | pax (required utility) is not installed: write mode fails with 127 (or setup 'pax -w failed' for read/list tests) |
| XCU.pax.033 | FAIL | SYSTEM | pax (required utility) is not installed: write mode fails with 127 (or setup 'pax -w failed' for read/list tests) |
| XCU.pax.034 | FAIL | SYSTEM | pax (required utility) is not installed: write mode fails with 127 (or setup 'pax -w failed' for read/list tests) |
| XCU.pax.035 | FAIL | SYSTEM | pax (required utility) is not installed: write mode fails with 127 (or setup 'pax -w failed' for read/list tests) |
| XCU.pax.036 | FAIL | SYSTEM | pax (required utility) is not installed: write mode fails with 127 (or setup 'pax -w failed' for read/list tests) |
| XCU.pax.037 | UNRESOLVED | SYSTEM | pax (required utility) is not installed: write mode fails with 127 (or setup 'pax -w failed' for read/list tests) |
| XCU.pax.038 | FAIL | SYSTEM | pax (required utility) is not installed: write mode fails with 127 (or setup 'pax -w failed' for read/list tests) |
| XCU.pax.039 | UNRESOLVED | SYSTEM | pax (required utility) is not installed: write mode fails with 127 (or setup 'pax -w failed' for read/list tests) |
| XCU.pax.040 | UNRESOLVED | SYSTEM | pax (required utility) is not installed: write mode fails with 127 (or setup 'pax -w failed' for read/list tests) |
| XCU.pax.041 | UNRESOLVED | SYSTEM | pax (required utility) is not installed: write mode fails with 127 (or setup 'pax -w failed' for read/list tests) |
| XCU.pax.042 | UNRESOLVED | SYSTEM | pax (required utility) is not installed: write mode fails with 127 (or setup 'pax -w failed' for read/list tests) |
| XCU.pax.043 | FAIL | SYSTEM | pax (required utility) is not installed: write mode fails with 127 (or setup 'pax -w failed' for read/list tests) |
| XCU.pax.044 | FAIL | SYSTEM | pax (required utility) is not installed: write mode fails with 127 (or setup 'pax -w failed' for read/list tests) |
| XCU.pax.045 | FAIL | SYSTEM | pax (required utility) is not installed: write mode fails with 127 (or setup 'pax -w failed' for read/list tests) |
| XCU.pax.046 | UNRESOLVED | SYSTEM | pax (required utility) is not installed: write mode fails with 127 (or setup 'pax -w failed' for read/list tests) |
| XCU.pr.001 | FAIL | SYSTEM | header date is not '%b %e %H:%M %Y' in the POSIX locale (an ISO date is written) (XCU pr STDOUT) |
| XCU.pr.002 | FAIL | SYSTEM | page header is padded/centred, not the form '\n\n%s %s Page %d\n\n\n' (XCU pr STDOUT) |
| XCU.pr.003 | FAIL | SYSTEM | header date is not '%b %e %H:%M %Y' in the POSIX locale (an ISO date is written) (XCU pr STDOUT) |
| XCU.pr.012 | FAIL | SYSTEM | page header is padded/centred, not the form '\n\n%s %s Page %d\n\n\n' (XCU pr STDOUT) |
| XCU.pr.013 | FAIL | SYSTEM | header date is not '%b %e %H:%M %Y' in the POSIX locale (an ISO date is written) (XCU pr STDOUT) |
| XCU.printf.012 | FAIL | SYSTEM | %n$ conversions are rejected by both the shell printf and the printf utility (XCU printf, Issue 8) |
| XCU.printf.015 | FAIL | TEST-BUG | locale helper issue, as XCU.nl.009 (pcloc_make trusted a locale that is not in effect); now UNRESOLVED |
| XCU.prs.001 | UNRESOLVED | ENV | setup needs admin to create the SCCS file; admin (XSI) is not installed |
| XCU.prs.002 | UNRESOLVED | ENV | setup needs admin to create the SCCS file; admin (XSI) is not installed |
| XCU.prs.003 | UNRESOLVED | ENV | setup needs admin to create the SCCS file; admin (XSI) is not installed |
| XCU.prs.004 | UNRESOLVED | ENV | setup needs admin to create the SCCS file; admin (XSI) is not installed |
| XCU.prs.005 | UNRESOLVED | ENV | setup needs admin to create the SCCS file; admin (XSI) is not installed |
| XCU.prs.006 | UNRESOLVED | ENV | setup needs admin to create the SCCS file; admin (XSI) is not installed |
| XCU.ps.015 | FAIL | SYSTEM | default listing shows a zombie as plain 'sh', not marked defunct (XCU ps STDOUT) |
| XCU.pwd.003 | FAIL | SYSTEM | the pwd utility run without options writes the physical path; the page says it behaves as -L |
| XCU.read.009 | FAIL | SYSTEM | the shell's read has no -d (Issue 8): 'Illegal option -d' |
| XCU.read.010 | FAIL | SYSTEM | the shell's read has no -d (Issue 8): 'Illegal option -d' |
| XCU.readlink.004 | FAIL | SYSTEM | no diagnostic for a file that is not a symbolic link (XCU readlink DESCRIPTION) |
| XCU.realpath.004 | FAIL | SYSTEM | realpath has no -E option (Issue 8) |
| XCU.realpath.006 | FAIL | SYSTEM | realpath has no -E option (Issue 8) |
| XCU.renice.005 | FAIL | SYSTEM | renice writes 'old priority/new priority' to standard output (XCU renice STDOUT: not used) |
| XCU.rm.015 | UNRESOLVED | TEST-BUG | setup built the 450-level tree with plain cd; the shell's cd failed at level 338 (cd -L path beyond {PATH_MAX} not converted to a relative one, XCU cd) so rm was never reached. Setup now uses cd -P. Re-run: PASS |
| XCU.rm.016 | FAIL | TEST-BUG | locale helper issue, as XCU.nl.009 (pcloc_make trusted a locale that is not in effect); now UNRESOLVED |
| XCU.rm.017 | FAIL | TEST-BUG | locale helper issue, as XCU.nl.009 (pcloc_make trusted a locale that is not in effect); now UNRESOLVED |
| XCU.rm.018 | FAIL | TEST-BUG | locale helper issue, as XCU.nl.009 (pcloc_make trusted a locale that is not in effect); now UNRESOLVED |
| XCU.rmdel.001 | UNRESOLVED | ENV | setup needs admin to create the SCCS file; admin (XSI) is not installed |
| XCU.rmdel.002 | UNRESOLVED | ENV | setup needs admin to create the SCCS file; admin (XSI) is not installed |
| XCU.rmdel.003 | UNRESOLVED | ENV | setup needs admin to create the SCCS file; admin (XSI) is not installed |
| XCU.rmdel.004 | UNRESOLVED | ENV | setup needs admin to create the SCCS file; admin (XSI) is not installed |
| XCU.rmdel.005 | UNRESOLVED | ENV | setup needs admin to create the SCCS file; admin (XSI) is not installed |
| XCU.sact.001 | UNRESOLVED | ENV | setup needs admin to create the SCCS file; admin (XSI) is not installed |
| XCU.sact.002 | UNRESOLVED | ENV | setup needs admin to create the SCCS file; admin (XSI) is not installed |
| XCU.sact.003 | UNRESOLVED | ENV | setup needs admin to create the SCCS file; admin (XSI) is not installed |
| XCU.sed.014 | FAIL | SYSTEM | N on the last line writes the pattern space (XCU sed: quit without copying it) |
| XCU.sed.021 | FAIL | SYSTEM | r file is read before the lines that w wrote in the same cycle are visible (XCU sed: contents as of the time of output) |
| XCU.sed.022 | FAIL | TEST-BUG | locale helper issue, as XCU.nl.009 (pcloc_make trusted a locale that is not in effect); now UNRESOLVED |
| XCU.sed.023 | FAIL | TEST-BUG | locale helper issue, as XCU.nl.009 (pcloc_make trusted a locale that is not in effect); now UNRESOLVED |
| XCU.sed.024 | FAIL | TEST-BUG | locale helper issue, as XCU.nl.009 (pcloc_make trusted a locale that is not in effect); now UNRESOLVED |
| XCU.set.018 | FAIL | SYSTEM | shell has no pipefail option (Issue 8): 'Illegal option -o pipefail' |
| XCU.sh.008 | FAIL | SYSTEM | sh with a missing command_file exits 2, not 127 (XCU sh EXIT STATUS) |
| XCU.sh.015 | FAIL | SYSTEM | shell has no pipefail option (Issue 8): 'Illegal option -o pipefail' |
| XCU.sh.017 | FAIL | SYSTEM | the shell reads ahead on a pipe, so cat after it gets nothing (XCU sh STDIN) |
| XCU.sh-compound.009 | FAIL | SYSTEM | case ;& is a syntax error in the shell (Issue 8) |
| XCU.sh-errors.010 | FAIL | SYSTEM | after an error the interactive shell discards the input it had already read, so later commands (given in the same read) are lost; with the later lines delayed the same commands pass (XCU 2.8.1) |
| XCU.sh-errors.011 | FAIL | SYSTEM | after an error the interactive shell discards the input it had already read, so later commands (given in the same read) are lost; with the later lines delayed the same commands pass (XCU 2.8.1) |
| XCU.sh-patterns.002 | FAIL | TEST-BUG | pair 'c' 'a[a-c]' cannot match (one character against a two-character pattern); intended 'ac'. Fixed. Re-run: PASS |
| XCU.sh-pipelines.004 | FAIL | SYSTEM | shell has no pipefail option (Issue 8): 'Illegal option -o pipefail' |
| XCU.sh-pipelines.005 | FAIL | SYSTEM | shell has no pipefail option (Issue 8): 'Illegal option -o pipefail' |
| XCU.sort.017 | FAIL | TEST-BUG | locale helper issue, as XCU.nl.009 (pcloc_make trusted a locale that is not in effect); now UNRESOLVED |
| XCU.sort.018 | FAIL | TEST-BUG | locale helper issue, as XCU.nl.009 (pcloc_make trusted a locale that is not in effect); now UNRESOLVED |
| XCU.sort.019 | FAIL | TEST-BUG | locale helper issue, as XCU.nl.009 (pcloc_make trusted a locale that is not in effect); now UNRESOLVED |
| XCU.strings.003 | FAIL | SYSTEM | -t offsets are padded to a width (' 4 defgh') but the page gives '%d %s' |
| XCU.strings.004 | FAIL | TEST-BUG | assertion is about -a scanning the whole file but the test also demanded the exact '-t d' offset format (another assertion's job; padded here). Now checks the string is found without -t. Re-run: PASS |
| XCU.strip.001 | UNRESOLVED | ENV | setup needs c17 to build an object file; c17 is not installed |
| XCU.strip.002 | UNRESOLVED | ENV | setup needs c17 to build an object file; c17 is not installed |
| XCU.strip.004 | UNRESOLVED | ENV | setup needs c17 to build an object file; c17 is not installed |
| XCU.stty.012 | FAIL | SYSTEM | raw leaves erase, kill, intr, quit, eof set; the page makes raw equivalent to 'erase ^- kill ^- intr ^- quit ^- eof ^- eol ^-' (XCU stty raw) |
| XCU.tail.004 | FAIL | SYSTEM | tail has no -r option (Issue 8) |
| XCU.test.005 | FAIL | SYSTEM | the test/[ utility rejects the < and > string operators ('binary operator expected', status 2) (XCU test, Issue 8); the shell built-in accepts them |
| XCU.test.009 | FAIL | SYSTEM | the shell built-in gives false for 'file -nt nonexistent'; the page says true when pathname2 cannot be resolved (XCU test) |
| XCU.test.015 | FAIL | TEST-BUG | locale helper issue, as XCU.nl.009 (pcloc_make trusted a locale that is not in effect); now UNRESOLVED |
| XCU.time.001 | FAIL | SYSTEM | time (required utility) is not installed: exit 127 |
| XCU.time.002 | FAIL | SYSTEM | time (required utility) is not installed: exit 127 |
| XCU.time.003 | FAIL | SYSTEM | time (required utility) is not installed: exit 127 |
| XCU.time.004 | FAIL | SYSTEM | time (required utility) is not installed: exit 127 |
| XCU.time.005 | FAIL | SYSTEM | time (required utility) is not installed: exit 127 |
| XCU.timeout.003 | FAIL | SYSTEM | timeout -s KILL gives 137 not 124 (XCU timeout EXIT STATUS: 124 when the limit was reached and -k not given) |
| XCU.timeout.004 | FAIL | SYSTEM | timeout has no -p option (Issue 8): status 125 |
| XCU.timeout.010 | FAIL | SYSTEM | timeout has no -f option (Issue 8): status 125 |
| XCU.tr.009 | FAIL | TEST-BUG | locale helper issue, as XCU.nl.009 (pcloc_make trusted a locale that is not in effect); now UNRESOLVED |
| XCU.trap.010 | FAIL | SYSTEM | trap -p is not supported by the shell ('Illegal option -p', status 2) (Issue 8) |
| XCU.trap.011 | INSPECT | SYSTEM | trap -p not supported at all (status 2) so the disputed question (what is written for a default condition) is never reached; INSPECT only because the assertion is disputed |
| XCU.trap.012 | FAIL | SYSTEM | trap -p is not supported by the shell ('Illegal option -p', status 2) (Issue 8) |
| XCU.trap.017 | FAIL | SYSTEM | trap with no operands does not list a signal ignored on entry (XCU 2.15 trap) |
| XCU.trap.019 | FAIL | SYSTEM | trap in a command substitution does not list the parent's traps (XCU 2.15 trap) |
| XCU.tsort.004 | FAIL | SYSTEM | tsort has no -w option (Issue 8). The test accepted any usage-error status 1 for the one-cycle case; reordered so the no-cycle case is checked first and the utility is not credited with the rest. Re-run: FAIL (correct) |
| XCU.uncompress.001 | UNRESOLVED | ENV | setup needs compress (XSI) which is not installed |
| XCU.uncompress.002 | UNRESOLVED | ENV | setup needs compress (XSI) which is not installed |
| XCU.uncompress.003 | UNRESOLVED | ENV | setup needs compress (XSI) which is not installed |
| XCU.uncompress.004 | UNRESOLVED | ENV | setup needs compress (XSI) which is not installed |
| XCU.uncompress.005 | UNRESOLVED | ENV | setup needs compress (XSI) which is not installed |
| XCU.uncompress.006 | UNRESOLVED | ENV | setup needs compress (XSI) which is not installed |
| XCU.uncompress.007 | UNRESOLVED | ENV | setup needs compress (XSI) which is not installed |
| XCU.unget.001 | UNRESOLVED | ENV | setup needs admin to create the SCCS file; admin (XSI) is not installed |
| XCU.unget.002 | UNRESOLVED | ENV | setup needs admin to create the SCCS file; admin (XSI) is not installed |
| XCU.unget.003 | UNRESOLVED | ENV | setup needs admin to create the SCCS file; admin (XSI) is not installed |
| XCU.uudecode.001 | FAIL | SYSTEM | uudecode (required utility) is not installed: exit 127 |
| XCU.uudecode.002 | FAIL | SYSTEM | uudecode (required utility) is not installed: exit 127 |
| XCU.uudecode.003 | FAIL | SYSTEM | uudecode (required utility) is not installed: exit 127 |
| XCU.uudecode.004 | FAIL | SYSTEM | uudecode (required utility) is not installed: exit 127 |
| XCU.uudecode.005 | FAIL | SYSTEM | uudecode (required utility) is not installed: exit 127 |
| XCU.uudecode.006 | FAIL | SYSTEM | uuencode (required) is not installed; round trip cannot start |
| XCU.uudecode.007 | FAIL | SYSTEM | uudecode (required utility) is not installed: exit 127 |
| XCU.uudecode.008 | FAIL | SYSTEM | uudecode (required utility) is not installed: exit 127 |
| XCU.uudecode.010 | FAIL | SYSTEM | uudecode (required utility) is not installed: exit 127 |
| XCU.uudecode.011 | FAIL | SYSTEM | uudecode (required utility) is not installed: exit 127 |
| XCU.uuencode.001 | FAIL | SYSTEM | uuencode (required utility) is not installed: exit 127 |
| XCU.uuencode.002 | FAIL | SYSTEM | uuencode (required utility) is not installed: exit 127 |
| XCU.uuencode.003 | FAIL | SYSTEM | uuencode (required utility) is not installed: exit 127 |
| XCU.uuencode.004 | FAIL | SYSTEM | uuencode (required utility) is not installed: exit 127 |
| XCU.uuencode.005 | FAIL | SYSTEM | uuencode (required utility) is not installed: exit 127 |
| XCU.uuencode.006 | FAIL | SYSTEM | uuencode (required utility) is not installed: exit 127 |
| XCU.uuencode.007 | FAIL | SYSTEM | uuencode (required utility) is not installed: exit 127 |
| XCU.uuencode.008 | FAIL | SYSTEM | uuencode (required utility) is not installed: exit 127 |
| XCU.uuencode.009 | FAIL | SYSTEM | uuencode (required utility) is not installed: exit 127 |
| XCU.val.001 | UNRESOLVED | ENV | setup needs admin to create the SCCS file; admin (XSI) is not installed |
| XCU.val.002 | UNRESOLVED | ENV | setup needs admin to create the SCCS file; admin (XSI) is not installed |
| XCU.val.003 | UNRESOLVED | ENV | setup needs admin to create the SCCS file; admin (XSI) is not installed |
| XCU.val.004 | UNRESOLVED | ENV | setup needs admin to create the SCCS file; admin (XSI) is not installed |
| XCU.val.005 | UNRESOLVED | ENV | setup needs admin to create the SCCS file; admin (XSI) is not installed |
| XCU.val.006 | UNRESOLVED | ENV | setup needs admin to create the SCCS file; admin (XSI) is not installed |
| XCU.wait.005 | FAIL | SYSTEM | a second wait for an already waited-for pid returns 0, not 127 (XCU wait) |
| XCU.wc.009 | FAIL | TEST-BUG | the match looked for ' 1 g ' inside the whole output but the entry is followed by a newline (and a total line follows), so the right output failed. Now looks at each line. Re-run: PASS |
| XCU.wc.010 | FAIL | TEST-BUG | locale helper issue, as XCU.nl.009 (pcloc_make trusted a locale that is not in effect); now UNRESOLVED |
| XCU.what.001 | FAIL | SYSTEM | what (XSI, claimed) is not installed: exit 127 |
| XCU.what.002 | FAIL | SYSTEM | what (XSI, claimed) is not installed: exit 127 |
| XCU.what.003 | FAIL | SYSTEM | what (XSI, claimed) is not installed: exit 127 |
| XCU.what.004 | FAIL | SYSTEM | what (XSI, claimed) is not installed: exit 127 |
| XCU.what.005 | UNRESOLVED | ENV | setup needs admin; admin is not installed |
| XCU.who.003 | FAIL | SYSTEM | who given a nonexistent file operand exits 0 (XCU who EXIT STATUS; unreadable operand file is an error) |
| XCU.xgettext.001 | FAIL | SYSTEM | xgettext (CD option, claimed by the headers) is not installed: exit 127 |
| XCU.xgettext.002 | FAIL | SYSTEM | xgettext (CD option, claimed by the headers) is not installed: exit 127 |
| XCU.xgettext.003 | FAIL | SYSTEM | xgettext (CD option, claimed by the headers) is not installed: exit 127 |
| XCU.xgettext.004 | FAIL | SYSTEM | xgettext (CD option, claimed by the headers) is not installed: exit 127 |
| XCU.xgettext.005 | FAIL | SYSTEM | xgettext (CD option, claimed by the headers) is not installed: exit 127 |
| XCU.xgettext.006 | FAIL | SYSTEM | xgettext (CD option, claimed by the headers) is not installed: exit 127 |
| XCU.xgettext.007 | FAIL | SYSTEM | xgettext (CD option, claimed by the headers) is not installed: exit 127 |
| XCU.xgettext.008 | FAIL | SYSTEM | xgettext (CD option, claimed by the headers) is not installed: exit 127 |
| XCU.yacc.001 | UNRESOLVED | ENV | setup needs c17 to compile the generated parser; c17 is not installed |
| XCU.yacc.002 | UNRESOLVED | ENV | setup needs c17 to compile the generated parser; c17 is not installed |
| XCU.yacc.003 | UNRESOLVED | ENV | setup needs c17 to compile the generated parser; c17 is not installed |
| XCU.yacc.004 | UNRESOLVED | ENV | setup needs c17 to compile the generated parser; c17 is not installed |
| XCU.yacc.005 | UNRESOLVED | ENV | setup needs c17 to compile the generated parser; c17 is not installed |
| XCU.yacc.006 | UNRESOLVED | ENV | setup needs c17 to compile the generated parser; c17 is not installed |
| XCU.yacc.007 | UNRESOLVED | ENV | setup needs c17 to compile the generated parser; c17 is not installed |
| XCU.yacc.008 | UNRESOLVED | ENV | setup needs c17 to compile the generated parser; c17 is not installed |
| XCU.yacc.009 | UNRESOLVED | ENV | setup needs c17 to compile the generated parser; c17 is not installed |
| XCU.yacc.010 | UNRESOLVED | ENV | setup needs c17 to compile the generated parser; c17 is not installed |
| XCU.yacc.011 | UNRESOLVED | ENV | setup needs c17 to compile the generated parser; c17 is not installed |
| XCU.yacc.013 | UNRESOLVED | ENV | setup needs c17 to compile the generated parser; c17 is not installed |
| XCU.yacc.014 | UNRESOLVED | ENV | setup needs c17 to compile the generated parser; c17 is not installed |
| XCU.yacc.015 | UNRESOLVED | ENV | setup needs c17 to compile the generated parser; c17 is not installed |
| XCU.yacc.016 | UNRESOLVED | ENV | setup needs c17 to compile the generated parser; c17 is not installed |
| XCU.yacc.017 | UNRESOLVED | ENV | setup needs c17 to compile the generated parser; c17 is not installed |
| XCU.yacc.018 | UNRESOLVED | ENV | setup needs c17 to compile the generated parser; c17 is not installed |
| XCU.yacc.019 | UNRESOLVED | ENV | setup needs c17 to compile the generated parser; c17 is not installed |
| XCU.yacc.020 | UNRESOLVED | ENV | setup needs c17 to compile the generated parser; c17 is not installed |
| XCU.zcat.001 | UNRESOLVED | ENV | setup needs compress (XSI) which is not installed |
| XCU.zcat.002 | UNRESOLVED | ENV | setup needs compress (XSI) which is not installed |
| XCU.zcat.003 | UNRESOLVED | ENV | setup needs compress (XSI) which is not installed |
| XCU.zcat.005 | UNRESOLVED | ENV | setup needs compress (XSI) which is not installed |

## Test changes (TEST-BUG)

| ID | what changed | re-run now |
|---|---|---|
| locale tests (nl.009, nl.010, paste.009, printf.015, rm.016-018, sed.022-024, sort.017-019, test.015, tr.009, wc.010) | core/pcloc.sh (shared; the 16 above and every other module that uses pcloc_make): after localedef, `pcloc__verify` asks the `locale` utility for the radix character (comma) of the single-byte locale, and for a charmap different from the POSIX locale's for the multibyte one; a mismatch is UNRESOLVED in setup. If the system leaves keyword values out (the page allows it) the locale is trusted as before. Here localedef returned status 1 with messages that the charmap symbols (<c>, <comma>, <u>...) inside strings were not found, and `LC_ALL=<path>` was not selected (decimal_point stayed '.'). The 'ch' collating element, the LC_CTYPE class, the radix and LC_MESSAGES then came out as in the POSIX locale, so each test FAILed on a locale that was never used. | UNRESOLVED (setup: the locale made by localedef is not in effect). Whether a localedef that accepts charmap symbols in strings makes the tests PASS could not be seen on this machine. |
| rm.015 | tree built with `cd -P` instead of `cd` (setup only) | PASS |
| sh-patterns.002 | operand pair 'c' 'a[a-c]' (cannot match) corrected to 'ac' | PASS |
| strings.004 | no longer requires the `-t d` offset format; checks that the string after 131072 NULs is written | PASS |
| wc.009 | the match for the entry of g looks at each output line (the old pattern needed a blank after `g`, but a newline and a total line follow) | PASS |
| tsort.004 (still SYSTEM) | the no-cycle case is checked first, so a utility that refuses -w and exits 1 is not credited with the one-cycle status | FAIL (right: -w unsupported) |

Lint: only 'declares no MUTANT' messages remain.

Counts: ENV 66, SYSTEM 129, TEST-BUG 20 (total 215).

## Patterns

* Missing utilities are one cause for 150 of the 215 lines (84 SYSTEM, 66 ENV): pax (45 lines), uuencode/uudecode (19), time (5), ngettext (3), xgettext (8) and what (4) are absent (the run already reports them in XBD.2-conformance.001). They are SYSTEM: the page requires the utility (pax, uuencode, uudecode, ngettext, time are base; xgettext, what belong to options the headers claim). Lines that only needed another utility to set up (c17 for nm/strip/yacc, compress for uncompress/zcat, admin for prs/rmdel/sact/unget/val/what.005) are ENV and are already UNRESOLVED, as they should be; they could equally be called SYSTEM, since the options are claimed.
* Issue 8 additions the shell/utilities lack: pipefail, `trap -p`, `read -d`, `case ;&`, `printf %n$`, `realpath -E`, `tail -r`, `timeout -p/-f`, `tsort -w`, `test <`/`>` in the utility, `-nt` with a missing file.
* The shell reads ahead on a pipe and discards queued input after an interactive error; missing-file status 2 instead of 127; `cd` does not turn an over-long path into a relative one.
* pr, nl, strings -t, renice, patch -N/-b -o, sed N/r, stty raw, ps defunct, who: formats or statuses that differ from the pages.
* Test-side: locale helper trusting an unselected locale (16 lines), plus four single-test slips.
