# When no readable file is found, a non-interactive shell exits with a non-
# zero status and runs nothing more.
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.dot.005
. "$PC_CORE/pc.sh"
pc_begin XCU.dot.005
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
pcscript '. ./pc.nosuchfile' 'echo after' 
pc_test
pcrun
[ "$rc" -ge 1 ] || pc_fail "status $rc, expected non-zero"
[ -z "$out" ] || pc_fail "shell went on after the failed dot: '$out'"
pc_pass
