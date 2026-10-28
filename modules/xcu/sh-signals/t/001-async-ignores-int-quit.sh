# With job control off, a command in an asynchronous list inherits SIGINT
# and SIGQUIT as ignored.
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.sh-signals.001
. "$PC_CORE/pc.sh"
pc_begin XCU.sh-signals.001
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
printf '%s\n' 'kill -s INT $$' 'kill -s QUIT $$' 'echo survived' >pc.k8 || pc_unresolved "write"
pcscript '${PC_SH:-sh} ./pc.k8 &' 'wait' 
pc_test
pcrun
[ "$out" = survived ] || pc_fail "got '$out', expected survived"
pc_pass
