# shift with an operand that is not an unsigned decimal integer returns non-
# zero (or the non-interactive shell exits).
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.shift.004
. "$PC_CORE/pc.sh"
pc_begin XCU.shift.004
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
pc_test
for arg in x -1 1x; do
	pcscript 'set a b' "shift $arg" 'echo "rc=$?"'
	pcrun
	pcerr "shift $arg"
done
pc_pass
