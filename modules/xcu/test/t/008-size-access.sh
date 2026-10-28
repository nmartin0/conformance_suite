# -s, -r, -w and -x.
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.test.008
. "$PC_CORE/pc.sh"
pc_begin XCU.test.008
# chk WANT ARGS...: the expression ARGS must give status WANT (0 or 1)
# for test and for [ (with its closing bracket), each called through the
# shell and by a direct exec.
chk() {
	want=$1
	shift
	test "$@" </dev/null >/dev/null 2>&1
	rc=$?
	[ "$rc" -eq "$want" ] || pc_fail "test $* gave status $rc, wanted $want"
	[ "$@" ] </dev/null >/dev/null 2>&1
	rc=$?
	[ "$rc" -eq "$want" ] || pc_fail "[ $* ] gave status $rc, wanted $want"
	(exec test "$@") </dev/null >/dev/null 2>&1
	rc=$?
	[ "$rc" -eq "$want" ] || pc_fail "exec test $* gave status $rc, wanted $want"
	(exec [ "$@" ]) </dev/null >/dev/null 2>&1
	rc=$?
	[ "$rc" -eq "$want" ] || pc_fail "exec [ $* ] gave status $rc, wanted $want"
}
printf 'x\n' >full || pc_unresolved "setup: cannot create a file"
: >empty || pc_unresolved "setup: cannot create a file"
: >rw || pc_unresolved "setup: cannot create a file"
: >prog || pc_unresolved "setup: cannot create a file"
mkdir dir || pc_unresolved "setup: cannot create a directory"
chmod 644 rw || pc_unresolved "setup: chmod failed"
chmod 755 prog || pc_unresolved "setup: chmod failed"
chmod 755 dir || pc_unresolved "setup: chmod failed"
pc_test
chk 0 -s full
chk 1 -s empty
chk 1 -s none
chk 0 -r rw
chk 1 -r none
chk 0 -w rw
chk 1 -w none
chk 0 -x prog
chk 1 -x rw
chk 1 -x none
chk 0 -x dir
chk 0 -r dir
chk 0 -w dir
pc_pass
