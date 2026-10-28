# An error in a special built-in run directly makes a non-interactive shell
# exit with a non-zero status: an invalid option to set, unsetting a read-
# only variable, giving export a value for a read-only variable.
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.sh-errors.002
. "$PC_CORE/pc.sh"
pc_begin XCU.sh-errors.002
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
for cmd in 'set -Z' 'readonly PCR=1; unset PCR' 'readonly PCR=1; export PCR=2'; do
	pcscript "$cmd" 'echo after'
	pcrun
	[ "$rc" -ge 1 ] && [ "$rc" -le 125 ] || pc_fail "$cmd: status $rc, expected 1 to 125"
	[ -z "$out" ] || pc_fail "$cmd: the shell went on: '$out'"
done
pc_pass
