# -nt, -ot and -ef.
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.test.009
. "$PC_CORE/pc.sh"
pc_begin XCU.test.009
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
: >old || pc_unresolved "setup: cannot create a file"
: >new || pc_unresolved "setup: cannot create a file"
touch -t 200001010000 old || pc_unresolved "setup: touch -t failed"
touch -t 200101010000 new || pc_unresolved "setup: touch -t failed"
ln new hard || pc_unresolved "setup: cannot make a hard link"
ln -s new soft || pc_unresolved "setup: cannot make a symbolic link"
: >other || pc_unresolved "setup: cannot create a file"
pc_test
chk 0 new -nt old
chk 1 old -nt new
chk 1 new -nt new
chk 0 old -ot new
chk 1 new -ot old
chk 1 old -ot old
chk 0 new -nt none
chk 1 none -nt new
chk 0 none -ot new
chk 1 new -ot none
chk 0 new -ef new
chk 0 new -ef hard
chk 0 new -ef soft
chk 1 new -ef other
chk 1 new -ef old
chk 1 new -ef none
pc_pass
