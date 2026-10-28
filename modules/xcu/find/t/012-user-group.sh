# -user and -group match the owner and group of a file, given by name or by number.
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.find.012
. "$PC_CORE/pc.sh"
pc_begin XCU.find.012
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
me=$(id -un) || pc_fail "cannot determine the user name"
uid=$(id -u) || pc_fail "cannot determine the user ID"
set -- $(ls -ldn t/a.txt) || pc_fail "cannot list t/a.txt"
gid=$4
pc_test
got=$(find t -type f -user "$me" 2>err </dev/null)
rc=$?
[ "$rc" -eq 0 ] || pc_fail "-user name: find exited with status $rc"
same_set "$got" "t/a.txt t/B.TXT t/sub/c.txt t/sub/d.c t/.hid" || pc_fail "-user name: found '$got'"
got=$(find t -type f -user "$uid" 2>err </dev/null)
rc=$?
[ "$rc" -eq 0 ] || pc_fail "-user number: find exited with status $rc"
same_set "$got" "t/a.txt t/B.TXT t/sub/c.txt t/sub/d.c t/.hid" || pc_fail "-user number: found '$got'"
got=$(find t -type f -user 4242424 2>err </dev/null)
rc=$?
[ "$rc" -eq 0 ] || pc_fail "-user other: find exited with status $rc"
same_set "$got" "" || pc_fail "-user other: found '$got'"
got=$(find t -type f -group "$gid" 2>err </dev/null)
rc=$?
[ "$rc" -eq 0 ] || pc_fail "-group number: find exited with status $rc"
same_set "$got" "t/a.txt t/B.TXT t/sub/c.txt t/sub/d.c t/.hid" || pc_fail "-group number: found '$got'"
got=$(find t -type f -group 4242424 2>err </dev/null)
rc=$?
[ "$rc" -eq 0 ] || pc_fail "-group other: find exited with status $rc"
same_set "$got" "" || pc_fail "-group other: found '$got'"
pc_pass
