# sort -n orders by the arithmetic value of an initial numeric string; equal values fall back to comparing the whole line.
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.sort.003
. "$PC_CORE/pc.sh"
pc_begin XCU.sort.003
LC_ALL=POSIX
export LC_ALL
NL='
'
printf '10\n9\n-2\n  3\n0003\n2.5\nx\n' >in || pc_fail "cannot create input"
pc_test
got=$(sort -n in 2>/dev/null </dev/null; printf 'rc=%s' "$?")
[ "$got" = "-2${NL}x${NL}2.5${NL}  3${NL}0003${NL}9${NL}10${NL}rc=0" ] || pc_fail "-n order is wrong"
got=$(sort -nr in 2>/dev/null </dev/null; printf 'rc=%s' "$?")
[ "$got" = "10${NL}9${NL}0003${NL}  3${NL}2.5${NL}x${NL}-2${NL}rc=0" ] || pc_fail "-nr order is wrong"
pc_pass
