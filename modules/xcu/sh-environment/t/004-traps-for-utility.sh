# A utility run by the shell gets ignored signals still ignored, and signals
# the shell catches back at their default action.
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.sh-environment.004
. "$PC_CORE/pc.sh"
pc_begin XCU.sh-environment.004
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
printf '%s\n' 'kill -s USR1 $$' 'echo alive' >pc.k6 || pc_unresolved "write"
printf '%s\n' 'kill -s USR1 $$' 'echo notrun' >pc.k7 || pc_unresolved "write"
pc_test
pcscript 'trap "" USR1' '${PC_SH:-sh} ./pc.k6'
pcrun
[ "$out" = alive ] || pc_fail "ignored signal: got '$out', expected alive"
pcscript 'trap "echo hit" USR1' '${PC_SH:-sh} ./pc.k7' 'echo "st=$?"'
pcrun
case $out in
"st="[1-9]*) ;;
*) pc_fail "caught signal: got '$out', expected the utility to be ended by the signal" ;;
esac
[ "${out#st=}" -gt 128 ] || pc_fail "status ${out#st=}, expected above 128"
pc_pass
