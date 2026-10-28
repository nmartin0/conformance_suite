# An exit within an EXIT trap action ends the shell at once with that exit's
# status.
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.exit.005
. "$PC_CORE/pc.sh"
pc_begin XCU.exit.005
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
pcscript 'trap "echo one; exit 4; echo two" EXIT' 'exit 1' 
pc_test
pcrun
[ "$out" = one ] || pc_fail "got '$out', expected one"
[ "$rc" -eq 4 ] || pc_fail "status $rc, expected 4"
pc_pass
