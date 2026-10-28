# exec accepts "--" as the end of options before the utility.
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.exec.005
. "$PC_CORE/pc.sh"
pc_begin XCU.exec.005
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
pcscript 'exec -- printf "%s\n" dd' 
pc_test
pcrun
[ "$out" = dd ] || pc_fail "got '$out' (status $rc)"
pc_pass
