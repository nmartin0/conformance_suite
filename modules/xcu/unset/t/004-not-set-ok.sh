# Unsetting a name that is not set is not an error: status zero and the
# shell goes on.
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.unset.004
. "$PC_CORE/pc.sh"
pc_begin XCU.unset.004
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
pcscript 'unset PCNOSUCH' 'echo "a=$?"' 'unset -f pcnosuchfn' 'echo "b=$?"' 'unset -v PCNOSUCH PCNOSUCH2' 'echo "c=$?"' 
pc_test
pcrun
[ "$out" = "a=0${nl}b=0${nl}c=0" ] || pc_fail "got '$out'"
pc_pass
