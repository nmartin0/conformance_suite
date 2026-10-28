# sort -u writes one line from each set of lines with equal keys.
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.sort.004
. "$PC_CORE/pc.sh"
pc_begin XCU.sort.004
LC_ALL=POSIX
export LC_ALL
NL='
'
printf 'b\na\nb\nc\na\n' >in1 || pc_fail "cannot create input"
printf 'a\nA\nb\n' >in2 || pc_fail "cannot create input"
printf '3\n03\n10\n' >in3 || pc_fail "cannot create input"
pc_test
got=$(sort -u in1 2>/dev/null </dev/null; printf 'rc=%s' "$?")
[ "$got" = "a${NL}b${NL}c${NL}rc=0" ] || pc_fail "-u did not remove duplicate lines"
got=$(sort -f -u in2 2>/dev/null </dev/null; printf 'rc=%s' "$?")
case $got in
"a${NL}b${NL}rc=0" | "A${NL}b${NL}rc=0") ;;
*) pc_fail "-f -u did not keep one of a and A" ;;
esac
got=$(sort -n -u in3 2>/dev/null </dev/null; printf 'rc=%s' "$?")
case $got in
"3${NL}10${NL}rc=0" | "03${NL}10${NL}rc=0") ;;
*) pc_fail "-n -u did not keep one of 3 and 03" ;;
esac
pc_pass
