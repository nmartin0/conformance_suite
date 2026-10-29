# Giving a new value to a read-only variable with export is an error: the
# status is non-zero (a non-interactive shell may exit) and the value is
# unchanged.
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.export.005
. "$PC_CORE/pc.sh"
pc_begin XCU.export.005
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
# pcerr LABEL: after pcrun of a script whose failing command was followed by
# echo "rc=$?": accept a shell that exited with a non-zero status without
# further output, or one that went on and reported a non-zero status.
pcerr() {
	if [ "$rc" -eq 0 ]; then
		case $out in
		rc=[1-9]*) ;;
		*) pc_fail "$1: the shell neither exited with an error nor reported a non-zero status (out='$out')" ;;
		esac
	elif [ -n "$out" ]; then
		pc_fail "$1: the shell exited with status $rc after writing '$out'"
	fi
}
pcscript 'readonly PCR=1' 'export PCR=2' 'echo "rc=$?"' 'echo "val=$PCR"' 
printf '%s\n' 'echo ran' >pc.probe || pc_unresolved "cannot write pc.probe"
pcprobe=$(${PC_SH:-sh} pc.probe 2>/dev/null </dev/null)
[ "$pcprobe" = ran ] || pc_unresolved "cannot run the shell under test"
pc_test
pcrun
pcerr "export PCR=2"
if [ "$rc" -eq 0 ]; then
	case $out in
	*"${nl}val=1") ;;
	*) pc_fail "the read-only variable changed (out='$out')" ;;
	esac
fi
pc_pass
