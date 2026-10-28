# A signal with a trap that arrives while the shell is in wait makes wait
# return at once with a status above 128, and the trap action then runs.
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.sh-signals.003
. "$PC_CORE/pc.sh"
pc_begin XCU.sh-signals.003
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
pcscript 'trap "echo trapped" USR1' 'sleep 10 &' 'bg=$!' '(sleep 1; kill -s USR1 $$) &' 'wait "$bg"' 'echo "wait-rc=$?"' 'kill "$bg"' 'wait' 
pc_test
pcrun
case $out in
"trapped${nl}wait-rc="[1-9]*) ;;
*) pc_fail "got '$out', expected trapped then wait-rc with a status above 128" ;;
esac
[ "${out#*wait-rc=}" -gt 128 ] || pc_fail "wait returned ${out#*wait-rc=}, expected above 128"
pc_pass
