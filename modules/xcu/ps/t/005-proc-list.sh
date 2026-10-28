# -p with comma and blank separated lists.
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.ps.005
. "$PC_CORE/pc.sh"
pc_begin XCU.ps.005
LC_ALL=C; export LC_ALL
bg=
trap '[ -n "$bg" ] && { kill "$bg" 2>/dev/null; wait "$bg" 2>/dev/null; }' EXIT
sleep 301 </dev/null >/dev/null 2>&1 &
bg=$!
pc_test
for list in "$$,$bg" "$$ $bg"; do
	ps -o pid= -p "$list" >out 2>err </dev/null || pc_fail "ps -p '$list' failed"
	fa=no fb=no n=0
	while IFS= read -r l; do
		n=$((n + 1))
		set -- $l
		[ "$1" = "$$" ] && fa=yes
		[ "$1" = "$bg" ] && fb=yes
	done <out
	[ "$fa" = yes ] || pc_fail "-p '$list' did not list $$"
	[ "$fb" = yes ] || pc_fail "-p '$list' did not list $bg"
	[ "$n" -eq 2 ] || pc_fail "-p '$list' listed $n processes, expected 2"
done
pc_pass
