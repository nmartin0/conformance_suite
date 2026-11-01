# < and > compare strings by byte order.
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.test.005
# MUTANT    test.lt_gt_swap
# MUTANT    test.lt_nocase
# MUTANT    test.lt_case_inverted
# MUTANT    test.lt_len
# MUTANT    test.lt_le
# MUTANT    test.lt_prefix_false
# MUTANT    test.lt_first_only
# MUTANT    test.fn_lt_gt_swap
. "$PC_CORE/pc.sh"
pc_begin XCU.test.005
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
chk 0 a '<' b
chk 1 b '<' a
chk 1 a '<' a
chk 0 b '>' a
chk 1 a '>' b
chk 0 A '<' a
chk 1 a '<' A
chk 0 ab '>' a
chk 0 a '<' ab
chk 1 ab '<' a
chk 1 a '>' a
chk 0 B '<' a
chk 1 B '>' a
chk 0 b '>' A
chk 0 b '>' ab
chk 1 b '<' ab
chk 0 ab '<' b
pc_pass
