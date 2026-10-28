# Several -k keys apply in order; a type letter on a key applies to that key only; field.character positions restrict a key, and -u compares only the key.
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.sort.008
. "$PC_CORE/pc.sh"
pc_begin XCU.sort.008
LC_ALL=POSIX
export LC_ALL
NL='
'
printf 'b 2\na 2\nc 1\nb 10\n' >in1 || pc_fail "cannot create input"
printf 'xb\nya\nzc\n' >in2 || pc_fail "cannot create input"
printf 'abz\nabc\nacb\n' >in3 || pc_fail "cannot create input"
pc_test
got=$(sort -k 2,2n -k 1,1r in1 2>/dev/null </dev/null; printf 'rc=%s' "$?")
[ "$got" = "c 1${NL}b 2${NL}a 2${NL}b 10${NL}rc=0" ] || pc_fail "-k 2,2n -k 1,1r order is wrong"
got=$(sort -k 1.2,1.2 in2 2>/dev/null </dev/null; printf 'rc=%s' "$?")
[ "$got" = "ya${NL}xb${NL}zc${NL}rc=0" ] || pc_fail "-k 1.2,1.2 order is wrong"
got=$(sort -u -k 1.1,1.2 in3 2>/dev/null </dev/null; printf 'rc=%s' "$?")
case $got in
"abz${NL}acb${NL}rc=0" | "abc${NL}acb${NL}rc=0") ;;
*) pc_fail "-u -k 1.1,1.2 did not compare only the first two characters" ;;
esac
pc_pass
