# shift n with n larger than $# returns non-zero (a non-interactive shell
# may exit instead) and, when it goes on, writes a warning to standard
# error.
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.shift.003
. "$PC_CORE/pc.sh"
pc_begin XCU.shift.003
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
pcscript 'set a b' 'shift 3' 'echo "rc=$?"' 
pc_test
pcrun
pcerr "shift 3 with two parameters"
if [ "$rc" -eq 0 ]; then
	[ -s pc.err ] || pc_fail "no warning on standard error"
fi
pc_pass
