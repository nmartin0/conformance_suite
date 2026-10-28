# Plain text in the format is written as it is.
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.printf.001
. "$PC_CORE/pc.sh"
pc_begin XCU.printf.001
nl='
'
bsl='\'
tab=$(echo a | tr a '\011')
bel=$(echo a | tr a '\007')
bsp=$(echo a | tr a '\010')
ff=$(echo a | tr a '\014')
cr=$(echo a | tr a '\015')
vt=$(echo a | tr a '\013')
# pf WANT FORMAT [ARG...]: printf must write exactly WANT, called by the
# shell and by a direct exec, and exit zero.
pf() {
	want=$1
	shift
	got=$(printf "$@" </dev/null 2>/dev/null; echo "x$?")
	[ "$got" = "${want}x0" ] || pc_fail "printf $* did not write what was wanted, or did not exit zero"
	got=$( (exec printf "$@") </dev/null 2>/dev/null; echo "x$?")
	[ "$got" = "${want}x0" ] || pc_fail "exec printf $* did not write what was wanted, or did not exit zero"
}
pc_test
pf "hello" 'hello'
pf "a  b c" 'a  b c'
pf "" ''
pf "no newline" 'no newline'
pf "x${nl}y" 'x
y'
pc_pass
