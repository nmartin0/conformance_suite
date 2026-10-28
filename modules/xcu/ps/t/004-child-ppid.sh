# pid and ppid of a background child.
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.ps.004
. "$PC_CORE/pc.sh"
pc_begin XCU.ps.004
LC_ALL=C; export LC_ALL
bg=
trap '[ -n "$bg" ] && { kill "$bg" 2>/dev/null; wait "$bg" 2>/dev/null; }' EXIT
sleep 301 </dev/null >/dev/null 2>&1 &
bg=$!
pc_test
ps -o pid= -o ppid= -p "$bg" >out 2>err </dev/null || pc_fail "ps -p $bg failed"
n=0
while IFS= read -r l; do
	n=$((n + 1))
	set -- $l
	[ "$1" = "$bg" ] || pc_fail "pid field is '$1', expected $bg"
	[ "$2" = "$$" ] || pc_fail "ppid field is '$2', expected the shell's process ID $$"
done <out
[ "$n" -eq 1 ] || pc_fail "ps wrote $n lines, expected 1"
pc_pass
