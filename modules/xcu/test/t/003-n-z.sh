# -n and -z test the length of a string.
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.test.003
# MUTANT    test.n_z_swap
# MUTANT    test.n_empty_true
# MUTANT    test.z_nonempty_true
# MUTANT    test.n_opstring_false
# MUTANT    test.z_opstring_true
# MUTANT    test.n_blank_false
# MUTANT    test.fn_n_z_swap
# MUTANT    test.te_n_z_swap
# MUTANT    test.br_n_z_swap
. "$PC_CORE/pc.sh"
pc_begin XCU.test.003
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
chk 0 -n abc
chk 1 -n ''
chk 1 -z abc
chk 0 -z ''
chk 0 -n -n
chk 0 -n =
chk 1 -z -z
chk 0 -n ' '
pc_pass
