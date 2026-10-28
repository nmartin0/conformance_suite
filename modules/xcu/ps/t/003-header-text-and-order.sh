# name=text, null headings, repeated -o, field order.
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.ps.003
. "$PC_CORE/pc.sh"
pc_begin XCU.ps.003
LC_ALL=C; export LC_ALL
pc_test
ps -o pid=MYPID -p "$$" >out 2>err </dev/null || pc_fail "ps -o pid=MYPID failed"
read -r h <out
[ "$h" = MYPID ] || pc_fail "heading is '$h', expected MYPID"
ps -o pid= -o ppid= -p "$$" >out 2>err </dev/null || pc_fail "ps with two null headings failed"
n=0
while IFS= read -r l; do
	n=$((n + 1))
	set -- $l
	[ "$#" -eq 2 ] || pc_fail "line '$l' does not hold two words"
	[ "$1" = "$$" ] || pc_fail "first field is '$1', expected the process ID $$"
	[ "$2" = "$PPID" ] || pc_fail "second field is '$2', expected the parent process ID $PPID"
done <out
[ "$n" -eq 1 ] || pc_fail "two null headings: ps wrote $n lines, expected 1"
ps -o ppid= -o pid= -p "$$" >out 2>err </dev/null || pc_fail "ps -o ppid= -o pid= failed"
read -r a b <out
[ "$a" = "$PPID" ] && [ "$b" = "$$" ] || pc_fail "order of fields wrong: '$a $b'"
ps -o pid -o ppid -p "$$" >out 2>err </dev/null || pc_fail "ps -o pid -o ppid failed"
read -r l <out
set -- $l
[ "$*" = "PID PPID" ] || pc_fail "headings for two -o options are '$l'"
pc_pass
