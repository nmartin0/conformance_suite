# -exec utility {} + passes the collected pathnames to few invocations of the utility, always evaluates true, and find exits non-zero if an invocation fails.
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.find.015
. "$PC_CORE/pc.sh"
pc_begin XCU.find.015
same_set() {
	s_a=$(printf '%s\n' $1 | sort)
	s_b=$(printf '%s\n' $2 | sort)
	[ "$s_a" = "$s_b" ]
}

mkdir t t/sub t/empty || pc_fail "cannot create directories"
printf 'a\n' >t/a.txt || pc_fail "cannot create t/a.txt"
printf 'bb\n' >t/B.TXT || pc_fail "cannot create t/B.TXT"
printf 'c\n' >t/sub/c.txt || pc_fail "cannot create t/sub/c.txt"
printf 'd\n' >t/sub/d.c || pc_fail "cannot create t/sub/d.c"
: >t/.hid || pc_fail "cannot create t/.hid"
ln -s a.txt t/l || pc_fail "cannot create symbolic link"
mkfifo t/p || pc_fail "cannot create FIFO"
pc_test
got=$(find t -type f -exec echo {} + 2>err </dev/null)
rc=$?
[ "$rc" -eq 0 ] || pc_fail "find -exec + exited with status $rc"
same_set "$got" "t/a.txt t/B.TXT t/sub/c.txt t/sub/d.c t/.hid" || pc_fail "the pathnames passed were: $got"
find t -type f -exec false {} + >out 2>err </dev/null
rc=$?
[ "$rc" -ne 0 ] || pc_fail "find exited 0 although the utility failed"
pc_pass
