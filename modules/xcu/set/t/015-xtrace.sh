# With set -x the shell writes a trace of each command to standard error
# after expanding it.
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.set.015
# MUTANT    set.x_ignored
# MUTANT    set.x_default_on
# MUTANT    set.plus_x_ignored
# MUTANT    set.x_stdout
# MUTANT    set.x_before_expansion
# MUTANT    set.x_first_only
. "$PC_CORE/pc.sh"
pc_begin XCU.set.015
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
pcscript 'pcv=expanded_val' 'echo plain' 'set -x' 'echo "$pcv" two' 'echo second' 'set +x' 'echo third'
pc_test
pcrun
[ "$out" = "plain${nl}expanded_val two${nl}second${nl}third" ] || pc_fail "standard output was '$out'"
grep 'echo' pc.err | grep 'expanded_val' | grep two >/dev/null || pc_fail "no trace line with the expanded command on standard error"
grep '\$pcv' pc.err >/dev/null && pc_fail "the trace shows the command before expansion"
pchas pc.err 'echo second' || pc_fail "the second command was not traced"
pchas pc.err 'echo plain' && pc_fail "a command run before set -x was traced"
pchas pc.err 'echo third' && pc_fail "a command run after set +x was traced"
pc_pass
