# With pipefail the status is 0 when every command succeeded, else the
# status of the rightmost failing command; with ! the result is negated.
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.sh-pipelines.004
. "$PC_CORE/pc.sh"
pc_begin XCU.sh-pipelines.004
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
pcscript 'set -o pipefail' 'false | true; echo "a=$?"' '(exit 3) | (exit 0) | (exit 2) | true; echo "b=$?"' 'true | true; echo "c=$?"' '! false | true; echo "d=$?"' '! true | true; echo "e=$?"' 'set +o pipefail' '! false | true; echo "f=$?"' 'false | true; echo "g=$?"' 
pc_test
pcrun
want="a=1${nl}b=2${nl}c=0${nl}d=0${nl}e=1${nl}f=1${nl}g=0"
[ "$out" = "$want" ] || pc_fail "got '$out', expected '$want'"
pc_pass
