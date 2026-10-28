# unexpand handles file operands in order, reads standard input with no operand, and exits nonzero for a file it cannot read.
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.unexpand.005
. "$PC_CORE/pc.sh"
pc_begin XCU.unexpand.005
LC_ALL=POSIX
export LC_ALL
NL='
'
T=$(printf '\t')
printf '        a\n' >f1 && printf '        b\n' >f2 || pc_fail "cannot create input"
[ ! -e nosuchfile ] || pc_unresolved "nosuchfile exists"
pc_test
got=$(unexpand f1 f2 2>/dev/null </dev/null; printf 'rc=%s' "$?")
[ "$got" = "${T}a${NL}${T}b${NL}rc=0" ] || pc_fail "two files were not converted in order"
got=$(unexpand <f2 2>/dev/null; printf 'rc=%s' "$?")
[ "$got" = "${T}b${NL}rc=0" ] || pc_fail "standard input was not converted"
unexpand nosuchfile >out 2>/dev/null </dev/null
rc=$?
[ "$rc" -ne 0 ] || pc_fail "status 0 for a file that cannot be read"
pc_pass
