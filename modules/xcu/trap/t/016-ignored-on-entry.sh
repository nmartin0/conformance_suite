# A signal that was ignored when a non-interactive shell started cannot be
# trapped: the shell goes on and the signal stays ignored.
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.trap.016
# MUTANT    trap.entry_trappable
# MUTANT    trap.entry_resettable
# VARIANT   trap.ignored_entry_diag
. "$PC_CORE/pc.sh"
pc_begin XCU.trap.016
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
printf '%s\n' 'trap "echo hit" USR1' 'kill -s USR1 $$' 'echo alive' >pc.k4 || pc_unresolved "write"
pcscript 'trap "" USR1' 'exec ${PC_SH:-sh} ./pc.k4' 
pc_test
pcrun
[ "$out" = alive ] || pc_fail "got '$out', expected only alive"
# nor can it be reset: trying is not an error that ends the shell, the signal stays ignored
printf '%s\n' 'trap - USR1' 'kill -s USR1 $$' 'echo alive' >pc.k4b || pc_unresolved "write"
pcscript 'trap "" USR1' 'exec ${PC_SH:-sh} ./pc.k4b'
pcrun
[ "$out" = alive ] || pc_fail "after trap - on a signal ignored on entry: got '$out', expected only alive"
pc_pass
