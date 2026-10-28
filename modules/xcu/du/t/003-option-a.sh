# du -a also writes a line for every file that is not a directory.
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.du.003
. "$PC_CORE/pc.sh"
pc_begin XCU.du.003
same_set() {
	s_a=$(printf '%s\n' $1 | sort)
	s_b=$(printf '%s\n' $2 | sort)
	[ "$s_a" = "$s_b" ]
}

mkdir d d/sub || pc_fail "cannot create directories"
printf 'abc\n' >d/a || pc_fail "cannot create d/a"
printf 'def\n' >d/sub/b || pc_fail "cannot create d/sub/b"
pc_test
du -a d >out 2>err </dev/null
rc=$?
[ "$rc" -eq 0 ] || pc_fail "du -a exited with status $rc"
names=
while read -r sz name; do
	names="$names $name"
done <out
same_set "$names" "d d/a d/sub d/sub/b" || pc_fail "du -a listed: $names"
pc_pass
