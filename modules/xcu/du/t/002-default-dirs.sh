# Without options du writes a line for the operand directory and each subdirectory, but not for plain files.
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.du.002
. "$PC_CORE/pc.sh"
pc_begin XCU.du.002
same_set() {
	s_a=$(printf '%s\n' $1 | sort)
	s_b=$(printf '%s\n' $2 | sort)
	[ "$s_a" = "$s_b" ]
}

mkdir d d/sub d/sub/deep || pc_fail "cannot create directories"
printf 'abc\n' >d/a || pc_fail "cannot create d/a"
printf 'def\n' >d/sub/b || pc_fail "cannot create d/sub/b"
pc_test
du d >out 2>err </dev/null
rc=$?
[ "$rc" -eq 0 ] || pc_fail "du exited with status $rc"
names=
while read -r sz name; do
	case $sz in
	''|*[!0-9]*) pc_fail "size '$sz' is not a number" ;;
	esac
	names="$names $name"
done <out
same_set "$names" "d d/sub d/sub/deep" || pc_fail "du listed: $names"
pc_pass
