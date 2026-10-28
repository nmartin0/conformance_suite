# The same errors do not end the shell when the special built-in is run
# through the command utility.
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.sh-errors.003
. "$PC_CORE/pc.sh"
pc_begin XCU.sh-errors.003
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
for cmd in 'command set -Z' 'readonly PCR=1; command export PCR=2'; do
	pcscript "$cmd" 'echo "after $?"'
	pcrun
	case $out in
	"after "[1-9]*) ;;
	*) pc_fail "$cmd: got '$out', expected after and a non-zero status" ;;
	esac
done
pc_pass
