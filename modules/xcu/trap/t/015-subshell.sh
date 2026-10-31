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
# The signal goes to the parent of the shell that runs pc.k2. When the
# subshell is a process of its own that parent is the subshell. A shell is
# not required to make one (the page does not say so), and then the parent is
# the main shell and the trap that runs there says nothing about the subshell.
pcscript 'trap "v=1; echo hit" USR1' '(${PC_SH:-sh} ./pc.k2; echo sub-end)' 'echo "st=$? v=$v"'
pcrun
case $out in
"st="[1-9]*" v=") ;;
*"v=1") pc_unresolved "the signal reached the main shell: the subshell is not a process of its own here" ;;
*) pc_fail "caught trap in subshell: got '$out', expected the subshell to be ended by the signal" ;;
esac
st=${out#st=}
st=${st%% *}
[ "$st" -gt 128 ] || pc_fail "subshell status $st, expected above 128"
pcscript 'trap "" USR2' '(${PC_SH:-sh} ./pc.k3; echo sub-end)'
pcrun
[ "$out" = sub-end ] || pc_fail "ignored signal in subshell: got '$out', expected sub-end"
pc_pass
