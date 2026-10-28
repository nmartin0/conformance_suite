# Without -u the zone in TZ is used.
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.date.006
. "$PC_CORE/pc.sh"
pc_begin XCU.date.006
LC_ALL=C; export LC_ALL
dec() {
	v=$1
	while :; do
		case $v in
		0?*) v=${v#0} ;;
		*) break ;;
		esac
	done
}
pc_test
z=$(TZ=XYZ5 date +%Z)
[ "$z" = XYZ ] || pc_fail "%Z is '$z' for TZ=XYZ5, expected XYZ"
zz=$(TZ=XYZ5 date +%z)
[ "$zz" = -0500 ] || pc_fail "%z is '$zz' for TZ=XYZ5, expected -0500"
n=0
while [ "$n" -lt 5 ]; do
	l=$(TZ=XYZ5 date +%H)
	u=$(TZ=UTC0 date +%H)
	l2=$(TZ=XYZ5 date +%H)
	if [ "$l" = "$l2" ]; then
		dec "$l"
		lh=$v
		dec "$u"
		uh=$v
		want=$(((uh + 19) % 24))
		[ "$lh" -eq "$want" ] || pc_fail "hour is $l with TZ=XYZ5 while UTC shows $u"
		pc_pass
	fi
	n=$((n + 1))
done
pc_unresolved "the hour changed during every attempt"
