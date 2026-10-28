# The EXIT trap action runs in the environment the shell had after its last
# command (variables and working directory).
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.trap.014
. "$PC_CORE/pc.sh"
pc_begin XCU.trap.014
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
mkdir pc.sub || pc_unresolved "mkdir"
d=$(cd pc.sub && pwd -P) || pc_unresolved "cd"
pcscript 'pcx=5' 'trap "echo \$pcx; pwd -P" EXIT' 'pcx=6' 'cd pc.sub' 
pc_test
pcrun
[ "$out" = "6${nl}$d" ] || pc_fail "got '$out', expected 6 and $d"
pc_pass
