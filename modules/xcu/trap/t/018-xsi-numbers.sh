# On an XSI system the conditions may be given as the signal numbers 1, 2,
# 3, 6, 9, 14 and 15 and 0 for exit; number 15 is TERM.
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.trap.018
. "$PC_CORE/pc.sh"
pc_begin XCU.trap.018
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
pc_test
pcscript 'trap "echo got" 15' 'kill -s TERM $$' 'echo end'
pcrun
[ "$out" = "got${nl}end" ] || pc_fail "trap on 15: got '$out'"
pcscript 'trap "echo got" TERM' 'trap - 15' 'kill -s TERM $$' 'echo notrun'
pcrun
[ -z "$out" ] && [ "$rc" -gt 128 ] || pc_fail "trap - 15 did not reset TERM (status $rc out '$out')"
pc_pass
