# A trap action runs in the current shell environment: its assignments stay.
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.trap.007
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
pcscript 'trap "pcx=hit" USR1' 'kill -s USR1 $$' 'echo "$pcx"' 
pc_test
pcrun
[ "$out" = hit ] || pc_fail "got '$out'"
pc_pass
