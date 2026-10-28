# Status and standard error for valid operands.
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.cal.007
. "$PC_CORE/pc.sh"
pc_begin XCU.cal.007
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
for spec in "2 2024" "9 1752" "12 9999" "1 1" "2023"; do
	cal $spec >out 2>err </dev/null
	rc=$?
	[ "$rc" -eq 0 ] || pc_fail "cal $spec exited with status $rc"
	[ -s err ] && pc_fail "cal $spec wrote to standard error"
	[ -s out ] || pc_fail "cal $spec wrote nothing"
done
cal >out 2>err </dev/null || pc_fail "cal failed"
[ -s err ] && pc_fail "cal wrote to standard error"
pc_pass
