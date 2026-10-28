# -links n matches files with exactly, more than (+n) or fewer than (-n) n links.
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.find.009
. "$PC_CORE/pc.sh"
pc_begin XCU.find.009
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
ln t/a.txt t/hard || pc_fail "cannot create hard link"
pc_test
got=$(find t -type f -links 2 2>err </dev/null)
rc=$?
[ "$rc" -eq 0 ] || pc_fail "-links 2: find exited with status $rc"
same_set "$got" "t/a.txt t/hard" || pc_fail "-links 2: found '$got'"
got=$(find t -type f -links +1 2>err </dev/null)
rc=$?
[ "$rc" -eq 0 ] || pc_fail "-links +1: find exited with status $rc"
same_set "$got" "t/a.txt t/hard" || pc_fail "-links +1: found '$got'"
got=$(find t -type f -links -2 2>err </dev/null)
rc=$?
[ "$rc" -eq 0 ] || pc_fail "-links -2: find exited with status $rc"
same_set "$got" "t/B.TXT t/sub/c.txt t/sub/d.c t/.hid" || pc_fail "-links -2: found '$got'"
pc_pass
