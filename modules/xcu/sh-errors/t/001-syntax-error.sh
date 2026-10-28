# A syntax error makes a non-interactive shell write a diagnostic and exit
# with a non-zero status without running later commands.
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.sh-errors.001
. "$PC_CORE/pc.sh"
pc_begin XCU.sh-errors.001
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
pcscript 'echo (' 'echo after' 
pc_test
pcrun
[ "$rc" -ge 1 ] && [ "$rc" -le 125 ] || pc_fail "status $rc, expected 1 to 125"
[ -z "$out" ] || pc_fail "the shell went on: '$out'"
[ -s pc.err ] || pc_fail "no diagnostic on standard error"
pc_pass
