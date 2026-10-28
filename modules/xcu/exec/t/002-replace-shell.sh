# With a utility operand, exec replaces the shell with that utility: later
# commands are not run and the shell's exit status is the utility's.
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.exec.002
. "$PC_CORE/pc.sh"
pc_begin XCU.exec.002
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
pcscript 'exec printf "%s\n" run' 'echo after' 
pc_test
pcrun
[ "$out" = run ] || pc_fail "got '$out', expected only 'run'"
[ "$rc" -eq 0 ] || pc_fail "status $rc"
pcscript 'exec ${PC_SH:-sh} -c "exit 7"' 'echo after'
pcrun
[ "$rc" -eq 7 ] && [ -z "$out" ] || pc_fail "exit status $rc (expected 7), output '$out'"
pc_pass
