# With set -v the shell writes the input it reads to standard error.
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.set.014
# MUTANT    set.v_ignored
# MUTANT    set.v_default_on
# MUTANT    set.plus_v_ignored
# MUTANT    set.v_stdout
# MUTANT    set.v_expanded
# MUTANT    set.v_first_only
. "$PC_CORE/pc.sh"
pc_begin XCU.set.014
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
pcscript 'echo plain' 'set -v' 'echo marker_one' 'pcv=val' 'echo "$pcv"' 'set +v' 'echo marker_two'
pc_test
pcrun
[ "$out" = "plain${nl}marker_one${nl}val${nl}marker_two" ] || pc_fail "standard output was '$out'"
pchas pc.err 'echo marker_one' || pc_fail "the command line was not written to standard error"
pchas pc.err 'pcv=val' || pc_fail "the assignment line was not written to standard error"
pchas pc.err 'echo "$pcv"' || pc_fail "the input was not written as it was read (before expansion)"
pchas pc.err 'echo plain' && pc_fail "a line read before set -v was written to standard error"
pchas pc.err 'echo marker_two' && pc_fail "a line read after set +v was written to standard error"
pc_pass
