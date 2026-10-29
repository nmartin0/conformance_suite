# The long forms set -o errexit, nounset, noexec, verbose and xtrace act
# like -e, -u, -n, -v and -x.
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.set.017
. "$PC_CORE/pc.sh"
pc_begin XCU.set.017
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
# pchas FILE STRING: some line of FILE contains STRING (shell only: no grep)
pchas() { while IFS= read -r pcl; do case $pcl in *"$2"*) return 0 ;; esac; done <"$1" 2>/dev/null; return 1; }
pc_test
pcscript 'set -o errexit' 'false' 'echo after'
pcrun
[ -z "$out" ] && [ "$rc" -eq 1 ] || pc_fail "errexit: status $rc out '$out'"
pcscript 'set -o nounset' 'echo "$PCU1"' 'echo after'
pcrun
[ -z "$out" ] && [ "$rc" -ge 1 ] || pc_fail "nounset: status $rc out '$out'"
pcscript 'set -o noexec' 'echo notrun'
pcrun
[ -z "$out" ] && [ "$rc" -eq 0 ] || pc_fail "noexec: status $rc out '$out'"
pcscript 'set -o verbose' 'echo marker_two'
pcrun
pchas pc.err 'echo marker_two' || pc_fail "verbose: input not written to standard error"
pcscript 'pcv=expanded_two' 'set -o xtrace' 'echo "$pcv"'
pcrun
pchas pc.err 'expanded_two' || pc_fail "xtrace: no trace on standard error"
pc_pass
