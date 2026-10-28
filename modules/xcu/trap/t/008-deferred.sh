# A trap for a signal received while the shell waits for a foreground
# utility is not run until that utility has finished.
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.trap.008
. "$PC_CORE/pc.sh"
pc_begin XCU.trap.008
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
printf '%s\n' 'kill -s USR1 $PPID' 'echo child-end' >pc.k || pc_unresolved "write"
pcscript 'trap "echo trapped" USR1' '${PC_SH:-sh} ./pc.k' 'echo end' 
pc_test
pcrun
case $out in
"child-end${nl}trapped${nl}end"|"child-end${nl}end${nl}trapped") ;;
*) pc_fail "got '$out'" ;;
esac
pc_pass
