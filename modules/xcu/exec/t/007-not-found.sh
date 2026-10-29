# exec of a utility that does not exist makes a non-interactive shell exit
# with status 127; no later command is run.
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.exec.007
. "$PC_CORE/pc.sh"
pc_begin XCU.exec.007
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
pcscript 'exec pc-no-such-utility-xyz' 'echo after' 
printf '%s\n' 'echo ran' >pc.probe || pc_unresolved "cannot write pc.probe"
pcprobe=$(${PC_SH:-sh} pc.probe 2>/dev/null </dev/null)
[ "$pcprobe" = ran ] || pc_unresolved "cannot run the shell under test"
pc_test
pcrun
[ "$rc" -eq 127 ] || pc_fail "status $rc, expected 127"
[ -z "$out" ] || pc_fail "shell went on: '$out'"
pc_pass
