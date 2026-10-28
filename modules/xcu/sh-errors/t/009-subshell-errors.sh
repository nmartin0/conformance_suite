# An error that would end a shell ends only the subshell it happens in; the
# shell goes on with a non-zero status for the subshell.
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.sh-errors.009
. "$PC_CORE/pc.sh"
pc_begin XCU.sh-errors.009
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
pcscript 'readonly PCR=1' '(PCR=2; echo sub-after) 2>/dev/null' 'echo "parent $?"' 
pc_test
pcrun
case $out in
"parent "[1-9]*) ;;
*) pc_fail "got '$out', expected only a parent line with a non-zero status" ;;
esac
pc_pass
