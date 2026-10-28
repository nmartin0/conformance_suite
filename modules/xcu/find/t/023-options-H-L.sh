# Without -H or -L a symbolic link is itself the file; -H follows links given as path operands; -L follows all links.
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.find.023
. "$PC_CORE/pc.sh"
pc_begin XCU.find.023
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
ln -s t/sub lsub || pc_fail "cannot create symbolic link"
pc_test
got=$(find lsub 2>err </dev/null)
rc=$?
[ "$rc" -eq 0 ] || pc_fail "find lsub: find exited with status $rc"
same_set "$got" "lsub" || pc_fail "find lsub: found '$got'"
got=$(find -H lsub 2>err </dev/null)
rc=$?
[ "$rc" -eq 0 ] || pc_fail "find -H lsub: find exited with status $rc"
same_set "$got" "lsub lsub/c.txt lsub/d.c" || pc_fail "find -H lsub: found '$got'"
got=$(find -H t -type l 2>err </dev/null)
rc=$?
[ "$rc" -eq 0 ] || pc_fail "find -H t -type l: find exited with status $rc"
same_set "$got" "t/l" || pc_fail "find -H t -type l: found '$got'"
got=$(find -L t -type f 2>err </dev/null)
rc=$?
[ "$rc" -eq 0 ] || pc_fail "find -L t -type f: find exited with status $rc"
same_set "$got" "t/a.txt t/B.TXT t/sub/c.txt t/sub/d.c t/.hid t/l" || pc_fail "find -L t -type f: found '$got'"
got=$(find t -type f 2>err </dev/null)
rc=$?
[ "$rc" -eq 0 ] || pc_fail "find t -type f: find exited with status $rc"
same_set "$got" "t/a.txt t/B.TXT t/sub/c.txt t/sub/d.c t/.hid" || pc_fail "find t -type f: found '$got'"
pc_pass
