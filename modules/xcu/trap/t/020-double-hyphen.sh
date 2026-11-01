# trap accepts "--" before its operands.
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.trap.020
# MUTANT    trap.dd_error
# MUTANT    trap.dd_action
. "$PC_CORE/pc.sh"
pc_begin XCU.trap.020
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
pcscript 'trap -- "echo hi" USR1' 'kill -s USR1 $$' 'echo end' 
pc_test
pcrun
[ "$out" = "hi${nl}end" ] || pc_fail "got '$out'"
pcscript 'trap "echo hi" USR1' 'trap -- - USR1' 'kill -s USR1 $$' 'echo notrun'
pcrun
[ -z "$out" ] && [ "$rc" -gt 128 ] || pc_fail "trap -- - USR1 did not reset (status $rc out '$out')"
pcscript 'trap -- "" USR1' 'kill -s USR1 $$' 'echo alive'
pcrun
[ "$out" = alive ] || pc_fail "trap -- '' USR1: got '$out'"
pc_pass
