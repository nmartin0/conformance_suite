# trap action condition runs the action (as eval would) when the signal
# arrives, and the shell then goes on.
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.trap.001
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
pc_pass
