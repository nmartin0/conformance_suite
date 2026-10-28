# trap -p with no operands lists all conditions, those in their default
# state too, each as a trap -- line.
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.trap.012
. "$PC_CORE/pc.sh"
pc_begin XCU.trap.012
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
pcscript 'trap -p >pc.ta' 
pc_test
pcrun
[ "$rc" -eq 0 ] || pc_fail "setup shell failed with $rc"
for c in USR1 USR2 TERM; do
	grep "$c" pc.ta >/dev/null || pc_fail "no line for $c"
done
grep -v '^trap -- ' pc.ta >/dev/null && pc_fail "a line is not of the form 'trap -- ...'"
pc_pass
