# Under set -e, a compound command (other than a subshell) whose non-zero
# status came from a failure while -e was ignored does not end the shell.
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.set.010
# MUTANT    set.e_compound_group_exits
# MUTANT    set.e_compound_if_exits
# MUTANT    set.e_compound_for_exits
# MUTANT    set.e_compound_case_exits
# MUTANT    set.e_subshell_compound_ignored
# MUTANT    set.e_ignored
. "$PC_CORE/pc.sh"
pc_begin XCU.set.010
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
pcscript 'set -e' '{ false && true; }' 'echo m1' 'if true; then false && true; fi' 'echo m2' 'for pci in 1; do false && true; done' 'echo m3' 'case x in x) false && true ;; esac' 'echo m4' 'false' 'echo notrun'
pcexp "compound commands whose status comes from an ignored failure" "m1${nl}m2${nl}m3${nl}m4" 1
pcscript 'set -e' '( false && true )' 'echo after'
pcexp "subshell command" "" 1
pc_pass
