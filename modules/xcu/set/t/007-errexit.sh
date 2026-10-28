# With set -e, a failing command ends the shell at once with the failed
# command's status.
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.set.007
. "$PC_CORE/pc.sh"
pc_begin XCU.set.007
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
pc_test
pcscript 'set -e' 'echo before' 'false' 'echo after'
pcrun
[ "$out" = before ] || pc_fail "got '$out', expected only before"
[ "$rc" -eq 1 ] || pc_fail "status $rc, expected 1"
pcscript 'set -e' '(exit 7)' 'echo after'
pcrun
[ -z "$out" ] && [ "$rc" -eq 7 ] || pc_fail "(exit 7): status $rc out '$out', expected 7 and no output"
pc_pass
