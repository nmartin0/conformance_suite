# With set -u, expanding an unset parameter other than @ and * writes a
# message to standard error and the non-interactive shell exits with a non-
# zero status; $@ and $* are exempt.
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.set.013
. "$PC_CORE/pc.sh"
pc_begin XCU.set.013
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
pcscript 'set -u' 'echo "x:$PCU1"' 'echo after'
pcrun
[ -z "$out" ] || pc_fail "output '$out', expected none"
[ "$rc" -ge 1 ] && [ "$rc" -le 125 ] || pc_fail "status $rc, expected 1 to 125"
[ -s pc.err ] || pc_fail "no message on standard error"
pcscript 'set -u' 'set --' 'echo "[$@][$*]"'
pcrun
[ "$out" = "[][]" ] && [ "$rc" -eq 0 ] || pc_fail "\$@ and \$* with no parameters: status $rc out '$out'"
pc_pass
