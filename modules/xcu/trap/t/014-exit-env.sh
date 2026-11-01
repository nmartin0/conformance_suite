# The EXIT trap action runs in the environment the shell had after its last
# command (variables and working directory).
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.trap.014
# MUTANT    trap.exit_cwd_reset
# MUTANT    trap.exit_vars_stale
# MUTANT    trap.exit_function_lost
# MUTANT    trap.exit_umask_reset
# VARIANT   trap.exit_action_subshell
. "$PC_CORE/pc.sh"
pc_begin XCU.trap.014
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
mkdir pc.sub || pc_unresolved "mkdir"
d=$(cd pc.sub && pwd -P) || pc_unresolved "cd"
# the mask the script sets last, as this shell prints it
um=$(${PC_SH:-sh} -c 'umask 027; umask') || pc_unresolved "umask"
pcscript 'pcx=5' 'trap "echo \$pcx; pwd -P; pcf; umask" EXIT' 'pcx=6' 'cd pc.sub' 'pcf() { echo fn; }' 'umask 027'
pc_test
pcrun
[ "$out" = "6${nl}$d${nl}fn${nl}$um" ] || pc_fail "got '$out', expected the last variable value, directory, function and mask"
pc_pass
