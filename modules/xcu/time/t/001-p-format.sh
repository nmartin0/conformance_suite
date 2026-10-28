# time -p writes the real, user and sys lines.
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.time.001
. "$PC_CORE/pc.sh"
pc_begin XCU.time.001
nl='
'
# tfmt FILE: check the three lines of time -p in FILE; sets treal to the
# number after real. Fails the test on a bad line.
tfmt() {
	treal= tuser= tsys=
	while IFS= read -r line; do
		set -- $line
		case $1 in
		real|user|sys) ;;
		*) continue ;;
		esac
		[ "$#" -ge 2 ] || pc_fail "the $1 line '$line' is not a name and a number"
		v=$2
		case $v in
		''|*[!0-9.]*|*.*.*|*.|.*|[!0-9]*) pc_fail "the $1 value '$v' is not a decimal number with a radix and digits after it" ;;
		*.*) ;;
		*) pc_fail "the $1 value '$v' has no radix character" ;;
		esac
		case $1 in
		real) [ -n "$treal" ] || treal=$v ;;
		user) [ -n "$tuser" ] || tuser=$v ;;
		sys) [ -n "$tsys" ] || tsys=$v ;;
		esac
	done <"$1"
	[ -n "$treal" ] || pc_fail "no real line was written"
	[ -n "$tuser" ] || pc_fail "no user line was written"
	[ -n "$tsys" ] || pc_fail "no sys line was written"
}
pc_test
{ time -p true; } </dev/null >/dev/null 2>err
rc=$?
[ "$rc" -eq 0 ] || pc_fail "time -p true exited $rc"
tfmt err
(exec time -p true) </dev/null >/dev/null 2>err
rc=$?
[ "$rc" -eq 0 ] || pc_fail "exec time -p true exited $rc"
tfmt err
pc_pass
