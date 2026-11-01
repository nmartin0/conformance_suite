# With no operand date writes the +%a %b %e %H:%M:%S %Z %Y format.
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.date.001
# MUTANT    date.default_no_zone
# MUTANT    date.default_no_weekday
# MUTANT    date.default_zone_last
# MUTANT    date.default_iso
# MUTANT    date.default_no_seconds
# MUTANT    date.default_no_newline
# MUTANT    date.default_day_zero
# VARIANT   date.leap_second
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
	# the x keeps the final newline: the plain run must end its line too
	a=$(date "+%a %b %e %H:%M:%S %Z %Y"; printf x)
	b=$(date; printf x)
	c=$(date "+%a %b %e %H:%M:%S %Z %Y"; printf x)
	if [ "$a" = "$c" ]; then
		[ "$a" != x ] || pc_fail "date with the explicit format wrote nothing"
		[ "$b" = "$a" ] || pc_fail "default output '$b' differs from '$a'"
		pc_pass
	fi
	n=$((n + 1))
done
pc_unresolved "the clock changed during every attempt"
