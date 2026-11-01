# A new trap on a condition replaces the earlier one.
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.trap.005
# MUTANT    trap.keeps_first
# MUTANT    trap.runs_both
# MUTANT    trap.ignore_sticks
# MUTANT    trap.reset_sticks
. "$PC_CORE/pc.sh"
pc_begin XCU.trap.005
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
pcscript 'trap "echo a" USR1' 'trap "echo b" USR1' 'kill -s USR1 $$' 
pc_test
pcrun
[ "$out" = b ] || pc_fail "got '$out'"
# it also overrides ignoring, and the default restored by -
pcscript 'trap "" USR1' 'trap "echo b" USR1' 'kill -s USR1 $$' 'echo end'
pcrun
[ "$out" = "b${nl}end" ] || pc_fail "after ignore: got '$out'"
pcscript 'trap "echo a" USR1' 'trap - USR1' 'trap "echo c" USR1' 'kill -s USR1 $$' 'echo end'
pcrun
[ "$out" = "c${nl}end" ] || pc_fail "after reset: got '$out'"
pc_pass
