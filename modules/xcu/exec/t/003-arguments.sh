# The operands after the utility name are its arguments, passed one to one
# (including empty ones and ones with spaces).
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.exec.003
. "$PC_CORE/pc.sh"
pc_begin XCU.exec.003
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
pcscript 'exec printf "<%s>" a "b c" "" d' 
pc_test
pcrun
[ "$out" = "<a><b c><><d>" ] || pc_fail "got '$out'"
pc_pass
