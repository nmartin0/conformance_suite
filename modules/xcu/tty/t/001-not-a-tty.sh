# tty without a terminal on standard input.
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.tty.001
. "$PC_CORE/pc.sh"
pc_begin XCU.tty.001
: >reg || pc_unresolved "setup: cannot create a file"
nl='
'
pc_test
for src in reg /dev/null; do
	tty <"$src" >out 2>/dev/null
	rc=$?
	[ "$rc" -eq 1 ] || pc_fail "tty with stdin from $src exited $rc, wanted 1"
	got=$(cat out; echo x)
	[ "$got" = "not a tty${nl}x" ] || pc_fail "tty with stdin from $src wrote something other than 'not a tty'"
done
echo hello | tty >out 2>/dev/null
rc=$?
[ "$rc" -eq 1 ] || pc_fail "tty with a pipe on stdin exited $rc, wanted 1"
got=$(cat out; echo x)
[ "$got" = "not a tty${nl}x" ] || pc_fail "tty with a pipe on stdin wrote something other than 'not a tty'"
(exec tty) <reg >out 2>/dev/null
rc=$?
[ "$rc" -eq 1 ] || pc_fail "exec tty exited $rc, wanted 1"
got=$(cat out; echo x)
[ "$got" = "not a tty${nl}x" ] || pc_fail "exec tty wrote something other than 'not a tty'"
pc_pass
