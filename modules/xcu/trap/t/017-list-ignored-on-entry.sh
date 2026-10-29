# The trap listing includes a signal that was ignored on entry to a non-
# interactive shell.
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.trap.017
. "$PC_CORE/pc.sh"
pc_begin XCU.trap.017
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
printf '%s\n' 'trap >pc.tl5' >pc.k5 || pc_unresolved "write"
# pchas FILE STRING: some line of FILE contains STRING (shell only: no grep)
pchas() { while IFS= read -r pcl; do case $pcl in *"$2"*) return 0 ;; esac; done <"$1" 2>/dev/null; return 1; }
pcscript 'trap "" USR1' 'exec ${PC_SH:-sh} ./pc.k5' 
pc_test
pcrun
[ "$rc" -eq 0 ] || pc_fail "status $rc"
pchas pc.tl5 'USR1' || pc_fail "USR1, ignored on entry, is not in the listing"
pc_pass
