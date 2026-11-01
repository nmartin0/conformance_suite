# With set -e, a failing command ends the shell at once with the failed
# command's status.
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.set.007
# MUTANT    set.e_ignored
# MUTANT    set.e_default_on
# MUTANT    set.e_status0
# MUTANT    set.e_status1
# MUTANT    set.e_subshell_ignored
# MUTANT    set.e_group_ignored
# MUTANT    set.e_function_ignored
# MUTANT    set.e_loop_ignored
# MUTANT    set.e_case_ignored
# MUTANT    set.e_assign_ignored
# MUTANT    set.e_notfound_ignored
# MUTANT    set.e_late_exit_group
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
# pcexp LABEL OUT RC: run pc.s; it must write OUT and exit with RC
pcexp() {
	pcrun
	[ "$out" = "$2" ] && [ "$rc" -eq "$3" ] || pc_fail "$1: status $rc and output '$out', expected status $3 and output '$2'"
}
pc_test
pcscript 'false' 'echo survived'
pcexp "without -e" survived 0
pcscript 'set -e' 'echo before' 'false' 'echo after'
pcexp "false" before 1
pcscript 'set -e' '(exit 7)' 'echo after'
pcexp "subshell" "" 7
pcscript 'set -e' '{ false; echo after; }' 'echo after2'
pcexp "brace group" "" 1
pcscript 'set -e' 'pcf() { false; echo after; }' 'pcf' 'echo after2'
pcexp "function" "" 1
pcscript 'set -e' 'for pci in 1 2; do false; echo after; done' 'echo after2'
pcexp "for loop" "" 1
pcscript 'set -e' 'case x in x) false; echo after ;; esac' 'echo after2'
pcexp "case" "" 1
pcscript 'set -e' 'pcv=$(exit 3)' 'echo after'
pcexp "assignment from a failing command substitution" "" 3
pcscript 'set -e' 'pcm_nosuch_cmd_x' 'echo after'
pcexp "command not found" "" 127
pc_pass
