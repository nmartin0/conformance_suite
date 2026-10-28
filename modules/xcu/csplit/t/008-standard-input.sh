# A file operand of - makes csplit read standard input.
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.csplit.008
. "$PC_CORE/pc.sh"
pc_begin XCU.csplit.008
LC_ALL=POSIX
export LC_ALL
NL='
'
printf '%s\n' 1 2 3 4 5 6 7 8 9 10 >in10 || pc_fail "cannot create input"
pc_test
got=$(csplit - 3 <in10 2>/dev/null; printf 'rc=%s' "$?")
[ "$got" = "4${NL}17${NL}rc=0" ] || pc_fail "the size lines for standard input are not 4 and 17"
got=$(cat xx01; printf x)
[ "$got" = "3${NL}4${NL}5${NL}6${NL}7${NL}8${NL}9${NL}10${NL}x" ] || pc_fail "xx01 is wrong"
pc_pass
