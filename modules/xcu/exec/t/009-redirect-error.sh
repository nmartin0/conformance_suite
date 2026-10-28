# A redirection error in exec makes a non-interactive shell exit with a
# status in the range 1 to 125.
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.exec.009
. "$PC_CORE/pc.sh"
pc_begin XCU.exec.009
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
pcscript 'exec 3<pc.nosuchfile' 'echo after' 
pc_test
pcrun
[ "$rc" -ge 1 ] && [ "$rc" -le 125 ] || pc_fail "status $rc, expected 1 to 125"
[ -z "$out" ] || pc_fail "shell went on: '$out'"
pc_pass
