# Tabs in the input count as blanks: unexpand writes the most tabs and fewest spaces for the columns they fill.
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.unexpand.004
. "$PC_CORE/pc.sh"
pc_begin XCU.unexpand.004
LC_ALL=POSIX
export LC_ALL
NL='
'
T=$(printf '\t')
printf '    %s  x\n' "$T" >in1 && printf '  %s%sx\n' "$T" "$T" >in2 || pc_fail "cannot create input"
pc_test
got=$(unexpand in1 2>/dev/null </dev/null; printf 'rc=%s' "$?")
[ "$got" = "${T}  x${NL}rc=0" ] || pc_fail "four spaces, a tab and two spaces were not a tab and two spaces"
got=$(unexpand in2 2>/dev/null </dev/null; printf 'rc=%s' "$?")
[ "$got" = "${T}${T}x${NL}rc=0" ] || pc_fail "two spaces and two tabs were not two tabs"
pc_pass
