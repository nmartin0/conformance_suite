# grep -l writes each matching file's name once, with the standard input called (standard input), and -q writes nothing.
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.grep.006
. "$PC_CORE/pc.sh"
pc_begin XCU.grep.006
LC_ALL=POSIX
export LC_ALL
NL='
'
printf 'x\nx\n' >f1 && printf 'y\n' >f2 || pc_fail "cannot create input"
pc_test
got=$(grep -l x f1 f2 2>/dev/null </dev/null; printf 'rc=%s' "$?")
[ "$got" = "f1${NL}rc=0" ] || pc_fail "-l did not name f1 exactly once"
got=$(grep -l '[xy]' f1 f2 2>/dev/null </dev/null; printf 'rc=%s' "$?")
[ "$got" = "f1${NL}f2${NL}rc=0" ] || pc_fail "-l did not name both files"
got=$(grep -l x <f1 2>/dev/null; printf 'rc=%s' "$?")
[ "$got" = "(standard input)${NL}rc=0" ] || pc_fail "-l on standard input did not write (standard input)"
got=$(grep -q x f1 2>/dev/null </dev/null; printf 'rc=%s' "$?")
[ "$got" = "rc=0" ] || pc_fail "-q wrote output or gave a wrong status for a match"
got=$(grep -q zzz f1 2>/dev/null </dev/null; printf 'rc=%s' "$?")
[ "$got" = "rc=1" ] || pc_fail "-q gave a wrong status for no match"
pc_pass
