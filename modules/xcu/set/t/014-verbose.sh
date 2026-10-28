# With set -v the shell writes the input it reads to standard error.
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.set.014
. "$PC_CORE/pc.sh"
pc_begin XCU.set.014
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
pcscript 'set -v' 'echo marker_one' 
pc_test
pcrun
[ "$out" = marker_one ] || pc_fail "standard output was '$out'"
grep 'echo marker_one' pc.err >/dev/null || pc_fail "the command line was not written to standard error"
pc_pass
