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
got=$(echo xyz | awk '{ n = 12; print n / 3 / 2; print n /3/ 2; n /= 4; print n; print /y/ + 0, /q/ + 0 }' 2>/dev/null)
[ "$got" = "2${NL}2${NL}3${NL}1 0" ] || pc_fail "slash as division or ERE: $got"
got=$(awk 'BEGIN { x = 1; y = 2; r = x+++y; print r, x; r = x---y; print r, x }' 2>/dev/null </dev/null)
[ "$got" = "3 2${NL}0 1" ] || pc_fail "the longest token was not taken: $got"
got=$(awk 'BEGIN { if (1) if (0) print "a"; else print "b" }' 2>/dev/null </dev/null)
[ "$got" = "b" ] || pc_fail "else not bound to the nearest if: $got"
got=$(awk 'BEGIN { if (0) if (1) print "a"; else print "b"; else print "c" }' 2>/dev/null </dev/null)
[ "$got" = "c" ] || pc_fail "else after a nested if/else: $got"
pc_pass
