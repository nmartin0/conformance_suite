# LC_ALL overrides the other variables.
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.locale.003
. "$PC_CORE/pc.sh"
pc_begin XCU.locale.003
pc_test
env -i PATH="$PATH" LANG=C LC_ALL=POSIX LC_TIME=C locale >out 2>err </dev/null || pc_fail "locale failed"
n=0 last= first=
t=no
while IFS= read -r l; do
	n=$((n + 1))
	[ "$n" -eq 1 ] && first=$l
	last=$l
	case $l in
	LC_TIME=\"C\" | LC_TIME=\"POSIX\") t=yes ;;
	LC_TIME=*) pc_fail "overridden LC_TIME written as '$l'" ;;
	esac
done <out
[ "$first" = "LANG=C" ] || pc_fail "first line is '$first', expected LANG=C"
[ "$last" = "LC_ALL=POSIX" ] || pc_fail "last line is '$last', expected LC_ALL=POSIX"
[ "$t" = yes ] || pc_fail "no quoted line for the overridden LC_TIME"
pc_pass
