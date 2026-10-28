# The file type primaries.
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.test.007
. "$PC_CORE/pc.sh"
pc_begin XCU.test.007
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
: >reg || pc_unresolved "setup: cannot create a file"
mkdir dir || pc_unresolved "setup: cannot create a directory"
ln -s reg lnreg || pc_unresolved "setup: cannot create a symbolic link"
ln -s nowhere lnbad || pc_unresolved "setup: cannot create a dangling link"
mkfifo fifo || pc_unresolved "setup: cannot create a FIFO"
pc_test
chk 0 -e reg
chk 0 -e dir
chk 0 -e fifo
chk 1 -e none
chk 0 -f reg
chk 1 -f dir
chk 1 -f fifo
chk 1 -f none
chk 0 -d dir
chk 1 -d reg
chk 1 -d none
chk 0 -L lnreg
chk 0 -L lnbad
chk 1 -L reg
chk 1 -L dir
chk 1 -L none
chk 0 -h lnreg
chk 0 -h lnbad
chk 1 -h reg
chk 1 -h none
chk 0 -p fifo
chk 1 -p reg
chk 1 -p dir
chk 1 -p none
pc_pass
