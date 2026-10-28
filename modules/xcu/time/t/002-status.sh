# time exits with the status of the utility.
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.time.002
. "$PC_CORE/pc.sh"
pc_begin XCU.time.002
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
{ time -p "${PC_SH:-sh}" -c 'exit 6'; } </dev/null >/dev/null 2>/dev/null
rc=$?
[ "$rc" -eq 6 ] || pc_fail "time -p sh -c 'exit 6' exited $rc, wanted 6"
(exec time -p "${PC_SH:-sh}" -c 'exit 5') </dev/null >/dev/null 2>/dev/null
rc=$?
[ "$rc" -eq 5 ] || pc_fail "exec time -p sh -c 'exit 5' exited $rc, wanted 5"
(exec time -p false) </dev/null >/dev/null 2>/dev/null
rc=$?
[ "$rc" -eq 1 ] || pc_fail "exec time -p false exited $rc, wanted 1"
pc_pass
