# An error in a utility that is not a special built-in does not end the
# shell.
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.sh-errors.004
. "$PC_CORE/pc.sh"
pc_begin XCU.sh-errors.004
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
pcscript 'false' 'echo "after $?"' 'cat pc.nosuchfile 2>/dev/null' 'echo "after2 $?"'
pcrun
case $out in
"after 1${nl}after2 "[1-9]*) ;;
*) pc_fail "got '$out'" ;;
esac
[ "$rc" -eq 0 ] || pc_fail "status $rc"
pc_pass
