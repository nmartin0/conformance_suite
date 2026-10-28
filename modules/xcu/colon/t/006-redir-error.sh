# When the redirection of the null utility fails, a non-interactive shell
# exits with a non-zero status and runs no further command.
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.colon.006
. "$PC_CORE/pc.sh"
pc_begin XCU.colon.006
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
pcscript ': >pc.nodir/f' 'echo after' 
pc_test
pcrun
[ "$rc" -ge 1 ] && [ "$rc" -le 125 ] || pc_fail "shell exited with status $rc, expected 1 to 125"
[ -z "$out" ] || pc_fail "shell went on after the redirection error: '$out'"
pc_pass
