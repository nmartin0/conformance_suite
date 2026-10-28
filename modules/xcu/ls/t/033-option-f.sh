# ls -f lists every entry, including those starting with a period, in unspecified order.
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.ls.033
. "$PC_CORE/pc.sh"
pc_begin XCU.ls.033
same_set() {
	s_a=$(printf '%s\n' $1 | sort)
	s_b=$(printf '%s\n' $2 | sort)
	[ "$s_a" = "$s_b" ]
}

LC_ALL=POSIX; export LC_ALL
mkdir d || pc_fail "cannot create d"
for n in b a C .h; do
	: >"d/$n" || pc_fail "cannot create d/$n"
done
pc_test
ls -f d >out 2>err </dev/null
rc=$?
[ "$rc" -eq 0 ] || pc_fail "ls -f exited with status $rc"
got=
while IFS= read -r l; do
	case $l in
	.|..) ;;
	*) got="$got $l" ;;
	esac
done <out
same_set "$got" ".h C a b" || pc_fail "ls -f lists '$got'"
pc_pass
