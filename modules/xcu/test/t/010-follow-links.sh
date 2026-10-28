# Symbolic links are followed except by -h and -L.
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.test.010
. "$PC_CORE/pc.sh"
pc_begin XCU.test.010
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
printf 'x\n' >data || pc_unresolved "setup: cannot create a file"
mkdir dir || pc_unresolved "setup: cannot create a directory"
ln -s data lfile || pc_unresolved "setup: cannot create a link"
ln -s dir ldir || pc_unresolved "setup: cannot create a link"
ln -s nowhere lbad || pc_unresolved "setup: cannot create a link"
pc_test
chk 0 -f lfile
chk 0 -s lfile
chk 0 -e lfile
chk 1 -d lfile
chk 0 -d ldir
chk 0 -e ldir
chk 1 -f ldir
chk 1 -e lbad
chk 1 -f lbad
chk 1 -d lbad
chk 0 -L lfile
chk 0 -L ldir
chk 0 -h ldir
pc_pass
