# Under set -e, a compound command (other than a subshell) whose non-zero
# status came from a failure while -e was ignored does not end the shell.
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.set.010
. "$PC_CORE/pc.sh"
pc_begin XCU.set.010
nl='
'
# pcscript: write the arguments, one per line, to pc.s
pcscript() {
	printf '%s\n' "$@" >pc.s || pc_unresolved "cannot write pc.s"
}
# pcrun: run pc.s in a fresh shell; out = its standard output, rc = its
# exit status, its standard error is left in pc.err.
pcrun() {
	out=$(${PC_SH:-sh} pc.s 2>pc.err </dev/null)
	rc=$?
}
pcscript 'set -e' '{ false && true; }' 'echo m1' 'if true; then false && true; fi' 'echo m2' 'false' 'echo notrun' 
pc_test
pcrun
[ "$out" = "m1${nl}m2" ] || pc_fail "got '$out'"
[ "$rc" -eq 1 ] || pc_fail "status $rc"
pc_pass
