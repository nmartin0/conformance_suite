# With set -n the shell reads but does not execute the commands that follow,
# and exits with status zero.
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.set.012
# MUTANT    set.n_ignored
# MUTANT    set.n_default_on
# MUTANT    set.n_status1
# MUTANT    set.n_executes_one
# MUTANT    set.n_no_parse
. "$PC_CORE/pc.sh"
pc_begin XCU.set.012
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
pcscript 'echo before' 'set -n' 'echo hi' ': >pc.made'
pc_test
pcrun
[ "$out" = before ] && [ "$rc" -eq 0 ] || pc_fail "status $rc out '$out', expected only the output of the command before set -n and status 0"
[ -e pc.made ] && pc_fail "a command after set -n was executed"
pcscript 'echo before' 'set -n' 'if then fi' ': >pc.made'
pcrun
[ "$out" = before ] || pc_fail "syntax error after set -n: output '$out', expected only the output before set -n"
[ "$rc" -ge 1 ] && [ "$rc" -le 125 ] || pc_fail "syntax error after set -n: status $rc, expected 1 to 125"
[ -s pc.err ] || pc_fail "syntax error after set -n: no message on standard error"
[ -e pc.made ] && pc_fail "a command after the syntax error was executed"
pc_pass
