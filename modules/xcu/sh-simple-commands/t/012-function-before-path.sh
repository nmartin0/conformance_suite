# A function is used before a utility found in PATH, and once the function
# is removed the utility is found.
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.sh-simple-commands.012
. "$PC_CORE/pc.sh"
pc_begin XCU.sh-simple-commands.012
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
printf '%s\n' ': >pc.mark.util' >pc.bin/pcutil || pc_unresolved "write"
chmod 755 pc.bin/pcutil || pc_unresolved "chmod"
pcscript "PATH=$PWD/pc.bin:\$PATH" 'pcutil() { echo fn; }' 'pcutil' 'unset -f pcutil' 'pcutil' 'echo end' 
pc_test
pcrun
[ "$out" = "fn${nl}end" ] || pc_fail "got '$out'"
[ -e pc.mark.util ] || pc_fail "after unset -f the utility in PATH was not run"
pc_pass
