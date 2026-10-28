# expand handles file operands in order, reads standard input with no operand, and exits nonzero for a file it cannot read.
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.expand.005
. "$PC_CORE/pc.sh"
pc_begin XCU.expand.005
LC_ALL=POSIX
export LC_ALL
NL='
'
T=$(printf '\t')
printf 'a%sb\n' "$T" >f1 && printf 'c%sd\n' "$T" >f2 || pc_fail "cannot create input"
[ ! -e nosuchfile ] || pc_unresolved "nosuchfile exists"
pc_test
got=$(expand -t 2 f1 f2 2>/dev/null </dev/null; printf 'rc=%s' "$?")
[ "$got" = "a b${NL}c d${NL}rc=0" ] || pc_fail "two files were not expanded in order"
got=$(expand -t 2 <f2 2>/dev/null; printf 'rc=%s' "$?")
[ "$got" = "c d${NL}rc=0" ] || pc_fail "standard input was not expanded"
expand nosuchfile >out 2>/dev/null </dev/null
rc=$?
[ "$rc" -ne 0 ] || pc_fail "status 0 for a file that cannot be read"
pc_pass
