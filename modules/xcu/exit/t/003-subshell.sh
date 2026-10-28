# In a subshell, exit ends only the subshell with the given status; the
# parent continues.
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.exit.003
. "$PC_CORE/pc.sh"
pc_begin XCU.exit.003
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
pcscript '(exit 3; echo notrun)' 'echo "rc=$?"' 'x=$(exit 4; echo notrun); echo "sub=$?"' 
pc_test
pcrun
[ "$out" = "rc=3${nl}sub=4" ] || pc_fail "got '$out'"
pc_pass
