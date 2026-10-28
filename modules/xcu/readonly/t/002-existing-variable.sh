# readonly name (no value) makes an existing variable read-only while
# keeping its value.
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.readonly.002
. "$PC_CORE/pc.sh"
pc_begin XCU.readonly.002
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
pcscript 'PCR=5' 'readonly PCR' '(PCR=6) 2>/dev/null' 'echo "rc=$? val=$PCR"' 
pc_test
pcrun
case $out in
"rc="[1-9]*" val=5") ;;
*) pc_fail "got '$out', expected a non-zero rc and val=5" ;;
esac
pc_pass
