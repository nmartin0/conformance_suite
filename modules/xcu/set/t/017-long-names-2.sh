# The long forms set -o errexit, nounset, noexec, verbose and xtrace act
# like -e, -u, -n, -v and -x.
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.set.017
# MUTANT    set.o_errexit_ignored
# MUTANT    set.plus_o_errexit_ignored
# MUTANT    set.o_nounset_ignored
# MUTANT    set.plus_o_nounset_ignored
# MUTANT    set.o_noexec_ignored
# MUTANT    set.o_verbose_ignored
# MUTANT    set.plus_o_verbose_ignored
# MUTANT    set.o_xtrace_ignored
# MUTANT    set.plus_o_xtrace_ignored
# MUTANT    set.o_errexit_is_noexec
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
# pcexp LABEL OUT RC: run pc.s; it must write OUT and exit with RC
pcexp() {
	pcrun
	[ "$out" = "$2" ] && [ "$rc" -eq "$3" ] || pc_fail "$1: status $rc and output '$out', expected status $3 and output '$2'"
}
pc_test
pcscript 'set -o errexit' 'false' 'echo after'
pcexp "errexit" "" 1
pcscript 'set -o errexit' 'set +o errexit' 'false' 'echo after'
pcexp "errexit turned off" after 0
pcscript 'set -o nounset' 'echo "$PCU1"' 'echo after'
pcrun
[ -z "$out" ] && [ "$rc" -ge 1 ] && [ "$rc" -le 125 ] && [ -s pc.err ] || pc_fail "nounset: status $rc out '$out'"
pcscript 'set -o nounset' 'set +o nounset' 'echo "[$PCU1]"'
pcexp "nounset turned off" "[]" 0
pcscript 'set -o noexec' 'echo notrun'
pcexp "noexec" "" 0
pcscript 'set -o verbose' 'echo marker_two' 'set +o verbose' 'echo marker_three'
pcrun
pchas pc.err 'echo marker_two' || pc_fail "verbose: input not written to standard error"
pchas pc.err 'echo marker_three' && pc_fail "verbose: input written after set +o verbose"
pcscript 'pcv=expanded_two' 'set -o xtrace' 'echo "$pcv"' 'set +o xtrace' 'echo third'
pcrun
pchas pc.err 'expanded_two' || pc_fail "xtrace: no trace on standard error"
pchas pc.err 'echo third' && pc_fail "xtrace: trace written after set +o xtrace"
pc_pass
