# By default unexpand turns leading blanks into the most tabs (every eighth column) and then the fewest spaces, and leaves later blanks alone.
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.unexpand.001
. "$PC_CORE/pc.sh"
pc_begin XCU.unexpand.001
LC_ALL=POSIX
export LC_ALL
NL='
'
T=$(printf '\t')
printf '        x\n' >in1 && printf '          x\n' >in2 && printf '                x\n' >in3 && printf '    x\n' >in4 && printf 'a        b\n' >in5 || pc_fail "cannot create input"
pc_test
got=$(unexpand in1 2>/dev/null </dev/null; printf 'rc=%s' "$?")
[ "$got" = "${T}x${NL}rc=0" ] || pc_fail "eight leading spaces were not one tab"
got=$(unexpand in2 2>/dev/null </dev/null; printf 'rc=%s' "$?")
[ "$got" = "${T}  x${NL}rc=0" ] || pc_fail "ten leading spaces were not a tab and two spaces"
got=$(unexpand in3 2>/dev/null </dev/null; printf 'rc=%s' "$?")
[ "$got" = "${T}${T}x${NL}rc=0" ] || pc_fail "sixteen leading spaces were not two tabs"
got=$(unexpand in4 2>/dev/null </dev/null; printf 'rc=%s' "$?")
[ "$got" = "    x${NL}rc=0" ] || pc_fail "four leading spaces were changed"
got=$(unexpand in5 2>/dev/null </dev/null; printf 'rc=%s' "$?")
[ "$got" = "a        b${NL}rc=0" ] || pc_fail "blanks that are not leading were converted without -a"
pc_pass
