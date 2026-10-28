# A read-only variable cannot be unset: unset returns non-zero (the shell
# may exit) and the value stays.
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.unset.005
. "$PC_CORE/pc.sh"
pc_begin XCU.unset.005
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
pcscript 'readonly PCR=1' '(unset PCR) 2>/dev/null' 'echo "a=$? v=$PCR"'
pcrun
case $out in
"a="[1-9]*" v=1") ;;
*) pc_fail "subshell form: got '$out'" ;;
esac
pcscript 'readonly PCR=1' 'unset PCR' 'echo "rc=$?"' 'echo "v=$PCR"'
pcrun
pcerr "unset of a read-only variable"
if [ "$rc" -eq 0 ]; then
	case $out in
	*"${nl}v=1") ;;
	*) pc_fail "the read-only variable changed (out='$out')" ;;
	esac
fi
pc_pass
