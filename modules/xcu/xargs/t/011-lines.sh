# -L number of lines.
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.xargs.011
. "$PC_CORE/pc.sh"
pc_begin XCU.xargs.011
LC_ALL=C; export LC_ALL
nl='
'
pc_test
printf 'a b\nc\n\nd e f\n' | xargs -L 2 echo >out 2>/dev/null || pc_fail "xargs -L 2 failed"
v=$(cat out; printf x)
v=${v%x}
[ "$v" = "a b c${nl}d e f$nl" ] || pc_fail "-L 2 wrote '$v', expected 'a b c' and 'd e f'"
printf 'a \nb\nc\n' | xargs -L 1 echo >out 2>/dev/null || pc_fail "xargs -L 1 with a continued line failed"
n=0
while IFS= read -r l; do
	n=$((n + 1))
	case $n in
	1)
		case $l in
		a*b) ;;
		*) pc_fail "the continued line gave '$l', expected a line from a to b" ;;
		esac
		;;
	2) [ "$l" = c ] || pc_fail "the second run wrote '$l', expected c" ;;
	esac
done <out
[ "$n" -eq 2 ] || pc_fail "a continued line and one more line gave $n runs, expected 2"
pc_pass
