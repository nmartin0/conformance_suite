# The options -b, -m, ignoreeof, monitor, notify and nolog are accepted in
# both forms.
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.set.020
# MUTANT    set.up_m_rejected
# MUTANT    set.up_plus_m_rejected
# MUTANT    set.up_b_rejected
# MUTANT    set.up_plus_b_rejected
# MUTANT    set.up_monitor_rejected
# MUTANT    set.up_notify_rejected
# MUTANT    set.up_ignoreeof_rejected
# MUTANT    set.up_nolog_rejected
. "$PC_CORE/pc.sh"
pc_begin XCU.set.020
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
pcscript 'set -m' 'set +m' 'set -b' 'set +b' 'set -o monitor' 'set +o monitor' 'set -o notify' 'set +o notify' 'set -o ignoreeof' 'set +o ignoreeof' 'set -o nolog' 'set +o nolog' 'echo ok' 
pc_test
pcrun
[ "$out" = ok ] && [ "$rc" -eq 0 ] || pc_fail "status $rc out '$out' (stderr: $(cat pc.err))"
pc_pass
