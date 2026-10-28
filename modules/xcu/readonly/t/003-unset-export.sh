# A read-only variable cannot be unset or exported with a new value: both
# fail with a non-zero status and the value stays.
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.readonly.003
. "$PC_CORE/pc.sh"
pc_begin XCU.readonly.003
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
pcscript 'readonly PCR=1' '(unset PCR) 2>/dev/null' 'echo "a=$?"' '(export PCR=2) 2>/dev/null' 'echo "b=$?"' 'echo "val=$PCR"' 
pc_test
pcrun
case $out in
"a="[1-9]*"${nl}b="[1-9]*"${nl}val=1") ;;
*) pc_fail "got '$out', expected non-zero a and b and val=1" ;;
esac
pc_pass
