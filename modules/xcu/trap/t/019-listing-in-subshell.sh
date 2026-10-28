# trap with no operands inside a command substitution lists the traps of the
# shell that started the substitution.
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.trap.019
. "$PC_CORE/pc.sh"
pc_begin XCU.trap.019
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
pcscript 'trap "echo hi" USR1' 's=$(trap)' 'case $s in *USR1*) echo yes ;; *) echo no ;; esac' 
pc_test
pcrun
[ "$out" = yes ] || pc_fail "got '$out'"
pc_pass
