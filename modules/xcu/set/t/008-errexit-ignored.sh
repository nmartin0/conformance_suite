# set -e does not end the shell for a failure in an if, elif, while or until
# condition, in a command preceded by !, or in a command of an AND-OR list
# other than the last.
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.set.008
. "$PC_CORE/pc.sh"
pc_begin XCU.set.008
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
pcscript 'set -e' 'if false; then echo bad; fi' 'echo m1' 'false || true' 'echo m2' 'false && true' 'echo m3' '! true' 'echo m4' 'while false; do :; done' 'echo m5' 'if false; then :; elif false; then :; fi' 'echo m6' 'until true; do :; done' 'echo m7' 
pc_test
pcrun
want="m1${nl}m2${nl}m3${nl}m4${nl}m5${nl}m6${nl}m7"
[ "$out" = "$want" ] || pc_fail "got '$out'"
[ "$rc" -eq 0 ] || pc_fail "status $rc"
pc_pass
