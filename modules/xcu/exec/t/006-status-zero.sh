# exec with no operands, or with only redirections that succeed, returns
# status zero.
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.exec.006
. "$PC_CORE/pc.sh"
pc_begin XCU.exec.006
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
pcscript 'false' 'exec' 'echo "a=$?"' 'false' 'exec 3>pc.f3' 'echo "b=$?"' 
pc_test
pcrun
[ "$out" = "a=0${nl}b=0" ] || pc_fail "got '$out'"
pc_pass
