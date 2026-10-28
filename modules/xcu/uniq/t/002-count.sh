# uniq -c puts the number of occurrences before each line, a blank away from the line.
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.uniq.002
. "$PC_CORE/pc.sh"
pc_begin XCU.uniq.002
LC_ALL=POSIX
export LC_ALL
NL='
'
T=$(printf '\t')
printf 'a\na\nb\na\nc\nc\nc\n' >in || pc_fail "cannot create input"
pc_test
uniq -c in >out 2>/dev/null </dev/null
rc=$?
[ "$rc" -eq 0 ] || pc_fail "uniq -c gave status $rc"
norm=
while IFS= read -r l; do
	while :; do
		case $l in
		" "*|"$T"*) l=${l#?} ;;
		*) break ;;
		esac
	done
	norm=$norm$l$NL
done <out
[ "$norm" = "2 a${NL}1 b${NL}1 a${NL}3 c${NL}" ] || pc_fail "-c output is not count, blank, line for each run"
pc_pass
