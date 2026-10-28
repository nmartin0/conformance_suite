# After readonly name=word, a plain assignment to name is an error: a non-
# interactive shell exits with a non-zero status; in a subshell only the
# subshell ends, and the value is unchanged.
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.readonly.001
. "$PC_CORE/pc.sh"
pc_begin XCU.readonly.001
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
pcscript 'readonly PCR=1' 'PCR=2' 'echo after' 
pc_test
pcrun
[ "$rc" -ge 1 ] && [ "$rc" -le 125 ] || pc_fail "status $rc, expected 1 to 125"
[ -z "$out" ] || pc_fail "shell went on after the assignment error: '$out'"
pcscript 'readonly PCR=1' '(PCR=2; echo sub) 2>/dev/null' 'echo "rc=$? val=$PCR"'
pcrun
case $out in
"rc="[1-9]*" val=1") ;;
*) pc_fail "subshell case: got '$out', expected rc non-zero and val=1" ;;
esac
pc_pass
