# An empty argument takes part in the spacing.
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.echo.003
. "$PC_CORE/pc.sh"
pc_begin XCU.echo.003
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
ech "a  b${nl}" a "" b
ech " a${nl}" "" a
ech "a ${nl}" a ""
ech "${nl}" ""
ech " ${nl}" "" ""
pc_pass
