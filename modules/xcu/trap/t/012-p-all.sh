# trap -p with no operands lists all conditions, those in their default
# state too, each as a trap -- line.
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.trap.012
# MUTANT    trap.p_all_nondefault
# MUTANT    trap.p_all_truncated
# MUTANT    trap.p_all_stderr
# MUTANT    trap.p_all_no_dashdash
# MUTANT    trap.p_all_kill_rejected
# VARIANT   trap.p_conform
# VARIANT   trap.p_conform_kill
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
for c in HUP INT QUIT ABRT ALRM TERM USR1 USR2 PIPE CHLD; do
	grep "$c" pc.ta >/dev/null || pc_fail "no line for $c"
done
grep -v '^trap -- ' pc.ta >/dev/null && pc_fail "a line is not of the form 'trap -- ...'"
# Every line must be accepted when read back by the shell: no failure and no
# diagnostic, also for the two signals that cannot be caught if they are listed.
pcscript '. ./pc.ta' 'echo end'
pcrun
[ "$rc" -eq 0 ] && [ "$out" = end ] || pc_fail "reading the listing back gave status $rc, output '$out'"
[ -s pc.err ] && pc_fail "reading the listing back wrote a diagnostic"
pc_pass
