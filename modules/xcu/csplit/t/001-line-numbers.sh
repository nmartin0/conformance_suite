# csplit with line-number operands writes each piece to xxNN files and prints each piece's size in bytes.
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.csplit.001
. "$PC_CORE/pc.sh"
pc_begin XCU.csplit.001
LC_ALL=POSIX
export LC_ALL
NL='
'
printf '%s\n' 1 2 3 4 5 6 7 8 9 10 >in10 || pc_fail "cannot create input"
pc_test
got=$(csplit in10 3 6 2>/dev/null </dev/null; printf 'rc=%s' "$?")
[ "$got" = "4${NL}6${NL}11${NL}rc=0" ] || pc_fail "the size lines are not 4, 6, 11"
got=$(cat xx00; printf x)
[ "$got" = "1${NL}2${NL}x" ] || pc_fail "xx00 does not hold lines 1 and 2"
got=$(cat xx01; printf x)
[ "$got" = "3${NL}4${NL}5${NL}x" ] || pc_fail "xx01 does not hold lines 3 to 5"
got=$(cat xx02; printf x)
[ "$got" = "6${NL}7${NL}8${NL}9${NL}10${NL}x" ] || pc_fail "xx02 does not hold lines 6 to 10"
[ -e xx03 ] && pc_fail "a fourth file was created"
pc_pass
