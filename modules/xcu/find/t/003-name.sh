# -name matches the base name of each file with the pattern notation, not the whole path.
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.find.003
. "$PC_CORE/pc.sh"
pc_begin XCU.find.003
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
got=$(find t -name '*.txt' 2>err </dev/null)
rc=$?
[ "$rc" -eq 0 ] || pc_fail "-name *.txt: find exited with status $rc"
same_set "$got" "t/a.txt t/sub/c.txt" || pc_fail "-name *.txt: found '$got'"
got=$(find t -name a.txt 2>err </dev/null)
rc=$?
[ "$rc" -eq 0 ] || pc_fail "-name a.txt: find exited with status $rc"
same_set "$got" "t/a.txt" || pc_fail "-name a.txt: found '$got'"
got=$(find t -name sub 2>err </dev/null)
rc=$?
[ "$rc" -eq 0 ] || pc_fail "-name sub: find exited with status $rc"
same_set "$got" "t/sub" || pc_fail "-name sub: found '$got'"
got=$(find t -name 't/a.txt' 2>err </dev/null)
rc=$?
[ "$rc" -eq 0 ] || pc_fail "-name with a slash: find exited with status $rc"
same_set "$got" "" || pc_fail "-name with a slash: found '$got'"
pc_pass
