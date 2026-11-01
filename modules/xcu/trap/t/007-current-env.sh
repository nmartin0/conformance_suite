# A trap action runs in the current shell environment: its assignments stay.
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.trap.007
# MUTANT    trap.action_subshell
# MUTANT    trap.action_new_shell
. "$PC_CORE/pc.sh"
pc_begin XCU.trap.007
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
mkdir pc.sub7 || pc_unresolved "mkdir"
d=$(cd pc.sub7 && pwd -P) || pc_unresolved "cd"
pcscript 'trap "pcx=hit" USR1' 'kill -s USR1 $$' 'echo "$pcx"' 
pc_test
pcrun
[ "$out" = hit ] || pc_fail "got '$out'"
# the working directory and a function definition made by the action stay too
pcscript 'trap "cd pc.sub7; pcf() { echo fn; }" USR1' 'kill -s USR1 $$' 'pwd -P' 'pcf'
pcrun
[ "$out" = "$d${nl}fn" ] || pc_fail "cd and function definition in the action: got '$out', expected the directory and fn"
pc_pass
