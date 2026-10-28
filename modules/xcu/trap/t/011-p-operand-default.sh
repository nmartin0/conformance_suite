# trap -p condition for a condition in its default state writes a line that,
# read back, leaves the condition in the default state.
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.trap.011
. "$PC_CORE/pc.sh"
pc_begin XCU.trap.011
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
pcscript 'trap -p USR2 >pc.tq' 
pc_test
pcrun
[ "$rc" -eq 0 ] || pc_fail "setup shell failed with $rc"
grep 'USR2' pc.tq >/dev/null || pc_fail "no line from trap -p USR2 for a condition in its default state"
pcscript 'trap "echo x" USR2' '. ./pc.tq' 'kill -s USR2 $$' 'echo notrun'
pcrun
[ -z "$out" ] && [ "$rc" -gt 128 ] || pc_fail "USR2 line read back did not restore the default (status $rc out '$out')"
pc_pass
