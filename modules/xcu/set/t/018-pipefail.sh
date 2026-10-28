# set -o pipefail makes a pipeline fail when any command in it fails, using
# the rightmost failing status; set +o pipefail restores the default.
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.set.018
. "$PC_CORE/pc.sh"
pc_begin XCU.set.018
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
pcscript 'set -o pipefail' 'false | true' 'echo "a=$?"' '(exit 3) | (exit 0) | (exit 2) | true' 'echo "b=$?"' 'true | true' 'echo "c=$?"' 'set +o pipefail' 'false | true' 'echo "d=$?"' 
pc_test
pcrun
[ "$out" = "a=1${nl}b=2${nl}c=0${nl}d=0" ] || pc_fail "got '$out'"
pc_pass
