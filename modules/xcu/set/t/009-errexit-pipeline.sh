# Under set -e, a failing command that is not the last of a pipeline, a
# failing subshell inside a pipeline whose last command succeeds, and a
# failing subshell for a command substitution do not end the shell; a
# failing pipeline does.
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.set.009
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
pc_test
pcscript 'set -e' '(false; echo one) | while read -r pcl; do echo "$pcl"; done' 'echo two' 'echo $(false; echo one) three' 'echo four' 'false | true' 'echo five'
pcrun
want="two${nl}three${nl}four${nl}five"
[ "$out" = "$want" ] || pc_fail "got '$out'"
[ "$rc" -eq 0 ] || pc_fail "status $rc"
pcscript 'set -e' 'true | false' 'echo after'
pcrun
[ -z "$out" ] && [ "$rc" -eq 1 ] || pc_fail "failing pipeline: status $rc out '$out'"
pc_pass
