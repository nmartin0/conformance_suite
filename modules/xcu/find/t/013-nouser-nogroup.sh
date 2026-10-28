# -nouser and -nogroup match files whose owner or group has no entry in the user or group database.
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.find.013
. "$PC_CORE/pc.sh"
pc_begin XCU.find.013
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
chown 4242424:4242425 t/a.txt || pc_fail "cannot chown t/a.txt"
set -- $(ls -ld t/a.txt)
case $3 in
''|*[!0-9]*) pc_unresolved "setup: user 4242424 exists ($3)" ;;
esac
case $4 in
''|*[!0-9]*) pc_unresolved "setup: group 4242425 exists ($4)" ;;
esac
pc_test
got=$(find t -nouser 2>err </dev/null)
rc=$?
[ "$rc" -eq 0 ] || pc_fail "-nouser: find exited with status $rc"
same_set "$got" "t/a.txt" || pc_fail "-nouser: found '$got'"
got=$(find t -nogroup 2>err </dev/null)
rc=$?
[ "$rc" -eq 0 ] || pc_fail "-nogroup: find exited with status $rc"
same_set "$got" "t/a.txt" || pc_fail "-nogroup: found '$got'"
pc_pass
