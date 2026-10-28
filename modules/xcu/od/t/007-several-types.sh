# With several -t types od writes, for each block of input, one line per type in the order given.
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.od.007
. "$PC_CORE/pc.sh"
pc_begin XCU.od.007
LC_ALL=POSIX
export LC_ALL
NL='
'
printf 'ab' >f1 || pc_fail "cannot create input"
pc_test
od -A n -v -t x1 -t c f1 >out 2>/dev/null </dev/null
rc=$?
[ "$rc" -eq 0 ] || pc_fail "od -t x1 -t c gave status $rc"
n=0
while IFS= read -r line; do
	set -- $line
	[ $# -gt 0 ] || continue
	n=$((n + 1))
	case $n in
	1) [ "$*" = "61 62" ] || pc_fail "the first line is not the x1 form" ;;
	2) [ "$*" = "a b" ] || pc_fail "the second line is not the c form" ;;
	esac
done <out
[ "$n" -eq 2 ] || pc_fail "two types for one block did not give two lines"
od -A n -v -t c -t x1 f1 >out 2>/dev/null </dev/null
n=0
while IFS= read -r line; do
	set -- $line
	[ $# -gt 0 ] || continue
	n=$((n + 1))
	case $n in
	1) [ "$*" = "a b" ] || pc_fail "with the order reversed the first line is not the c form" ;;
	2) [ "$*" = "61 62" ] || pc_fail "with the order reversed the second line is not the x1 form" ;;
	esac
done <out
[ "$n" -eq 2 ] || pc_fail "reversed types did not give two lines"
od -A n -v -t x1c f1 >out 2>/dev/null </dev/null
n=0
while IFS= read -r line; do
	set -- $line
	[ $# -gt 0 ] || continue
	n=$((n + 1))
done <out
[ "$n" -eq 2 ] || pc_fail "-t x1c did not give two lines"
pc_pass
