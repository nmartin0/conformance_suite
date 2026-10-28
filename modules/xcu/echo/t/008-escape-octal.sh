# \0num writes a byte (XSI).
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.echo.008
. "$PC_CORE/pc.sh"
pc_begin XCU.echo.008
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
ech "A${nl}" '\0101'
ech "AB${nl}" '\0101\0102'
ech "aAb${nl}" 'a\0101b'
ech "a A${nl}" a '\0101'
ech "A1${nl}" '\01011'
pc_pass
