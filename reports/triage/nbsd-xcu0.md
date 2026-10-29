# Triage of the second-system run: group xcu0

Second system: BSD-derived, run as root. Standard is the oracle. The per-test output files of the
second run are empty for most tests, so the one-line notes, the test sources and the standard were
used; where a note was cut at its first line and nothing else settles the matter the class is UNKNOWN.

Class key: T = TEST-BUG, S = SYSTEM, A = STD-AMBIG, E = ENV, U = UNKNOWN.

| ID | 2nd | Linux | class | reason |
|---|---|---|---|---|
| XCU.1.1-relationship.006 | FAIL | PASS | S | `sort -o` replaced the symbolic link instead of writing the target (XCU 1.1.1.4, "Follow link") |
| XCU.1.4-utility-defaults.011 | FAIL | PASS | S | `sed -n 1q` read ahead and did not leave the offset after the last byte processed (XCU 1.4, INPUT FILES) |
| XCU.1.4-utility-defaults.012 | FAIL | FAIL | S | a shell reading a script from standard input read ahead of the command it ran (XCU sh, INPUT FILES / 1.4) |
| XCU.1.4-utility-defaults.013 | FAIL | FAIL | S | awk does not leave a seekable standard input just past the last record processed (XCU 1.4) |
| XCU.2.5-parameters.017 | FAIL | FAIL | S | PWD from the environment with a dot component was kept; the text sends it to `pwd -P` (XCU 2.5.3, PWD) |
| XCU.2.6-word-expansions.029 | FAIL | FAIL | S | `$((echo a); (echo b))` is not parsed as a command substitution though it cannot be arithmetic (XCU 2.6.3); the sentence telling applications to separate `$(` and `(` does not lift the shell's own rule |
| XCU.2.8-exit-status-and-errors.003 | FAIL | PASS | S | unsetting a readonly variable (exit status above zero, CONSEQUENCES OF ERRORS default) did not end a non-interactive shell (XCU 2.8.1 special built-in error) |
| XCU.2.8-exit-status-and-errors.008 | FAIL | PASS | S | a redirection error on a function call ended the non-interactive shell (XCU 2.8.1) |
| XCU.alias.002 | FAIL | PASS | S | the line starts with `alias `, the format is `name=value` (XCU alias, STDOUT) |
| XCU.alias.004 | FAIL | PASS | S | same format deviation for the full listing |
| XCU.alias.013 | FAIL | PASS | S | same format deviation for several names |
| XCU.at.001 | FAIL | UNRESOLVED | U | status 0 but no `job ID at DATE` line on standard error; the text of what was written is needed (messages now carry it) |
| XCU.at.002 | UNRESOLVED | UNRESOLVED | U | setup: same missing job line as at.001 |
| XCU.at.003 | UNRESOLVED | UNRESOLVED | U | setup: same |
| XCU.at.004 | FAIL | UNRESOLVED | U | `at -f` accepted, no job line; same open question |
| XCU.at.005 | UNRESOLVED | UNRESOLVED | U | `at noon Dec 9, 2035` refused with status 1; the diagnostic is needed to tell a refusal of the grammar from a permission refusal |
| XCU.at.007 | UNRESOLVED | UNRESOLVED | U | setup: same as at.002 |
| XCU.at.008 | FAIL | FAIL | S | `at -r` of an unknown job exited 0; status 0 is for "all of the specified jobs were successfully removed" (XCU at, EXIT STATUS). (Linux run: no `at` installed, status 127) |
| XCU.awk.019 | FAIL | PASS | S | FILENAME is empty, not `-`, for standard input (XCU awk, FILENAME) |
| XCU.awk.025 | FAIL | PASS | U | first line printed 2 as expected; which of the four slash cases failed was hidden by the note (check split, see below) |
| XCU.awk.032 | FAIL | FAIL | S | a nonexistent field compared equal to 0 is false; the text says an uninitialized value compares numerically with a number (XCU awk, Expressions). Also fails with mawk here |
| XCU.awk.033 | FAIL | PASS | S | with the only operand nulled, standard input is read but FILENAME is not `-` (XCU awk, ARGV) |
| XCU.batch.001 | FAIL | UNRESOLVED | U | same open question as at.001 |
| XCU.cd.004 | FAIL | PASS | S | `cd -` writes the directory only when interactive; the text requires it always (XCU cd, STDOUT) |
| XCU.cd.006 | FAIL | PASS | S | `cd a/link` left a physical PWD; the default is -L (XCU cd, OPTIONS) |
| XCU.cd.008 | FAIL | PASS | S | `cd -P -L` rejected: "Unknown option -L"; both options must be supported, last decides (XCU cd) |
| XCU.cd.009 | FAIL | PASS | S | CDPATH hit not written to standard output (XCU cd, STDOUT) |
| XCU.cd.010 | FAIL | PASS | S | same, for a non-empty CDPATH entry |
| XCU.cmp.003 | FAIL | PASS | S | `-l` pads the byte number (`     2 142 130`); the format is `%d %o %o` (XCU cmp, STDOUT) |
| XCU.cmp.006 | FAIL | PASS | S | additional info is `: char 1, line 1`; it must start with a blank (XCU cmp, STDERR) |
| XCU.cmp.008 | FAIL | PASS | A | `f stdin differ`: the text does not say what name is written for the operand `-` (Q-206) |
| XCU.command.004 | FAIL | FAIL | S | `command -v ./bin/x` wrote a relative name; a name with a slash is written as an absolute pathname (XCU command, -v) |
| XCU.command.006 | FAIL | PASS | S | for an alias only the value was written, no name: not a command line that represents the definition (XCU command, -v) |
| XCU.crontab.002 | UNRESOLVED | UNRESOLVED | S | `crontab` with no file operand failed (status 1) while the file form works for the same user (crontab.001, .003 to .005 pass); the text reads standard input (XCU crontab, DESCRIPTION) |
| XCU.dd.005 | FAIL | PASS | U | `ibs=7 obs=9 bs=100` exited 1; nothing in the note says why (operands are legal together, bs supersedes) |
| XCU.dd.016 | FAIL | PASS | S | the truncation line is not `1 truncated record` (XCU dd, STDERR); data were right |
| XCU.dd.020 | FAIL | FAIL | S | `iflags=fullblock` not accepted (added in 2024; the other dd on this machine knows only a differently spelled operand) |
| XCU.df.007 | FAIL | PASS | S | `df -P nonexistent` exited 0 (XCU df, EXIT STATUS) |
| XCU.diff.003 | FAIL | FAIL | S | -c header lines are `*** %s %s` / `--- %s %s` with a `date +"%a %b %e %T %Y"` stamp; both systems write something else (XCU diff, STDOUT) |
| XCU.dot.006 | UNRESOLVED | FAIL | U | Linux: the shell exits on a missing dot file even when interactive (S there). Second system: a 60 s time-out of an interactive shell fed from a file; needs a trace of that shell |
| XCU.ed.001 | FAIL | PASS | S | no byte count written on load without -s (XCU ed, Edit command / OPTIONS -s) |
| XCU.ed.007 | FAIL | PASS | S | the `l` flag of s and `l` itself write no `$` at the end of the line (seen directly in ed.015) |
| XCU.ed.012 | FAIL | PASS | S | without -s neither the byte count nor the `!` after a shell command is written |
| XCU.ed.013 | FAIL | PASS | S | no count and no `?` on standard output (the text puts `?` on standard output) |
| XCU.ed.015 | FAIL | PASS | S | `l` does not mark the end of each line with `$` |
| XCU.ed.017 | FAIL | PASS | S | no `?` on standard output for an invalid command |
| XCU.ed.018 | FAIL | PASS | S | no `?` on standard output for an address error |
| XCU.ed.019 | FAIL | PASS | S | `,2p` (first address omitted) accepted by the text as `1,2`; the script stopped there |
| XCU.ed.023 | FAIL | PASS | S | same `$` omission as ed.015 |
| XCU.ed.024 | FAIL | PASS | S | no `?` for a move into the moved range |
| XCU.ed.025 | FAIL | PASS | S | no `?` for a lone `%` with no earlier substitute |
| XCU.ed.026 | UNRESOLVED | PASS | T | waited for ed to flush its standard output, which is not specified (changed, below) |
| XCU.export.003 | FAIL | PASS | S | `export PCE5=~/a:~/b` got no tilde expansion: export is a declaration utility (XCU 2.9.1.1, export) |
| XCU.file.006 | FAIL | FAIL | S | `-i` gave a MIME type instead of "regular file" (XCU file, OPTIONS -i, STDOUT) |
| XCU.file.008 | FAIL | FAIL | S | a link to a directory is not resolved by default (XCU file, DESCRIPTION) |
| XCU.file.009 | FAIL | FAIL | S | `-M magicfile` did not apply the tests (XCU file, OPTIONS -M) |
| XCU.find.011 | FAIL | PASS | S | `-atime 0` missed a fresh file: the age is rounded up, the text discards the remainder (XCU find, -atime) |
| XCU.find.027 | FAIL | PASS | S | `-mount` not supported (2024, XCU find) |
| XCU.find.032 | FAIL | PASS | S | `-newer dang` (dangling link) is an error; the text uses the link's own time (XCU find, -newer) |
| XCU.fold.004 | FAIL | PASS | S | `-s` does not break after the last blank within the width (XCU fold, -s) |
| XCU.fold.006 | FAIL | PASS | S | an unreadable operand stops the run; the other operands must still be processed (XCU 1.4 CONSEQUENCES OF ERRORS) |
| XCU.gencat.002 | FAIL | UNRESOLVED | U | first message read back right, a later one differs; message now flattened to one line |
| XCU.getconf.005 | FAIL | FAIL | S | `POSIX2_C_BIND` and the other names without the underscore must be accepted (XCU getconf) |
| XCU.getopts.011 | FAIL | FAIL | S | OPTARG is set (to empty) after a flag; it must be unset (XCU getopts, DESCRIPTION) |
| XCU.getopts.016 | FAIL | FAIL | S | the diagnostic does not name the program (XCU getopts, STDERR) |
| XCU.gettext.002 | FAIL | FAIL | S | `\"` is left alone under -e; it is a C escape sequence. (Linux: no gettext installed) |
| XCU.head.007 | FAIL | PASS | S | head reads ahead and leaves the offset at end (XCU 1.4, INPUT FILES) |
| XCU.iconv.003 | FAIL | PASS | S | a `-` operand is not standard input (XCU iconv, OPERANDS) |
| XCU.join.004 | FAIL | PASS | U | `-a 1` result differs; got text was not in the note (messages now carry it) |
| XCU.join.006 | FAIL | PASS | U | `-o 1.2,2.3,0` differs; maybe `0` unsupported, not shown |
| XCU.join.007 | FAIL | PASS | U | same family |
| XCU.locale.001 | FAIL | PASS | S | LANG written as `LANG=""`; the value is not double-quoted (XCU locale, STDOUT) |
| XCU.locale.002 | FAIL | PASS | S | a set LC_ variable written with double quotes |
| XCU.locale.003 | FAIL | PASS | S | `LANG="C"` quoted |
| XCU.locale.006 | FAIL | PASS | U | `-c decimal_point` wrote `LC_NUMERIC` then something other than `.`; rest hidden (message flattened now) |
| XCU.locale.007 | FAIL | PASS | S | an unknown name exits 0 (XCU locale, EXIT STATUS) |
| XCU.ls.001 | FAIL | PASS | T | the second system lists dot files for the superuser, which the text allows ("an implementation-defined condition"); test now ignores `.`, `..` and `.h` |
| XCU.ls.017 | FAIL | PASS | S | no `total` line with `-s` (XCU ls, STDOUT) |
| XCU.ls.020 | FAIL | PASS | T | same condition: a listed `.hid` is recursed into; the test accepts the `-A` output as the alternative |
| XCU.ls.029 | FAIL | PASS | U | `ls -lH` on a link exited non-zero; diagnostic not available (now in the message) |
| XCU.ls.032 | FAIL | PASS | U | `ls -H -l` exited 1; same |
| XCU.ls.037 | FAIL | PASS | T | same condition (dot file in the listing); `.h` ignored for the no-operand listing |
| XCU.ls.044 | FAIL | PASS | S | `-s` without `-l` writes no `total` line (XCU ls, STDOUT) |
| XCU.ls.046 | FAIL | PASS | S | operand links are sorted by the target's size without -H/-L; the link's own size is used (XCU ls, -S) |
| XCU.ls.048 | FAIL | PASS | S | `-a -A` lists dot and dot-dot; the last of the exclusive set decides (XCU ls, OPTIONS). Also latent T: the later checks would see `.h` under the dot-file condition; `.h` is removed after the two checks that need it |
| XCU.ls.050 | FAIL | PASS | S | `-1` after/before `-l` disables the long format; it must not (XCU ls, -1) |
| XCU.m4.008 | FAIL | PASS | S | `-U define` leaves the built-in defined (XCU m4, -U) |

