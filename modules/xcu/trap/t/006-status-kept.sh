# The value of $? after a trap action finishes is the value it had before
# the action.
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.trap.006
. "$PC_CORE/pc.sh"
pc_begin XCU.trap.006
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
pcscript 'trap "(exit 5)" USR1' 'kill -s USR1 $$' 'echo "st=$?"' 
pc_test
pcrun
[ "$out" = "st=0" ] || pc_fail "got '$out', expected st=0 (status of kill, not of the trap action)"
pc_pass
