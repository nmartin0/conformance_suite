# tee accepts at least thirteen file operands.
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.tee.006
. "$PC_CORE/pc.sh"
pc_begin XCU.tee.006
LC_ALL=POSIX
export LC_ALL
NL='
'
printf 'data\n' >in || pc_fail "cannot create input"
set --
i=1
while [ $i -le 13 ]; do
	set -- "$@" "t$i"
	i=$((i + 1))
done
pc_test
tee "$@" <in >out 2>/dev/null
rc=$?
[ "$rc" -eq 0 ] || pc_fail "tee with thirteen files gave status $rc"
for f in "$@"; do
	got=$(cat "$f"; printf x)
	[ "$got" = "data${NL}x" ] || pc_fail "$f does not hold the data"
done
pc_pass
