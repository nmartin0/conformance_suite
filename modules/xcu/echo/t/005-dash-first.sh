# -n, -e and -E first are strings (XSI).
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.echo.005
. "$PC_CORE/pc.sh"
pc_begin XCU.echo.005
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
ech "-n${nl}" -n
ech "-n x${nl}" -n x
ech "-e x${nl}" -e x
ech "-E x${nl}" -E x
ech "-en x${nl}" -en x
ech "-neE y${nl}" -neE y
ech "x -n${nl}" x -n
pc_pass
