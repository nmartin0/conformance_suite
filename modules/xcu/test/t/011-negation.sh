# The ! operator.
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.test.011
. "$PC_CORE/pc.sh"
pc_begin XCU.test.011
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
pc_test
chk 0 '!' ''
chk 1 '!' x
chk 1 '!' -n abc
chk 0 '!' -n ''
chk 0 '!' -z abc
chk 1 '!' -z ''
chk 0 '!' -f none
chk 1 '!' -f reg
chk 0 '!' a = b
chk 1 '!' a = a
chk 0 '!' 3 -gt 4
chk 1 '!' 4 -gt 3
chk 0 '!'
pc_pass
