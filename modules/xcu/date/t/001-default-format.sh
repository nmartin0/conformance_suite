# With no operand date writes the +%a %b %e %H:%M:%S %Z %Y format.
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.date.001
#
# The clock moves between invocations, so the plain run sits between two
# runs with the explicit format and is only judged when those agree.
. "$PC_CORE/pc.sh"
pc_begin XCU.date.001
LC_ALL=C; export LC_ALL
TZ=UTC0; export TZ
pc_test
n=0
while [ "$n" -lt 5 ]; do
	a=$(date "+%a %b %e %H:%M:%S %Z %Y")
	b=$(date)
	c=$(date "+%a %b %e %H:%M:%S %Z %Y")
	if [ "$a" = "$c" ]; then
		[ -n "$a" ] || pc_fail "date with the explicit format wrote nothing"
		[ "$b" = "$a" ] || pc_fail "default output '$b' differs from '$a'"
		pc_pass
	fi
	n=$((n + 1))
done
pc_unresolved "the clock changed during every attempt"
