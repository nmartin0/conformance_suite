# The utility operand is looked up through PATH; a script without a magic
# line found that way is run by a shell.
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.exec.004
. "$PC_CORE/pc.sh"
pc_begin XCU.exec.004
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
mkdir pc.bin || pc_unresolved "mkdir"
printf '%s\n' ': >"pc.mark.$1"' >pc.bin/pcutil || pc_unresolved "write"
chmod 755 pc.bin/pcutil || pc_unresolved "chmod"
pcscript "PATH=$PWD/pc.bin:\$PATH" 'exec pcutil x' 
pc_test
pcrun
[ -f pc.mark.x ] || pc_fail "script found through PATH was not run with its argument (status $rc)"
pc_pass
