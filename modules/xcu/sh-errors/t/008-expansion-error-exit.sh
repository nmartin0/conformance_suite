# An expansion error (an unset parameter with ${name?}, ${name:?}, or an
# invalid expansion operator) makes a non-interactive shell exit with a non-
# zero status and a diagnostic.
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.sh-errors.008
. "$PC_CORE/pc.sh"
pc_begin XCU.sh-errors.008
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
for cmd in 'echo ${PCU1?msg}' 'echo ${PCU1:?msg}' 'echo "${PCU1!y}"'; do
	pcscript "$cmd" 'echo after'
	pcrun
	[ "$rc" -ge 1 ] && [ "$rc" -le 125 ] || pc_fail "$cmd: status $rc, expected 1 to 125"
	[ -z "$out" ] || pc_fail "$cmd: the shell went on: '$out'"
	[ -s pc.err ] || pc_fail "$cmd: no diagnostic"
done
pc_pass
