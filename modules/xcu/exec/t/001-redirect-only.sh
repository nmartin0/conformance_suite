# With no utility operand, exec makes its redirections in the current shell:
# later commands write to the redirected output, read the redirected input,
# and use a newly opened descriptor.
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.exec.001
. "$PC_CORE/pc.sh"
pc_begin XCU.exec.001
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
printf '%s\n' 1 2 >pc.in || pc_unresolved "write"
pcscript 'exec >pc.o1' 'echo captured' 'exec 3>pc.o2' 'echo fd3 >&3' 'exec 3>&-' 'exec <pc.in' 'read a' 'read b' 'echo "$b$a" >pc.o3' 
pc_test
pcrun
[ -z "$out" ] || pc_fail "output not redirected: '$out'"
[ "$(cat pc.o1)" = captured ] || pc_fail "pc.o1 holds '$(cat pc.o1)'"
[ "$(cat pc.o2)" = fd3 ] || pc_fail "pc.o2 holds '$(cat pc.o2)'"
[ "$(cat pc.o3)" = 21 ] || pc_fail "input redirection: pc.o3 holds '$(cat pc.o3)'"
pc_pass
