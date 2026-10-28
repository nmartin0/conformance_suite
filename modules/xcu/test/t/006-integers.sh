# The integer comparisons.
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.test.006
. "$PC_CORE/pc.sh"
pc_begin XCU.test.006
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
pc_test
chk 0 5 -eq 5
chk 1 5 -eq 6
chk 0 5 -ne 6
chk 1 5 -ne 5
chk 0 10 -gt 9
chk 1 9 -gt 10
chk 1 5 -gt 5
chk 0 5 -ge 5
chk 0 10 -ge 9
chk 1 9 -ge 10
chk 0 9 -lt 10
chk 1 10 -lt 9
chk 1 5 -lt 5
chk 0 5 -le 5
chk 0 9 -le 10
chk 1 10 -le 9
chk 0 -3 -lt 2
chk 0 -10 -lt -9
chk 0 -9 -gt -10
chk 0 -5 -eq -5
chk 1 -5 -eq 5
chk 0 0 -eq 0
pc_pass
