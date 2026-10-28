# cal alone is the current month.
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.cal.005
. "$PC_CORE/pc.sh"
pc_begin XCU.cal.005
LC_ALL=C; export LC_ALL
TZ=UTC0; export TZ
# days FILE: dn = the words of FILE that are decimal numbers up to 31, in order.
days() {
	dn=
	set -f
	for w in $(cat "$1"); do
		case $w in
		'' | *[!0-9]*) continue ;;
		esac
		v=$w
		while :; do
			case $v in
			0?*) v=${v#0} ;;
			*) break ;;
			esac
		done
		[ "${#v}" -le 2 ] && [ "$v" -ge 1 ] && [ "$v" -le 31 ] && dn="$dn $v"
	done
	set +f
}
# count N: c = how many times the day number N is in $dn.
count() {
	c=0
	for x in $dn; do
		[ "$x" = "$1" ] && c=$((c + 1))
	done
}
# upto N: dn is exactly 1 to N, each once, in any order.
upto() {
	count_all=0
	for x in $dn; do count_all=$((count_all + 1)); done
	[ "$count_all" -eq "$1" ] || return 1
	i=1
	while [ "$i" -le "$1" ]; do
		count "$i"
		[ "$c" -eq 1 ] || return 1
		i=$((i + 1))
	done
	return 0
}
pc_test
n=0
while [ "$n" -lt 5 ]; do
	m1=$(date +%m)
	y1=$(date +%Y)
	cal >out 2>err </dev/null || pc_fail "cal failed"
	m2=$(date +%m)
	y2=$(date +%Y)
	if [ "$m1" = "$m2" ] && [ "$y1" = "$y2" ]; then
		m=${m1#0}
		cal "$m" "$y1" >out2 2>err </dev/null || pc_fail "cal $m $y1 failed"
		days out
		a=$dn
		days out2
		[ -n "$a" ] || pc_fail "cal showed no day numbers"
		[ "$a" = "$dn" ] || pc_fail "cal shows '$a' but cal $m $y1 shows '$dn'"
		pc_pass
	fi
	n=$((n + 1))
done
pc_unresolved "the month changed during every attempt"
