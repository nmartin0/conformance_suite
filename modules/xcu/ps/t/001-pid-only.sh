# ps -o pid= -p PID.
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.ps.001
. "$PC_CORE/pc.sh"
pc_begin XCU.ps.001
LC_ALL=C; export LC_ALL
pc_test
ps -o pid= -p "$$" >out 2>err </dev/null || pc_fail "ps -o pid= -p $$ failed"
n=0
while IFS= read -r l; do
	n=$((n + 1))
	set -- $l
	[ "$#" -eq 1 ] || pc_fail "line '$l' does not hold exactly one word"
	[ "$1" = "$$" ] || pc_fail "line '$l' is not the process ID $$"
done <out
[ "$n" -eq 1 ] || pc_fail "ps wrote $n lines, expected 1"
pc_pass
