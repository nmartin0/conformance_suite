# -u shows UTC whatever TZ holds.
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.date.005
. "$PC_CORE/pc.sh"
pc_begin XCU.date.005
LC_ALL=C; export LC_ALL
pc_test
z=$(TZ=XYZ5 date -u +%Z)
case $z in
UTC | GMT) ;;
*) pc_fail "date -u gave the zone name '$z', expected UTC or GMT" ;;
esac
n=0
while [ "$n" -lt 5 ]; do
	a=$(TZ=XYZ5 date -u +%H%M)
	b=$(TZ=UTC0 date +%H%M)
	c=$(TZ=XYZ5 date -u +%H%M)
	if [ "$a" = "$c" ]; then
		[ -n "$a" ] || pc_fail "date -u wrote nothing"
		[ "$b" = "$a" ] || pc_fail "date -u with TZ=XYZ5 shows $a, UTC is $b"
		pc_pass
	fi
	n=$((n + 1))
done
pc_unresolved "the minute changed during every attempt"
