# trap -p condition... writes the trap for each named condition as a trap --
# line that restores the same trap when read back, and writes only the named
# conditions.
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.trap.010
. "$PC_CORE/pc.sh"
pc_begin XCU.trap.010
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
pcscript 'trap "echo hi" USR1' 'trap "" USR2' 'trap -p USR1 >pc.tp' 
pc_test
pcrun
[ "$rc" -eq 0 ] || pc_fail "setup shell failed with $rc"
grep 'USR1' pc.tp >/dev/null || pc_fail "no USR1 line from trap -p USR1"
grep 'USR2' pc.tp >/dev/null && pc_fail "trap -p USR1 mentioned USR2"
grep -v '^trap -- ' pc.tp >/dev/null && pc_fail "a line is not of the form 'trap -- ...'"
pcscript '. ./pc.tp' 'kill -s USR1 $$' 'echo end'
pcrun
[ "$out" = "hi${nl}end" ] || pc_fail "USR1 line read back gave '$out'"
pc_pass
