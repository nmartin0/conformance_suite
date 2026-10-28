# With set -C, > does not overwrite an existing regular file (the command
# fails) but >| does, and a file that does not exist yet or a non-regular
# file such as /dev/null can still be the target of >.
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.set.006
. "$PC_CORE/pc.sh"
pc_begin XCU.set.006
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
pcscript 'set -C' 'echo one >pc.nc1' 'echo two >pc.nc1' 'echo "rc=$?"' 'echo three >|pc.nc1' 'echo four >/dev/null' 'echo "rc2=$?"' 
pc_test
pcrun
case $out in
"rc="[1-9]*"${nl}rc2=0") ;;
*) pc_fail "got '$out', expected a non-zero rc then rc2=0" ;;
esac
[ "$(cat pc.nc1)" = three ] || pc_fail "file holds '$(cat pc.nc1)', expected three"
pc_pass
