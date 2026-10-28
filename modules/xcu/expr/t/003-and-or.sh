# The & and | operators.
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.expr.003
. "$PC_CORE/pc.sh"
pc_begin XCU.expr.003
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
ex 1 0 1 '&' 2
ex 0 1 0 '&' 2
ex 0 1 5 '&' 0
ex 0 1 '' '&' 2
ex 0 1 2 '&' ''
ex a 0 a '&' a
ex a 0 a '|' b
ex 5 0 0 '|' 5
ex 3 0 3 '|' 5
ex 5 0 '' '|' 5
ex 0 1 '' '|' ''
ex 0 1 0 '|' 0
ex 0 1 0 '|' ''
ex 7 0 0 '|' 3 + 4
ex 2 0 1 + 1 '&' 3
pc_pass
