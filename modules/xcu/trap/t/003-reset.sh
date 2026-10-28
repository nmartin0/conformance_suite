# trap - condition restores the default action, and a first operand that is
# an unsigned decimal integer makes all operands conditions to reset.
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.trap.003
. "$PC_CORE/pc.sh"
pc_begin XCU.trap.003
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
pcscript 'trap "echo got" USR1' 'trap - USR1' 'kill -s USR1 $$' 'echo notrun'
pcrun
[ -z "$out" ] || pc_fail "after trap - USR1, output '$out'"
[ "$rc" -gt 128 ] || pc_fail "after trap - USR1 the default action did not end the shell (status $rc)"
pcscript 'trap "echo bye" EXIT' 'trap - EXIT' 'echo run'
pcrun
[ "$out" = run ] || pc_fail "after trap - EXIT, output '$out'"
pcscript 'trap "echo bye" EXIT' 'trap 0' 'echo run'
pcrun
[ "$out" = run ] || pc_fail "after trap 0, output '$out'"
pc_pass
