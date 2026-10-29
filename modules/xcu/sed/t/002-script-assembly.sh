# sed joins -e and -f pieces in order, putting a newline between a -e piece and what follows, and a command may be preceded by blanks and semicolons.
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.sed.002
. "$PC_CORE/pc.sh"
pc_begin XCU.sed.002
LC_ALL=POSIX
export LC_ALL
NL='
'
printf 'one\ntwo\n' >in || pc_fail "cannot create input"
printf 's/one/1/\n' >sf || pc_fail "cannot create script file"
pc_test
got=$(sed -e 's/one/1/' -e 's/two/2/' in 2>/dev/null </dev/null; printf 'rc=%s' "$?")
[ "$got" = "1${NL}2${NL}rc=0" ] || pc_fail "two -e options were not both applied"
got=$(sed -f sf -e 's/two/2/' in 2>/dev/null </dev/null; printf 'rc=%s' "$?")
[ "$got" = "1${NL}2${NL}rc=0" ] || pc_fail "-f then -e were not both applied"
got=$(sed -e 's/1/X/' -f sf -e 's/two/2/' in 2>/dev/null </dev/null; printf 'rc=%s' "$?")
[ "$got" = "1${NL}2${NL}rc=0" ] || pc_fail "the order of -e and -f options was not respected"
got=$(sed -e '1a\' -e 'added' in 2>/dev/null </dev/null; printf 'rc=%s' "$?")
[ "$got" = "one${NL}added${NL}two${NL}rc=0" ] || pc_fail "a newline was not put between two -e pieces (got: $got)"
got=$(sed ' ; ;  s/one/1/;;  s/two/2/' in 2>/dev/null </dev/null; printf 'rc=%s' "$?")
[ "$got" = "1${NL}2${NL}rc=0" ] || pc_fail "leading blanks and semicolons were not ignored"
pc_pass
