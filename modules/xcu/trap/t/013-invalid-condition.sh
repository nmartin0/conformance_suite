# An unknown condition name makes trap write a warning to standard error and
# return non-zero, but the shell does not exit.
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.trap.013
. "$PC_CORE/pc.sh"
pc_begin XCU.trap.013
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
pcscript 'trap "echo x" PCNOSUCHSIG_XYZ' 'echo "rc=$?"' 'echo after' 
pc_test
pcrun
case $out in
"rc="[1-9]*"${nl}after") ;;
*) pc_fail "got '$out', expected a non-zero rc line and after" ;;
esac
[ -s pc.err ] || pc_fail "no warning on standard error"
pc_pass
