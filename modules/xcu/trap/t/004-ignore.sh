# trap "" condition makes the shell ignore the signal.
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.trap.004
# MUTANT    trap.ignore_default
# MUTANT    trap.ignore_first_only
# MUTANT    trap.ignore_exits
. "$PC_CORE/pc.sh"
pc_begin XCU.trap.004
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
pcscript 'trap "" USR1' 'kill -s USR1 $$' 'echo alive' 
pc_test
pcrun
[ "$out" = alive ] && [ "$rc" -eq 0 ] || pc_fail "status $rc out '$out'"
# several conditions: each is ignored
pcscript 'trap "" USR1 USR2' 'kill -s USR2 $$' 'kill -s USR1 $$' 'echo alive'
pcrun
[ "$out" = alive ] && [ "$rc" -eq 0 ] || pc_fail "two conditions: status $rc out '$out'"
pc_pass
