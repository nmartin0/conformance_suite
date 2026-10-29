# division or regular expression, longest token, else binding
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.awk.025
# A slash where division could appear is division (also /=), elsewhere it
# starts an ERE; the longest token is taken, so x+++y is x++ + y; an else
# goes with the nearest if.
. "$PC_CORE/pc.sh"
pc_begin XCU.awk.025
LC_ALL=POSIX
export LC_ALL
NL='
'
pc_test
got=$(echo xyz | awk '{ n = 12; print n / 3 / 2 }' 2>/dev/null)
[ "$got" = 2 ] || pc_fail "slash after a name as division: $got"
got=$(echo xyz | awk '{ n = 12; print n /3/ 2 }' 2>/dev/null)
[ "$got" = 2 ] || pc_fail "slash after a name, no blank before the divisor: $got"
got=$(echo xyz | awk '{ n = 12; n /= 4; print n }' 2>/dev/null)
[ "$got" = 3 ] || pc_fail "/= as an assignment: $got"
got=$(echo xyz | awk '{ print /y/ + 0, /q/ + 0 }' 2>/dev/null)
[ "$got" = "1 0" ] || pc_fail "slash where an operand may begin as an ERE: $got"
got=$(awk 'BEGIN { x = 1; y = 2; r = x+++y; print r, x; r = x---y; print r, x }' 2>/dev/null </dev/null)
[ "$got" = "3 2${NL}0 1" ] || pc_fail "the longest token was not taken: $got"
got=$(awk 'BEGIN { if (1) if (0) print "a"; else print "b" }' 2>/dev/null </dev/null)
[ "$got" = "b" ] || pc_fail "else not bound to the nearest if: $got"
got=$(awk 'BEGIN { if (0) if (1) print "a"; else print "b"; else print "c" }' 2>/dev/null </dev/null)
[ "$got" = "c" ] || pc_fail "else after a nested if/else: $got"
pc_pass
