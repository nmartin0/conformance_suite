# readonly accepts "--" as the end of options.
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.readonly.008
. "$PC_CORE/pc.sh"
pc_begin XCU.readonly.008
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
pcscript 'readonly -- PCRC=1' 'echo "$PCRC"' 
pc_test
pcrun
[ "$out" = 1 ] || pc_fail "got '$out' (status $rc)"
pc_pass
