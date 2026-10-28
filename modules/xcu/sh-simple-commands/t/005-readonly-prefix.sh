# An assignment before a command to a read-only variable is a variable
# assignment error: a non-interactive shell exits with a non-zero status,
# whether the command is a built-in or an external utility.
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.sh-simple-commands.005
. "$PC_CORE/pc.sh"
pc_begin XCU.sh-simple-commands.005
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
for cmd in 'true' '${PC_SH:-sh} -c :'; do
	pcscript 'readonly PCR=1' "PCR=2 $cmd" 'echo after'
	pcrun
	[ "$rc" -ge 1 ] && [ "$rc" -le 125 ] || pc_fail "$cmd: status $rc, expected 1 to 125"
	[ -z "$out" ] || pc_fail "$cmd: shell went on: '$out'"
done
pc_pass
