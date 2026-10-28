# September 1752.
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.cal.003
. "$PC_CORE/pc.sh"
pc_begin XCU.cal.003
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
cal 9 1752 >out 2>err </dev/null || pc_fail "cal 9 1752 failed"
days out
count_all=0
for x in $dn; do count_all=$((count_all + 1)); done
[ "$count_all" -eq 19 ] || pc_fail "cal 9 1752 shows the day numbers '$dn', expected 1, 2 and 14 to 30"
for n in 1 2 14 15 16 17 18 19 20 21 22 23 24 25 26 27 28 29 30; do
	count "$n"
	[ "$c" -eq 1 ] || pc_fail "cal 9 1752 shows day $n $c times"
done
for n in 3 4 5 6 7 8 9 10 11 12 13; do
	count "$n"
	[ "$c" -eq 0 ] || pc_fail "cal 9 1752 shows day $n, which does not exist"
done
pc_pass
