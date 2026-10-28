# An operand of the form name=value is a variable assignment performed just before the next file is read, with escape sequences processed.
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.awk.007
. "$PC_CORE/pc.sh"
pc_begin XCU.awk.007
LC_ALL=POSIX
export LC_ALL
NL='
'
printf '1\n2\n' >f1 && printf '3\n' >f2 || pc_fail "cannot create input"
pc_test
got=$(awk '{ print x, $0 }' x=1 f1 x=2 f2 2>/dev/null </dev/null)
[ "$got" = "1 1${NL}1 2${NL}2 3" ] || pc_fail "an assignment was not performed between files"
got=$(awk 'BEGIN { print "[" x "]" } { print } END { print x }' f1 x=9 2>/dev/null </dev/null)
[ "$got" = "[]${NL}1${NL}2${NL}9" ] || pc_fail "an assignment after the last file did not occur before END"
got=$(awk 'BEGIN { print "[" x "]" } { print x }' x=4 f2 2>/dev/null </dev/null)
[ "$got" = "[]${NL}4" ] || pc_fail "an assignment before the first file did not follow BEGIN"
got=$(printf 'a\n' | awk '{ print x }' x=7 2>/dev/null)
[ "$got" = "7" ] || pc_fail "an assignment was not made before reading standard input"
tab=$(printf '\t')
got=$(awk '{ print x }' 'x=a\tb' f2 2>/dev/null </dev/null)
[ "$got" = "a${tab}b" ] || pc_fail "escape sequences in an assignment operand were not processed"
got=$(awk '{ print (x == 10), (x < 9) }' x=10.0 f2 2>/dev/null </dev/null)
[ "$got" = "1 0" ] || pc_fail "an assignment operand was not a numeric string"
got=$(awk '{ print x y }' x=a y=b f2 2>/dev/null </dev/null)
[ "$got" = "ab" ] || pc_fail "two assignments in a row were not both made"
got=$(awk '{ print _v1 }' _v1=z f2 2>/dev/null </dev/null)
[ "$got" = "z" ] || pc_fail "a name beginning with an underscore was not an assignment"
pc_pass
