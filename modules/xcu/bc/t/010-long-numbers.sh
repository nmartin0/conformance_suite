# bc folds very long numbers into lines of at most 70 characters, each continued line ending in a backslash.
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.bc.010
. "$PC_CORE/pc.sh"
pc_begin XCU.bc.010
LC_ALL=POSIX
export LC_ALL
NL='
'
pc_test
printf '10^100\n' | bc >bc.out 2>/dev/null
lines=0
joined=
last=
while IFS= read -r l; do
	lines=$((lines + 1))
	# 70 characters of the number per line: with the continuation backslash a line may have 71
	[ ${#l} -le 71 ] || pc_fail "a line is longer than 70 characters of the number plus a backslash"
	case $l in
	*\\) joined=$joined${l%\\} ;;
	*) joined=$joined$l ;;
	esac
	last=$l
done <bc.out
[ $lines -ge 2 ] || pc_fail "a number of 101 digits was not folded onto several lines"
case $last in
*\\) pc_fail "the last line ends with a backslash" ;;
esac
want=1
i=0
while [ $i -lt 100 ]; do
	want=${want}0
	i=$((i + 1))
done
[ "$joined" = "$want" ] || pc_fail "the folded lines do not join to 10^100"
pc_pass
