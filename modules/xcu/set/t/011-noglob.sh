# With set -f pathname expansion is off, with set +f it is on again.
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.set.011
. "$PC_CORE/pc.sh"
pc_begin XCU.set.011
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
LC_ALL=POSIX
export LC_ALL
: >pc.f1
: >pc.f2
pcscript 'set -f' 'echo pc.f*' 'set +f' 'echo pc.f*' 
pc_test
pcrun
[ "$out" = "pc.f*${nl}pc.f1 pc.f2" ] || pc_fail "got '$out'"
pc_pass
