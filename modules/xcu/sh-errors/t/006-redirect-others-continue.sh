# A failing redirection on a compound command, on a function call or on a
# regular utility does not end the shell; the command is not run and its
# status is non-zero.
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.sh-errors.006
. "$PC_CORE/pc.sh"
pc_begin XCU.sh-errors.006
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
pcscript '{ echo inside; } <pc.nosuchfile' 'echo "after $?"'
pcrun
case $out in
"after "[1-9]*) ;;
*) pc_fail "compound command: got '$out'" ;;
esac
pcscript 'pcf() { echo inside; } <pc.nosuchfile' 'pcf' 'echo "after $?"'
pcrun
case $out in
"after "[1-9]*) ;;
*) pc_fail "function call: got '$out'" ;;
esac
pcscript 'cat <pc.nosuchfile' 'echo "after $?"'
pcrun
case $out in
"after "[1-9]*) ;;
*) pc_fail "regular utility: got '$out'" ;;
esac
pc_pass
