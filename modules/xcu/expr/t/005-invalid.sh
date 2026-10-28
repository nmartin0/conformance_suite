# Invalid expressions exit with status 2 or more.
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.expr.005
. "$PC_CORE/pc.sh"
pc_begin XCU.expr.005
nl='
'
# ex WANT STATUS ARGS...: expr ARGS must write WANT (without its newline)
# and exit with STATUS, called by the shell and by a direct exec.
ex() {
	want=$1
	wrc=$2
	shift 2
	got=$(expr "$@" </dev/null 2>/dev/null; echo "x$?")
	[ "$got" = "${want}${nl}x${wrc}" ] || pc_fail "expr $* did not write '$want' with status $wrc"
	got=$( (exec expr "$@") </dev/null 2>/dev/null; echo "x$?")
	[ "$got" = "${want}${nl}x${wrc}" ] || pc_fail "exec expr $* did not write '$want' with status $wrc"
}
# exrc MINSTATUS MAXSTATUS ARGS...: expr ARGS must exit with a status in the range.
exrc() {
	lo=$1
	hi=$2
	shift 2
	expr "$@" </dev/null >/dev/null 2>&1
	rc=$?
	{ [ "$rc" -ge "$lo" ] && [ "$rc" -le "$hi" ]; } || pc_fail "expr $* exited $rc, wanted $lo to $hi"
	(exec expr "$@") </dev/null >/dev/null 2>&1
	rc=$?
	{ [ "$rc" -ge "$lo" ] && [ "$rc" -le "$hi" ]; } || pc_fail "exec expr $* exited $rc, wanted $lo to $hi"
}
pc_test
exrc 2 2 1 +
exrc 2 2 1 '*'
exrc 2 2 1 2
# 1 + + 2 is not tried: the second + can be read as a string operand, and a string that is an operator symbol is not an argument the application can rely on
exrc 2 2 '(' 1
exrc 2 2 1 ')'
exrc 2 2 '(' ')'
exrc 2 125 1 / 0
exrc 2 125 1 % 0
pc_pass
