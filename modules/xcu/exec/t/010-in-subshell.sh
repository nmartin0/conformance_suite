# exec in a subshell replaces only the subshell: the parent shell continues
# and sees the utility's status.
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.exec.010
. "$PC_CORE/pc.sh"
pc_begin XCU.exec.010
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
pcscript '(exec ${PC_SH:-sh} -c "exit 5")' 'echo "rc=$? alive"' 
pc_test
pcrun
[ "$out" = "rc=5 alive" ] || pc_fail "got '$out'"
pc_pass
