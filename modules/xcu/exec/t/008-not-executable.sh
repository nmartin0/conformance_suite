# exec of an existing file that is not executable makes a non-interactive
# shell exit with status 126.
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.exec.008
. "$PC_CORE/pc.sh"
pc_begin XCU.exec.008
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
printf '%s\n' ':' >pc.ne || pc_unresolved "write"
chmod 644 pc.ne || pc_unresolved "chmod"
pcscript 'exec ./pc.ne' 'echo after' 
pc_test
pcrun
[ "$rc" -eq 126 ] || pc_fail "status $rc, expected 126"
[ -z "$out" ] || pc_fail "shell went on: '$out'"
pc_pass
