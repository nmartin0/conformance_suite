# readonly name=word on a variable that is already read-only fails with a
# non-zero status and does not change the value.
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.readonly.006
. "$PC_CORE/pc.sh"
pc_begin XCU.readonly.006
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
pcscript 'readonly PCR=1' '(readonly PCR=2) 2>/dev/null' 'echo "rc=$? v=$PCR"' 
pc_test
pcrun
case $out in
"rc="[1-9]*" v=1") ;;
*) pc_fail "got '$out'" ;;
esac
pc_pass
