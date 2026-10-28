# Without -l or -b, split writes pieces of 1000 lines.
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.split.002
. "$PC_CORE/pc.sh"
pc_begin XCU.split.002
LC_ALL=POSIX
export LC_ALL
NL='
'
i=0
while [ $i -lt 1001 ]; do
	printf '%s\n' "$i"
	i=$((i + 1))
done >big || pc_fail "cannot create input"
pc_test
split big >/dev/null 2>&1 </dev/null
rc=$?
[ "$rc" -eq 0 ] || pc_fail "split gave status $rc"
n=0
last=
first=
while IFS= read -r l; do
	[ $n -eq 0 ] && first=$l
	last=$l
	n=$((n + 1))
done <xaa
[ "$n" -eq 1000 ] || pc_fail "xaa holds $n lines, not 1000"
[ "$first" = 0 ] && [ "$last" = 999 ] || pc_fail "xaa does not hold lines 0 to 999"
got=$(cat xab; printf x)
[ "$got" = "1000${NL}x" ] || pc_fail "xab does not hold the 1001st line"
[ -e xac ] && pc_fail "a third file was created"
pc_pass
