# trap with no operands writes, as trap -- action condition lines that read
# back, the conditions whose action is not the default, and no others.
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.trap.009
. "$PC_CORE/pc.sh"
pc_begin XCU.trap.009
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
pcscript 'trap "echo hi" USR1' 'trap "" USR2' 'trap >pc.tl' 
pc_test
pcrun
[ "$rc" -eq 0 ] || pc_fail "setup shell failed with $rc"
grep 'USR1' pc.tl >/dev/null || pc_fail "no line for USR1"
grep 'USR2' pc.tl >/dev/null || pc_fail "no line for USR2"
grep 'TERM' pc.tl >/dev/null && pc_fail "a line for TERM, which is in its default state"
grep -v '^trap -- ' pc.tl >/dev/null && pc_fail "a line is not of the form 'trap -- ...'"
pcscript '. ./pc.tl' 'kill -s USR2 $$' 'kill -s USR1 $$' 'echo end'
pcrun
[ "$out" = "hi${nl}end" ] || pc_fail "reading the list back gave '$out'"
pc_pass
