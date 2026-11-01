# trap action condition runs the action (as eval would) when the signal
# arrives, and the shell then goes on.
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.trap.001
# MUTANT    trap.action_never
# MUTANT    trap.action_exits
# MUTANT    trap.action_twice
# MUTANT    trap.first_condition_only
# MUTANT    trap.signal_swapped
# MUTANT    trap.action_word
. "$PC_CORE/pc.sh"
pc_begin XCU.trap.001
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
pc_test
for sig in USR1 USR2 TERM; do
	pcscript "trap 'echo got; echo two' $sig" "kill -s $sig \$\$" 'echo end'
	pcrun
	[ "$out" = "got${nl}two${nl}end" ] || pc_fail "$sig: got '$out'"
done
# one command, two conditions: each one runs the action
pcscript "trap 'echo got' USR1 USR2" 'kill -s USR2 $$' 'kill -s USR1 $$' 'echo end'
pcrun
[ "$out" = "got${nl}got${nl}end" ] || pc_fail "two conditions: got '$out'"
# the action is shell text read and run as a whole, with its own variables
pcscript "trap 'x=1; echo \"v=\$x\"' USR1" 'kill -s USR1 $$' 'echo end'
pcrun
[ "$out" = "v=1${nl}end" ] || pc_fail "action text: got '$out'"
pc_pass
