# ! negates, ( ) groups, -o is alternation with lower precedence than the implied and, and -a is accepted.
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.find.021
. "$PC_CORE/pc.sh"
pc_begin XCU.find.021
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
got=$(find t -type f ! -name '*.txt' 2>err </dev/null)
rc=$?
[ "$rc" -eq 0 ] || pc_fail "!: find exited with status $rc"
same_set "$got" "t/B.TXT t/sub/d.c t/.hid" || pc_fail "!: found '$got'"
got=$(find t -type f \( -name a.txt -o -name d.c \) 2>err </dev/null)
rc=$?
[ "$rc" -eq 0 ] || pc_fail "( -o ): find exited with status $rc"
same_set "$got" "t/a.txt t/sub/d.c" || pc_fail "( -o ): found '$got'"
got=$(find t -type f -name '*.txt' -o -name d.c 2>err </dev/null)
rc=$?
[ "$rc" -eq 0 ] || pc_fail "precedence: find exited with status $rc"
same_set "$got" "t/a.txt t/sub/c.txt t/sub/d.c" || pc_fail "precedence: found '$got'"
got=$(find t -type f -a -name a.txt 2>err </dev/null)
rc=$?
[ "$rc" -eq 0 ] || pc_fail "-a: find exited with status $rc"
same_set "$got" "t/a.txt" || pc_fail "-a: found '$got'"
pc_pass
