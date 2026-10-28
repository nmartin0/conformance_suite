# With set -n the shell reads but does not execute the commands that follow,
# and exits with status zero.
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.set.012
. "$PC_CORE/pc.sh"
pc_begin XCU.set.012
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
pcscript 'set -n' 'echo hi' ': >pc.made' 
pc_test
pcrun
[ -z "$out" ] && [ "$rc" -eq 0 ] || pc_fail "status $rc out '$out'"
[ -e pc.made ] && pc_fail "a command after set -n was executed"
pc_pass
