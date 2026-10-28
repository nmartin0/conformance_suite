# -perm mode matches the permission bits exactly, -perm -mode matches files having at least those bits; modes may be octal or symbolic.
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.find.008
. "$PC_CORE/pc.sh"
pc_begin XCU.find.008
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
chmod 600 t/a.txt || pc_fail "cannot chmod t/a.txt"
chmod 640 t/B.TXT || pc_fail "cannot chmod t/B.TXT"
chmod 644 t/sub/c.txt t/sub/d.c t/.hid || pc_fail "cannot chmod"
pc_test
got=$(find t -type f -perm 600 2>err </dev/null)
rc=$?
[ "$rc" -eq 0 ] || pc_fail "-perm 600: find exited with status $rc"
same_set "$got" "t/a.txt" || pc_fail "-perm 600: found '$got'"
got=$(find t -type f -perm -040 2>err </dev/null)
rc=$?
[ "$rc" -eq 0 ] || pc_fail "-perm -040: find exited with status $rc"
same_set "$got" "t/B.TXT t/sub/c.txt t/sub/d.c t/.hid" || pc_fail "-perm -040: found '$got'"
got=$(find t -type f -perm u=rw,g=r 2>err </dev/null)
rc=$?
[ "$rc" -eq 0 ] || pc_fail "-perm u=rw,g=r: find exited with status $rc"
same_set "$got" "t/B.TXT" || pc_fail "-perm u=rw,g=r: found '$got'"
got=$(find t -type f -perm -g=r 2>err </dev/null)
rc=$?
[ "$rc" -eq 0 ] || pc_fail "-perm -g=r: find exited with status $rc"
same_set "$got" "t/B.TXT t/sub/c.txt t/sub/d.c t/.hid" || pc_fail "-perm -g=r: found '$got'"
got=$(find t -type f -perm -u=rw 2>err </dev/null)
rc=$?
[ "$rc" -eq 0 ] || pc_fail "-perm -u=rw: find exited with status $rc"
same_set "$got" "t/a.txt t/B.TXT t/sub/c.txt t/sub/d.c t/.hid" || pc_fail "-perm -u=rw: found '$got'"
pc_pass
