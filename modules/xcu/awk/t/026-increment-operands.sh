# increment and decrement values
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.awk.026
# Post-increment and post-decrement give the old numeric value, pre-increment
# and pre-decrement the new one, on numbers, uninitialized variables and
# strings (converted to numbers).
. "$PC_CORE/pc.sh"
pc_begin XCU.awk.026
LC_ALL=POSIX
export LC_ALL
NL='
'
pc_test
got=$(awk 'BEGIN { a = x++; b = x; c = ++x; d = x--; e = --x; print a, b, c, d, e }' 2>/dev/null </dev/null)
[ "$got" = "0 1 2 2 0" ] || pc_fail "increments on an uninitialized variable: $got"
got=$(awk 'BEGIN { s = "3abc"; t = s++; print t, s; u = "xyz"; v = ++u; print v, u; a["k"]++; print a["k"] }' 2>/dev/null </dev/null)
[ "$got" = "3 4${NL}1 1${NL}1" ] || pc_fail "increments on strings or array elements: $got"
got=$(echo '5 7' | awk '{ $1++; --$2; print $1, $2, $0 }' 2>/dev/null)
[ "$got" = "6 6 6 6" ] || pc_fail "increments on fields: $got"
pc_pass
