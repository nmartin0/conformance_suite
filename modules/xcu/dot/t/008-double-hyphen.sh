# The dot utility accepts "--" as the end of options before the file
# operand.
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.dot.008
. "$PC_CORE/pc.sh"
pc_begin XCU.dot.008
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
printf '%s\n' 'pcw=ok' >pc.d8 || pc_unresolved "write"
pcscript '. -- ./pc.d8' 'echo "$pcw"' 
pc_test
pcrun
[ "$out" = ok ] || pc_fail "got '$out', expected ok (status $rc)"
pc_pass