## Test changes (all re-run on this machine with `--only`)

| ID | change | re-run |
|---|---|---|
| XCU.ls.001 | `nodot` helper drops `.`, `..`, `.h` from the listing before judging; no fault or variant declared in these tests | PASS; also PASS with a wrapper that adds -A |
| XCU.ls.020 | first `ls -R d` output may equal the -A output | PASS (also with the -A wrapper) |
| XCU.ls.037 | no-operand listing judged through `nodot` | PASS (also with wrapper) |
| XCU.ls.048 | `rm d/.h` after the `-A`/`-a` checks | PASS (also with wrapper) |
| XCU.ed.026 | wait for a file made by `!: >ready` instead of for output on standard output | PASS |
| XCU.cmp.008 | class disputed (Q-206 added to docs/standard-issues.md; asr note and STDOUT anchor); accepts `-` or `stdin` as the name | PASS |
| XCU.crontab.002 | status != 0 from the stdin form is a FAIL when `crontab FILE` accepts the same entries, else UNRESOLVED as before | UNRESOLVED here (no crontab, 127), unchanged |
| XCU.at.001-005,007, XCU.batch.001 | messages now include the first 160 bytes of standard error and 80 of standard output | UNRESOLVED here (no `at`), message shows the text |
| XCU.awk.025 | the first check split into four, each with its own message | PASS |
| XCU.gencat.002, XCU.join.004/006/007, XCU.locale.006, XCU.ls.029/032 | messages flattened or extended with the output/diagnostic (no change of expectation) | PASS (gencat: no c17 here) |

Lint: 6373 problems before and after (only "declares no MUTANT").

## Counts

Lines: 87. TEST-BUG 4 (ls.001, ls.020, ls.037, ed.026), STD-AMBIG 1 (cmp.008), SYSTEM 65, ENV 0, UNKNOWN 17 (at.001-005/007, batch.001, awk.025, dd.005, dot.006, gencat.002, join.004/006/007, locale.006, ls.029/032).

## Patterns

- Superuser dot-file listing: the text of ls allows names beginning with a period to be written by default under an implementation-defined condition; three ls tests and one latent case over-reached.
- Ed on the second system writes no byte counts or `!`, puts `?`/error text elsewhere than standard output and has no `$` in `l`; the suite's ed tests are correct against the text.
- Newer (2024) requirements the second system lacks: file offset after reading (sed, head, awk, sh), find -mount, dd iflags=fullblock, awk FILENAME `-`, export as declaration utility, command -v absolute pathname.
- Output-format deviations: alias, locale, cmp -l, cmp EOF, diff -c, ls total.
- `cd`/CDPATH output only when interactive; physical default for cd.
- Many second-run output files were empty, so six families stay UNKNOWN until a run keeps their output; messages were extended so a re-run shows it.
