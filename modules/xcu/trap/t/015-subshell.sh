# A subshell resets caught traps to the default but keeps ignored signals
# ignored.
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.trap.015
. "$PC_CORE/pc.sh"
pc_begin XCU.trap.015
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
printf '%s\n' 'kill -s USR1 $PPID' >pc.k2 || pc_unresolved "write"
printf '%s\n' 'kill -s USR2 $PPID' >pc.k3 || pc_unresolved "write"
pc_test
pcscript 'trap "echo hit" USR1' '(${PC_SH:-sh} ./pc.k2; echo sub-end)' 'echo "st=$?"'
pcrun
case $out in
"st="[1-9]*) ;;
*) pc_fail "caught trap in subshell: got '$out', expected the subshell to be ended by the signal" ;;
esac
[ "${out#st=}" -gt 128 ] || pc_fail "subshell status ${out#st=}, expected above 128"
pcscript 'trap "" USR2' '(${PC_SH:-sh} ./pc.k3; echo sub-end)'
pcrun
[ "$out" = sub-end ] || pc_fail "ignored signal in subshell: got '$out', expected sub-end"
pc_pass
