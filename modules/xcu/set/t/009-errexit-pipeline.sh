# Under set -e, a failing command that is not the last of a pipeline, a
# failing subshell inside a pipeline whose last command succeeds, and a
# failing subshell for a command substitution do not end the shell; a
# failing pipeline does.
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.set.009
# MUTANT    set.e_pipe_first_status_subshell
# MUTANT    set.e_pipe_first_status_simple
# MUTANT    set.e_cmdsub_exits
# MUTANT    set.e_pipe_subshell_no_e
# MUTANT    set.e_cmdsub_no_e
# MUTANT    set.e_pipe_fail_ignored
# MUTANT    set.e_status1
# MUTANT    set.e_ignored
. "$PC_CORE/pc.sh"
pc_begin XCU.set.009
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
# pcexp LABEL OUT RC: run pc.s; it must write OUT and exit with RC
pcexp() {
	pcrun
	[ "$out" = "$2" ] && [ "$rc" -eq "$3" ] || pc_fail "$1: status $rc and output '$out', expected status $3 and output '$2'"
}
pc_test
pcscript 'set -e' '(false; echo one) | cat' 'echo two' 'echo $(false; echo one) three' 'echo four' 'false | true' 'echo five'
pcexp "failures that are not the failure of a pipeline" "two${nl}three${nl}four${nl}five" 0
pcscript 'set -e' 'true | false' 'echo after'
pcexp "failing pipeline" "" 1
pcscript 'set -e' 'true | (exit 5)' 'echo after'
pcexp "pipeline whose last command exits 5" "" 5
pc_pass
