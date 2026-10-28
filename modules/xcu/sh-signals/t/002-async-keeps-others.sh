# Other signals keep the action the shell had in an asynchronous list: a
# default-action signal still ends the command.
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.sh-signals.002
. "$PC_CORE/pc.sh"
pc_begin XCU.sh-signals.002
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
printf '%s\n' 'kill -s TERM $$' 'echo notrun' >pc.k9 || pc_unresolved "write"
pcscript '${PC_SH:-sh} ./pc.k9 &' 'wait $!' 'echo "st=$?"' 
pc_test
pcrun
case $out in
"st="[1-9]*) ;;
*) pc_fail "got '$out'" ;;
esac
[ "${out#st=}" -gt 128 ] || pc_fail "status ${out#st=}, expected above 128"
pc_pass
