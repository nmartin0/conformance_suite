# echo writes its arguments and a newline.
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.echo.001
. "$PC_CORE/pc.sh"
pc_begin XCU.echo.001
nl='
'
# ech WANT ARGS...: echo ARGS must write exactly WANT, called by the
# shell and by a direct exec.
ech() {
	want=$1
	shift
	got=$(echo "$@" </dev/null 2>/dev/null; echo x)
	[ "$got" = "${want}x" ] || pc_fail "echo $* wrote something other than what was wanted"
	got=$( (exec echo "$@") </dev/null 2>/dev/null; echo x)
	[ "$got" = "${want}x" ] || pc_fail "exec echo $* wrote something other than what was wanted"
}
pc_test
ech "hello${nl}" hello
ech "a b c${nl}" a b c
ech "a  b${nl}" "a  b"
ech "one two${nl}" "one" "two"
echo a b </dev/null >/dev/null 2>&1
rc=$?
[ "$rc" -eq 0 ] || pc_fail "echo exited $rc"
(exec echo a b) </dev/null >/dev/null 2>&1
rc=$?
[ "$rc" -eq 0 ] || pc_fail "exec echo exited $rc"
pc_pass
