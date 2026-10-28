# A path operand that does not exist gets a diagnostic and a non-zero status, and the other operands are still searched.
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.find.025
. "$PC_CORE/pc.sh"
pc_begin XCU.find.025
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
got=$(find nonexistent t -name a.txt 2>err </dev/null)
rc=$?
[ "$rc" -ne 0 ] || pc_fail "find exited 0 although a path does not exist"
[ -s err ] || pc_fail "no diagnostic"
same_set "$got" "t/a.txt" || pc_fail "found '$got'"
pc_pass
