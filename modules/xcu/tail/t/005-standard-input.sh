# With no file operand tail reads standard input.
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.tail.005
. "$PC_CORE/pc.sh"
pc_begin XCU.tail.005
LC_ALL=POSIX
export LC_ALL
NL='
'
printf '%s\n' 1 2 3 4 5 6 7 8 9 10 11 12 >f12 || pc_fail "cannot create input"
pc_test
got=$(tail -n 2 <f12 2>/dev/null; printf 'rc=%s' "$?")
[ "$got" = "11${NL}12${NL}rc=0" ] || pc_fail "-n 2 on standard input is wrong"
got=$(tail <f12 2>/dev/null; printf 'rc=%s' "$?")
[ "$got" = "3${NL}4${NL}5${NL}6${NL}7${NL}8${NL}9${NL}10${NL}11${NL}12${NL}rc=0" ] || pc_fail "default count on standard input is wrong"
pc_pass
