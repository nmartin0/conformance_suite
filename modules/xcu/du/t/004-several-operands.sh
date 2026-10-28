# du -s with several operands writes one line for each, and a plain file operand is written even without -a.
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.du.004
. "$PC_CORE/pc.sh"
pc_begin XCU.du.004
same_set() {
	s_a=$(printf '%s\n' $1 | sort)
	s_b=$(printf '%s\n' $2 | sort)
	[ "$s_a" = "$s_b" ]
}

mkdir d1 d2 || pc_fail "cannot create directories"
printf 'abc\n' >d1/a || pc_fail "cannot create d1/a"
printf 'abc\n' >plain || pc_fail "cannot create plain"
pc_test
du -s d1 d2 plain >out 2>err </dev/null
rc=$?
[ "$rc" -eq 0 ] || pc_fail "du -s exited with status $rc"
names=
while read -r sz name; do
	names="$names $name"
done <out
same_set "$names" "d1 d2 plain" || pc_fail "du -s listed:$names"
du plain >out 2>err </dev/null
set -- $(cat out)
[ "$#" -eq 2 ] && [ "$2" = plain ] || pc_fail "du plain wrote: $*"
pc_pass
