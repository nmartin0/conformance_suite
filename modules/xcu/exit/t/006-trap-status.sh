# An exit with no operand inside a trap action uses the exit status the
# shell had before the trap action began.
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.exit.006
. "$PC_CORE/pc.sh"
pc_begin XCU.exit.006
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
pcscript 'trap "false; exit" EXIT' '(exit 6)' 
pc_test
pcrun
[ "$rc" -eq 6 ] || pc_fail "status $rc, expected 6"
pc_pass
