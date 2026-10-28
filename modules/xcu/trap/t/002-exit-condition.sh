# A trap on EXIT (or on 0) runs when the shell exits normally.
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.trap.002
. "$PC_CORE/pc.sh"
pc_begin XCU.trap.002
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
for cond in EXIT 0; do
	pcscript "trap 'echo bye' $cond" 'echo run'
	pcrun
	[ "$out" = "run${nl}bye" ] || pc_fail "$cond: got '$out'"
done
pc_pass
