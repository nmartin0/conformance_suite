# Backslash sequences in arguments (XSI).
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.echo.006
. "$PC_CORE/pc.sh"
pc_begin XCU.echo.006
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
tab=$(printf '\t')
cr=$(printf '\r')
bs=$(printf '\b')
ff=$(printf '\f')
vt=$(printf '\v')
bsl='\'
pc_test
ech "a${tab}b${nl}" 'a\tb'
ech "a${nl}b${nl}" 'a\nb'
ech "a${bsl}b${nl}" 'a\\b'
ech "a${ff}b${nl}" 'a\fb'
ech "a${vt}b${nl}" 'a\vb'
ech "a${cr}b${nl}" 'a\rb'
ech "a${bs}b${nl}" 'a\bb'
ech "x${tab}y z${nl}" 'x\ty' z
pc_pass
