# With set -x the shell writes a trace of each command to standard error
# after expanding it.
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.set.015
. "$PC_CORE/pc.sh"
pc_begin XCU.set.015
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
pcscript 'pcv=expanded_val' 'set -x' 'echo "$pcv" two' 
pc_test
pcrun
[ "$out" = "expanded_val two" ] || pc_fail "standard output was '$out'"
grep 'echo' pc.err | grep 'expanded_val' >/dev/null || pc_fail "no trace line with the expanded command on standard error"
grep '\$pcv' pc.err >/dev/null && pc_fail "the trace shows the command before expansion"
pc_pass
