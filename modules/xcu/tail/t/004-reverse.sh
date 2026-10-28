# tail -r writes lines last first, all of them or the last number given by -n.
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.tail.004
. "$PC_CORE/pc.sh"
pc_begin XCU.tail.004
LC_ALL=POSIX
export LC_ALL
NL='
'
printf '%s\n' 1 2 3 4 5 >in || pc_fail "cannot create input"
pc_test
got=$(tail -r in 2>/dev/null </dev/null; printf 'rc=%s' "$?")
[ "$got" = "5${NL}4${NL}3${NL}2${NL}1${NL}rc=0" ] || pc_fail "-r did not reverse all lines"
got=$(tail -r -n 2 in 2>/dev/null </dev/null; printf 'rc=%s' "$?")
[ "$got" = "5${NL}4${NL}rc=0" ] || pc_fail "-r -n 2 did not write the last two lines reversed"
pc_pass
