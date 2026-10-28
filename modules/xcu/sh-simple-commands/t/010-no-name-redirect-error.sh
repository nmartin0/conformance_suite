# A failing redirection of a command with no command name fails with a non-
# zero status and a diagnostic.
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.sh-simple-commands.010
. "$PC_CORE/pc.sh"
pc_begin XCU.sh-simple-commands.010
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
pcscript 'false' '<pc.nosuchfile' 'echo "rc=$?"' 
pc_test
pcrun
pcerr "redirection of a missing file"
[ -s pc.err ] || pc_fail "no diagnostic on standard error"
pc_pass
