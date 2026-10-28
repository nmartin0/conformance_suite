# A trap on EXIT runs before the shell terminates after exit n, and the exit
# status is still n.
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.exit.004
. "$PC_CORE/pc.sh"
pc_begin XCU.exit.004
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
pcscript 'trap "echo bye" EXIT' 'exit 2' 'echo notrun' 
pc_test
pcrun
[ "$out" = bye ] || pc_fail "got '$out', expected bye"
[ "$rc" -eq 2 ] || pc_fail "status $rc, expected 2"
pc_pass
