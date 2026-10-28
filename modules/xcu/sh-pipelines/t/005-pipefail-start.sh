# The pipefail setting used is the one in force when the pipeline begins:
# false | set -o pipefail takes its status from the last command, not from
# false.
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.sh-pipelines.005
. "$PC_CORE/pc.sh"
pc_begin XCU.sh-pipelines.005
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
pcscript 'false | set -o pipefail' 'echo "a=$?"' 
pc_test
pcrun
[ "$out" = "a=0" ] || pc_fail "got '$out', expected a=0"
pc_pass
