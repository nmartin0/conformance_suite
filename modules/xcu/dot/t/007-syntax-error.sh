# A syntax error in the file read by dot makes a non-interactive shell exit
# with a non-zero status; commands after the dot are not run.
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.dot.007
. "$PC_CORE/pc.sh"
pc_begin XCU.dot.007
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
printf '%s\n' 'echo (' >pc.bad || pc_unresolved "write"
pcscript '. ./pc.bad' 'echo after' 
pc_test
pcrun
[ "$rc" -ge 1 ] && [ "$rc" -le 125 ] || pc_fail "status $rc, expected 1 to 125"
case $out in
*after*) pc_fail "shell went on after the syntax error" ;;
esac
pc_pass
