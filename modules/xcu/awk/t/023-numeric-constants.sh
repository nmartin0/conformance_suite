# numeric constants
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.awk.023
# Numeric constants take the forms of decimal floating constants with a
# period (1. .5 1e3 1E-2) or plain decimal digits, which are decimal even
# with a leading zero.
. "$PC_CORE/pc.sh"
pc_begin XCU.awk.023
LC_ALL=POSIX
export LC_ALL
NL='
'
pc_test
got=$(awk 'BEGIN { print 1., .5, 1e3, 1E-2, 100, 010, 1.5e+1, 2.E1 }' 2>/dev/null </dev/null)
[ "$got" = "1 0.5 1000 0.01 100 10 15 20" ] || pc_fail "numeric constants: $got"
got=$(awk 'BEGIN { x = 1e3; print x + 1 }' 2>/dev/null </dev/null)
[ "$got" = "1001" ] || pc_fail "an exponent constant was not numeric: $got"
pc_pass
