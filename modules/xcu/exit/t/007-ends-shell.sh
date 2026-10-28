# exit within a function, an eval string or a dot file ends the whole shell,
# not only that construct.
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.exit.007
. "$PC_CORE/pc.sh"
pc_begin XCU.exit.007
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
printf '%s\n' 'exit 8' >pc.d9 || pc_unresolved "write"
pcscript 'f() { exit 5; }' 'f' 'echo notrun' >/dev/null
pc_test
pcrun
[ "$rc" -eq 5 ] && [ -z "$out" ] || pc_fail "function: status $rc out '$out'"
pcscript 'eval "exit 3"' 'echo notrun'
pcrun
[ "$rc" -eq 3 ] && [ -z "$out" ] || pc_fail "eval: status $rc out '$out'"
pcscript '. ./pc.d9' 'echo notrun'
pcrun
[ "$rc" -eq 8 ] && [ -z "$out" ] || pc_fail "dot: status $rc out '$out'"
pc_pass
