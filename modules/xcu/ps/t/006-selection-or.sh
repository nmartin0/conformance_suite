# Selection criteria are ORed.
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.ps.006
. "$PC_CORE/pc.sh"
pc_begin XCU.ps.006
LC_ALL=C; export LC_ALL
bg=
trap '[ -n "$bg" ] && { kill "$bg" 2>/dev/null; wait "$bg" 2>/dev/null; }' EXIT
sleep 301 </dev/null >/dev/null 2>&1 &
bg=$!
ru=$(id -ur) || pc_fail "id -ur failed"
pc_test
ps -o pid= -p "$bg" -U "$ru" >out 2>err </dev/null || pc_fail "ps -p $bg -U $ru failed"
fa=no fb=no
while IFS= read -r l; do
	set -- $l
	[ "$1" = "$$" ] && fa=yes
	[ "$1" = "$bg" ] && fb=yes
done <out
[ "$fb" = yes ] || pc_fail "the child $bg was not listed"
[ "$fa" = yes ] || pc_fail "the shell $$ (real user $ru) was not listed: criteria were not ORed"
pc_pass
