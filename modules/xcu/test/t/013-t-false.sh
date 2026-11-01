# -t fd is false for non-terminals.
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.test.013
# MUTANT    test.t_open_true
# MUTANT    test.t_unopen_true
# MUTANT    test.t_always_true
# MUTANT    test.fn_t_always_true
# MUTANT    test.te_t_always_true
# MUTANT    test.br_t_always_true
. "$PC_CORE/pc.sh"
pc_begin XCU.test.013
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
chk 1 -t 9
chk 1 -t 99
test -t 0 <reg && pc_fail "test -t 0 true with stdin on a regular file"
test -t 1 >reg && pc_fail "test -t 1 true with stdout on a regular file"
test -t 2 2>reg && pc_fail "test -t 2 true with stderr on a regular file"
test -t 0 </dev/null && pc_fail "test -t 0 true with stdin on the null device"
test -t 1 >/dev/null && pc_fail "test -t 1 true with stdout on the null device"
[ -t 0 ] <reg && pc_fail "[ -t 0 ] true with stdin on a regular file"
[ -t 1 ] >reg && pc_fail "[ -t 1 ] true with stdout on a regular file"
[ -t 0 ] </dev/null && pc_fail "[ -t 0 ] true with stdin on the null device"
(exec test -t 0) <reg && pc_fail "exec test -t 0 true with stdin on a regular file"
(exec test -t 1) >/dev/null && pc_fail "exec test -t 1 true with stdout on the null device"
[ -t 1 ] >/dev/null && pc_fail "[ -t 1 ] true with stdout on the null device"
[ -t 2 ] 2>reg && pc_fail "[ -t 2 ] true with stderr on a regular file"
(exec test -t 2) 2>reg && pc_fail "exec test -t 2 true with stderr on a regular file"
(exec test -t 0) </dev/null && pc_fail "exec test -t 0 true with stdin on the null device"
(exec [ -t 0 ]) <reg && pc_fail "exec [ -t 0 ] true with stdin on a regular file"
(exec [ -t 1 ]) >reg && pc_fail "exec [ -t 1 ] true with stdout on a regular file"
(exec [ -t 1 ]) >/dev/null && pc_fail "exec [ -t 1 ] true with stdout on the null device"
pc_pass
