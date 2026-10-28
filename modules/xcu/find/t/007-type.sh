# -type d, f, l and p select directories, regular files, symbolic links and FIFOs.
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.find.007
. "$PC_CORE/pc.sh"
pc_begin XCU.find.007
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
got=$(find t -type d 2>err </dev/null)
rc=$?
[ "$rc" -eq 0 ] || pc_fail "-type d: find exited with status $rc"
same_set "$got" "t t/sub t/empty" || pc_fail "-type d: found '$got'"
got=$(find t -type f 2>err </dev/null)
rc=$?
[ "$rc" -eq 0 ] || pc_fail "-type f: find exited with status $rc"
same_set "$got" "t/a.txt t/B.TXT t/sub/c.txt t/sub/d.c t/.hid" || pc_fail "-type f: found '$got'"
got=$(find t -type l 2>err </dev/null)
rc=$?
[ "$rc" -eq 0 ] || pc_fail "-type l: find exited with status $rc"
same_set "$got" "t/l" || pc_fail "-type l: found '$got'"
got=$(find t -type p 2>err </dev/null)
rc=$?
[ "$rc" -eq 0 ] || pc_fail "-type p: find exited with status $rc"
same_set "$got" "t/p" || pc_fail "-type p: found '$got'"
pc_pass
